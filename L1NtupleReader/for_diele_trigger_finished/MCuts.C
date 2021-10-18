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

void MCuts(const TString samplename="ZeroBias2018",
                      const int nEvents = -1){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	int counter_0 = 0;
	int counter_1 = 0;
	int counter_2 = 0;
	int counter_3 = 0;
	int counter_4 = 0;
	int counter_5 = 0;
	
	TH1F* hist_1 = new TH1F("hist1","p_{T} K (low q^{2})",100,0,50);
	TH1F* hist_2 = new TH1F("hist2","dR(e,e) (low q^{2})",100,0,3);
	TH1F* hist_3 = new TH1F("hist3","min dR(e, K) (low q^{2})",100,0,3);
	TH1F* hist_11 = new TH1F("hist11","p_{T} K (high q^{2})",100,0,50);
	TH1F* hist_12 = new TH1F("hist12","dR(e,e) (high q^{2})",100,0,3);
	TH1F* hist_13 = new TH1F("hist13","min dR(e, K) (high q^{2})",100,0,3);
	
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
		
		TDirectoryFile *df_GEN = (TDirectoryFile *)infile->Get("genTree");
		TTree* eventTree_GEN = (TTree*)df_GEN->Get("L1GenTree");
		assert(eventTree_GEN);
		fReader_GEN.SetTree(eventTree_GEN);
		
		int N2process = eventTree_GEN->GetEntries();
		if(nEvents != -1) N2process = nEvents;
		
		for(UInt_t ientry=0; ientry<N2process; ientry++){
			fReader_GEN.SetLocalEntry(ientry);
			counter_0++;
			if(ientry % 10000 == 0) cout<<"Processing: "<<float(ientry) / N2process<<endl;
			int N_genpart = *nGenPart;
			
			//find GEN level electrons from B+ decay
			std::vector<TLorentzVector> gen_el;
			std::vector<TLorentzVector> gen_K;	
			for(UInt_t i=0; i<N_genpart; i++){
				if((abs(genPartId[i]) != 11 && abs(genPartId[i]) != 321) || genPartParent[i] != 521) continue;
				if(abs(genPartId[i]) == 11){
					TLorentzVector el;
					el.SetPtEtaPhiE(genPartPt[i],genPartEta[i],genPartPhi[i],genPartE[i]);
					gen_el.push_back(el);
				}
				else{
					TLorentzVector K;
					K.SetPtEtaPhiE(genPartPt[i],genPartEta[i],genPartPhi[i],genPartE[i]);
					gen_K.push_back(K);
				}
			}
			if(gen_el.size() < 2){
				// cout<<"Did not find 2 GEN level electrons!";
				continue;
			}
			if(gen_K.size() < 1){
				// cout<<"Did not find 1 GEN level K!";
				continue;
			}
			if(gen_K.size() > 1){
				// for(UInt_t i=0; i<N_genpart; i++){
					// if((abs(genPartId[i]) != 11 && abs(genPartId[i]) != 321) || abs(genPartParent[i]) != 521) continue;
					// cout<<"i"<<" "<<genPartId[i]<<" "<<genPartParent[i]<<" "<<genPartStat[i]<<" "<<genPartPt[i]<<" "<<genPartEta[i]<<endl;
				// }
				// cout<<"========================"<<endl;
				continue;
			}
			
			counter_1++;
			float q2 = (gen_el[0]+gen_el[1]).M();
			if(q2 > 1.05 && q2 < 2.45)
				counter_2++;
			if(q2 > 3.9 && q2 < 5)
				counter_3++;
			
			if(gen_el[0].Pt() < 7 || gen_el[1].Pt() < 7 || abs(gen_el[0].Eta()) > 1.1 || abs(gen_el[1].Eta()) > 1.1) continue;
			if(q2 > 1.05 && q2 < 2.45){
				counter_4++;
				hist_1->Fill(gen_K[0].Pt());
				hist_2->Fill(deltaR(gen_el[0].Eta(),gen_el[0].Phi(),gen_el[1].Eta(),gen_el[1].Phi()));
				hist_3->Fill(TMath::Min(deltaR(gen_el[0].Eta(),gen_el[0].Phi(),gen_K[0].Eta(),gen_K[0].Phi()),deltaR(gen_el[1].Eta(),gen_el[1].Phi(),gen_K[0].Eta(),gen_K[0].Phi())));
			}
			if(q2 > 3.9 && q2 < 5){
				counter_5++;
				hist_11->Fill(gen_K[0].Pt());
				hist_12->Fill(deltaR(gen_el[0].Eta(),gen_el[0].Phi(),gen_el[1].Eta(),gen_el[1].Phi()));
				hist_13->Fill(TMath::Min(deltaR(gen_el[0].Eta(),gen_el[0].Phi(),gen_K[0].Eta(),gen_K[0].Phi()),deltaR(gen_el[1].Eta(),gen_el[1].Phi(),gen_K[0].Eta(),gen_K[0].Phi())));
			}
			
		}
		infile->Close();
	}//end of file loop	hist2d_1->Scale(1.0 / globcounter_0);
	
	TAxis* yaxis = NULL;
	TAxis* xaxis = NULL;
	
	TCanvas *c0 = new TCanvas("","",1200,900);
	c0->cd();
	hist_1->SetLineWidth(2);
	yaxis = hist_1->GetYaxis();
	xaxis = hist_1->GetXaxis();
	xaxis->SetTitle("K p_{T} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_1->Draw("HIST");
	hist_11->Draw("SAME");
	gStyle->SetOptTitle(0);
	c0->Print("K_pt.png");
	
  TCanvas *c1 = new TCanvas("","",1200,900);
	c1->cd();
	hist_2->SetLineWidth(2);
	yaxis = hist_2->GetYaxis();
	xaxis = hist_2->GetXaxis();
	xaxis->SetTitle("dR(e,e)");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_2->Draw("HIST");
	hist_12->Draw("SAME");
	gStyle->SetOptTitle(0);
	c1->Print("dRee.png");
	
	TCanvas *c2 = new TCanvas("","",1200,900);
	c2->cd();
	hist_3->SetLineWidth(2);
	yaxis = hist_3->GetYaxis();
	xaxis = hist_3->GetXaxis();
	xaxis->SetTitle("min dR(e,K)");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_3->Draw("HIST");
	hist_13->Draw("SAME");
	gStyle->SetOptTitle(0);
	c2->Print("mindReK.png");
	
	cout<<counter_0<<" "<<counter_1<<" "<<counter_2<<" "<<counter_3<<" "<<counter_4<<" "<<counter_5<<endl;
	cout<<float(counter_4)/counter_2<<endl;
	cout<<float(counter_5)/counter_3<<endl;
		
	gBenchmark->Show("L1NtupleReader");
}