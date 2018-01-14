#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void draw_set(TString cut, bool norm=false) {
//	vector<int> masses = {100, 150, 200, 300, 400, 500};
//	vector<int> colors = {kRed, kOrange, kGreen, kBlue, kViolet, kBlack};
	vector<int> masses = {100, 200, 300, 400, 500};
	vector<int> colors = {kRed-4, kOrange-1, kSpring+4, kBlue-5, kViolet-6};
	TString suffix = "_" + cut;
	if (norm) suffix = suffix + "_norm";
	TString name = "mavg_sqto4j" + suffix;
	TCanvas* tc = new TCanvas(name, name);
//	TLegend* leg = new TLegend(0.60, 0.58, 0.80, 0.80);
	TLegend* leg = get_legend(1, masses.size(), 0.8);
	TFile* tf_in = TFile::Open("../study_basic/basic_plots_sqto4j.root");
	
	for (int i = 0; i < masses.size(); ++ i) {
		TString signame = "sq" + to_string(masses[i]) + "to4j";
		
//		TFile* tf_in;
//		if (signame == "sq100to4j") tf_in = TFile::Open("../study_basic/basic_plots_sq100.root");		// KLUDGE
//		tf_in = TFile::Open("../study_basic/basic_plots_sqto4j.root");
		
		TH1* h_sqto4j = (TH1*) tf_in->Get(TString("mavg_") + signame + "_" + cut);
//		if (signame == "sq100to4j") h_sqto4j->Scale(0.1);		// KLUDGE
		h_sqto4j->Rebin(30);
		TH1* h_sqto4je = (TH1*) h_sqto4j->Clone(signame + suffix + "_e");
		h_sqto4j->SetLineColor(colors[i]);
		h_sqto4j->SetMarkerSize(0);
		h_sqto4j->SetFillStyle(1001);
		h_sqto4j->SetFillColorAlpha(colors[i], 0.3);
		if (norm) h_sqto4j->Scale(1.0/h_sqto4j->Integral());
		if (norm) {
			h_sqto4j->SetMaximum(0.7);
			h_sqto4j->SetMinimum(0);
		}
		else {
			h_sqto4j->SetMaximum(350);
//			h_sqto4j->SetMinimum(0);
		}
		h_sqto4j->SetTitle("");
		h_sqto4j->GetXaxis()->SetTitle(get_xtitle("mavg"));
		h_sqto4j->GetXaxis()->SetRangeUser(0, 700);
		
		if (norm) h_sqto4je->Scale(1.0/h_sqto4je->Integral());
		h_sqto4je->SetFillStyle(1001);
//		h_sqto4je->SetFillColor(colors[i]);
		h_sqto4je->SetFillColorAlpha(kBlack, 0.2);
		h_sqto4je->SetLineColor(colors[i]);
		h_sqto4je->SetMarkerSize(0);
		
		style_ylabel(h_sqto4j, "GeV", "Events");
		h_sqto4j->Draw("same hist");
		h_sqto4je->Draw("same e2");
		leg->AddEntry(h_sqto4j, name_proper[signame], "f");
	}
	leg->Draw();
	style_info();
	style_cut(cut);
	gPad->RedrawAxis();
	tc->SaveAs(name + ".pdf");
	tc->SetLogy();
	tc->SaveAs(name + "_logy.pdf");
}

void signal_mavg_styler() {
//	gROOT->SetBatch();
	gStyle->SetOptStat(0);
	
	vector<TString> cuts = {"sig"};
	for (int i = 0; i < cuts.size(); ++ i) {
		draw_set(cuts[i]);
	}
}
