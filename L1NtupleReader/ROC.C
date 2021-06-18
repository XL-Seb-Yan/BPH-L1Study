void ROC(){
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
	
	float efficiency_0[8] = {0.002301,0.003049,0.005063,0.007077,0.009608,0.014672,0.017433,0.017951};
	float rate_0[8] = {1,1.58,2.42,3.76,5.82,10.38,12.84,13.38};
	float efficiency_1[8] = {0.0002301,0.0004603,0.0005754,0.0008631,0.001323,0.002014,0.003509,0.004948};
	float rate_1[8] = {0.4308,0.5710,0.7778,1.1658,1.6338,2.3615,3.6402,4.4426};
	float efficiency_2[8] = {0.00069,0.00092,0.001496,0.00276,0.00483,0.00984,0.0347,0.0883};
	float rate_2[8] = {0.2659,0.4718,0.9171,2.3634,6.2468,16.2404,62.541,149.647};
	
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
	
	
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	TAxis *yaxis = g0->GetYaxis();
	TAxis *xaxis = g0->GetXaxis();
	xaxis->SetTitle("Signal efficiency");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Rate (normalized to L1_SingleMu9er1p5)");
	xaxis->SetLimits(0.0001,1);
	yaxis->SetRangeUser(0.01,1000);
	c11->SetLogy();
	c11->SetLogx();
	c11->SetGrid();
	g0->Draw("APL");
	g1->Draw("PLSAME");
	g2->Draw("PLSAME");
	gStyle->SetOptTitle(0);
	TLegend *leg = new TLegend(0.6,0.75,0.9,0.9);
	leg->AddEntry(g0, "SingleMuXer1p5","pl");
	leg->AddEntry(g1, "Mu4er1p0EgXer1p1","pl");
	leg->AddEntry(g2, "EgXer1p0EgXer1p0dR1p0","pl");
	leg->Draw();
	c11->Print("ROC.png");
	
}