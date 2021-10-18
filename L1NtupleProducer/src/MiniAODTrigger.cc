#include "BPHL1Study/L1NtupleProducer/interface/MiniAODTrigger.h"

L1Analysis::MiniAODTrigger::MiniAODTrigger()
{
}

L1Analysis::MiniAODTrigger::~MiniAODTrigger()
{

}

void L1Analysis::MiniAODTrigger::SetTrigObj(const edm::Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects, const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerBits)
{
  
  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
  int nTrigObjets = 0;
  //for (l1t::EGammaBxCollection::const_iterator it=em->begin(ibx); it!=em->end(ibx) && l1upgrade_.nEGs<maxL1Upgrade; it++){
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { // note: not "const &" since we want to call unpackPathNames
    nTrigObjets++;

    obj.unpackPathNames(names);
    obj.unpackFilterLabels(iEvent, *triggerBits);
    // For type definition, see https://github.com/cms-sw/cmssw/blob/master/DataFormats/HLTReco/interface/TriggerTypeDefs.h
    bool isEl = false;
    for (unsigned h = 0; h < obj.filterIds().size(); ++h){
      if(obj.filterIds()[h] != 82) continue;
      isEl = true;
      break;
    }
    if(isEl){
      hlt_.ptTriObj.push_back(obj.pt());
      hlt_.etaTriObj.push_back(obj.eta());
      hlt_.phiTriObj.push_back(obj.phi());
      hlt_.energyTriObj.push_back(obj.energy());
    }
  }
  hlt_.nTrigObjs = nTrigObjets;
  
}






