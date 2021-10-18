void ROC(){
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	
	
	//Mu
	float efficiency_0[9] = {3.078e-5,3.645e-5,4.536e-5,5.104e-5,5.225e-5,5.347e-5,5.387e-5,5.428e-5,5.428e-5};
	float rate_0[9] = {14220,17630,27741,42098,64438,98886,168825,211737,221066};
	//ElEl
	float efficiency_1[9] = {0.0003459,0.0004593,0.0006663,0.001006,0.001563,0.0022489,0.003035,0.004156,0.004698};
	float rate_1[9] = {1976,2884,5318,10047,24733,63156,164300,672755,1746580};
	//ElElJet
	// float efficiency_2[9] = {0.000197677,0.000252038,0.000380528,0.00059303,0.000940611,0.00142492,0.00188452,0.00248743,0.00282019};
	// float rate_2[9] = {92,132,249,458,1157,3297,8475,33727,85572};
	
	
	
	int ref = 17630;
	for(int i=0; i<9; i++){
		rate_0[i] = float(rate_0[i]) / ref;
		rate_1[i] = float(rate_1[i]) / ref;
		// rate_2[i] = float(rate_2[i]) / ref;
	}

	TGraph *g0 = new TGraph(9,efficiency_0,rate_0);
	g0->SetLineColor(1);
	g0->SetLineWidth(2);
	g0->SetLineStyle(1);
	g0->SetMarkerStyle(20);
	g0->SetMarkerColor(1);
	g0->SetMarkerSize(2);
	TGraph *g1 = new TGraph(9,efficiency_1,rate_1);
	g1->SetLineColor(4);
	g1->SetLineWidth(2);
	g1->SetLineStyle(2);
	g1->SetMarkerStyle(21);
	g1->SetMarkerColor(4);
	g1->SetMarkerSize(2);
	// TGraph *g2 = new TGraph(9,efficiency_2,rate_2);
	// g2->SetLineColor(2);
	// g2->SetLineWidth(2);
	// g2->SetLineStyle(9);
	// g2->SetMarkerStyle(22);
	// g2->SetMarkerColor(2);
	// g2->SetMarkerSize(2);
	
	
	TCanvas *c11 = new TCanvas("","",2400,1800);
	c11->cd();
	TAxis *yaxis = g0->GetYaxis();
	TAxis *xaxis = g0->GetXaxis();
	xaxis->SetTitle("Trigger efficiency * signal acceptance");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Rate (normalized to L1_SingleMu9er1p5)");
	xaxis->SetLimits(0.000001,0.1);
	yaxis->SetRangeUser(0.05,50);
	c11->SetLogy();
	c11->SetLogx();
	c11->SetGrid();
	g0->Draw("APL");
	g1->Draw("PLSAME");
	gStyle->SetOptTitle(0);
	TLegend *leg = new TLegend(0.1,0.7,0.3,0.9);
	leg->AddEntry(g0, "SingleMuXer1p5","pl");
	leg->AddEntry(g1, "DoubleElXer1p2dR0p7","pl");
	leg->Draw();
	c11->Print("ROCZOOM.png");
	c11->Print("ROCZOOM.pdf");
}