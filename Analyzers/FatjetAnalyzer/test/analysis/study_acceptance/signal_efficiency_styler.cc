#include "/home/tote/decortication/macros/common.cc"

void signal_efficiency_styler() {
	TFile* tf_in = TFile::Open("signal_efficiency_plots.root");
	TH1* pass_sq = (TH1*) tf_in->Get("pass_sq");
	TH1* pass_pre_sq = (TH1*) tf_in->Get("pass_pre_sq");
	TH1* total_sq = (TH1*) tf_in->Get("total_sq");
	TH1* pass_sg = (TH1*) tf_in->Get("pass_sg");
	TH1* pass_pre_sg = (TH1*) tf_in->Get("pass_pre_sg");
	TH1* total_sg = (TH1*) tf_in->Get("total_sg");
	TString name = "eff_sig";
	
	gStyle->SetOptStat(0);
	
	TCanvas* tc = new TCanvas(name, name);
	// sq:
	TEfficiency* teff_sq = new TEfficiency(*pass_sq, *total_sq);
	TGraphAsymmErrors* eff_sq = teff_sq->CreateGraph();
	TEfficiency* teff_pre_sq = new TEfficiency(*pass_pre_sq, *total_sq);
	TGraphAsymmErrors* eff_pre_sq = teff_pre_sq->CreateGraph();
	for (unsigned i = 0; i < 10; ++i){
		eff_sq->SetPointEXhigh(i, 0);
		eff_sq->SetPointEXlow(i, 0);
		eff_pre_sq->SetPointEXhigh(i, 0);
		eff_pre_sq->SetPointEXlow(i, 0);
	}
	// sg:
	TEfficiency* teff_sg = new TEfficiency(*pass_sg, *total_sg);
	TGraphAsymmErrors* eff_sg = teff_sg->CreateGraph();
	TEfficiency* teff_pre_sg = new TEfficiency(*pass_pre_sg, *total_sg);
	TGraphAsymmErrors* eff_pre_sg = teff_pre_sg->CreateGraph();
	for (unsigned i = 0; i < 13; ++i){
		eff_sg->SetPointEXhigh(i, 0);
		eff_sg->SetPointEXlow(i, 0);
		eff_pre_sg->SetPointEXhigh(i, 0);
		eff_pre_sg->SetPointEXlow(i, 0);
	}
	
	
//	total_sg->Draw("histe");
	
//	cout << teff_sg->GetConfidenceLevel() << endl;
//	teff_sg->Draw("alpe");
//	eff_sg->SetMarkerColor(kBlue);
//	eff_sg->SetLineColor(kBlue);
////	eff_sg->Draw("pe same");
	
//	return;
	
	
	eff_sq->SetLineWidth(2);
//	eff_pre_sq->SetLineColor(kBlue);
	eff_pre_sq->SetLineStyle(2);
	eff_pre_sq->SetLineWidth(2);
//	eff_pre_sq->SetMarkerColor(kBlack);
	
	eff_sg->SetLineWidth(2);
	eff_sg->SetLineColor(kRed);
	eff_sg->SetMarkerColor(kRed);
	eff_sg->SetMarkerStyle(21);
	eff_pre_sg->SetLineColor(kRed);
	eff_pre_sg->SetMarkerColor(kRed);
	eff_pre_sg->SetMarkerStyle(21);
	eff_pre_sg->SetLineStyle(6);
	eff_pre_sg->SetLineWidth(2);
	
	eff_sq->Draw("alpz");
	eff_sg->Draw("lpz same");
	eff_pre_sg->Draw("lpz same");
	eff_pre_sq->Draw("lpz same");
	eff_sq->Draw("lpz same");
	
	set_xtitle(eff_sq, "msq");
	eff_sq->GetYaxis()->SetTitle("Selection acceptance");
	eff_sq->GetXaxis()->SetLimits(75, 725);
	eff_sq->GetXaxis()->SetNdivisions(207);
	eff_sq->SetMaximum(1.0);
	eff_sq->SetMinimum(5.0e-8);
	
	TLegend* leg = get_legend(2, 4, 1.2, 0.8);
	leg->AddEntry(eff_pre_sq, "Squark acceptance", "lp");
	leg->AddEntry(eff_sq, "Squark accept. #times eff.", "lp");
	leg->AddEntry(eff_pre_sg, "Gluino acceptance", "lp");
	leg->AddEntry(eff_sg, "Gluino accept. #times eff.", "lp");
	leg->Draw();
	
	style_info(true, lum_string["all"], 0);
//	style_cut("sig");
	
	tc->SetLogy();
	save(tc);
}
