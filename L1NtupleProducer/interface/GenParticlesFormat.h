#ifndef GenParticlesFormat_H
#define GenParticlesFormat_H

#include <vector>

namespace L1Analysis
{
  
  struct GenParticlesFormat
  {

   GenParticlesFormat(){ Reset();};
   ~GenParticlesFormat(){};

   void Reset(){
   /** genParticle */
      genParticle_N = 0;
      genParticle_pt.clear();
      genParticle_eta.clear();
      genParticle_phi.clear();
      genParticle_mass.clear();
      genParticle_pdgId.clear();
      genParticle_status.clear();
      genParticle_mother.clear();
      genParticle_mother_pt.clear();
      genParticle_mother_eta.clear();
      genParticle_mother_phi.clear();
      genParticle_mother_e.clear();
      genParticle_nMoth.clear();
      genParticle_nDau.clear();
      genParticle_dau.clear();
      genParticle_dau_pt.clear();
      genParticle_dau_eta.clear();
      genParticle_dau_phi.clear();
      genParticle_dau_e.clear();
   }
   
    /** genParticles */
   int                             genParticle_N;
   std::vector<float>              genParticle_pt;
   std::vector<float>              genParticle_px;
   std::vector<float>              genParticle_py;
   std::vector<float>              genParticle_pz;
   std::vector<float>              genParticle_e;
   std::vector<float>              genParticle_eta;
   std::vector<float>              genParticle_phi;
   std::vector<float>              genParticle_mass;
   std::vector<int  >              genParticle_pdgId;
   std::vector<int  >              genParticle_status;
   std::vector<int  >              genParticle_nDau;
   std::vector<int  >              genParticle_nMoth;
   std::vector<std::vector<int> >  genParticle_mother;
   std::vector<std::vector<float> >  genParticle_mother_pt;
   std::vector<std::vector<float> >  genParticle_mother_eta;
   std::vector<std::vector<float> >  genParticle_mother_phi;
   std::vector<std::vector<float> >  genParticle_mother_e; 
   std::vector<std::vector<int> >    genParticle_dau;
   std::vector<std::vector<float> >  genParticle_dau_pt;
   std::vector<std::vector<float> >  genParticle_dau_eta;
   std::vector<std::vector<float> >  genParticle_dau_phi;
   std::vector<std::vector<float> >  genParticle_dau_e;
  };
}

#endif // GenParticlesFormat_H
