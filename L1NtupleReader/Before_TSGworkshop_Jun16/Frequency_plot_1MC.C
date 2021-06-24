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

void Frequency_plot_1MC(const TString samplename="ZeroBias2018",
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
	
	// Histograms
	// TH1F* hist_11 = new TH1F("NEg","Number of e/#gamma", 40, 0, 20);
	// TH1F* hist_12 = new TH1F("EgEt","e/#gamma E_{T}", 100, 0, 10);
	// TH1F* hist_13 = new TH1F("EgEta","e/#gamma #eta", 100, -5, 5);
	// TH1F* hist_14 = new TH1F("EgPhi","e/#gamma #phi", 100, -3.15, 3.15);
	// TH1F* hist_21 = new TH1F("NMu","Number of #mu", 20, 0, 10);
	// TH1F* hist_22 = new TH1F("MuEt","#mu E_{T}", 100, 0, 100);
	// TH1F* hist_23 = new TH1F("MuEta","#mu #eta", 100, -5, 5);
	// TH1F* hist_24 = new TH1F("MuPhi","#mu #phi", 100, -3.15, 3.15);
	
	TString histname = "";
	TH1F* hist_11 = new TH1F("muon pt","muon pt",100,0,100);
	TH2F* hist2d_1 = new TH2F("hist",histname,10,0,10,9,1,10);
	
	int counter_0 = 0, counter_1 = 0, counter_2 = 0, counter_3 = 0;
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

			counter_0++;
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
			counter_1++;
			
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
			
			int MuSel_index = -99;
			
			for(UInt_t i=0; i<N_mu; i++){
				if(abs(muonEta[i]) > MuEtaCut) continue;
				if(muonQual[i] < 12) continue;
				MuSel_index = i;
				break;
			}
			if(MuSel_index < 0) continue;
				
			
			std::vector<int> eg_sel_arr;
			for(UInt_t i=0; i<N_eg; i++){
				if(abs(egEta[i]) > EgEtaCut) continue;
				eg_sel_arr.push_back(i);
			}
			if(eg_sel_arr.size() < 1) continue;
			
			bool isMuMatch = false;
			for(int j=0; j<gen_mu.size(); j++){
				if(deltaR(muonEta[MuSel_index], muonPhi[MuSel_index], gen_mu[j].Eta(), gen_mu[j].Phi()) > 0.4) continue;
				isMuMatch = true;
			}
			if(!isMuMatch) continue;
			
			int isEgMatch = 0;
			if(deltaR(egEta[eg_sel_arr[0]], egPhi[eg_sel_arr[0]], gen_el[0].Eta(), gen_el[0].Phi()) <= 0.4)
				isEgMatch += 1;
			if(deltaR(egEta[eg_sel_arr[0]], egPhi[eg_sel_arr[0]], gen_el[1].Eta(), gen_el[1].Phi()) <= 0.4)
				isEgMatch += 2;
			if(isEgMatch == 0) continue;
			
			bool isEgslPre = false;
			for(UInt_t i=0; i<N_eg; i++){
				if(i == eg_sel_arr[0]) continue;
				if(abs(egEta[i]) > 2.5) continue;
				if(egEt[i] < 2) continue;
				if(isEgMatch == 1){
				if(deltaR(egEta[i], egPhi[i], gen_el[1].Eta(), gen_el[1].Phi()) > 0.4) continue;}
				if(isEgMatch == 2){
				if(deltaR(egEta[i], egPhi[i], gen_el[0].Eta(), gen_el[0].Phi()) > 0.4) continue;}
				isEgslPre = true;
			}
			if(!isEgslPre) continue;
			
			
			for(int MuThr = 1; MuThr < 10; MuThr+=1){
				for(int EgThr = 0; EgThr < 10; EgThr+=1){
			
					if(egEt[eg_sel_arr[0]] >= EgThr && muonEt[MuSel_index] >= MuThr)
								hist2d_1->Fill(EgThr+0.5, MuThr+0.5);
				}
			}
			
		}//end of event loop
		infile->Close();
	}//end of file loop
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
		
	Int_t refBinGlobal = hist2d_1->GetBin(1,10);
	float refBinEntry = hist2d_1->GetBinContent(refBinGlobal);
	//hist2d_1->Scale(1/refBinEntry);
	hist2d_1->Scale(1.0/counter_0);
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	yaxis = hist2d_1->GetYaxis();
	xaxis = hist2d_1->GetXaxis();
	xaxis->SetTitle("Threshold (>=) on E_{T e/#gamma} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Threshold (>=) on p_{T #mu} [GeV]");
	hist2d_1->Draw("COLZ TEXT");
	TString outName = "Histo_frequency_MuEta" + MuEtaCut_str + "_EgEta" + EgEtaCut_str;
	if(samplename.Contains("MC"))
		outName = "Histo_efficiency_MuEta" + MuEtaCut_str + "_EgEta" + EgEtaCut_str;
	gStyle->SetOptTitle(0);
	c11->Print(outName+".png");
	c11->Print(outName+".svg");
	
	TCanvas *c12 = new TCanvas("","",1200,900);
	c12->cd();
	hist_11->Draw();
	c12->Print("muonpt.png");
	
	cout<<counter_0<<" "<<counter_1<<" "<<counter_2<<" "<<counter_3<<endl;
		
	gBenchmark->Show("L1NtupleReader");
}