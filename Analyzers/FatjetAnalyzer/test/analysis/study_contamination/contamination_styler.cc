#include "/home/tote/decortication/macros/common.cc"

void draw_set(TString name, int mass) {
	TString suffix = name + "_" + to_string(mass);
	TString newname = "contamination_" + suffix;
	TString signame = "sq" + to_string(mass) + "to4j";
	TCanvas* tc = new TCanvas(newname, newname);
	THStack* hs = new THStack(TString("hs_") + suffix, "");
	
	TH1* h_qcdmg = (TH1*) gDirectory->Get(TString("qcdmg_") + name);
	h_qcdmg->SetName("qcdmg_" + suffix);
	TH1* h_ttbar = (TH1*) gDirectory->Get(TString("ttbar_") + name);
	h_ttbar->SetName("ttbar_" + suffix);
	TH1* h_sqto4j = (TH1*) gDirectory->Get(signame + "_" + name);
	h_qcdmg->Rebin(50);
	h_ttbar->Rebin(50);
	h_sqto4j->Rebin(50);
//	h_qcdmg->GetXaxis()->SetRangeUser(0, 1000);
//	h_ttbar->GetXaxis()->SetRangeUser(0, 1000);
//	h_sqto4j->GetXaxis()->SetRangeUser(0, 1000);
	
	TH1* h_bkge = (TH1*) h_qcdmg->Clone(TString("bkge_") + suffix);
	TH1* h_bkg = (TH1*) h_qcdmg->Clone(TString("bkg_") + suffix);
//	h_bkge->Sumw2();		// Seems to already be set.
	h_bkge->Add(h_ttbar);
	h_bkg->Add(h_ttbar);
	
	for (int i = 1; i < h_qcdmg->GetXaxis()->GetNbins() + 1; ++i) h_qcdmg->SetBinError(i, 0);
	for (int i = 1; i < h_ttbar->GetXaxis()->GetNbins() + 1; ++i) h_ttbar->SetBinError(i, 0);
	
	h_qcdmg->SetFillColorAlpha(kBlue, 0.1);
	h_qcdmg->SetFillStyle(1001);
	h_qcdmg->SetLineStyle(2);
	h_qcdmg->SetLineWidth(2);
	h_qcdmg->SetMarkerSize(0);
	hs->Add(h_qcdmg);
	h_ttbar->SetFillColor(kRed - 4);
	h_ttbar->SetFillStyle(3003);
	h_ttbar->SetMarkerSize(0);
	h_ttbar->SetLineWidth(2);
	hs->Add(h_ttbar);
	h_sqto4j->SetFillColorAlpha(kGreen-5, 0.5);
	h_sqto4j->SetFillStyle(1001);
	h_sqto4j->SetLineWidth(2);
	h_sqto4j->SetMarkerSize(0);
	hs->Add(h_sqto4j);
	
	
	hs->Draw("hist e");
	hs->GetXaxis()->SetRangeUser(0, 1000);
	hs->SetMaximum(hs->GetMaximum()*1.2);
	hs->GetXaxis()->SetNdivisions(406);
	h_bkg->SetFillStyle(0);
	h_bkg->Draw("same hist");
	h_bkge->SetMarkerSize(0);
//	h_bkge->SetFillStyle(3244);
	h_bkge->SetFillColorAlpha(kBlack, 0.2);
	h_bkge->Draw("same e2");
	
	
	/// Legend:
	TLegend* leg = new TLegend(0.47, 0.60, 0.77, 0.75);
	leg->AddEntry(h_qcdmg, "QCD (2 #rightarrow 4)", "f");
	leg->AddEntry(h_ttbar, "t#bar{t}", "f");
	leg->AddEntry(h_bkge, "Total background", "lf");
	leg->AddEntry(h_sqto4j, TString("#it{m}_{#tilde{q}} = ") + to_string(mass) + " GeV", "lfe");
	leg->Draw();
	/// Axes:
	hs->GetXaxis()->SetTitle("Average jet mass [GeV]");
	style_ylabel(hs);
	
	// Write info:
	style_info();
	
	double sb = h_sqto4j->Integral()/(h_ttbar->Integral() + h_qcdmg->Integral());
	std::ostringstream oss;
	oss << "s/b = " << std::fixed << std::setprecision(1) << sb * 100 << " %";
	style_write(oss.str().c_str(), 0.6, 0.50);
	
	tc->SaveAs(newname + ".pdf");
	
	cout << "bkg integral = " << h_qcdmg->Integral() + h_ttbar->Integral() << endl;
}

void contamination_styler(TString sb_name="sbl", TString cut_name="sig") {
	gROOT->SetBatch();
	gStyle->SetOptStat(0);
	
	TFile* tf = TFile::Open("contamination_plots.root");
	
	vector<int> masses = {100, 150, 200, 250, 300, 400, 500};
	for (int i = 0; i < masses.size(); ++ i) {
		draw_set(cut_name, masses[i]);
		draw_set(sb_name, masses[i]);
		draw_set(sb_name + "b", masses[i]);
	}
}
