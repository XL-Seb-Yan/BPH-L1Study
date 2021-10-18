#ifndef __L1Analysis_MiniAODHLObjects_H__
#define __L1Analysis_MiniAODHLObjects_H__

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "FWCore/Framework/interface/Event.h"

#include "MiniAODHLDataFormat.h"


namespace L1Analysis
{
  class MiniAODHightLevel
  {
  public:
    enum {TEST=0};
    MiniAODHightLevel();
    ~MiniAODHightLevel();
    void Reset() {highLevel_.Reset();}
    void SetEvt(const edm::Event& iEvent);
    void SetEl(const edm::Handle<std::vector<pat::Electron> > electrons);
    MiniAODHLDataFormat * getData() {return &highLevel_;}

  private :
    MiniAODHLDataFormat highLevel_;
  }; 
}
#endif