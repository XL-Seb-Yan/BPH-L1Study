#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TClonesArray.h>           // ROOT array class
#include <TBenchmark.h>             // class to track macro running statistics
#include <TVector2.h>               // 2D vector class
#include <TMath.h>                  // ROOT math library
#include <vector>                   // STL vector class
#include <utility>
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include "TLorentzVector.h"         // 4-vector class
#include <THStack.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TF1.h>
#include <TStopwatch.h>
#include "TH1D.h"
#include "TH2.h"
#include "TRandom3.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
// C++ tool
#include <algorithm>
#include <map>
#endif

using namespace std;

float deltaR(float eta1, float phi1, float eta2, float phi2){
	float deta = eta1 - eta2;
	float dphi = TVector2::Phi_mpi_pi(phi1 - phi2);
	float dR = TMath::Sqrt(deta*deta+dphi*dphi);
	return dR;
}

void Frequency_plot_2MC(const TString samplename="ZeroBias2018",
									const int nEvents = -1,
									const float MuEtaCut = 5,
									const float EgEtaCut = 5){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	TString MuEtaCut_str = std::to_string(MuEtaCut).substr(0, std::to_string(MuEtaCut).find(".") + 2);
	TString EgEtaCut_str = std::to_string(EgEtaCut).substr(0, std::to_string(EgEtaCut).find(".") + 2);
	
	// Local variables:
	UShort_t N_eg, N_mu, N_genpart;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
	
	TString histname = "Mu_Eta" + MuEtaCut_str + "_El_Eta" + EgEtaCut_str;
	TH2F* hist2d_1 = new TH2F("hist",histname,10,0,10,9,1,10);
	
	TTreeReader fReader_L1;  //!the tree reader
	TTreeReaderValue<UShort_t> nEGs = {fReader_L1, "nEGs"};
	TTreeReaderArray<float> egEt    = {fReader_L1, "egEt"};
	TTreeReaderArray<float> egEta   = {fReader_L1, "egEta"};
	TTreeReaderArray<float> egPhi   = {fReader_L1, "egPhi"};
	TTreeReaderArray<short> egIso   = {fReader_L1, "egIso"};
	
	TTreeReaderValue<UShort_t> nMuons = {fReader_L1, "nMuons"};
	TTreeReaderArray<float> muonEt    = {fReader_L1, "muonEt"};
	TTreeReaderArray<float> muonEta   = {fReader_L1, "muonEta"};
	TTreeReaderArray<float> muonPhi   = {fReader_L1, "muonPhi"};
	TTreeReaderArray<unsigned short> muonIso   = {fReader_L1, "muonIso"};
	TTreeReaderArray<unsigned short> muonQual   = {fReader_L1, "muonQual"};
	
	TTreeReader fReader_GEN;  //!the tree reader
	TTreeReaderValue<int> nGenPart  = {fReader_GEN, "nPart"};
	TTreeReaderArray<float> genPartPt    = {fReader_GEN, "partPt"};
	TTreeReaderArray<float> genPartEta   = {fReader_GEN, "partEta"};
	TTreeReaderArray<float> genPartPhi   = {fReader_GEN, "partPhi"};
	TTreeReaderArray<float> genPartE     = {fReader_GEN, "partE"};
	TTreeReaderArray<int> genPartId    = {fReader_GEN, "partId"};
	TTreeReaderArray<int> genPartStat    = {fReader_GEN, "partStat"};
	TTreeReaderArray<int> genPartParent    = {fReader_GEN, "partParent"};
		
	int globcounter_0, globcounter_1;
	for(int MuEtThr = 2; MuEtThr < 3; MuEtThr+=1){
		for(int EgEtThr = 0; EgEtThr < 10; EgEtThr+=1){
			int counter_0 = 0; //count how many events have two right electrons at GEN level
			int counter_1 = 0; //count how many events have two right electrons and final state muon at GEN level
			ifstream insample(samplename+TString(".txt"));
			std::string line;
			while (std::getline(insample, line)){
				TString file_name(line);
				
				// Read input file and get the TTrees
				cout << "Processing " << file_name <<endl; cout.flush();
				TFile *infile = TFile::Open(file_name,"READ");
				assert(infile);
			
				TDirectoryFile *df_L1 = (TDirectoryFile *)infile->Get("l1UpgradeTree");
				TTree* eventTree_L1 = (TTree*)df_L1->Get("L1UpgradeTree");
				TDirectoryFile *df_GEN = (TDirectoryFile *)infile->Get("genTree");
				TTree* eventTree_GEN = (TTree*)df_GEN->Get("L1GenTree");
				assert(eventTree_L1);
				assert(eventTree_GEN);
				
				fReader_L1.SetTree(eventTree_L1);
				fReader_GEN.SetTree(eventTree_GEN);
				
				int Evt2Process_L1 = eventTree_L1->GetEntries();
				int Evt2Process_GEN = eventTree_GEN->GetEntries();
				if(Evt2Process_L1 != Evt2Process_GEN){
					cout<<"Two trees does not match!"<<endl;
					break;
				}
				if(nEvents != -1)
					Evt2Process_L1 = nEvents;
				
				for(UInt_t ientry=0; ientry<Evt2Process_L1; ientry++){
					fReader_L1.SetLocalEntry(ientry);
					fReader_GEN.SetLocalEntry(ientry);
					
					N_eg = *nEGs;
					N_mu = *nMuons;
					N_genpart = *nGenPart;
					
					//find GEN level electrons from B+ decay
					std::vector<TLorentzVector> gen_el;	
					for(UInt_t i=0; i<N_genpart; i++){
						if(abs(genPartId[i]) != 11 || abs(genPartParent[i]) != 521) continue; 
						TLorentzVector el;
						el.SetPtEtaPhiE(genPartPt[i],genPartEta[i],genPartPhi[i],genPartE[i]);
						gen_el.push_back(el);
					}
					if(gen_el.size() < 2){
						cout<<"Did not find 2 GEN level electrons!";
						continue;
					}
					counter_0++;
					
					//find GEN level muons with status 1
					std::vector<TLorentzVector> gen_mu;	
					for(UInt_t i=0; i<N_genpart; i++){
						if(abs(genPartId[i]) != 13 || genPartStat[i] != 1) continue; 
						TLorentzVector mu;
						mu.SetPtEtaPhiE(genPartPt[i],genPartEta[i],genPartPhi[i],genPartE[i]);
						gen_mu.push_back(mu);
					}
					if(gen_mu.size() < 1){
						//cout<<"Did not find 1 GEN level muons!";
						continue;
					}
					counter_1++;

					// L1
					bool isMuPass = false;
					bool isEgPass = false;
					
					for(UInt_t i=0; i<N_eg; i++){
						if(abs(egEta[i]) > EgEtaCut || egEt[i] < EgEtThr) continue;
						float mindR = 99;
						for(int iGENel = 0; iGENel < gen_el.size(); iGENel++){
							float dR = deltaR(egEta[i], egPhi[i], gen_el[iGENel].Eta(), gen_el[iGENel].Phi());
							if(dR < mindR) mindR = dR;
						}
						if(mindR > 0.4) continue;
						isEgPass = true;
						break;
					}
					
					for(UInt_t i=0; i<N_mu; i++){
						if(abs(muonEta[i]) > MuEtaCut || muonEt[i] < MuEtThr) continue;
						if(muonQual[i] < 12) continue;
						float mindR = 99;
						for(int iGENmu = 0; iGENmu < gen_mu.size(); iGENmu++){
							float dR = deltaR(muonEta[i], muonPhi[i], gen_mu[iGENmu].Eta(), gen_mu[iGENmu].Phi());
							if(dR < mindR) mindR = dR;
						}
						if(mindR > 0.4) continue;
						isMuPass = true;
						break;
					}
					
					/*
					// Check if this event has Kee falling withing the acceptance
					bool isElInAcceptance_0 = false;
					bool isElInAcceptance_1 = false;
					float mindR_0 = 99;
					float mindR_1 = 99;
					int matchL1Index_0 = -1;
					int matchL1Index_1 = -1;
					// matching the first GEN electron
					for(UInt_t i=0; i<N_eg; i++){
						if(deltaR(egEta[i], egPhi[i], gen_el[0].Eta(), gen_el[0].Phi()) < mindR_0){
							mindR_0 = deltaR(egEta[i], egPhi[i], gen_el[0].Eta(), gen_el[0].Phi());
							matchL1Index_0 = i;
						}
					}
					if(matchL1Index_0 < 0 || mindR_0 > 0.4) continue;
					if(egEt[matchL1Index_0] > 2 && abs(egEta[matchL1Index_0]) < 2.4) isElInAcceptance_0 = true;
					// matching the second GEN electron
					for(UInt_t i=0; i<N_eg; i++){
						if(i == matchL1Index_0) continue;
						if(deltaR(egEta[i], egPhi[i], gen_el[1].Eta(), gen_el[1].Phi()) < mindR_1){
							mindR_1 = deltaR(egEta[i], egPhi[i], gen_el[1].Eta(), gen_el[1].Phi());
							matchL1Index_1 = i;
						}
					}
					if(matchL1Index_1 < 0 || mindR_1 > 0.4) continue;
					if(egEt[matchL1Index_1] > 2 && abs(egEta[matchL1Index_1]) < 2.4) isElInAcceptance_1 = true;
					*/
					
					if(isMuPass && isEgPass)
					//if(isMuPass && isEgPass && isElInAcceptance_0 && isElInAcceptance_1)
						hist2d_1->Fill(EgEtThr+0.5, MuEtThr+0.5);
					
				}//end of event loop
				infile->Close();
			}//end of file loop
			globcounter_0 = counter_0;
			globcounter_1 = counter_1;
		}
	}
	cout<<globcounter_0<<" "<<globcounter_1<<endl;
	hist2d_1->Scale(1.0 / globcounter_0);
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	hist2d_1->SetLineWidth(2);
	yaxis = hist2d_1->GetYaxis();
	xaxis = hist2d_1->GetXaxis();
	xaxis->SetTitle("Threshold (>=) on E_{T e/#gamma} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Threshold (>=) on p_{T #mu} [GeV]");
	hist2d_1->Draw("COLZ TEXT");
	TString outName = "Eff_mu_Eta"+MuEtaCut_str+"_el_Eta"+EgEtaCut_str+"matchingdR0.4";
	gStyle->SetOptTitle(0);
	c11->Print(outName+".png");
	//c11->Print(outName+".svg");
		
	gBenchmark->Show("L1NtupleReader");
}