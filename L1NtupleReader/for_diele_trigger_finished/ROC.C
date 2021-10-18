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

		/*
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
	// e+2mu, mu 2 GeV, eta 2.5, eg eta 2.5
	float efficiency_6[8] = {0.0000164731,0.0000247096,0.0000345934,0.0000543611,0.0000724814,0.000107075,0.000172967,0.000225681};
	float rate_6[8] = {50,88,153,483,1340,2886,9751,19787};
	*/
	/*
	//double-e effacc, eta0.8dr0.8
	float efficiency_0[8] = {0.000253685,0.000390411,0.000616092,0.00100156,0.00155176,0.00211514,0.00286137,0.00327978};
	float rate_0[8] = {171,313,575,1449,4090,10605,42056,104941};
	//double-e, eta0.8dr0.9
	float efficiency_1[8] = {0.00025698,0.000393706,0.000624329,0.00103286,0.00162918,0.00224528,0.00309034,0.00356642};
	float rate_1[8] = {201,367,686,1751,4849,12706,49909,123078};
	//double-e, eta0.8dr1.0
	float efficiency_2[8] = {0.00025698,0.000395353,0.000625976,0.00105263,0.00167037,0.00233753,0.00323037,0.00376245};
	float rate_2[8] = {237,423,804,2029,5631,14648,56968,138619};
	//double-e, eta1.0dr0.8
	float efficiency_3[8] = {0.000311341,0.0004908947,0.000777528,0.00126513,0.00193229,0.0026258,0.00361748,0.004129};
	float rate_3[8] = {210,381,715,1825,4913,12755,50249,124295};
	//double-e, eta1.0dr0.9
	float efficiency_4[8] = {0.000314636,0.000494192,0.000785765,0.00129808,0.00201301,0.00278889,0.00392553,0.0045235};
	float rate_4[8] = {247,448,860,2237,5891,15401,59941,145849};
	//double-e, eta1.0dr1.0
	float efficiency_5[8] = {0.000314635,0.000495839,0.000787412,0.00131784,0.00205913,0.00289267,0.00410673,0.00477554};
	float rate_5[8] = {292,518,1007,2595,6859,17832,68670,164312};
	//double-e, eta1.2dr0.8
	float efficiency_6[8] = {0.000380528,0.000588088,0.0009345669,0.00150564,0.00228646,0.00319248,0.00446749,0.00515936};
	float rate_6[8] = {255,462,864,2192,5688,14730,57812,141615};
	//double-e, eta1.2dr0.9
	float efficiency_7[8] = {0.000383822,0.000591383,0.000943906,0.00154517,0.0023853,0.0033918,0.0048579,0.0056673};
	float rate_7[8] = {302,547,1046,2676,6843,17828,69136,166058};
	//double-e, eta1.2dr1.0
	float efficiency_8[8] = {0.000383822,0.00059303,0.000945553,0.00156494,0.00243801,0.00351205,0.00508688,0.00598466};
	float rate_8[8] = {359,641,1228,3125,8017,20749,79488,187044};
	//double-e, eta1.4dr0.8
	float efficiency_9[8] = {0.000410179,0.000640802,0.0010098,0.00163577,0.00249731,0.003517,0.00497486,0.00576886};
	float rate_9[8] = {293,532,983,2501,6390,16269,62860,150819};
	//double-e, eta1.4dr0.9
	float efficiency_10[8] = {0.000413474,0.000644096,0.00101803,0.00167531,0.0026011,0.00372291,0.00540151,0.00633389};
	float rate_10[8] = {353,639,1197,3067,7727,19753,75323,176804};
	//double-e, eta1.4dr1.0
	float efficiency_11[8] = {0.000413474,0.000645744,0.00101968,0.00169508,0.00265381,0.00384481,0.00564532,0.00667982};
	float rate_11[8] = {423,755,1419,3603,9094,23073,86780,199231};
	*/
	
	//double-e, eta0.8dr1.0
	float efficiency_0[9] = {0.000202619,0.00025698,0.000395353,0.000625976,0.00105263,0.00167037,0.00233753,0.00323037,0.00376245};
	float rate_0[9] = {167,237,423,804,2029,5631,14648,56968,138619};
	//double-e, eta0.8dr0.8
	float efficiency_1[9] = {0.000199324,0.000253685,0.000390411,0.000616092,0.00100156,0.00155176,0.00211514,0.00286137,0.00327978};
	float rate_1[9] = {114,171,313,575,1449,4090,10605,42056,104941};
	//double-e, eta0.8dr0.7
	float efficiency_2[9] = {0.000197677,0.000252038,0.000380528,0.00059303,0.000940611,0.00142492,0.00188452,0.00248743,0.00282019};
	float rate_2[9] = {92,132,249,458,1157,3297,8475,33727,85572};
	//double-e, eta0.8dr0.6
	float efficiency_3[9] = {0.000192735,0.000242154,0.000359113,0.000540316,0.000820358,0.0012256,0.00158306,0.00200971,0.00224692};
	float rate_3[9] = {67,101,178,346,896,2599,6599,26354,67118};
	//double-e, eta1.0dr1.0
	float efficiency_4[9] = {0.000245448,0.000314635,0.000495839,0.000787412,0.00131784,0.00205913,0.00289267,0.00410673,0.00477554};
	float rate_4[9] = {205,292,518,1007,2595,6859,17832,68670,164312};
	//double-e, eta1.0dr0.8
	float efficiency_5[9] = {0.000242154,0.000311341,0.0004908947,0.000777528,0.00126513,0.00193229,0.0026258,0.00361748,0.004129};
	float rate_5[9] = {141,210,381,715,1825,4913,12755,50249,124295};
	//double-e, eta1.0dr0.7
	float efficiency_6[9] = {0.000240507,0.000309693,0.000481013,0.000754466,0.0011943,0.00177929,0.00234247,0.00314141,0.00354171};
	float rate_6[9] = {117,167,303,565,1447,3895,10086,40120,101405};
	//double-e, eta1.0dr0.6
	float efficiency_7[9] = {0.000235565,0.00029981,0.000456304,0.000691868,0.00104274,0.00153199,0.00197347,0.00254344,0.00282678};
	float rate_7[9] = {87,130,218,430,1116,3050,7801,31232,79599};
	//double-e, eta1.2dr1.0
	float efficiency_8[9] = {0.000291573,0.000383822,0.00059303,0.000945553,0.00156494,0.00243801,0.00351205,0.00508688,0.00598466};
	float rate_8[9] = {252,359,641,1228,3125,8017,20749,79488,187044};
	//double-e, eta1.2dr0.8
	float efficiency_9[9] = {0.000288278,0.000380528,0.000588088,0.0009345669,0.00150564,0.00228646,0.00319248,0.00446749,0.00515936};
	float rate_9[9] = {174,255,462,864,2192,5688,14730,57812,141615};
	//double-e, eta1.2dr0.7
	float efficiency_10[9] = {0.000286631,0.00037233,0.00057491,0.000906018,0.00142162,0.00211349,0.00285807,0.00390082,0.00444608};
	float rate_10[9] = {145,205,370,682,1725,4472,11596,46126,115631};
	//double-e, eta1.2dr0.6
	float efficiency_11[9] = {0.000281689,0.000367349,0.000548553,0.000831889,0.00124866,0.0018351,0.00242978,0.003176,0.00355983};
	float rate_11[9] = {109,162,273,523,1334,3480,8936,35814,90911};
	//double-e, eta1.4dr1.0
	float efficiency_12[9] = {0.000312988,0.000413474,0.000645744,0.00101968,0.00169508,0.00265381,0.00384481,0.00564532,0.00667982};
	float rate_12[9] = {292,423,755,1419,3603,9094,23073,86780,199231};
	//double-e, eta1.4dr0.8
	float efficiency_13[9] = {0.000309693,0.000410179,0.000640802,0.0010098,0.00163577,0.00249731,0.003517,0.00497486,0.00576886};
	float rate_13[9] = {193,293,532,983,2501,6390,16269,62860,150819};
	//double-e, eta1.4dr0.7
	float efficiency_14[9] = {0.000308046,0.000406884,0.000627623,0.000980147,0.00154682,0.00231117,0.00316447,0.00436206,0.00498639};
	float rate_14[9] = {161,233,421,767,1966,5010,12777,50076,123116};
	//double-e, eta1.4dr0.6
	float efficiency_15[9] = {0.000303104,0.000397001,0.000601266,0.000901076,0.00136726,0.00200807,0.0026884,0.00355983,0.00400789};
	float rate_15[9] = {121,182,310,588,1521,3877,9831,38771,96734};
	
	
	
	int ref = 1098;
	for(int i=0; i<9; i++){
		rate_0[i] = float(rate_0[i]) / ref;
		rate_1[i] = float(rate_1[i]) / ref;
		rate_2[i] = float(rate_2[i]) / ref;
		rate_3[i] = float(rate_3[i]) / ref;
		rate_4[i] = float(rate_4[i]) / ref;
		rate_5[i] = float(rate_5[i]) / ref;
		rate_6[i] = float(rate_6[i]) / ref;
		rate_7[i] = float(rate_7[i]) / ref;
		rate_8[i] = float(rate_8[i]) / ref;
		rate_9[i] = float(rate_9[i]) / ref;
		rate_10[i] = float(rate_10[i]) / ref;
		rate_11[i] = float(rate_11[i]) / ref;
		rate_12[i] = float(rate_12[i]) / ref;
		rate_13[i] = float(rate_13[i]) / ref;
		rate_14[i] = float(rate_14[i]) / ref;
		rate_15[i] = float(rate_15[i]) / ref;
	}
	/*
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
	TGraph *g6 = new TGraph(8,efficiency_6,rate_6);
	g6->SetLineColor(15);
	g6->SetLineWidth(2);
	g6->SetMarkerStyle(23);
	g6->SetMarkerColor(15);
	g6->SetMarkerSize(2);
*/
/*
	TGraph *g0 = new TGraph(9,efficiency_0,rate_0);
	g0->SetLineColor(4);
	g0->SetLineWidth(2);
	g0->SetLineStyle(1);
	g0->SetMarkerStyle(20);
	g0->SetMarkerColor(4);
	g0->SetMarkerSize(2);
	TGraph *g1 = new TGraph(9,efficiency_1,rate_1);
	g1->SetLineColor(4);
	g1->SetLineWidth(2);
	g1->SetLineStyle(2);
	g1->SetMarkerStyle(20);
	g1->SetMarkerColor(4);
	g1->SetMarkerSize(2);
	TGraph *g2 = new TGraph(9,efficiency_2,rate_2);
	g2->SetLineColor(4);
	g2->SetLineWidth(2);
	g2->SetLineStyle(9);
	g2->SetMarkerStyle(20);
	g2->SetMarkerColor(4);
	g2->SetMarkerSize(2);
	TGraph *g3 = new TGraph(9,efficiency_3,rate_3);
	g3->SetLineColor(2);
	g3->SetLineWidth(2);
	g3->SetLineStyle(1);
	g3->SetMarkerStyle(21);
	g3->SetMarkerColor(2);
	g3->SetMarkerSize(2);
	TGraph *g4 = new TGraph(9,efficiency_4,rate_4);
	g4->SetLineColor(2);
	g4->SetLineWidth(2);
	g4->SetLineStyle(2);
	g4->SetMarkerStyle(21);
	g4->SetMarkerColor(2);
	g4->SetMarkerSize(2);
	TGraph *g5 = new TGraph(9,efficiency_5,rate_5);
	g5->SetLineColor(2);
	g5->SetLineWidth(2);
	g5->SetLineStyle(9);
	g5->SetMarkerStyle(21);
	g5->SetMarkerColor(2);
	g5->SetMarkerSize(2);
	TGraph *g6 = new TGraph(9,efficiency_6,rate_6);
	g6->SetLineColor(8);
	g6->SetLineWidth(2);
	g6->SetLineStyle(1);
	g6->SetMarkerStyle(23);
	g6->SetMarkerColor(8);
	g6->SetMarkerSize(2);
	TGraph *g7 = new TGraph(9,efficiency_7,rate_7);
	g7->SetLineColor(8);
	g7->SetLineWidth(2);
	g7->SetLineStyle(2);
	g7->SetMarkerStyle(23);
	g7->SetMarkerColor(8);
	g7->SetMarkerSize(2);
	TGraph *g8 = new TGraph(9,efficiency_8,rate_8);
	g8->SetLineColor(8);
	g8->SetLineWidth(2);
	g8->SetLineStyle(9);
	g8->SetMarkerStyle(23);
	g8->SetMarkerColor(8);
	g8->SetMarkerSize(2);
	TGraph *g9 = new TGraph(9,efficiency_9,rate_9);
	g9->SetLineColor(6);
	g9->SetLineWidth(2);
	g9->SetLineStyle(1);
	g9->SetMarkerStyle(24);
	g9->SetMarkerColor(6);
	g9->SetMarkerSize(2);
	TGraph *g10 = new TGraph(9,efficiency_10,rate_10);
	g10->SetLineColor(6);
	g10->SetLineWidth(2);
	g10->SetLineStyle(2);
	g10->SetMarkerStyle(24);
	g10->SetMarkerColor(6);
	g10->SetMarkerSize(2);
	TGraph *g11 = new TGraph(9,efficiency_11,rate_11);
	g11->SetLineColor(6);
	g11->SetLineWidth(2);
	g11->SetLineStyle(9);
	g11->SetMarkerStyle(24);
	g11->SetMarkerColor(6);
	g11->SetMarkerSize(2);
	*/
	
	TGraph *g0 = new TGraph(9,efficiency_0,rate_0);
	g0->SetLineColor(1);
	g0->SetLineWidth(2);
	g0->SetLineStyle(1);
	g0->SetMarkerStyle(20);
	g0->SetMarkerColor(1);
	g0->SetMarkerSize(2);
	TGraph *g1 = new TGraph(9,efficiency_1,rate_1);
	g1->SetLineColor(1);
	g1->SetLineWidth(2);
	g1->SetLineStyle(2);
	g1->SetMarkerStyle(21);
	g1->SetMarkerColor(1);
	g1->SetMarkerSize(2);
	TGraph *g2 = new TGraph(9,efficiency_2,rate_2);
	g2->SetLineColor(1);
	g2->SetLineWidth(2);
	g2->SetLineStyle(9);
	g2->SetMarkerStyle(22);
	g2->SetMarkerColor(1);
	g2->SetMarkerSize(2);
	TGraph *g3 = new TGraph(9,efficiency_3,rate_3);
	g3->SetLineColor(1);
	g3->SetLineWidth(2);
	g3->SetLineStyle(6);
	g3->SetMarkerStyle(23);
	g3->SetMarkerColor(1);
	g3->SetMarkerSize(2);
	
	TGraph *g4 = new TGraph(9,efficiency_4,rate_4);
	g4->SetLineColor(4);
	g4->SetLineWidth(2);
	g4->SetLineStyle(1);
	g4->SetMarkerStyle(20);
	g4->SetMarkerColor(4);
	g4->SetMarkerSize(2);
	TGraph *g5 = new TGraph(9,efficiency_5,rate_5);
	g5->SetLineColor(4);
	g5->SetLineWidth(2);
	g5->SetLineStyle(2);
	g5->SetMarkerStyle(21);
	g5->SetMarkerColor(4);
	g5->SetMarkerSize(2);
	TGraph *g6 = new TGraph(9,efficiency_6,rate_6);
	g6->SetLineColor(4);
	g6->SetLineWidth(2);
	g6->SetLineStyle(9);
	g6->SetMarkerStyle(22);
	g6->SetMarkerColor(4);
	g6->SetMarkerSize(2);
	TGraph *g7 = new TGraph(9,efficiency_7,rate_7);
	g7->SetLineColor(4);
	g7->SetLineWidth(2);
	g7->SetLineStyle(6);
	g7->SetMarkerStyle(23);
	g7->SetMarkerColor(4);
	g7->SetMarkerSize(2);
	
	TGraph *g8 = new TGraph(9,efficiency_8,rate_8);
	g8->SetLineColor(2);
	g8->SetLineWidth(2);
	g8->SetLineStyle(1);
	g8->SetMarkerStyle(20);
	g8->SetMarkerColor(2);
	g8->SetMarkerSize(2);
	TGraph *g9 = new TGraph(9,efficiency_9,rate_9);
	g9->SetLineColor(2);
	g9->SetLineWidth(2);
	g9->SetLineStyle(2);
	g9->SetMarkerStyle(21);
	g9->SetMarkerColor(2);
	g9->SetMarkerSize(2);
	TGraph *g10 = new TGraph(9,efficiency_10,rate_10);
	g10->SetLineColor(2);
	g10->SetLineWidth(2);
	g10->SetLineStyle(9);
	g10->SetMarkerStyle(22);
	g10->SetMarkerColor(2);
	g10->SetMarkerSize(2);
	TGraph *g11 = new TGraph(9,efficiency_11,rate_11);
	g11->SetLineColor(2);
	g11->SetLineWidth(2);
	g11->SetLineStyle(6);
	g11->SetMarkerStyle(23);
	g11->SetMarkerColor(2);
	g11->SetMarkerSize(2);
	
	TGraph *g12 = new TGraph(9,efficiency_12,rate_12);
	g12->SetLineColor(8);
	g12->SetLineWidth(2);
	g12->SetLineStyle(1);
	g12->SetMarkerStyle(20);
	g12->SetMarkerColor(8);
	g12->SetMarkerSize(2);
	TGraph *g13 = new TGraph(9,efficiency_13,rate_13);
	g13->SetLineColor(8);
	g13->SetLineWidth(2);
	g13->SetLineStyle(2);
	g13->SetMarkerStyle(21);
	g13->SetMarkerColor(8);
	g13->SetMarkerSize(2);
	TGraph *g14 = new TGraph(9,efficiency_14,rate_14);
	g14->SetLineColor(8);
	g14->SetLineWidth(2);
	g14->SetLineStyle(9);
	g14->SetMarkerStyle(22);
	g14->SetMarkerColor(8);
	g14->SetMarkerSize(2);
	TGraph *g15 = new TGraph(9,efficiency_15,rate_15);
	g15->SetLineColor(8);
	g15->SetLineWidth(2);
	g15->SetLineStyle(6);
	g15->SetMarkerStyle(23);
	g15->SetMarkerColor(8);
	g15->SetMarkerSize(2);
	
	
	TCanvas *c11 = new TCanvas("","",2400,1800);
	c11->cd();
	TAxis *yaxis = g3->GetYaxis();
	TAxis *xaxis = g3->GetXaxis();
	xaxis->SetTitle("Trigger efficiency * signal acceptance");
	xaxis->SetTitleOffset(1.2);
	yaxis->SetTitle("Rate (normalized to L1_SingleMu9er1p5)");
	xaxis->SetLimits(0.0001,0.002);
	yaxis->SetRangeUser(0.05,4);
	c11->SetLogy();
	c11->SetLogx();
	//c11->SetGrid();
	g3->Draw("APL");
	//g0->Draw("APL");
	// g1->Draw("PLSAME");
	// g2->Draw("PLSAME");
	// g3->Draw("PLSAME");
	g3->Draw("PLSAME");
	g7->Draw("PLSAME");
	g11->Draw("PLSAME");
	g15->Draw("PLSAME");
	// g8->Draw("PLSAME");
	// g9->Draw("PLSAME");
	// g10->Draw("PLSAME");
	// g11->Draw("PLSAME");
	// g12->Draw("PLSAME");
	// g13->Draw("PLSAME");
	// g14->Draw("PLSAME");
	// g15->Draw("PLSAME");
	gStyle->SetOptTitle(0);
	TLegend *leg = new TLegend(0.1,0.45,0.4,0.9);
	// leg->AddEntry(g0, "SingleMuXer1p5","pl");
	// leg->AddEntry(g1, "Mu4er1p5_EgXer1p0","pl");
	// leg->AddEntry(g3, "Mu4er1p5_EgXer2p5","pl");
	// leg->AddEntry(g4, "Mu2er1p0_EgXer2p5","pl");
	// leg->AddEntry(g2, "EgXer1p0_EgXer1p0_dR1p0","pl");
	// leg->AddEntry(g5, "Mu2er2p5_2EgXer1p0_dR1p0","pl");
	// leg->AddEntry(g6, "Mu2er2p5_2EgXer2p5_dR1p0","pl");
	// leg->AddEntry(g0, "EgXer1p0_EgXer1p0_dR1p0, I, 0.4","pl");
	// leg->AddEntry(g1, "EgXer1p0_EgXer1p0_dR1p0, I, 0.2","pl");
	// leg->AddEntry(g2, "EgXer1p0_EgXer1p0_dR1p0, II, 0.4","pl");
	// leg->AddEntry(g3, "EgXer1p0_EgXer1p0_dR1p0, II, 0.2","pl");
	// leg->AddEntry(g0, "EgXer0p8_EgXer0p8_dR0p8","pl");
	// leg->AddEntry(g1, "EgXer0p8_EgXer0p8_dR0p9","pl");
	// leg->AddEntry(g2, "EgXer0p8_EgXer0p8_dR1p0","pl");
	// leg->AddEntry(g3, "EgXer1p0_EgXer1p0_dR0p8","pl");
	// leg->AddEntry(g4, "EgXer1p0_EgXer1p0_dR0p9","pl");
	// leg->AddEntry(g5, "EgXer1p0_EgXer1p0_dR1p0","pl");
	// leg->AddEntry(g6, "EgXer1p2_EgXer1p2_dR0p8","pl");
	// leg->AddEntry(g7, "EgXer1p2_EgXer1p2_dR0p9","pl");
	// leg->AddEntry(g8, "EgXer1p2_EgXer1p2_dR1p0","pl");
	// leg->AddEntry(g9, "EgXer1p4_EgXer1p2_dR0p8","pl");
	// leg->AddEntry(g10, "EgXer1p4_EgXer1p2_dR1p0","pl");
	// leg->AddEntry(g11, "EgXer1p4_EgXer1p2_dR1p2","pl");
	// leg->AddEntry(g0, "2EgXer0p8_dR1p0","pl");
	// leg->AddEntry(g1, "2EgXer0p8_dR0p8","pl");
	// leg->AddEntry(g2, "2EgXer0p8_dR0p7","pl");
	leg->AddEntry(g3, "2EgXer0p8_dR0p6","pl");
	// leg->AddEntry(g4, "2EgXer1p0_dR1p0","pl");
	// leg->AddEntry(g5, "2EgXer1p0_dR0p8","pl");
	// leg->AddEntry(g6, "2EgXer1p0_dR0p7","pl");
	leg->AddEntry(g7, "2EgXer1p0_dR0p6","pl");
	// leg->AddEntry(g8, "2EgXer1p2_dR1p0","pl");
	// leg->AddEntry(g9, "2EgXer1p2_dR0p8","pl");
	// leg->AddEntry(g10, "2EgXer1p2_dR0p7","pl");
	leg->AddEntry(g11, "2EgXer1p2_dR0p6","pl");
	// leg->AddEntry(g12, "2EgXer1p4_dR1p0","pl");
	// leg->AddEntry(g13, "2EgXer1p4_dR0p8","pl");
	// leg->AddEntry(g14, "2EgXer1p4_dR0p7","pl");
	leg->AddEntry(g15, "2EgXer1p4_dR0p6","pl");
	leg->Draw();
	c11->Print("ROCZOOM.png");
	c11->Print("ROCZOOM.pdf");
}