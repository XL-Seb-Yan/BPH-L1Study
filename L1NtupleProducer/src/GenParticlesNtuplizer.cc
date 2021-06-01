#include "../interface/GenParticlesNtuplizer.h"

L1Analysis::GenParticlesNtuplizer::GenParticlesNtuplizer()
{
}

L1Analysis::GenParticlesNtuplizer::~GenParticlesNtuplizer()
{
}

void L1Analysis::GenParticlesNtuplizer::SetGenPart(const edm::Handle<reco::GenParticleCollection> genpart){
  std::vector<int> vDau ;
  std::vector<float> vDauPt ;
  std::vector<float> vDauEta ;
  std::vector<float> vDauPhi ;
  std::vector<float> vDauE ;
  std::vector<int> vMoth;
  std::vector<float> vMothPt ;
  std::vector<float> vMothEta ;
  std::vector<float> vMothPhi ;
  std::vector<float> vMothE ;
  int nMoth = 0;int nDau  = 0;  
  for( unsigned p=0; p<genParticles_->size(); ++p ){
    vDau.clear();
    vDauPt.clear();
    vDauEta.clear();
    vDauPhi.clear();
    vDauE.clear();
    vMoth.clear();
    vMothPt.clear();
    vMothEta.clear();
    vMothPhi.clear();
    vMothE.clear();
    nDau = 0; nMoth = 0;
    
		genPartData_.genParticle_pt    .push_back((*genParticles_)[p].pt()     );
		genPartData_.genParticle_eta   .push_back((*genParticles_)[p].eta()    );
		genPartData_.genParticle_phi   .push_back((*genParticles_)[p].phi()    );
		genPartData_.genParticle_mass  .push_back((*genParticles_)[p].mass()   );
		genPartData_.genParticle_status.push_back((*genParticles_)[p].status() );
		genPartData_.genParticle_pdgId .push_back((*genParticles_)[p].pdgId()  );
		
		for( unsigned int m=0; m<(*genParticles_)[p].numberOfMothers(); m++){
        vMoth.push_back( (*genParticles_)[p].mother(m)->pdgId() );
				vMothPt.push_back( (*genParticles_)[p].mother(m)->pt() );
				vMothEta.push_back( (*genParticles_)[p].mother(m)->eta() );
				vMothPhi.push_back( (*genParticles_)[p].mother(m)->phi() );
				vMothE.push_back( (*genParticles_)[p].mother(m)->energy() );
				nMoth++;
      }

      for( unsigned int d=0; d<(*genParticles_)[p].numberOfDaughters(); d++ ){
        vDau.push_back( (*genParticles_)[p].daughter(d)->pdgId() );
				vDauPt.push_back( (*genParticles_)[p].daughter(d)->pt() );
				vDauEta.push_back( (*genParticles_)[p].daughter(d)->eta() );
				vDauPhi.push_back( (*genParticles_)[p].daughter(d)->phi() );
				vDauE.push_back( (*genParticles_)[p].daughter(d)->energy() );
				nDau++;
      }
      
      genPartData_.genParticle_nDau  .push_back( nDau  );
      genPartData_.genParticle_nMoth .push_back( nMoth );      
      genPartData_.genParticle_mother.push_back( vMoth );
      genPartData_.genParticle_mother_pt.push_back( vMothPt );
      genPartData_.genParticle_mother_eta.push_back( vMothEta );
      genPartData_.genParticle_mother_phi.push_back( vMothPhi );
      genPartData_.genParticle_mother_e.push_back( vMothE );
      genPartData_.genParticle_dau   .push_back( vDau  );
      genPartData_.genParticle_dau_pt.push_back( vDauPt );
      genPartData_.genParticle_dau_eta.push_back( vDauEta );
      genPartData_.genParticle_dau_phi.push_back( vDauPhi );
      genPartData_.genParticle_dau_e.push_back( vDauE );
		
	}
	genPartData_.genParticle_N = genPartData_.genParticle_pt.size();
}