void compare_styler(){


	TLegend* leg = new TLegend(0.64,0.6818,0.86,0.8148);
    TFile* p = TFile::Open("comparer_plots.root");

    TH1* mavg_p_cp2 = (TH1*) p->Get("mavg_p_st300TuneCP2");
    TH1* mavg_p_cp5 = (TH1*) p->Get("mavg_p_st300TuneCP5");
	TH1* mavg_p_6j = (TH1*) p->Get("mavg_p_st300to6j");
	TH1* mavg_p_4b = (TH1*) p->Get("mavg_p_st300to4b");
	TH1* mavg_p_sq = (TH1*) p->Get("mavg_p_sq300to4j");


	gStyle->SetOptStat(0);

    mavg_p_cp2->Rebin(50);
    mavg_p_cp5->Rebin(50);
    mavg_p_6j->Rebin(50);                                                                                                                                                                                                              
    mavg_p_4b->Rebin(50);
	mavg_p_sq->Rebin(50);

    mavg_p_cp2->SetLineColor(kAzure+2);
    mavg_p_cp5->SetLineColor(kCyan-7);
    mavg_p_6j->SetLineColor(kGreen-7);
    mavg_p_4b->SetLineColor(kGreen-1);
	mavg_p_sq->SetLineColor(kRed);

    mavg_p_sq->Draw("hist e");
    mavg_p_cp5->Draw("same hist e");
    mavg_p_6j->Draw("same hist e");
    mavg_p_4b->Draw("same hist e");
	mavg_p_cp2->Draw("same hist e");

	// mavg_p_cp2->Scale(8.51615/);
    // mavg_p_cp5->Scale(8.51615/17628);
    // mavg_p_6j->Scale(8.51615/);
    // mavg_p_4b->Scale(8.51615/);
    // mavg_p_sq->Scale(8.51615/42652);


    // mavg_p_cp2->Scale(8.51615/511693);
    // mavg_p_cp5->Scale(8.51615/533878);
    // mavg_p_6j->Scale(8.51615/532773);
    // mavg_p_4b->Scale(8.51615/573000);                                                                                                                                                                                               
    // mavg_p_sq->Scale(8.51615/100000);

 
	mavg_p_sq->GetXaxis()->SetRangeUser(0, 600);

    leg->AddEntry(mavg_p_cp2, "RPV_2t6j_TuneCP2");
    leg->AddEntry(mavg_p_cp5, "RPV_2t6j_TuneCP5");
    leg->AddEntry(mavg_p_6j, "StealthSYY_2t6j");
	leg->AddEntry(mavg_p_4b, "StealthSHH_2t4b");
	leg->AddEntry(mavg_p_sq, "Sq300To4J");

    leg->Draw();
















    // TH1* mavg_p_n = (TH1*) p->Get("mavg_p_new");
    // TH1* mavg_p_o = (TH1*) p->Get("mavg_p_old");


	// gStyle->SetOptStat(0);

    // mavg_p_n->Rebin(50);
    // mavg_p_o->Rebin(50);

    // mavg_p_n->SetLineColor(kRed); 
	// mavg_p_o->SetLineColor(kBlue);

	// mavg_p_n->Draw("hist e");
	// mavg_p_o->Draw("same hist e");









}
