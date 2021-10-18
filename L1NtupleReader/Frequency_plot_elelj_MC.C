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

void Frequency_plot_elelj_MC(const TString samplename="KeeMC",
											const int nEvents = -1,
                      const float EgEtCut = 1.0,
											const float EgEtaCut = 1.0,
											const float EgEgdRCut = 1.0){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	TString EgEtaCut_str = std::to_string(EgEtaCut).substr(0, std::to_string(EgEtaCut).find(".") + 2);
	TString EgEgdRCut_str = std::to_string(EgEgdRCut).substr(0, std::to_string(EgEgdRCut).find(".") + 2);
	
	// Local variables:
	UShort_t N_eg, N_mu, N_jet, N_genpart;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
	
	// Histograms
	TString histname = "Rate (Double_el_eta"+EgEtaCut_str+"_dR"+EgEgdRCut_str+"jet)";
  TH2F* hist2d_1 = new TH2F("EgEtl_EgEts",histname,10,1,11,10,1,11);
  TH1F* hist_11 = new TH1F("jet_pt_1","pT jet",150,0,150);
  TH1F* hist_21 = new TH1F("jet_eta_1","eta jet",100,-3,3);
  TH1F* hist_31 = new TH1F("jet_dR_min_1","min dR(e,j)",100,0,3);
	TH1F* hist_12 = new TH1F("jet_pt_2","pT jet",150,0,150);
  TH1F* hist_22 = new TH1F("jet_eta_2","eta jet",100,-3,3);
  TH1F* hist_32 = new TH1F("jet_dR_min_2","min dR(e,j)",100,0,3);
	
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
	
	TTreeReaderValue<UShort_t> nJets = {fReader_L1, "nJets"};
  TTreeReaderArray<float> jetEt    = {fReader_L1, "jetEt"};
  TTreeReaderArray<float> jetEta   = {fReader_L1, "jetEta"};
  TTreeReaderArray<float> jetPhi   = {fReader_L1, "jetPhi"};
	
	TTreeReader fReader_GEN;  //!the tree reader
	TTreeReaderValue<int> nGenPart  = {fReader_GEN, "nPart"};
	TTreeReaderArray<float> genPartPt    = {fReader_GEN, "partPt"};
	TTreeReaderArray<float> genPartEta   = {fReader_GEN, "partEta"};
	TTreeReaderArray<float> genPartPhi   = {fReader_GEN, "partPhi"};
	TTreeReaderArray<float> genPartE     = {fReader_GEN, "partE"};
	TTreeReaderArray<int> genPartId    = {fReader_GEN, "partId"};
	TTreeReaderArray<int> genPartStat    = {fReader_GEN, "partStat"};
	TTreeReaderArray<int> genPartParent    = {fReader_GEN, "partParent"};
	
	int globcounter_0;
	for(int JetEtThr = 0; JetEtThr < 1; JetEtThr++){
    int counter_0 = 0; //count how many events have two right electrons at GEN level
		int counter_1 = 0; //count how many events have two right electrons at GEN level
		int counter_2 = 0; //count how many events have two right electrons at GEN level
		int counter_3 = 0; //count how many events have two right electrons at GEN level
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
			//for(UInt_t ientry=0; ientry<1000; ientry++){
				fReader_L1.SetLocalEntry(ientry);
				fReader_GEN.SetLocalEntry(ientry);
				
				if(ientry % 100000 == 0) cout<<"Processing: "<<float(ientry) / Evt2Process_L1<<endl;
				
				N_eg = *nEGs;
        N_mu = *nMuons;
        N_jet = *nJets;
				N_genpart = *nGenPart;									 
				
				std::vector<int> EgSel_index;
				
				//find GEN level electrons from B+ decay
				std::vector<TLorentzVector> gen_el;
				std::vector<TLorentzVector> gen_K;	
				for(UInt_t i=0; i<N_genpart; i++){
					if((abs(genPartId[i]) != 11 && abs(genPartId[i]) != 321) || genPartParent[i] != 521) continue;
					if(abs(genPartId[i]) == 11){
						TLorentzVector el;
						el.SetPtEtaPhiE(genPartPt[i],genPartEta[i],genPartPhi[i],genPartE[i]);
						gen_el.push_back(el);
					}
					else{
						TLorentzVector K;
						K.SetPtEtaPhiE(genPartPt[i],genPartEta[i],genPartPhi[i],genPartE[i]);
						gen_K.push_back(K);
					}
				}
				if(gen_el.size() < 2) continue;
				if(gen_K.size() < 1) continue;
				if(gen_K.size() > 1) continue;
				counter_0++;
				
				// Matching two electrons:
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
				if(matchL1Index_0 < 0 || mindR_0 > 0.2) continue;
				// matching the second GEN electron
				for(UInt_t i=0; i<N_eg; i++){
					if(i == matchL1Index_0) continue;
					if(deltaR(egEta[i], egPhi[i], gen_el[1].Eta(), gen_el[1].Phi()) < mindR_1){
						mindR_1 = deltaR(egEta[i], egPhi[i], gen_el[1].Eta(), gen_el[1].Phi());
						matchL1Index_1 = i;
					}
				}
				if(matchL1Index_1 < 0 || mindR_1 > 0.2) continue;
				
				// Matching jet:
				float mindR_2 = 99;
				int matchL1Index_2 = -1;
				for(UInt_t i=0; i<N_jet; i++){
					if(deltaR(jetEta[i], jetPhi[i], gen_K[0].Eta(), gen_K[0].Phi()) < mindR_2){
						mindR_2 = deltaR(jetEta[i], jetPhi[i], gen_K[0].Eta(), gen_K[0].Phi());
						matchL1Index_2 = i;
					}
				}
				if(matchL1Index_2 < 0 || mindR_2 > 0.4) continue;
				counter_1++;
				
				hist_11->Fill(jetEt[matchL1Index_2]);
				hist_21->Fill(jetEta[matchL1Index_2]);
				hist_31->Fill(TMath::Min(deltaR(jetEta[matchL1Index_2],jetPhi[matchL1Index_2],egEta[matchL1Index_1],egPhi[matchL1Index_1]),deltaR(jetEta[matchL1Index_2],jetPhi[matchL1Index_2],egEta[matchL1Index_0],egPhi[matchL1Index_0])));
				
				// Check if two matched EG fired trigger
				float isFired = false;
				float deltaR_egeg = deltaR(egEta[matchL1Index_0], egPhi[matchL1Index_0], egEta[matchL1Index_1], egPhi[matchL1Index_1]);
				if(egEt[matchL1Index_0] > EgEtCut && egEt[matchL1Index_1] > EgEtCut && abs(egEta[matchL1Index_0]) < EgEtaCut && abs(egEta[matchL1Index_1]) < EgEtaCut && deltaR_egeg < EgEgdRCut)
					isFired = true;
				if(!isFired) continue;
				counter_2++;
				
				hist_12->Fill(jetEt[matchL1Index_2]);
				hist_22->Fill(jetEta[matchL1Index_2]);
				hist_32->Fill(TMath::Min(deltaR(jetEta[matchL1Index_2],jetPhi[matchL1Index_2],egEta[matchL1Index_1],egPhi[matchL1Index_1]),deltaR(jetEta[matchL1Index_2],jetPhi[matchL1Index_2],egEta[matchL1Index_0],egPhi[matchL1Index_0])));
				
				// Check if this event has Kee falling withing the acceptance
				bool isElInAcceptance_0 = false;
				bool isElInAcceptance_1 = false;
				if(egEt[matchL1Index_0] > 2 && abs(egEta[matchL1Index_0]) < 2.4) isElInAcceptance_0 = true;
				if(egEt[matchL1Index_1] > 2 && abs(egEta[matchL1Index_1]) < 2.4) isElInAcceptance_1 = true;
				
				if(isFired && isElInAcceptance_0 && isElInAcceptance_1){
					//hist2d_1->Fill(EgEtCut+0.5,EgEtCut+0.5);
					counter_3++;
				}
				
			}//end of event loop
			infile->Close();
		}//end of file loop
		globcounter_0 = counter_0;
		cout<<counter_0<<" "<<counter_1<<" "<<counter_2<<" "<<counter_3<<endl;
	}
	cout<<globcounter_0<<endl;
	hist2d_1->Scale(1.0 / globcounter_0);
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c11 = new TCanvas("","",1200,900);
  c11->cd();
  hist2d_1->SetLineWidth(2);
  yaxis = hist2d_1->GetYaxis();
  xaxis = hist2d_1->GetXaxis();
  yaxis->SetTitle("Threshold on E_{T e/#gamma sL} [GeV]");
  xaxis->SetTitle("Threshold on E_{T e/#gamma L} [GeV]");
  xaxis->SetTitleOffset(1.2);
  hist2d_1->Draw("COLZ TEXT");
  TString outName = "EffAcc_eleljet_Eta"+EgEtaCut_str+"_dR"+EgEgdRCut_str;
  c11->Print(outName+".png");
  //c11->Print(outName+".svg");
	
	TLegend *leg = new TLegend(0.6,0.75,0.9,0.9);
	TCanvas *c0 = new TCanvas("","",1200,900);
	c0->cd();
	hist_11->SetLineWidth(1);
	hist_12->SetLineWidth(2);
	yaxis = hist_11->GetYaxis();
	xaxis = hist_11->GetXaxis();
	xaxis->SetTitle("K p_{T} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_11->Draw("HIST");
	hist_11->SetLineColor(2);
	hist_12->SetLineColor(2);
	hist_12->Draw("SAME");
	leg->AddEntry(hist_11,"fired L1","l");
	leg->AddEntry(hist_12,"fired L1 & in Acc","l");
	gStyle->SetOptTitle(0);
	c0->Print("jet_pt_MC.png");
	
  TCanvas *c1 = new TCanvas("","",1200,900);
	c1->cd();
	hist_21->SetLineWidth(1);
	hist_22->SetLineWidth(2);
	yaxis = hist_21->GetYaxis();
	xaxis = hist_21->GetXaxis();
	xaxis->SetTitle("dR(e,e)");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_21->Draw("HIST");
	hist_21->SetLineColor(2);
	hist_22->SetLineColor(2);
	hist_22->Draw("SAME");
	leg->AddEntry(hist_21,"fired L1","l");
	leg->AddEntry(hist_22,"fired L1 & in Acc","l");
	gStyle->SetOptTitle(0);
	c1->Print("jet_eta_MC.png");
	
	TCanvas *c2 = new TCanvas("","",1200,900);
	c2->cd();
	hist_31->SetLineWidth(1);
	hist_32->SetLineWidth(2);
	yaxis = hist_31->GetYaxis();
	xaxis = hist_31->GetXaxis();
	xaxis->SetTitle("min dR(e,K)");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_31->Draw("HIST");
	hist_31->SetLineColor(2);
	hist_32->SetLineColor(2);
	hist_32->Draw("SAME");
	leg->AddEntry(hist_31,"fired L1","l");
	leg->AddEntry(hist_32,"fired L1 & in Acc","l");
	gStyle->SetOptTitle(0);
	c2->Print("mindReK_MC.png");
		
	gBenchmark->Show("L1NtupleReader");
}