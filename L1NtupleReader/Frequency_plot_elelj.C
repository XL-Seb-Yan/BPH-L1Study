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

void Frequency_plot_elelj(const TString samplename="ZeroBias2018",
                      const int nEvents = -1,
                      const float EgEtCut = 1.0,
											const float EgEtaCut = 1.0,
											const float EgEgdRCut = 1.0){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	TString EgEtaCut_str = std::to_string(EgEtaCut).substr(0, std::to_string(EgEtaCut).find(".") + 2);
	TString EgEgdRCut_str = std::to_string(EgEgdRCut).substr(0, std::to_string(EgEgdRCut).find(".") + 2);
  
  gBenchmark->Start("L1NtupleReader");
  gROOT->SetBatch(1);
  gStyle->SetOptStat(0);
  //gStyle->SetOptTitle(0);
  gStyle->SetTitleSize(0.035,"XYZ");
  gStyle->SetLabelSize(0.035,"XYZ");
  gStyle->SetFrameLineWidth(2);
  gStyle->SetLegendTextSize(0.03);
  gStyle->SetBarWidth(2);
  gStyle->SetHistLineWidth(2);
  
  // Local variables:
  UShort_t N_eg, N_mu, N_jet;
  float Et_eg, Eta_eg, Phi_eg, Iso_eg;
  float Et_muon, Eta_muon, Phi_muon, Iso_muon;
  
  // Histograms
  TString histname = "Rate (Double_el_eta"+EgEtaCut_str+"_dR"+EgEgdRCut_str+"jet)";
  TH2F* hist2d_1 = new TH2F("EgEtl_EgEts",histname,10,1,11,10,1,11);
  TH1F* hist_1 = new TH1F("jet_pt","pT jet",150,0,150);
  TH1F* hist_2 = new TH1F("jet_eta","eta jet",100,-3,3);
  TH1F* hist_3 = new TH1F("jet_dR_min","min dR(e,j)",100,0,3);
  
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
  
  TTreeReaderValue<UShort_t> nJets = {fReader, "nJets"};
  TTreeReaderArray<float> jetEt    = {fReader, "jetEt"};
  TTreeReaderArray<float> jetEta   = {fReader, "jetEta"};
  TTreeReaderArray<float> jetPhi   = {fReader, "jetPhi"};
  
  for(int JetEtThr = 0; JetEtThr < 1; JetEtThr++){
    int counter_0 = 0;
    int counter_1 = 0;
    int counter_2 = 0;
    ifstream insample(samplename+TString(".txt"));
    std::string line;
    cout<<"Running through: "<<JetEtThr<<endl;
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
        if(ientry % 10000 == 0) cout<<"Processing: "<<float(ientry) / Evt2Process<<endl;
        counter_0++;
        
        N_eg = *nEGs;
        N_mu = *nMuons;
        N_jet = *nJets;
        
        std::vector<int> EgSel_index;
        for(UInt_t i=0; i<N_eg; i++){
          if(abs(egEta[i]) > EgEtaCut || egEt[i] < EgEtCut) continue;
          EgSel_index.push_back(i);
        }
        if(EgSel_index.size() < 2) continue;
        counter_1++;
        
        bool isElPass = false;
        int eg0_fire_index = -1;
        int eg1_fire_index = -1;
        for(int i=0; i<EgSel_index.size()-1; i++){
          for(int j=i+1; j<EgSel_index.size(); j++){
            float dR = deltaR(egEta[EgSel_index[i]],egPhi[EgSel_index[i]],egEta[EgSel_index[j]],egPhi[EgSel_index[j]]);
            if(dR < EgEgdRCut){
              isElPass = true;
              eg0_fire_index = i;
              eg1_fire_index = j;
              break;
              }
            // TLorentzVector eg1, eg2;
            // eg1.SetPtEtaPhiM(egEt[EgSel_index[i]],egEta[EgSel_index[i]],egPhi[EgSel_index[i]],0.0005110);
            // eg2.SetPtEtaPhiM(egEt[EgSel_index[j]],egEta[EgSel_index[j]],egPhi[EgSel_index[j]],0.0005110);
            // float invmass = (eg1+eg2).M();
            // hist_11->Fill(dR);
            // hist_12->Fill(invmass);
            // hist_13->Fill(invmass / dR);
            
          }
        }
        
        if(isElPass){
          counter_2++;
          if(N_jet == 0){
            hist_1->Fill(0);
            hist_2->Fill(-99);
            hist_3->Fill(-99);
          }
          else{
            hist_1->Fill(jetEt[0]);
            hist_2->Fill(jetEta[0]);
            hist_3->Fill(TMath::Min(deltaR(jetEta[0],jetPhi[0],egEta[eg0_fire_index],egPhi[eg0_fire_index]),deltaR(jetEta[0],jetPhi[0],egEta[eg1_fire_index],egPhi[eg1_fire_index])));
          }
        }
          
          //hist2d_1->Fill(EgEtThr+0.5,EgEtThr+0.5);
        
      }//end of event loop
      infile->Close();
    }//end of file loop
    cout<<counter_0<<" "<<counter_1<<" "<<counter_2<<endl;
  }
  
  TAxis* yaxis = NULL;
  TAxis* xaxis = NULL;
  
  TCanvas *c11 = new TCanvas("","",1200,900);
  c11->cd();
  hist2d_1->SetLineWidth(2);
  yaxis = hist2d_1->GetYaxis();
  xaxis = hist2d_1->GetXaxis();
  yaxis->SetTitle("Threshold on p_{T e/#gamma sL} [GeV]");
  xaxis->SetTitle("Threshold on p_{T e/#gamma L} [GeV]");
  xaxis->SetTitleOffset(1.2);
  hist2d_1->Draw("COLZ TEXT");
  TString outName = "Rate_eleljet_Eta"+EgEtaCut_str+"_dR"+EgEgdRCut_str;
  c11->Print(outName+".png");
  c11->Print(outName+".svg");
  
  TCanvas *c0 = new TCanvas("","",1200,900);
	c0->cd();
	hist_1->SetLineWidth(2);
	yaxis = hist_1->GetYaxis();
	xaxis = hist_1->GetXaxis();
	xaxis->SetTitle("jet p_{T} [GeV]");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_1->Draw("HIST");
	gStyle->SetOptTitle(0);
	c0->Print("jet_pt.png");
	
  TCanvas *c1 = new TCanvas("","",1200,900);
	c1->cd();
	hist_2->SetLineWidth(2);
	yaxis = hist_2->GetYaxis();
	xaxis = hist_2->GetXaxis();
	xaxis->SetTitle("jet eta");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_2->Draw("HIST");
	gStyle->SetOptTitle(0);
	c1->Print("jet eta.png");
	
	TCanvas *c2 = new TCanvas("","",1200,900);
	c2->cd();
	hist_3->SetLineWidth(2);
	yaxis = hist_3->GetYaxis();
	xaxis = hist_3->GetXaxis();
	xaxis->SetTitle("min dR(e,j)");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Entries");
	hist_3->Draw("HIST");
	gStyle->SetOptTitle(0);
	c2->Print("mindRej.png");
    
  gBenchmark->Show("L1NtupleReader");
}