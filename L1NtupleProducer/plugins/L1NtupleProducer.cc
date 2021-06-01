// system include files
#include <memory>

// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// data formats
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1Upgrade.h"
#include "../interface/GenParticlesNtuplizer.h"

//
// class declaration
//

class L1NtupleProducer : public edm::EDAnalyzer {
public:
  explicit L1NtupleProducer(const edm::ParameterSet&);
  ~L1NtupleProducer() override;
  
  
private:
  void beginJob(void) override ;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

public:
  
  L1Analysis::L1AnalysisL1Upgrade* l1Upgrade;
  L1Analysis::L1AnalysisL1UpgradeDataFormat * l1UpgradeData;
  L1Analysis::genPartTuplizer* genPartTuplizer;
  L1Analysis::GenParticlesFormat * genPartData;

private:

  unsigned maxL1Upgrade_;

  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;
 
  // EDM input tags
  edm::EDGetTokenT<l1t::EGammaBxCollection> egToken_;
  std::vector< edm::EDGetTokenT<l1t::TauBxCollection> > tauTokens_;
  edm::EDGetTokenT<l1t::JetBxCollection> jetToken_;
  edm::EDGetTokenT<l1t::EtSumBxCollection> sumToken_;
  edm::EDGetTokenT<l1t::MuonBxCollection> muonToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> genParticlesToken_;

};



L1NtupleProducer::L1NtupleProducer(const edm::ParameterSet& iConfig)
{

  egToken_ = consumes<l1t::EGammaBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("egToken"));
  //tauToken_ = consumes<l1t::TauBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tauToken"));
  jetToken_ = consumes<l1t::JetBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("jetToken"));
  sumToken_ = consumes<l1t::EtSumBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("sumToken"));
  muonToken_ = consumes<l1t::MuonBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("muonToken"));
  genParticlesToken_ = consumes<reco::GenParticleCollection>(iConfig.getUntrackedParameter<edm::InputTag>("prunedGenParticles"));

  const auto& taus = iConfig.getUntrackedParameter<std::vector<edm::InputTag>>("tauTokens");
  for (const auto& tau: taus) {
    tauTokens_.push_back(consumes<l1t::TauBxCollection>(tau));
  }

 
  maxL1Upgrade_ = iConfig.getParameter<unsigned int>("maxL1Upgrade");
 
  l1Upgrade     = new L1Analysis::L1AnalysisL1Upgrade();
  l1UpgradeData = l1Upgrade->getData();
  
  genPartTuplizer = new L1Analysis::GenParticlesNtuplizer();
  genPartData = genPartTuplizer->getData();
  
  // set up output
  tree_=fs_->make<TTree>("L1UpgradeTree", "L1UpgradeTree");
  tree_->Branch("L1Upgrade", "L1Analysis::L1AnalysisL1UpgradeDataFormat", &l1UpgradeData, 32000, 3);
  tree_->Branch("genParticle", "L1Analysis::GenParticlesFormat", &genPartData, 32000, 3);

}


L1NtupleProducer::~L1NtupleProducer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called to for each event  ------------
void
L1NtupleProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  l1Upgrade->Reset();

  edm::Handle<l1t::EGammaBxCollection> eg;
  edm::Handle<l1t::JetBxCollection> jet;
  edm::Handle<l1t::EtSumBxCollection> sums;
  edm::Handle<l1t::MuonBxCollection> muon; 
  edm::Handle<reco::GenParticleCollection> genpart; 

  iEvent.getByToken(egToken_,   eg);
  iEvent.getByToken(jetToken_,  jet);
  iEvent.getByToken(sumToken_, sums);
  iEvent.getByToken(muonToken_, muon);
  iEvent.getByToken(genParticlesToken_, genpart);

  if (eg.isValid()){ 
    l1Upgrade->SetEm(eg, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Em not found. Branch will not be filled" << std::endl;
  }
  if (jet.isValid()){ 
    l1Upgrade->SetJet(jet, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Jets not found. Branch will not be filled" << std::endl;
  }

  if (sums.isValid()){ 
    l1Upgrade->SetSum(sums, maxL1Upgrade_);  
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade EtSums not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()){ 
    l1Upgrade->SetMuon(muon, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Muons not found. Branch will not be filled" << std::endl;
  }

  for (auto & tautoken: tauTokens_){
    edm::Handle<l1t::TauBxCollection> tau;
    iEvent.getByToken(tautoken,  tau);
    if (tau.isValid()){ 
      l1Upgrade->SetTau(tau, maxL1Upgrade_);
    } else {
      edm::LogWarning("MissingProduct") << "L1Upgrade Tau not found. Branch will not be filled" << std::endl;
    }
  }
  
  if (genpart.isValid()){ 
    genPartData->SetGenPart(genpart);
  } else {
    edm::LogWarning("MissingProduct") << "GenParticles not found. Branch will not be filled" << std::endl;
  }

  tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
L1NtupleProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1NtupleProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1NtupleProducer);
