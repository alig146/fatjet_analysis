#include "/home/tote/decortication/macros/common.cc"

void draw_plot(TString name, TString ds, TH1* post, TH1* mc, TH1* mce, int logy=0, int norm=0) {
	TH1* post_clone = (TH1*) post->Clone(TString(post->GetName()) + "_clone");
	TH1* mc_clone = (TH1*) mc->Clone(TString(mc->GetName()) + "_clone");
	TH1* mce_clone = (TH1*) mce->Clone(TString(mce->GetName()) + "_clone");
	
	post_clone->SetTitle("");
	TString yunit = "Events";
	if (norm != 0) yunit = "Normalized events";
	style_ylabel(post_clone, "GeV", yunit);
	post_clone->GetXaxis()->SetTitle("Average fatjet mass [GeV]");
	
	if (norm != 0) {
		normalize_th1(post_clone);
		normalize_th1(mc_clone);
		normalize_th1(mce_clone);
	}
	
	TCanvas* tc = draw_pull(name, post_clone, mc_clone, 0, 1200);
	TPad* pad = (TPad*) tc->GetPad(1);
	pad->cd();
	tc->SetLogy(logy);
	
	post_clone->GetXaxis()->SetLabelOffset(100);
	
	post_clone->Draw("e");
	mc_clone->Draw("same hist");
	mce_clone->Draw("same e2");
	post_clone->Draw("same e");
	pad->SetLogy(logy);
	
	if (logy == 0) {
		double max = post_clone->GetMaximum();
		if (mc_clone->GetMaximum() > max) max = mc_clone->GetMaximum();
		post_clone->SetMinimum(0);
		post_clone->SetMaximum(max*1.4);
	}
	else if (logy == 1) {
		post_clone->SetMinimum(0.05);
		if (norm != 0) post_clone->SetMinimum(post_clone->GetMinimum()/40);
		post_clone->SetMaximum(post_clone->GetMaximum()*3);
	}
	
	TLegend* leg;
	leg = new TLegend(0.55, 0.55, 0.80, 0.75);
//	if (logy == 0) leg = new TLegend(0.55, 0.42, 0.80, 0.57);
//	else if (logy == 1) leg = new TLegend(0.22, 0.1, 0.48, 0.25);
	leg->AddEntry(post_clone, "QCD posterior", "ple");
	leg->AddEntry(mc_clone, name_proper[ds] + " MC", "f");
	leg->AddEntry(mce_clone, name_proper[ds] + " MC uncert.", "lf");
	leg->Draw();
	style_info(false, lum_string["15"], 1, true);
	
//	style_write(name_proper[ds], 0.61, 0.74, 0.04);
	
	tc->SaveAs(name + ".pdf");
//	tc->SaveAs(name + ".png");
}

void compare_qcd() {
	TFile* tf_post = TFile::Open("theta_plots_sig15_sb_prediction.root");
	TFile* tf_mle = TFile::Open("theta_plots_sig15_sb_mle.root");
	TFile* tf_mc = TFile::Open("../study_contamination/contamination_plots.root");
	
//	TH1* post = (TH1*) tf_post->Get("Ms100__mAvgPSel__0");
	TH1* post_qcd = (TH1*) tf_mle->Get("mAvgPSel__QCD");
	TH1* mc_qcdmg = (TH1*) tf_mc->Get("qcdmg_sig");
	TH1* mc_qcdp = (TH1*) tf_mc->Get("qcdp_sig");
	
	// In order to scale things, make sure Sumw2 is set (it's not set by default in the theta-made plots):
	post_qcd->Sumw2();
	
	// Deweight MC to '15:
	double w15 = get_weight("", "15");
//	double wall = get_weight();
	mc_qcdmg->Scale(w15);
	mc_qcdp->Scale(w15);
	
	// Style and draw:
	gStyle->SetOptStat(0);
	
	mc_qcdmg->Rebin(50);
	TH1* mce_qcdmg = (TH1*) mc_qcdmg->Clone("qcdmge_sig");
	mc_qcdmg->SetFillColorAlpha(kBlue, 0.2);
	mc_qcdmg->SetLineStyle(2);
	mce_qcdmg->SetMarkerSize(0);
	mce_qcdmg->SetFillColorAlpha(kBlack, 0.2);
	mce_qcdmg->SetLineStyle(2);
	
	mc_qcdp->Rebin(50);
	TH1* mce_qcdp = (TH1*) mc_qcdp->Clone("qcdpe_sig");
	mc_qcdp->SetFillColorAlpha(kBlue, 0.2);
	mc_qcdp->SetLineStyle(2);
	mce_qcdp->SetMarkerSize(0);
	mce_qcdp->SetFillColorAlpha(kBlack, 0.2);
	mce_qcdp->SetLineStyle(2);
	
	for (int inorm=0; inorm < 2; ++ inorm) {
		for (int ilog=0; ilog < 2; ++ ilog) {
			TString name = "compare_qcdmg";
			if (ilog == 1) name = name + "_logy";
			if (inorm == 1) name = name + "_norm";
			draw_plot(name, "qcdmg", post_qcd, mc_qcdmg, mce_qcdmg, ilog, inorm);
			
			name = "compare_qcdp";
			if (ilog == 1) name = name + "_logy";
			if (inorm == 1) name = name + "_norm";
			draw_plot(name, "qcdp", post_qcd, mc_qcdp, mce_qcdp, ilog, inorm);
		}
	}
}
