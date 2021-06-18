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

void Frequency_plot_13MC(const TString samplename="ZeroBias2018",
									const int nEvents = -1,
									const float MuEtaCut = 5){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	TString MuEtaCut_str = std::to_string(MuEtaCut).substr(0, std::to_string(MuEtaCut).find(".") + 2);
	
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
		
	int counter_0 = 0;
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
			
			for(int MuThr = 1; MuThr < 10; MuThr+=1){
					float MuEt_temp = 0.01;

				
				for(UInt_t i=0; i<N_mu; i++){
					if(abs(muonEta[i]) > MuEtaCut) continue;
					if(muonQual[i] < 12) continue;
					MuEt_temp = muonEt[i];
					break;
				}
				
				if(MuEt_temp >= MuThr)
					hist2d_1->Fill(0.0, MuThr);
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
	TString outName = "Histo_frequency_MuEta" + MuEtaCut_str;
	if(samplename.Contains("MC"))
		outName = "Histo_efficiency_MuEta" + MuEtaCut_str;
	gStyle->SetOptTitle(0);
	c11->Print(outName+".png");
	c11->Print(outName+".svg");
		
	gBenchmark->Show("L1NtupleReader");
}