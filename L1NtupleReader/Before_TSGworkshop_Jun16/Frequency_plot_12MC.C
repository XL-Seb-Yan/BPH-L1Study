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

void Frequency_plot_12MC(const TString samplename="KeeMC",
											const int nEvents = -1,
											const float EgEtaCut = 2.5){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	TString EgEtaCut_str = std::to_string(EgEtaCut).substr(0, std::to_string(EgEtaCut).find(".") + 2);
	
	
	// Local variables:
	UShort_t N_eg, N_mu, N_genpart;
	float Et_eg, Eta_eg, Phi_eg, Iso_eg;
	float Et_muon, Eta_muon, Phi_muon, Iso_muon;
	
	// Histograms
	TString histname = "";
	TH1F* hist_11 = new TH1F("dR",histname, 50, 0, 5);
	TH1F* hist_12 = new TH1F("mass",histname, 50, 0, 20);
	TH1F* hist_13 = new TH1F("massoverdR",histname, 50, 0, 10);
	
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
		cout<<df_L1<<endl;
		TTree* eventTree_L1 = (TTree*)df_L1->Get("L1UpgradeTree");
		cout<<eventTree_L1<<endl;
		TDirectoryFile *df_GEN = (TDirectoryFile *)infile->Get("genTree");
		cout<<df_GEN<<endl;
		TTree* eventTree_GEN = (TTree*)df_GEN->Get("L1GenTree");
		cout<<eventTree_GEN<<endl;
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
			
			int MuSel_index = -99;
			std::vector<int> EgSel_index;
			
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
			
			for(UInt_t i=0; i<N_eg; i++){
				if(abs(egEta[i]) > EgEtaCut) continue;
				//if(deltaR(egEta[i], egPhi[i], gen_el[0].Eta(), gen_el[0].Phi()) > 0.4 && deltaR(egEta[i], egPhi[i], gen_el[1].Eta(), gen_el[1].Phi()) > 0.4) continue;
				EgSel_index.push_back(i);
			}
			if(EgSel_index.size() < 2) continue;
			
			for(int i=0; i<EgSel_index.size()-1; i++){
				for(int j=i+1; j<EgSel_index.size(); j++){
					float dR = deltaR(egEta[EgSel_index[i]],egPhi[EgSel_index[i]],egEta[EgSel_index[j]],egPhi[EgSel_index[j]]);
					TLorentzVector eg1, eg2;
					eg1.SetPtEtaPhiM(egEt[EgSel_index[i]],egEta[EgSel_index[i]],egPhi[EgSel_index[i]],0.0005110);
					eg2.SetPtEtaPhiM(egEt[EgSel_index[j]],egEta[EgSel_index[j]],egPhi[EgSel_index[j]],0.0005110);
					float invmass = (eg1+eg2).M();
					hist_11->Fill(dR);
					hist_12->Fill(invmass);
					hist_13->Fill(invmass / dR);
				}
			}
			
		}//end of event loop
		infile->Close();
	}//end of file loop
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;	
	
	hist_11->Scale(1.0/hist_11->GetEntries());
	hist_12->Scale(1.0/hist_12->GetEntries());
	hist_13->Scale(1.0/hist_13->GetEntries());
	hist_11->SetLineWidth(4);
	hist_12->SetLineWidth(4);
	hist_13->SetLineWidth(4);
	hist_11->SetLineColor(4);
	hist_12->SetLineColor(4);
	hist_13->SetLineColor(4);
	
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	yaxis = hist_11->GetYaxis();
	xaxis = hist_11->GetXaxis();
	yaxis->SetTitle("Entries / 0.1");
	yaxis->SetRangeUser(0.001,1);
	xaxis->SetTitle("#Delta R (e/#gamma_{L}, e/#gamma_{sL})");
	xaxis->SetTitleOffset(1.2);
	hist_11->Draw("HIST");
	c11->SetLogy();
	TString outName = "Histo_dR_EgLMaxEta"+EgEtaCut_str + "_EgsLMaxEta"+EgEtaCut_str+"_Kee.png";
	c11->Print(outName);
	
	TCanvas *c12 = new TCanvas("","",1200,900);
	c12->cd();
	yaxis = hist_12->GetYaxis();
	xaxis = hist_12->GetXaxis();
	yaxis->SetTitle("Entries / 0.4 GeV");
	yaxis->SetRangeUser(0.001,1);
	xaxis->SetTitle("M (e/#gamma_{L}, e/#gamma_{sL}) [GeV]");
	xaxis->SetTitleOffset(1.2);
	hist_12->Draw("HIST");
	c12->SetLogy();
	outName = "Histo_mass_EgLMaxEta"+EgEtaCut_str + "_EgsLMaxEta"+EgEtaCut_str+"_Kee.png";
	c12->Print(outName);
	
	TCanvas *c13 = new TCanvas("","",1200,900);
	c13->cd();
	yaxis = hist_13->GetYaxis();
	xaxis = hist_13->GetXaxis();
	yaxis->SetTitle("Entries / 0.2 GeV");
	yaxis->SetRangeUser(0.001,1);
	xaxis->SetTitle("M / dR (e/#gamma_{L}, e/#gamma_{sL}) [GeV]");
	xaxis->SetTitleOffset(1.2);
	hist_13->Draw("HIST");
	c13->SetLogy();
	outName = "Histo_massoverdR_EgLMaxEta"+EgEtaCut_str + "_EgsLMaxEta"+EgEtaCut_str+"_Kee.png";
	c13->Print(outName);
	
	cout<<hist_11->GetEntries()<<endl;
		
	gBenchmark->Show("L1NtupleReader");
}