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

void Frequency_plot_mu(const TString samplename="ZeroBias2018",
									const int nEvents = -1){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	// Local variables:
	UShort_t N_eg, N_mu;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
		
	TString histname = "SingleMu";
	TH2D* hist2d_1 = new TH2D("hist",histname,11,0,11,25,0,2.5);
	
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
	
	for(int MuEtThr = 2; MuEtThr < 11; MuEtThr+=1){
		for(double MuEtaThr = 1.5; MuEtaThr < 1.6 ; MuEtaThr+=0.1){
			int counter_0 = 0;
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
				
				for(int ientry=0; ientry<Evt2Process; ientry++){
					fReader.SetLocalEntry(ientry);
					counter_0++;
					
					N_eg = *nEGs;
					N_mu = *nMuons;

					bool isPass = false;
					
					for(UInt_t i=0; i<N_mu; i++){
						if(muonEt[i] < MuEtThr) continue;
						if(muonQual[i] < 12) continue;
						if(abs(muonEta[i]) > MuEtaThr+0.001) continue;
						isPass = true;
						break;
					}
					
					if(N_mu > 0)
						//cout<<"Evt: "<<ientry<<" # EGs: "<<N_mu<<" leading pT: "<<muonEt[EgSel_index[0]]<<" eta: "<<muonEta[EgSel_index[0]]<<" phi: "<<muonPhi[EgSel_index[0]]<<" current trig: "<<MuEtThr<<" is fired: "<<isFired<<endl;
							
					if(isPass)
						hist2d_1->Fill(MuEtThr+0.25, MuEtaThr+0.05);
					
				}//end of event loop
				infile->Close();
				df = NULL;
				eventTree = NULL;
			}//end of file loop
			cout<<counter_0<<endl;
		}
	}
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c11 = new TCanvas("","",1600,900);
	c11->cd();
	hist2d_1->SetLineWidth(2);
	yaxis = hist2d_1->GetYaxis();
	xaxis = hist2d_1->GetXaxis();
	xaxis->SetTitle("Threshold on p_{T #mu} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Threshold on |#eta|_{#mu}");
	hist2d_1->Draw("COLZ TEXT");
	TString outName = "Rate_singleMu.png";
	c11->Print(outName);
		
	gBenchmark->Show("L1NtupleReader");
}