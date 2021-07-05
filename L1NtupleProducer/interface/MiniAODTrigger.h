#ifndef __HLTAnalysis_MiniAODTrigger_H__
#define __HLTAnalysis_MiniAODTrigger_H__

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "FWCore/Framework/interface/Event.h"

#include "MiniAODTriggerDataFormat.h"


namespace L1Analysis
{
  class MiniAODTrigger
  {
  public:
    enum {TEST=0};
    MiniAODTrigger();
    ~MiniAODTrigger();
    void Reset() {hlt_.Reset();}
    void SetTrigObj   (const edm::Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects, const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerBits);
    MiniAODTriggerDataFormat * getData() {return &hlt_;}

  private :
    MiniAODTriggerDataFormat hlt_;
  }; 
}
#endif