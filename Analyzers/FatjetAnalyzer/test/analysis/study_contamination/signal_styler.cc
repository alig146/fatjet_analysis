#include "/home/tote/decortication/macros/common.cc"

void draw_set(TString cut, bool norm=false) {
//	vector<int> masses = {100, 150, 200, 300, 400, 500};
//	vector<int> colors = {kRed, kOrange, kGreen, kBlue, kViolet, kBlack};
	vector<int> masses = {100, 200, 300, 400, 500};
	vector<int> colors = {kRed-4, kOrange-1, kSpring+4, kBlue-5, kViolet-6};
	TString suffix = "_" + cut;
	if (norm) suffix = suffix + "_norm";
	TString fullname = "signals_mavg" + suffix;
	TCanvas* tc = new TCanvas(fullname, fullname);
	TLegend* leg = new TLegend(0.60, 0.58, 0.80, 0.80);
	
	for (int i = 0; i < masses.size(); ++ i) {
		TString signame = "sq" + to_string(masses[i]) + "to4j";
		TH1* h_sqto4j = (TH1*) gDirectory->Get(signame + suffix);
		h_sqto4j->Rebin(20);
		TH1* h_sqto4je = (TH1*) h_sqto4j->Clone(signame + suffix + "_e");
		h_sqto4j->SetLineColor(colors[i]);
		h_sqto4j->SetMarkerSize(0);
		h_sqto4j->SetFillColorAlpha(colors[i], 0.3);
		if (norm) h_sqto4j->Scale(1.0/h_sqto4j->Integral());
		if (norm) {
			h_sqto4j->SetMaximum(0.6);
			h_sqto4j->SetMinimum(0);
		}
		else {
			h_sqto4j->SetMaximum(280);
			h_sqto4j->SetMinimum(1);
		}
		h_sqto4j->SetTitle("");
		h_sqto4j->GetXaxis()->SetTitle("Average jet mass [GeV]");
		h_sqto4j->GetXaxis()->SetRangeUser(0, 700);
		
		if (norm) h_sqto4je->Scale(1.0/h_sqto4je->Integral());
//		h_sqto4je->SetFillStyle(3144);
//		h_sqto4je->SetFillColor(colors[i]);
		h_sqto4je->SetFillColorAlpha(kBlack, 0.2);
		h_sqto4je->SetLineColor(colors[i]);
		h_sqto4je->SetMarkerSize(0);
		
		style_ylabel(h_sqto4j, "GeV", "Events");
		h_sqto4j->Draw("same hist");
		h_sqto4je->Draw("same e2");
		leg->AddEntry(h_sqto4je, TString("#it{m}_{#tilde{q}} = ") + to_string(masses[i]) + " GeV", "lf");
	}
	leg->Draw();
	style_info();
	gPad->RedrawAxis();
	tc->SaveAs(fullname + ".pdf");
	tc->SetLogy();
	tc->SaveAs(fullname + "_logy.pdf");
//	TH1* h_bkge = (TH1*) h_qcdmg->Clone(TString("bkge_") + suffix);
//	TH1* h_bkg = (TH1*) h_qcdmg->Clone(TString("bkg_") + suffix);
////	h_bkge->Sumw2();		// Seems to already be set.
//	h_bkge->Add(h_ttbar);
//	h_bkg->Add(h_ttbar);
//	
//	for (int i = 1; i < h_qcdmg->GetXaxis()->GetNbins() + 1; ++i) h_qcdmg->SetBinError(i, 0);
//	for (int i = 1; i < h_ttbar->GetXaxis()->GetNbins() + 1; ++i) h_ttbar->SetBinError(i, 0);
//	
//	h_qcdmg->SetFillColorAlpha(kBlue-10, 0.5);
//	h_qcdmg->SetFillStyle(1001);
//	h_qcdmg->SetLineStyle(2);
//	h_qcdmg->SetLineWidth(2);
//	h_qcdmg->SetMarkerSize(0);
//	hs->Add(h_qcdmg);
//	h_ttbar->SetFillColor(kRed - 4);
//	h_ttbar->SetFillStyle(3003);
//	h_ttbar->SetMarkerSize(0);
//	h_ttbar->SetLineWidth(2);
//	hs->Add(h_ttbar);
//	h_sqto4j->SetFillColorAlpha(kGreen-5, 0.5);
//	h_sqto4j->SetFillStyle(1001);
//	h_sqto4j->SetLineWidth(2);
//	h_sqto4j->SetMarkerSize(0);
//	hs->Add(h_sqto4j);
//	
//	hs->Draw("hist e");
//	hs->SetMaximum(hs->GetMaximum()*1.2);
//	hs->GetXaxis()->SetNdivisions(406);
//	h_bkg->SetFillStyle(0);
//	h_bkg->Draw("same hist");
//	h_bkge->SetMarkerSize(0);
////	h_bkge->SetFillStyle(3244);
//	h_bkge->SetFillColorAlpha(kBlack, 0.2);
//	h_bkge->Draw("same e2");
//	
//	
//	/// Legend:
//	leg->AddEntry(h_qcdmg, "QCD (2 #rightarrow 4)", "lf");
//	leg->AddEntry(h_ttbar, "t#bar{t}", "lf");
//	leg->AddEntry(h_bkge, "Total background", "lf");
//	leg->Draw();
//	/// Axes:
//	hs->GetXaxis()->SetTitle("Average jet mass [GeV]");
//	style_ylabel(hs);
//	
//	// Write info:
//	
//	double sb = h_sqto4j->Integral()/(h_ttbar->Integral() + h_qcdmg->Integral());
//	std::ostringstream oss;
//	oss << "s/b = " << std::fixed << std::setprecision(1) << sb * 100 << " %";
//	style_write(oss.str().c_str(), 0.6, 0.50);
//	
//	tc->SaveAs(newname + ".pdf");
//	
//	cout << "bkg integral = " << h_qcdmg->Integral() + h_ttbar->Integral() << endl;
}

void signal_styler() {
//	gROOT->SetBatch();
	gStyle->SetOptStat(0);
	
	TFile* tf = TFile::Open("contamination_plots_fine.root");
	
	vector<TString> cuts = {"sig"};
	for (int i = 0; i < cuts.size(); ++ i) {
		draw_set(cuts[i]);
	}
}
