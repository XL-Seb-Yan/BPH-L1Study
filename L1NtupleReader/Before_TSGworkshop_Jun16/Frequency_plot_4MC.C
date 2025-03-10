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

void Frequency_plot_4MC(const TString samplename="KeeMC",
											const int nEvents = -1,
											const int FixedMuEtMin = 0,
											const float MuEtaCut = 2.5,
											const float EgEtaCut = 2.5){
	
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
	TString histname = "#Delta R (MuMinEt"+std::to_string(FixedMuEtMin)+"MaxEta"+MuEtaCut_str+"_EgMaxEta"+EgEtaCut_str + ")";
	TH1F* hist_11 = new TH1F("dR",histname, 50, 0, 5);
	
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
			
			float MuEt_temp = 0;
			float EglEt_temp = 0;
			
			int MuSel_index = -99;
			int EgSel_index = -99;
			
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
			
			for(UInt_t i=0; i<N_mu; i++){
				if(abs(muonEta[i]) > MuEtaCut || muonEt[i] < FixedMuEtMin) continue;
				if(muonQual[i] < 12) continue;
				MuSel_index = i;
				break;
			}
			if(MuSel_index < 0) continue;
			
			for(UInt_t i=0; i<N_eg; i++){
				if(abs(egEta[i]) > EgEtaCut) continue;
				if(deltaR(egEta[i], egPhi[i], gen_el[0].Eta(), gen_el[0].Phi()) > 0.3 && deltaR(egEta[i], egPhi[i], gen_el[1].Eta(), gen_el[1].Phi()) > 0.3) continue;
				EgSel_index = i;
				break;
			}
			if(EgSel_index < 0) continue;
			
			float dR = deltaR(muonEta[MuSel_index], muonPhi[MuSel_index], egEta[EgSel_index], egPhi[EgSel_index]);
			hist_11->Fill(dR);
			
		}//end of event loop
		infile->Close();
	}//end of file loop
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	yaxis = hist_11->GetYaxis();
	xaxis = hist_11->GetXaxis();
	yaxis->SetTitle("Entries / 0.1");
	xaxis->SetTitle("#Delta R (#mu_{L}, e/#gamma_{L})");
	xaxis->SetTitleOffset(1.2);
	hist_11->Draw("HIST");
	TString outName = "Histo_dR_MuMinEt"+std::to_string(FixedMuEtMin)+"MaxEta_"+MuEtaCut_str+"_EgMaxEta"+EgEtaCut_str+".png";
	c11->Print(outName);
	
	cout<<hist_11->GetEntries()<<endl;
	
	gBenchmark->Show("L1NtupleReader");
}