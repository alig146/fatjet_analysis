#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void draw_set(TString cut, bool norm=false) {
//	vector<int> masses = {100, 150, 200, 300, 400, 500};
//	vector<int> colors = {kRed, kOrange, kGreen, kBlue, kViolet, kBlack};
	vector<int> masses = {100, 200, 300, 400, 500};
	vector<int> colors = {kRed-4, kOrange-1, kSpring+4, kBlue-5, kViolet-6};
	TString suffix = "_" + cut;
	if (norm) suffix = suffix + "_norm";
	
	TFile* tf_in = TFile::Open("../study_basic/basic_plots.root");
	
	for (int is = 0; is < 2; ++is) {
		TString name = "mavg_sqto4j" + suffix;
		if (is == 1) name = "mavg_sgto5j" + suffix;
		TCanvas* tc = new TCanvas(name, name);
	//	TLegend* leg = new TLegend(0.60, 0.58, 0.80, 0.80);
		TLegend* leg = get_legend(1, masses.size(), 0.8);
//		TFile* tf_in = TFile::Open("../study_basic/basic_plots_sqto4j.root");
//		if (is == 1) tf_in = TFile::Open("../study_basic/basic_plots_sgto5j.root");
	
	
		for (int i = 0; i < masses.size(); ++ i) {
			TString signame = "sq" + to_string(masses[i]) + "to4j";
			if (is == 1) signame = "sg" + to_string(masses[i]) + "to5j";
		
	//		TFile* tf_in;
	//		if (signame == "sq100to4j") tf_in = TFile::Open("../study_basic/basic_plots_sq100.root");		// KLUDGE
	//		tf_in = TFile::Open("../study_basic/basic_plots_sqto4j.root");
		
			TH1* h_sig = (TH1*) tf_in->Get(TString("mavg_") + signame + "_" + cut);
			h_sig->Rebin(30);
			TH1* h_sige = (TH1*) h_sig->Clone(signame + suffix + "_e");
			h_sig->SetLineColor(colors[i]);
			h_sig->SetMarkerSize(0);
			h_sig->SetFillStyle(1001);
//			h_sig->SetFillStyle(0);
			h_sig->SetFillColorAlpha(colors[i], 0.3);
			if (norm) h_sig->Scale(1.0/h_sig->Integral());
			if (norm) {
				h_sig->SetMaximum(0.7);
				h_sig->SetMinimum(0);
			}
			else {
				if (is == 0) h_sig->SetMaximum(300);
				else if (is == 1) h_sig->SetMaximum(6000);
			}
			h_sig->SetTitle("");
			h_sig->GetXaxis()->SetTitle(get_xtitle("mavg"));
			h_sig->GetXaxis()->SetRangeUser(0, 700);
		
			if (norm) h_sige->Scale(1.0/h_sige->Integral());
			h_sige->SetFillStyle(1001);
	//		h_sige->SetFillColor(colors[i]);
			h_sige->SetFillColorAlpha(kBlack, 0.2);
			h_sige->SetLineColor(colors[i]);
			h_sige->SetMarkerSize(0);
		
			style_ylabel(h_sig, "GeV", "Events");
			h_sig->Draw("same hist");
//			h_sige->Draw("same e2");
			leg->AddEntry(h_sig, name_proper[signame], "f");
		}
		leg->Draw();
		style_info();
		style_cut(cut);
		gPad->RedrawAxis();
		tc->SaveAs(name + ".pdf");
		tc->SetLogy();
		tc->SaveAs(name + "_logy.pdf");
	}
}

void signal_mavg_styler() {
//	gROOT->SetBatch();
	gStyle->SetOptStat(0);
	
//	vector<TString> cuts = {"sig", "sigprehtjec"};
	vector<TString> cuts = {"sig"};
	for (int i = 0; i < cuts.size(); ++ i) {
		draw_set(cuts[i]);
	}
}
