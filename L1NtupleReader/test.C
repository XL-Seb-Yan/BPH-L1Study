void test(){
	TH2D* hist2d_1 = new TH2D("hist","hist",10,0,10,30,0,3);
	double eta[20] = {1.3,1.4,1.5,1.6,1.6,1.7,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.5,2.6,2.7,2.8,2.9}; //simple data to fill
	for(int i=0; i<20; i++){			
		for(int MuEtThr = 0; MuEtThr < 1; MuEtThr+=1){ //just some operations
			for(double MuEtaThr = 1.5; MuEtaThr < 3; MuEtaThr+=0.1){
			if(abs(eta[i]) > MuEtaThr) continue;
			hist2d_1->Fill(MuEtThr, MuEtaThr);
			}
		}
	}
	TCanvas *c11 = new TCanvas("","",1200,900);
	c11->cd();
	hist2d_1->Draw("COLZ TEXT");
	c11->Print("test.png");
}