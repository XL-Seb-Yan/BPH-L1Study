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

void Frequency_plot_elel_MC(const TString samplename="KeeMC",
											const int nEvents = -1,
											const float EgEtaCut = 1.0,
											const float dRCut = 1.0){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	TString EgEtaCut_str = std::to_string(EgEtaCut).substr(0, std::to_string(EgEtaCut).find(".") + 2);
	TString dRCut_str = std::to_string(dRCut).substr(0, std::to_string(dRCut).find(".") + 2);
	
	// Local variables:
	UShort_t N_eg, N_mu, N_genpart;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
	
	// Histograms
	TString histname = "Rate (Double_el_eta"+EgEtaCut_str+"_dR"+dRCut_str+")";
  TH2F* hist2d_1 = new TH2F("EgEtl_EgEts",histname,10,1,11,10,1,11);
	TH1F* hist_11 = new TH1F("dR1","#Delta R of matching",70,-2,5);
	TH1F* hist_12 = new TH1F("dR2","#Delta R of matching",70,-2,5);
	TH1F* hist_13 = new TH1F("mf","Matching flag of di-EG fired trigger",40,0,20);
	
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
	
	int globcounter_0;
	for(int EgEtThr = 2; EgEtThr < 11; EgEtThr++){
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
				cout<<Evt2Process_L1<<" "<<Evt2Process_GEN<<endl;
				cout << file_name <<endl;
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
				N_genpart = *nGenPart;									 
				
				std::vector<int> EgSel_index;
				
				//find GEN level electrons from B+ decay
				std::vector<TLorentzVector> gen_el;	
				for(UInt_t i=0; i<N_genpart; i++){
					if(abs(genPartId[i]) != 11 || genPartParent[i] != 521) continue;
					TLorentzVector el;
					el.SetPtEtaPhiE(genPartPt[i],genPartEta[i],genPartPhi[i],genPartE[i]);
					gen_el.push_back(el);
				}
				if(gen_el.size() < 2){
					//cout<<"Did not find 2 GEN level electrons!";
					continue;
				}
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
				counter_1++;
				
				// Check if two matched EG fired trigger
				float isFired = false;
				float deltaR_egeg = deltaR(egEta[matchL1Index_0], egPhi[matchL1Index_0], egEta[matchL1Index_1], egPhi[matchL1Index_1]);
				if(egEt[matchL1Index_0] > EgEtThr && egEt[matchL1Index_1] > EgEtThr && abs(egEta[matchL1Index_0]) < EgEtaCut && abs(egEta[matchL1Index_1]) < EgEtaCut && deltaR_egeg < dRCut)
					isFired = true;
				if(!isFired) continue;
				counter_2++;
				
				// Check if this event has Kee falling withing the acceptance
				bool isElInAcceptance_0 = false;
				bool isElInAcceptance_1 = false;
				if(egEt[matchL1Index_0] > 2 && abs(egEta[matchL1Index_0]) < 2.4) isElInAcceptance_0 = true;
				if(egEt[matchL1Index_1] > 2 && abs(egEta[matchL1Index_1]) < 2.4) isElInAcceptance_1 = true;
				
				if(isFired && isElInAcceptance_0 && isElInAcceptance_1){
					hist2d_1->Fill(EgEtThr+0.5,EgEtThr+0.5);
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
  TString outName = "EffAcc_elel_Eta"+EgEtaCut_str+"_dR"+dRCut_str;
  c11->Print(outName+".png");
  //c11->Print(outName+".svg");
	
	TLegend *leg = new TLegend(0.6,0.75,0.9,0.9);
	TCanvas *c12 = new TCanvas("","",1200,900);
  c12->cd();
  hist_11->SetLineWidth(2);
	hist_12->SetLineWidth(2);
	hist_11->SetLineColor(2);
	//hist_11->SetFillColorAlpha(2,0.5);
  yaxis = hist_11->GetYaxis();
  xaxis = hist_11->GetXaxis();
  xaxis->SetTitle("#Delta R");
  yaxis->SetTitle("Entries / 0.1");
	yaxis->SetRangeUser(0.1,1000000);
  xaxis->SetTitleOffset(1.2);
  hist_11->Draw("HIST");
	hist_12->Draw("SAME");
	leg->AddEntry(hist_11,"electron 1");
	leg->AddEntry(hist_12,"electron 2");
	leg->Draw();
	c12->SetLogy();
  //c12->Print("dR.png");
	
	TCanvas *c13 = new TCanvas("","",1200,900);
  c13->cd();
  hist_13->SetLineWidth(2);
	hist_13->SetLineWidth(2);
  yaxis = hist_13->GetYaxis();
  xaxis = hist_13->GetXaxis();
  xaxis->SetTitle("Matching flag");
  yaxis->SetTitle("Entries");
	//yaxis->SetRangeUser(0, 5300);
  xaxis->SetTitleOffset(1.2);
  hist_13->Draw("HIST");
  //c13->Print("MF_dR0.2.png");
		
	gBenchmark->Show("L1NtupleReader");
}