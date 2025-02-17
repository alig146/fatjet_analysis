void compare()
{
	TFile* tf_out = new TFile("comparer_plots.root","recreate");

	TString cut = "abs(W[0])*(deta<1.0&&masy_p<0.1&&tau21[0]<0.75&&tau21[1]<0.75&&tau42[0]<0.50&&tau42[1]<0.50&&tau43[0]<0.80&&tau43[1]<0.80)";
	//TString cut = "abs(W[0])";

	//	TString cut2 = "(abs(w)*(deta<1.0&&masy_p<0.1&&tau21[0]<0.75&&tau21[1]<0.75&&tau42[0]<0.50&&tau42[1]<0.50&&tau43[0]<0.80&&tau43[1]<0.80))";
	//    TFile* o = TFile::Open("/home/ag1378/cmslink/second/CMSSW_8_0_28_patch1/src/Analyzers/FatjetAnalyzer/test/analysis/sq300to4j_orig.root");

    // TFile* n1 = TFile::Open("~/cmslink/store/anatuplizer_st300TuneCP2_pre.root");
	// TFile* n2 = TFile::Open("~/cmslink/store/anatuplizer_st300TuneCP5_pre.root");
	// TFile* n3 = TFile::Open("~/cmslink/store/anatuplizer_st300to6j_pre.root");
	// TFile* n4 = TFile::Open("~/cmslink/store/anatuplizer_st300to4b_pre.root");
	// TFile* n5 = TFile::Open("~/cmslink/store/anatuplizer_sq300to4j_pre.root");
	//
	// TTree* n1m = (TTree*) n1->Get("st300TuneCP2");
	// TTree* n2m = (TTree*) n2->Get("st300TuneCP5");
	// TTree* n3m = (TTree*) n3->Get("st300to6j");
	// TTree* n4m = (TTree*) n4->Get("st300to4b");
	// TTree* n5m = (TTree*) n5->Get("sq300to4j");


	TFile* n1 = TFile::Open("~/cmslink/store/anatuplizer_st500TuneCP2_pre.root");
	TFile* n2 = TFile::Open("~/cmslink/store/anatuplizer_st500TuneCP5_pre.root");
	TFile* n3 = TFile::Open("~/cmslink/store/anatuplizer_st500to6j_pre.root");
	TFile* n4 = TFile::Open("~/cmslink/store/anatuplizer_st500to4b_pre.root");
	TFile* n5 = TFile::Open("~/cmslink/store/anatuplizer_sq500to4j_pre.root");

	TTree* n1m = (TTree*) n1->Get("st500TuneCP2");
	TTree* n2m = (TTree*) n2->Get("st500TuneCP5");
	TTree* n3m = (TTree*) n3->Get("st500to6j");
	TTree* n4m = (TTree*) n4->Get("st500to4b");
	TTree* n5m = (TTree*) n5->Get("sq500to4j");


    n1m->Draw("mavg_p >> mavg_p_st300TuneCP2 (1200, 0, 1200)", cut);
    TH1* mavg_p_TuneCP2 = (TH1*) gDirectory->Get("mavg_p_st300TuneCP2");
    tf_out->WriteTObject(mavg_p_TuneCP2);

	n2m->Draw("mavg_p >> mavg_p_st300TuneCP5 (1200, 0, 1200)", cut);
    TH1* mavg_p_TuneCP5 = (TH1*) gDirectory->Get("mavg_p_st300TuneCP5");
    tf_out->WriteTObject(mavg_p_TuneCP5);

    n3m->Draw("mavg_p >> mavg_p_st300to6j (1200, 0, 1200)", cut);
    TH1* mavg_p_6j = (TH1*) gDirectory->Get("mavg_p_st300to6j");
    tf_out->WriteTObject(mavg_p_6j);

    n4m->Draw("mavg_p >> mavg_p_st300to4b (1200, 0, 1200)", cut);
    TH1* mavg_p_4b = (TH1*) gDirectory->Get("mavg_p_st300to4b");
    tf_out->WriteTObject(mavg_p_4b);

	n5m->Draw("mavg_p >> mavg_p_sq300to4j (1200, 0, 1200)", cut);
    TH1* mavg_p_sq = (TH1*) gDirectory->Get("mavg_p_sq300to4j");
    tf_out->WriteTObject(mavg_p_sq);







    //n5m->Draw("mavg_p >> mavg_p_new (1200, 0, 1200)");
    //TH1* mavg_new = (TH1*) gDirectory->Get("mavg_p_new");
    //tf_out->WriteTObject(mavg_new);

    //o1->Draw("mavg_p >> mavg_p_old (1200, 0, 1200)");
    //TH1* mavg_old = (TH1*) gDirectory->Get("mavg_p_old");
    //tf_out->WriteTObject(mavg_old);


















}
