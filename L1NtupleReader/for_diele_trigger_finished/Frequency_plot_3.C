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

void Frequency_plot_3(const TString samplename="ZeroBias2018",
                      const int nEvents = -1,
											const float EgEtaCut = 1.0,
											const float dRCut = 1.0){
	
	gBenchmark->Start("L1NtupleReader");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	TString EgEtaCut_str = std::to_string(EgEtaCut).substr(0, std::to_string(EgEtaCut).find(".") + 2);
	TString dRCut_str = std::to_string(dRCut).substr(0, std::to_string(dRCut).find(".") + 2);
  
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
  UShort_t N_eg, N_mu;
  float Et_eg, Eta_eg, Phi_eg, Iso_eg;
  float Et_muon, Eta_muon, Phi_muon, Iso_muon;
  
  // Histograms
  TString histname = "Rate (Double_el_eta"+EgEtaCut_str+"_dR1.0)";
  TH2F* hist2d_1 = new TH2F("EgEtl_EgEts",histname,10,1,11,9,10,11);
  
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
  
  for(int EgEtThr = 0; EgEtThr < 11; EgEtThr++){
    int counter_0 = 0;
    ifstream insample(samplename+TString(".txt"));
    std::string line;
    cout<<"Running through: "<<EgEtThr<<endl;
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
        counter_0++;
        
        N_eg = *nEGs;
        N_mu = *nMuons;
        
        std::vector<int> EgSel_index;
        
        for(UInt_t i=0; i<N_eg; i++){
          if(abs(egEta[i]) > EgEtaCut || egEt[i] < EgEtThr) continue;
          EgSel_index.push_back(i);
        }
        if(EgSel_index.size() < 2) continue;
        
        bool isFired = false;
        for(int i=0; i<EgSel_index.size()-1; i++){
          for(int j=i+1; j<EgSel_index.size(); j++){
            float dR = deltaR(egEta[EgSel_index[i]],egPhi[EgSel_index[i]],egEta[EgSel_index[j]],egPhi[EgSel_index[j]]);
            if(dR > dRCut) continue;
            // TLorentzVector eg1, eg2;
            // eg1.SetPtEtaPhiM(egEt[EgSel_index[i]],egEta[EgSel_index[i]],egPhi[EgSel_index[i]],0.0005110);
            // eg2.SetPtEtaPhiM(egEt[EgSel_index[j]],egEta[EgSel_index[j]],egPhi[EgSel_index[j]],0.0005110);
            // float invmass = (eg1+eg2).M();
            // hist_11->Fill(dR);
            // hist_12->Fill(invmass);
            // hist_13->Fill(invmass / dR);
            isFired = true;
          }
        }
        
        if(isFired)
          hist2d_1->Fill(EgEtThr+0.5,EgEtThr+0.5);
        
      }//end of event loop
      infile->Close();
    }//end of file loop
    cout<<counter_0<<endl;
  }
  
  TAxis* yaxis = NULL;
  TAxis* xaxis = NULL;
  
  TCanvas *c11 = new TCanvas("","",1200,900);
  c11->cd();
  hist2d_1->SetLineWidth(2);
  yaxis = hist2d_1->GetYaxis();
  xaxis = hist2d_1->GetXaxis();
  yaxis->SetTitle("Threshold on E_{T e/#gamma sL} [GeV]");
  xaxis->SetTitle("Threshold on E_{T e/#gamma L} [GeV]");
  xaxis->SetTitleOffset(1.2);
  hist2d_1->Draw("COLZ TEXT");
  TString outName = "Rate__double_el_Eta"+EgEtaCut_str+"_dR"+dRCut_str;
  c11->Print(outName+".png");
  c11->Print(outName+".svg");
    
  gBenchmark->Show("L1NtupleReader");
}