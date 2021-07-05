#ifndef __L1Analysis_MiniAODHLDataFormat_H__
#define __L1Analysis_MiniAODHLDataFormat_H__

#include <vector>
#include <string>
#include <TROOT.h>

namespace L1Analysis
{

  struct MiniAODHLDataFormat
  {
  
    MiniAODHLDataFormat(){ Reset();};
    ~MiniAODHLDataFormat(){};
    
    void Reset()
    {
      nEl = 0;
      ptEl.clear();
      etaEl.clear();
      phiEl.clear();
      energyEl.clear();
      chargeEl.clear();
      isPassVetoEl.clear();
      isLooseEl.clear();
      isTightEl.clear();
    }
   
    unsigned short int nEl;
    std::vector<float> ptEl;
    std::vector<float> etaEl;
    std::vector<float> phiEl;
    std::vector<float> energyEl;
    std::vector<int> chargeEl;
    std::vector<bool> isPassVetoEl;
    std::vector<bool> isLooseEl;
    std::vector<bool> isTightEl;

  }; 
}
#endif