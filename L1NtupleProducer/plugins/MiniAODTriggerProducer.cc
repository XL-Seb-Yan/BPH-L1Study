// system include files
#include <memory>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "BPHL1Study/L1NtupleProducer/interface/MiniAODTrigger.h"

class MiniAODTriggerProducer : public edm::EDAnalyzer {
  public:
    explicit MiniAODTriggerProducer (const edm::ParameterSet&);
    ~MiniAODTriggerProducer() {}

  private:
    void beginJob(void) override ;
    void analyze(const edm::Event&, const edm::EventSetup&) override;
    void endJob() override;
      
  public:
    L1Analysis::MiniAODTrigger* hlt;
    L1Analysis::MiniAODTriggerDataFormat * hltData;
    
  private:
    // output file
    edm::Service<TFileService> fs_;
    
    // tree
    TTree * tree_;

    edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
    edm::EDGetTokenT<std::vector<pat::TriggerObjectStandAlone> > triggerObjects_;
    edm::EDGetTokenT <pat::PackedTriggerPrescales> triggerPrescales_;
};

MiniAODTriggerProducer::MiniAODTriggerProducer(const edm::ParameterSet& iConfig):
   triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
   triggerObjects_(consumes<std::vector<pat::TriggerObjectStandAlone> >(iConfig.getParameter<edm::InputTag>("objects"))),
   triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales")))
    
{
  
  hlt     = new L1Analysis::MiniAODTrigger();
  hltData = hlt->getData();
  
  // set up output
  tree_=fs_->make<TTree>("HLTTree", "HLTTree");
  tree_->Branch("HLT", "L1Analysis::MiniAODTriggerDataFormat", &hltData, 32000, 3);
}


void MiniAODTriggerProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<edm::TriggerResults> triggerBits;
  iEvent.getByToken(triggerBits_, triggerBits);

   edm::Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects;
  iEvent.getByToken(triggerObjects_, triggerObjects);

   edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  iEvent.getByToken(triggerPrescales_, triggerPrescales);

  if (triggerObjects.isValid()){ 
    hlt->SetTrigObj(triggerObjects, iEvent, triggerBits);
  } else {
    edm::LogWarning("MissingProduct") << "TriggerObjects not found. Branch will not be filled" << std::endl;
  }
  
  tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
MiniAODTriggerProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniAODTriggerProducer::endJob() {
}


//define this as a plug-in
DEFINE_FWK_MODULE(MiniAODTriggerProducer);