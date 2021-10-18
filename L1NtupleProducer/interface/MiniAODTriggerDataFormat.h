#ifndef __HLTAnalysis_MiniAODTriggerDataFormat_H__
#define __HLTAnalysis_MiniAODTriggerDataFormat_H__

#include <vector>
#include <string>
#include <TROOT.h>

namespace L1Analysis
{

  struct MiniAODTriggerDataFormat
  {
  
    MiniAODTriggerDataFormat(){ Reset();};
    ~MiniAODTriggerDataFormat(){};
    
    void Reset()
    {
      nTrigObjs = 0;
      ptTriObj.clear();
      etaTriObj.clear();
      phiTriObj.clear();
      energyTriObj.clear();
    }
   
    unsigned short int nTrigObjs;
    std::vector<float> ptTriObj;
    std::vector<float> etaTriObj;
    std::vector<float> phiTriObj;
    std::vector<float> energyTriObj;
  }; 
}
#endif