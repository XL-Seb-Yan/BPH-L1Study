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

void Frequency_plot_0(const TString samplename="ZeroBias2018",
									const int nEvents = -1){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	// Local variables:
	UShort_t N_eg, N_mu;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
	
	// Histograms
	TH1F* hist_11 = new TH1F("NEg","Number of e/#gamma", 40, 0, 20);
	TH1F* hist_12 = new TH1F("EgEt","e/#gamma E_{T}", 100, 0, 20);
	TH1F* hist_13 = new TH1F("EgEta","e/#gamma #eta", 100, -3, 3);
	TH1F* hist_14 = new TH1F("EgPhi","e/#gamma #phi", 100, -3.15, 3.15);
	TH1F* hist_21 = new TH1F("NMu","Number of #mu", 20, 0, 10);
	TH1F* hist_22 = new TH1F("MuEt","#mu E_{T}", 100, 0, 20);
	TH1F* hist_23 = new TH1F("MuEta","#mu #eta", 100, -3, 3);
	TH1F* hist_24 = new TH1F("MuPhi","#mu #phi", 100, -3.15, 3.15);
	
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

	int counter0 = 0;
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
			counter0++;
			
			N_eg = *nEGs;
			N_mu = *nMuons;
			
			hist_11->Fill(N_eg);
			for(UInt_t i=0; i<N_eg; i++){
				hist_12->Fill(egEt[i]);
				hist_13->Fill(egEta[i]);
				hist_14->Fill(egPhi[i]);
				break;
			}
			hist_21->Fill(N_mu);
			for(UInt_t i=0; i<N_mu; i++){
				hist_22->Fill(muonEt[i]);
				hist_23->Fill(muonEta[i]);
				hist_24->Fill(muonPhi[i]);
				break;
			}
		}//end of event loop
		infile->Close();
	}//end of file loop
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	yaxis = hist_11->GetYaxis();
	xaxis = hist_11->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("Number of e/#gamma");
	hist_11->Draw();
	c11->SetLogy();
	c11->Print("N_eg.png");
	
	TCanvas *c12 = new TCanvas("","",1200,900);
	c12->cd();
	yaxis = hist_12->GetYaxis();
	xaxis = hist_12->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("E_{T e/#gamma}");
	hist_12->Draw();
	c12->SetLogy();
	c12->Print("Et_eg.png");
	
	TCanvas *c13 = new TCanvas("","",1200,900);
	c13->cd();
	yaxis = hist_13->GetYaxis();
	xaxis = hist_13->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("#eta_{e/#gamma}");
	hist_13->Draw();
	c13->SetLogy();
	c13->Print("Eta_eg.png");
	
	TCanvas *c14 = new TCanvas("","",1200,900);
	c14->cd();
	yaxis = hist_14->GetYaxis();
	xaxis = hist_14->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("#phi_{e/#gamma}");
	hist_14->Draw();
	c14->SetLogy();
	c14->Print("Phi_eg.png");
	
	TCanvas *c21 = new TCanvas("","",1200,900);
	c21->cd();
	yaxis = hist_21->GetYaxis();
	xaxis = hist_21->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("Number of muon");
	hist_21->Draw();
	c21->SetLogy();
	c21->Print("N_mu.png");
	
	TCanvas *c22 = new TCanvas("","",1200,900);
	c22->cd();
	yaxis = hist_22->GetYaxis();
	xaxis = hist_22->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("E_{T #mu}");
	hist_22->Draw();
	c22->SetLogy();
	c22->Print("Et_mu.png");
	
	TCanvas *c23 = new TCanvas("","",1200,900);
	c23->cd();
	yaxis = hist_23->GetYaxis();
	xaxis = hist_23->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("#eta_{#mu}");
	hist_23->Draw();
	c23->SetLogy();
	c23->Print("Eta_mu.png");
	
	TCanvas *c24 = new TCanvas("","",1200,900);
	c24->cd();
	yaxis = hist_24->GetYaxis();
	xaxis = hist_24->GetXaxis();
	yaxis->SetRangeUser(0.5,100000000);
	yaxis->SetTitle("Entries");
	xaxis->SetTitle("#phi_{#mu}");
	hist_24->Draw();
	c24->SetLogy();
	c24->Print("Phi_mu.png");
	
	cout<<counter0<<endl;
		
	gBenchmark->Show("L1NtupleReader");
}