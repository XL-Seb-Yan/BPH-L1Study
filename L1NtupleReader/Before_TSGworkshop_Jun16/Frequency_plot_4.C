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

void Frequency_plot_4(const TString samplename="ZeroBias2018",
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
			
			float MuEt_temp = 0;
			float EglEt_temp = 0;
			
			int MuSel_index = -99;
			int EgSel_index = -99;
			
			for(UInt_t i=0; i<N_mu; i++){
				if(abs(muonEta[i]) > MuEtaCut || muonEt[i] < FixedMuEtMin) continue;
				if(muonQual[i] < 12) continue;
				MuSel_index = i;
				break;
			}
			if(MuSel_index < 0) continue;
			
			for(UInt_t i=0; i<N_eg; i++){
				if(abs(egEta[i]) > EgEtaCut) continue;
				EgSel_index = i;
				break;
			}
			if(EgSel_index < 0) continue;
			
			float deta = muonEta[MuSel_index]-egEta[EgSel_index];
      float dphi = TVector2::Phi_mpi_pi(muonPhi[MuSel_index]-egPhi[EgSel_index]);
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
	xaxis->SetTitle("#Delta R (#mu_{L}, e/#gamma_{L})");
	xaxis->SetTitleOffset(1.2);
	hist_11->Draw("HIST");
	TString outName = "Histo_dR_MuMinEt"+std::to_string(FixedMuEtMin)+"MaxEta_"+MuEtaCut_str+"_EgMaxEta"+EgEtaCut_str+".png";
	c11->Print(outName);
	
	cout<<hist_11->GetEntries()<<endl;
	
	gBenchmark->Show("L1NtupleReader");
}