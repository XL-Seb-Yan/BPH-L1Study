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

void Frequency_plot_5(const TString samplename="ZeroBias2018",
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
	UShort_t N_eg, N_mu;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
	
	// Histograms
	TString histname = "#Delta R (MuMinEt"+std::to_string(FixedMuEtMin)+"MaxEta"+MuEtaCut_str+"_EgLMaxEta"+EgEtaCut_str + "_EgsLMaxEta"+EgEtaCut_str + ")";
	TH1F* hist_11 = new TH1F("dR",histname, 50, 0, 5);
	
	TTreeReader fReader;  //!the tree reader
	TTreeReaderValue<UShort_t> nEGs = {fReader, "nEGs"};
	TTreeReaderArray<float> egEt    = {fReader, "egEt"};
	TTreeReaderArray<float> egEta   = {fReader, "egEta"};
	TTreeReaderArray<float> egPhi   = {fReader, "egPhi"};
	TTreeReaderArray<short> egIso   = {fReader, "egIso"};
	
	TTreeReaderValue<UShort_t> nMuons = {fReader, "nMuons"};
	TTreeReaderArray<float> muonEt    = {fReader, "muonEt"};
	TTreeReaderArray<float> muonEta   = {fReader, "muonEta"};
	TTreeReaderArray<float> muonPhi   = {fReader, "muonPhi"};
	TTreeReaderArray<unsigned short> muonIso   = {fReader, "muonIso"};
	TTreeReaderArray<unsigned short> muonQual   = {fReader, "muonQual"};
	
	ifstream insample(samplename+TString(".txt"));
	std::string line;
	while (std::getline(insample, line)){
		TString file_name(line);
		
		// Read input file and get the TTrees
		cout << "Processing " << file_name <<endl; cout.flush();
		TFile *infile = TFile::Open(file_name,"READ");
		assert(infile);
	
		TString treename = "l1UpgradeEmuTree";
		if(samplename.Contains("MC"))
			treename = "l1UpgradeTree";
		TDirectoryFile *df = (TDirectoryFile *)infile->Get(treename);
		TTree* eventTree = (TTree*)df->Get("L1UpgradeTree");
		assert(eventTree);
		
		fReader.SetTree(eventTree);
		
		int Evt2Process = eventTree->GetEntries();
		if(nEvents != -1)
			Evt2Process = nEvents;
		
		for(UInt_t ientry=0; ientry<Evt2Process; ientry++){
			fReader.SetLocalEntry(ientry);
			
			N_eg = *nEGs;
			N_mu = *nMuons;
			
			
			
			int MuSel_index = -99;
			std::vector<int> EgSel_index;
			
			for(UInt_t i=0; i<N_mu; i++){
				if(abs(muonEta[i]) > MuEtaCut || muonEt[i] < FixedMuEtMin) continue;
				if(muonQual[i] < 12) continue;
				MuSel_index = i;
				break;
			}
			if(MuSel_index < 0) continue;
			
			for(UInt_t i=0; i<N_eg; i++){
				if(abs(egEta[i]) > EgEtaCut || egEt[i] < 2) continue;
				EgSel_index.push_back(i);
			}
			if(EgSel_index.size() < 2) continue;
			
			float deta = egEta[EgSel_index[0]]-egEta[EgSel_index[1]];
      float dphi = TVector2::Phi_mpi_pi(egPhi[EgSel_index[0]]-egPhi[EgSel_index[1]]);
      float dR = TMath::Sqrt(deta*deta+dphi*dphi);
			
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
	xaxis->SetTitle("#Delta R (e/#gamma_{L}, e/#gamma_{sL})");
	xaxis->SetTitleOffset(1.2);
	hist_11->Draw("HIST");
	TString outName = "Histo_dR_MuMinEt"+std::to_string(FixedMuEtMin)+"MaxEta_"+MuEtaCut_str+"_EgLMaxEta"+EgEtaCut_str + "_EgsLMaxEta"+EgEtaCut_str+".png";
	c11->Print(outName);
	
	cout<<hist_11->GetEntries()<<endl;
		
	gBenchmark->Show("L1NtupleReader");
}