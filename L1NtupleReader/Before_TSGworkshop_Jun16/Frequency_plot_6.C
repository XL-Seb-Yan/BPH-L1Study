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

void Frequency_plot_6(const TString samplename="ZeroBias2018",
									const int nEvents = -1){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	//gStyle->SetOptStat(0);
	
	// Local variables:
	UShort_t N_eg, N_mu;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
	
	int count0 = 0, count1 = 0;
	
	// Histograms
	TH1F* hist_22 = new TH1F("sLMuPt","#mu_{sL} p_{T}", 40, 0, 20);
	
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
			
			std::vector<int> MuSel_index;
					
			for(UInt_t i=0; i<N_mu; i++){
				if(abs(muonEta[i]) > 1.5 || muonEt[i] < 9) continue;
				if(muonQual[i] < 12) continue;
				MuSel_index.push_back(i);
			}
			if(MuSel_index.size() > 0){
				count0++;
				std::vector<int> sLMuSel_index;
				for(UInt_t i=0; i<N_mu; i++){
					if(muonQual[i] < 12) continue;
					sLMuSel_index.push_back(i);
				}
				for(int idx = 0; idx < sLMuSel_index.size(); idx++){
					if(sLMuSel_index[idx] == MuSel_index[0]) continue;
					hist_22->Fill(muonEt[sLMuSel_index[idx]]);
					count1++;
					break;
				}
			}
		}//end of event loop
		infile->Close();
	}//end of file loop
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	yaxis = hist_22->GetYaxis();
	xaxis = hist_22->GetXaxis();
	xaxis->SetTitle("p_{T #mu(sL)} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries / 0.5 GeV");
	hist_22->Draw("HIST");
	TString outName = "Histo_singleMu9_1p5_sLMu_pt.png";
	c11->Print(outName);
	
	cout<<count0<<" "<<count1<<endl;
		
	gBenchmark->Show("L1NtupleReader");
}