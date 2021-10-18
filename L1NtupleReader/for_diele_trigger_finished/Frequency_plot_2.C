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

void Frequency_plot_2(const TString samplename="ZeroBias2018",
									const int nEvents = -1,
									const float MuEtaCut = 5,
									const float EgEtaCut = 5){
	
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
	// TH1F* hist_11 = new TH1F("NEg","Number of e/#gamma", 40, 0, 20);
	// TH1F* hist_12 = new TH1F("EgEt","e/#gamma E_{T}", 100, 0, 10);
	// TH1F* hist_13 = new TH1F("EgEta","e/#gamma #eta", 100, -5, 5);
	// TH1F* hist_14 = new TH1F("EgPhi","e/#gamma #phi", 100, -3.15, 3.15);
	// TH1F* hist_21 = new TH1F("NMu","Number of #mu", 20, 0, 10);
	// TH1F* hist_22 = new TH1F("MuEt","#mu E_{T}", 100, 0, 100);
	// TH1F* hist_23 = new TH1F("MuEta","#mu #eta", 100, -5, 5);
	// TH1F* hist_24 = new TH1F("MuPhi","#mu #phi", 100, -3.15, 3.15);
	
	TString histname = "Mu_Eta" + MuEtaCut_str + "_El_Eta" + EgEtaCut_str;
	TH2F* hist2d_1 = new TH2F("hist",histname,10,0,10,9,1,10);
	
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
	
	for(int MuThr = 2; MuThr < 3; MuThr+=1){
		for(int EgThr = 0; EgThr < 10; EgThr+=1){
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

					bool isMuPass = false;
					bool isEgPass = false;

					for(UInt_t i=0; i<N_eg; i++){
						if(abs(egEta[i]) > EgEtaCut || egEt[i] < EgThr) continue;
						isEgPass = true;
						break;
					}
					
					for(UInt_t i=0; i<N_mu; i++){
						if(abs(muonEta[i]) > MuEtaCut || muonEt[i] < MuThr) continue;
						if(muonQual[i] < 12) continue;
						isMuPass = true;
						break;
					}
					
					if(isEgPass && isMuPass)
						hist2d_1->Fill(EgThr+0.5, MuThr+0.5);
					
				}//end of event loop
				infile->Close();
			}//end of file loop
		}
	}
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	hist2d_1->SetLineWidth(2);
	yaxis = hist2d_1->GetYaxis();
	xaxis = hist2d_1->GetXaxis();
	xaxis->SetTitle("Threshold on E_{T e/#gamma} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Threshold on p_{T #mu} [GeV]");
	hist2d_1->Draw("COLZ TEXT");
	TString outName = "Rate_mu_Eta" + MuEtaCut_str + "_el_Eta" + EgEtaCut_str;
	gStyle->SetOptTitle(0);
	c11->Print(outName+".png");
	c11->Print(outName+".svg");
		
	gBenchmark->Show("L1NtupleReader");
}