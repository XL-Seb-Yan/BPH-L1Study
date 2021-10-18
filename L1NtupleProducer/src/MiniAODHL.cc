#include "BPHL1Study/L1NtupleProducer/interface/MiniAODHL.h"

L1Analysis::MiniAODHightLevel::MiniAODHightLevel()
{
}

L1Analysis::MiniAODHightLevel::~MiniAODHightLevel()
{

}

void L1Analysis::MiniAODHightLevel::SetEvt(const edm::Event& iEvent)
{
    highLevel_.evtEvent = iEvent.id().event();
    highLevel_.evtRun = iEvent.id().run();
    highLevel_.evtLumiBlock = iEvent.id().luminosityBlock();
    
}

void L1Analysis::MiniAODHightLevel::SetEl(const edm::Handle<std::vector<pat::Electron> > electrons)
{
  int nEls = 0;
  for (const pat::Electron &el : *electrons) {
    nEls++;
    if (el.pt() < 2) continue;
    highLevel_.ptEl.push_back(el.pt());
    highLevel_.etaEl.push_back(el.eta());
    highLevel_.phiEl.push_back(el.phi());
    highLevel_.energyEl.push_back(el.energy());
    highLevel_.chargeEl.push_back(el.charge());
    highLevel_.isPassVetoEl.push_back(el.electronID("cutBasedElectronID-Fall17-94X-V2-veto"));
    highLevel_.isLooseEl.push_back(el.electronID("cutBasedElectronID-Fall17-94X-V2-loose"));
    highLevel_.isTightEl.push_back(el.electronID("cutBasedElectronID-Fall17-94X-V2-tight"));
    }
  
  highLevel_.nEl = nEls;
}






