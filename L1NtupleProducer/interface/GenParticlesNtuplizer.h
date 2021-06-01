#ifndef __GenParticlesNtuplizer_H__
#define __GenParticlesNtuplizer_H__

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "GenParticlesFormat.h"
namespace L1Analysis
{
  class GenParticlesNtuplizer
  {
  public:
    GenParticlesNtuplizer();
    ~GenParticlesNtuplizer();
    void SetGenPart (const edm::Handle<reco::GenParticleCollection> genpart);
    GenParticlesFormat * getData() {return &genPartData_;}

  private :
    GenParticlesFormat genPartData_;
  }; 
}
#endif