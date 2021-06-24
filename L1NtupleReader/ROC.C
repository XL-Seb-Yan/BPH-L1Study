void ROC(){
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	/*
	// Eff, simple matching, dR = 0.4, trying to replicate the left figure Yuta showed in the workshop
	// SingleMu
	float efficiency_0[8] = {0.00109546,0.00140515,0.00162589,0.0017099,0.00174285,0.0017725,0.00179392,0.00179721};
	float rate_0[8] = {1098,1742,2674,4143,6396,11410,14097,14690};
	// e+mu, mu 4 GeV
	float efficiency_1[8] = {0.00009719,0.0001120,0.00014002,0.000168025,0.000210855,0.000235565,0.000316283,0.000368996};
	float rate_1[8] = {833,1090,1463,2235,3288,4840,7698,9613};
	//double-e, matching method 1 dR 0.4
	float efficiency_2[8] = {0.000441478,0.000662217,0.000999914,0.00167037,0.00270652,0.00414462,0.00805368,0.0123383};
	float rate_2[8] = {292,518,1007,2595,6859,17832,68670,164312};
	// e+mu, mu 4 GeV, eg eta 2.5
	float efficiency_3[8] = {0.000163083,0.000192735,0.000230623,0.000284984,0.000349229,0.000400295,0.000515607,0.000599619};
	float rate_3[8] = {1470,1963,2567,3991,5757,7269,9801,10870};
	// e+mu, mu 2 GeV, eta 1.0, eg eta 2.5
	float efficiency_4[8] = {0.000107075,0.000123548,0.000154847,0.000191087,0.000227328,0.00025698,0.000311341,0.000362407};
	float rate_4[8] = {1108,1508,1998,3027,4336,5428,7348,8158};
	// e+2mu, mu 2 GeV, eta 2.5, eg eta 1.0
	float efficiency_5[8] = {0.00000658922,0.0000115311,0.0000181204,0.0000312988,0.0000461246,0.0000708341,0.000115311,0.000158141};
	float rate_5[8] = {27,48,86,244,637,1630,5890,13408};
	*/

	
	
	// EffAcc, simple matching, dR = 0.4, trying to replicate the left figure Yuta showed in the workshop
	// SingleMu
	float efficiency_0[8] = {0.000069187,0.000080718,0.000102133,0.000105428,0.000105428,0.000105428,0.000105428,0.000107075};
	float rate_0[8] = {1098,1742,2674,4143,6396,11410,14097,14690};
	// e+mu, mu 4 GeV
	float efficiency_1[8] = {0.0000296515,0.0000345934,0.0000444772,0.000056008,0.0000658922,0.0000691868,0.000080718,0.0000856599};
	float rate_1[8] = {833,1090,1463,2235,3288,4840,7698,9613};
	//double-e, matching method 1 dR 0.4
	float efficiency_2[8] = {0.000406884,0.000616092,0.000943906,0.00156329,0.00249731,0.003773898,0.00697963,0.00994643};
	float rate_2[8] = {292,518,1007,2595,6859,17832,68670,164312};
	// e+mu, mu 4 GeV, eg eta 2.5
	float efficiency_3[8] = {0.0000411826,0.0000494192,0.0000625976,0.0000790707,0.0000906018,0.000097191,0.000105428,0.000105428};
	float rate_3[8] = {1470,1963,2567,3991,5757,7269,9801,10870};
	// e+mu, mu 2 GeV, eta 1.0, eg eta 2.5
	float efficiency_4[8] = {0.0000280042,0.0000329461,0.0000411826,0.0000510665,0.0000560084,0.0000609503,0.0000675395,0.0000675395};
	float rate_4[8] = {1108,1508,1998,3027,4336,5428,7348,8158};
	// e+2mu, mu 2 GeV, eta 2.5, eg eta 1.0
	float efficiency_5[8] = {0.00000658922,0.0000115311,0.0000181204,0.0000296515,0.0000395353,0.0000642449,0.000102133,0.000130137};
	float rate_5[8] = {27,48,86,244,637,1630,5890,13408};
	
	
	/*
	//double-e, matching method 1 dR 0.4
	float efficiency_0[8] = {0.000406884,0.000616092,0.000943906,0.00156329,0.00249731,0.003773898,0.00697963,0.00994643};
	float rate_0[8] = {292,518,1007,2595,6859,17832,68670,164312};
	//double-e, matching method 1 dR 0.2
	float efficiency_1[8] = {0.000398648,0.000597972,0.000904371,0.00146775,0.00225022,0.00313482,0.00442137,0.005133};
	float rate_1[8] = {292,518,1007,2595,6859,17832,68670,164312};
	//double-e, matching method 2 dR 0.4
	float efficiency_2[8] = {0.0000576557,0.000136726,0.000291573,0.000660569,0.00123713,0.00212173,0.00453338,0.00683796};
	float rate_2[8] = {292,518,1007,2595,6859,17832,68670,164312};
	//double-e, matching method 2 dR 0.2
	float efficiency_3[8] = {0.000314635,0.000495839,0.000787412,0.00131784,0.00205913,0.00289267,0.00410673,0.00477554};
	float rate_3[8] = {292,518,1007,2595,6859,17832,68670,164312};
	*/
	
	
	int ref = 1098;
	for(int i=0; i<8; i++){
		rate_0[i] = float(rate_0[i]) / ref;
		rate_1[i] = float(rate_1[i]) / ref;
		rate_2[i] = float(rate_2[i]) / ref;
		rate_3[i] = float(rate_3[i]) / ref;
		rate_4[i] = float(rate_4[i]) / ref;
		rate_5[i] = float(rate_5[i]) / ref;
	}
	
	TGraph *g0 = new TGraph(8,efficiency_0,rate_0);
	g0->SetLineColor(1);
	g0->SetLineWidth(2);
	g0->SetMarkerStyle(20);
	g0->SetMarkerColor(1);
	g0->SetMarkerSize(2);
	TGraph *g1 = new TGraph(8,efficiency_1,rate_1);
	g1->SetLineColor(2);
	g1->SetLineWidth(2);
	g1->SetMarkerStyle(22);
	g1->SetMarkerColor(2);
	g1->SetMarkerSize(2);
	TGraph *g2 = new TGraph(8,efficiency_2,rate_2);
	g2->SetLineColor(4);
	g2->SetLineWidth(2);
	g2->SetMarkerStyle(24);
	g2->SetMarkerColor(4);
	g2->SetMarkerSize(2);
	TGraph *g3 = new TGraph(8,efficiency_3,rate_3);
	g3->SetLineColor(8);
	g3->SetLineWidth(2);
	g3->SetMarkerStyle(26);
	g3->SetMarkerColor(8);
	g3->SetMarkerSize(2);
	TGraph *g4 = new TGraph(8,efficiency_4,rate_4);
	g4->SetLineColor(6);
	g4->SetLineWidth(2);
	g4->SetMarkerStyle(28);
	g4->SetMarkerColor(6);
	g4->SetMarkerSize(2);
	TGraph *g5 = new TGraph(8,efficiency_5,rate_5);
	g5->SetLineColor(11);
	g5->SetLineWidth(2);
	g5->SetMarkerStyle(21);
	g5->SetMarkerColor(11);
	g5->SetMarkerSize(2);
	
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	TAxis *yaxis = g0->GetYaxis();
	TAxis *xaxis = g0->GetXaxis();
	xaxis->SetTitle("Signal efficiency");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Rate (normalized to L1_SingleMu9er1p5)");
	xaxis->SetLimits(0.000001,0.1);
	yaxis->SetRangeUser(0.01,1000);
	c11->SetLogy();
	c11->SetLogx();
	c11->SetGrid();
	g0->Draw("APL");
	g1->Draw("PLSAME");
	g2->Draw("PLSAME");
	g3->Draw("PLSAME");
	g4->Draw("PLSAME");
	g5->Draw("PLSAME");
	gStyle->SetOptTitle(0);
	TLegend *leg = new TLegend(0.1,0.75,0.4,0.9);
	leg->AddEntry(g0, "SingleMuXer1p5","pl");
	leg->AddEntry(g1, "Mu4er1p5_EgXer1p0","pl");
	leg->AddEntry(g3, "Mu4er1p5_EgXer2p5","pl");
	leg->AddEntry(g4, "Mu2er1p0_EgXer2p5","pl");
	leg->AddEntry(g2, "EgXer1p0_EgXer1p0_dR1p0","pl");
	leg->AddEntry(g5, "Mu2er2p5_2EgXer1p0_dR1p0","pl");
	// leg->AddEntry(g0, "EgXer1p0_EgXer1p0_dR1p0, I, 0.4","pl");
	// leg->AddEntry(g1, "EgXer1p0_EgXer1p0_dR1p0, I, 0.2","pl");
	// leg->AddEntry(g2, "EgXer1p0_EgXer1p0_dR1p0, II, 0.4","pl");
	// leg->AddEntry(g3, "EgXer1p0_EgXer1p0_dR1p0, II, 0.2","pl");
	leg->Draw();
	c11->Print("ROC.png");
	
}