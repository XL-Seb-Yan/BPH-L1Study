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
    std::cout << "\tTrigger object:  pt " << obj.pt() << ", eta " << obj.eta() << ", phi " << obj.phi() << std::endl;
    // Print trigger object collection and type
    std::cout << "\t   Collection: " << obj.collection() << std::endl;
    std::cout << "\t   Type IDs:   ";
    for (unsigned h = 0; h < obj.filterIds().size(); ++h) std::cout << " " << obj.filterIds()[h] ;
    std::cout << std::endl;
    // Print associated trigger filters
    std::cout << "\t   Filters:    ";
    for (unsigned h = 0; h < obj.filterLabels().size(); ++h) std::cout << " " << obj.filterLabels()[h];
    std::cout << std::endl;
  }
  hlt_.nTrigObjs = nTrigObjets;
  
}






