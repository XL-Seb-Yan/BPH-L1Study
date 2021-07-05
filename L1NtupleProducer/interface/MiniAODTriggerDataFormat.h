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
      pt.clear();
      eta.clear();
      phi.clear();
      energy.clear();
      collection.clear();
    }
   
    unsigned short int nTrigObjs;
    std::vector<float> pt;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<float> energy;
    std::vector<std::string> collection;

  }; 
}
#endif