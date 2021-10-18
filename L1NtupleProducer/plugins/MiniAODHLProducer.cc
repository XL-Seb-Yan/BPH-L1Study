// system include files
#include <memory>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "BPHL1Study/L1NtupleProducer/interface/MiniAODHL.h"

class MiniAODHLProducer : public edm::EDAnalyzer {
  public:
    explicit MiniAODHLProducer (const edm::ParameterSet&);
    ~MiniAODHLProducer() {}

  private:
    void beginJob(void) override ;
    void analyze(const edm::Event&, const edm::EventSetup&) override;
    void endJob() override;
      
  public:
    L1Analysis::MiniAODHightLevel* highLevel;
    L1Analysis::MiniAODHLDataFormat * highLevelData;
    
  private:
    // output file
    edm::Service<TFileService> fs_;
    
    // tree
    TTree * tree_;

   edm::EDGetTokenT<std::vector<pat::Electron> > electronToken_;
};

MiniAODHLProducer::MiniAODHLProducer(const edm::ParameterSet& iConfig):
   electronToken_(consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electrons")))
    
{
  
  highLevel     = new L1Analysis::MiniAODHightLevel();
  highLevelData = highLevel->getData();
  
  // set up output
  tree_=fs_->make<TTree>("HighLevelTree", "HighLevelTree");
  tree_->Branch("HighLevel", "L1Analysis::MiniAODHLDataFormat", &highLevelData, 32000, 3);
}


void MiniAODHLProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  highLevelData->Reset();

  edm::Handle<std::vector<pat::Electron> > electrons;
  iEvent.getByToken(electronToken_, electrons);
  
  highLevel->SetEvt(iEvent);

  if (electrons.isValid()){ 
    highLevel->SetEl(electrons);
  } else {
    edm::LogWarning("MissingProduct") << "Electrons not found. Branch will not be filled" << std::endl;
  }
  
  tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
MiniAODHLProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniAODHLProducer::endJob() {
}


//define this as a plug-in
DEFINE_FWK_MODULE(MiniAODHLProducer);