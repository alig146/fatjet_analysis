#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void trigger_styler() {
	TFile* tf = TFile::Open("RUNTriggerEfficiencies_SingleMuon_Run2016_V2p1_v03.root");
	vector<TH1*> hs;
	vector<TCanvas*> canvases;
	
	TH2* total = (TH2*) tf->Get("BoostedTriggerEfficiencySeveralTriggers/jet1PrunedMassHTDenom_cutDijet");
	TH2* pass = (TH2*) tf->Get("BoostedTriggerEfficiencySeveralTriggers/jet1PrunedMassHTPassing_cutDijet");
	
	TH1* total_ht = total->ProjectionY("total_ht");
	TH1* pass_ht = pass->ProjectionY("pass_ht");
	TH1* total_m = total->ProjectionX("total_m", total->GetYaxis()->FindBin(900), total->GetNbinsY());
	TH1* total_m850 = total->ProjectionX("total_m850", total->GetYaxis()->FindBin(850), total->GetNbinsY());
	TH1* total_m_check = total->ProjectionX("total_m_check");
	TH1* pass_m = pass->ProjectionX("pass_m", pass->GetYaxis()->FindBin(900), pass->GetNbinsY());
	TH1* pass_m850 = pass->ProjectionX("pass_m850", pass->GetYaxis()->FindBin(850), pass->GetNbinsY());
	
//	total_ht->Rebin(10);
//	pass_ht->Rebin(10);
	total_ht->Rebin(5);
	pass_ht->Rebin(5);
	pass_ht->SetLineColor(kRed);
	
	
//	total_m->Rebin(3);
//	pass_m->Rebin(3);
	
	
	TEfficiency* teff_ht = new TEfficiency(*pass_ht, *total_ht);
	TGraphAsymmErrors* eff_ht = teff_ht->CreateGraph();
	TGraphAsymmErrors* eff_hte = (TGraphAsymmErrors*) eff_ht->Clone("eff_hte");
	
	eff_ht->GetXaxis()->SetRangeUser(0, 2000);
	eff_ht->GetXaxis()->SetNdivisions(805);
	eff_ht->SetMinimum(0.0);
	eff_ht->SetMaximum(1.2);
	eff_ht->GetXaxis()->SetRangeUser(0, 2000);
	eff_ht->GetYaxis()->SetTitle("Trigger efficiency");
	eff_ht->GetXaxis()->SetTitle(get_xtitle("ht"));
	eff_hte->SetFillColor(kRed);
	
	TEfficiency* teff_m = new TEfficiency(*pass_m, *total_m);
	TGraphAsymmErrors* eff_m = teff_m->CreateGraph();
//	eff_m->SetMinimum(0.5);
	eff_m->SetMinimum(0.0);
	eff_m->SetMaximum(1.2);
	eff_m->GetXaxis()->SetRangeUser(0, 500);
	eff_m->GetYaxis()->SetTitle("Trigger efficiency");
	eff_m->GetXaxis()->SetTitle(get_xtitle("m0ak8"));
	
	TEfficiency* teff_m850 = new TEfficiency(*pass_m850, *total_m850);
	TGraphAsymmErrors* eff_m850 = teff_m850->CreateGraph();
	eff_m850->SetMinimum(0.5);
	eff_m850->SetMaximum(1.2);
	eff_m850->GetXaxis()->SetRangeUser(0, 500);
	eff_m850->GetYaxis()->SetTitle("Trigger efficiency");
	eff_m850->GetXaxis()->SetTitle(get_xtitle("m0ak8"));
	
//	eff_ht->Draw("ap");
//	eff_ht->Draw("al");
//	eff_hte->Draw("3");
//	eff_ht->Draw("l");




	// HT plot canvas:
	TCanvas* tc_ht = new TCanvas("eff_ht", "eff_ht");
	
	eff_ht->Draw("ap");
	tc_ht->Update();
	
	Float_t axis_max_ht = 1.4*total_ht->GetMaximum();
	Float_t scale_ht = gPad->GetUymax()/axis_max_ht;
	total_ht->Scale(scale_ht);
	pass_ht->Scale(scale_ht);
	total_ht->SetFillStyle(0);
	total_ht->SetLineColor(kBlue - 9);
//	total_ht->SetLineColorAlpha(kBlue, 0.5);
//	pass_ht->SetFillStyle(3005);
	pass_ht->SetFillStyle(1001);
	pass_ht->SetLineWidth(0);
	pass_ht->SetLineColor(kBlue - 9);
	pass_ht->SetFillColor(kBlue - 9);
//	pass_ht->SetLineColorAlpha(kBlue, 0.5);
//	pass_ht->SetFillColorAlpha(kBlue, 0.5);
	total_ht->Draw("hist,same");
	pass_ht->Draw("hist,same");
	TGaxis* axis_ht = new TGaxis(
		gPad->GetUxmax(), gPad->GetUymin(),
		gPad->GetUxmax(), gPad->GetUymax(),
		0, axis_max_ht, 510, "L+"
	);
	axis_ht->SetLineColor(kBlue - 9);
	axis_ht->SetTextColor(kBlue - 9);
	axis_ht->SetTitleFont(43);
	axis_ht->SetTitleSize(50);
	axis_ht->SetTitle("Events/50 GeV");
	axis_ht->SetTitleOffset(1.6);
	axis_ht->SetLabelColor(kBlue - 9);
	axis_ht->SetLabelFont(43);
	axis_ht->SetLabelSize(50*0.83);
	axis_ht->Draw();
	
	TLine* line1_ht = new TLine(900, 0, 900, 1.2);
	line1_ht->SetLineColorAlpha(kRed, 0.5);
	line1_ht->SetLineWidth(2);
	line1_ht->SetLineStyle(2);
	line1_ht->Draw();
	TLine* line2_ht = new TLine(0, 0.90, 2020, 0.90);
//	line2_ht->SetLineColorAlpha(kBlue, 0.5);
	line2_ht->SetLineColorAlpha(kBlack, 0.5);
	line2_ht->SetLineWidth(2);
	line2_ht->SetLineStyle(6);
	line2_ht->Draw();
	eff_ht->Draw("p");
	
	TLegend* leg_ht = get_legend(2, 5, 1.0, 0.80);
	leg_ht->AddEntry(eff_ht, "Trigger efficiency", "pl");
	leg_ht->AddEntry(line2_ht, "90% efficient", "l");
	leg_ht->AddEntry(line1_ht, "#it{H}_{T} = 900 GeV", "l");
	leg_ht->AddEntry(total_ht, "Efficiency denominator", "f");
	leg_ht->AddEntry(pass_ht, "Efficiency numerator", "f");
	leg_ht->Draw();
	
	
	gPad->RedrawAxis();
	
	style_info(false, lum_string["16"], 0, false, 0.20);
	
	save(tc_ht);
	


	
	// Mass plot canvas:
	TCanvas* tc_m = new TCanvas("eff_m", "eff_m");
	
	eff_m->Draw("ap");
	tc_m->Update();
	
	Float_t axis_max_m = 1.5*total_m->GetMaximum();
	Float_t scale_m = gPad->GetUymax()/axis_max_m;
	total_m->Scale(scale_m);
	pass_m->Scale(scale_m);
	total_m->SetFillStyle(0);
	total_m->SetLineColor(kBlue - 9);
//	total_m->SetLineColorAlpha(kBlue, 0.5);
//	pass_m->SetFillStyle(3005);
	pass_m->SetFillStyle(1001);
	pass_m->SetLineWidth(0);
	pass_m->SetLineColor(kBlue - 9);
	pass_m->SetFillColor(kBlue - 9);
//	pass_m->SetLineColorAlpha(kBlue, 0.5);
//	pass_m->SetFillColorAlpha(kBlue, 0.5);
	total_m->Draw("hist,same");
	pass_m->Draw("hist,same");
	TGaxis* axis_m = new TGaxis(
		gPad->GetUxmax(), gPad->GetUymin(),
		gPad->GetUxmax(), gPad->GetUymax(),
		0, axis_max_m, 510, "L+"
	);
	axis_m->SetLineColor(kBlue - 9);
	axis_m->SetTextColor(kBlue - 9);
	axis_m->SetTitleFont(43);
	axis_m->SetTitleSize(50);
	axis_m->SetTitle("Events/10 GeV");
	axis_m->SetTitleOffset(1.6);
	axis_m->SetLabelColor(kBlue - 9);
	axis_m->SetLabelFont(43);
	axis_m->SetLabelSize(50*0.83);
	axis_m->Draw();
	
	TLine* line1_m = new TLine(100, 0.0, 100, 1.2);
	line1_m->SetLineColorAlpha(kRed, 0.5);
	line1_m->SetLineWidth(2);
	line1_m->SetLineStyle(2);
	line1_m->Draw();
	TLine* line2_m = new TLine(0, 0.99, 502, 0.99);
	line2_m->SetLineColorAlpha(kBlack, 0.5);
	line2_m->SetLineWidth(2);
	line2_m->SetLineStyle(6);
	line2_m->Draw();
	eff_m->Draw("p");
	
	TLegend* leg_m = get_legend(2, 5, 1.0, 0.80);
	leg_m->AddEntry(eff_m, "Trigger efficiency", "pl");
	leg_m->AddEntry(line2_m, "99% efficient", "l");
	leg_m->AddEntry(line1_m, "#it{m} = 100 GeV", "l");
	leg_m->AddEntry(total_m, "Efficiency denominator", "f");
	leg_m->AddEntry(pass_m, "Efficiency numerator", "f");
	leg_m->Draw();
	
	gPad->RedrawAxis();
	
	style_info(false, lum_string["16"], 1, false, 0.50);
	style_cut("ht900");
	
	save(tc_m);
	
	
	
	
	
	
	// Mass plot canvas (850 GeV cut):
	TCanvas* tc_m850 = new TCanvas("eff_m850", "eff_m850");
	
	eff_m850->Draw("ap");
	
	TLine* line1_m850 = new TLine(100, 0.5, 100, 1.2);
	line1_m850->SetLineColorAlpha(kRed, 0.5);
	line1_m850->SetLineWidth(2);
	line1_m850->SetLineStyle(2);
	line1_m850->Draw();
	TLine* line2_m850 = new TLine(0, 0.99, 502, 0.99);
	line2_m850->SetLineColorAlpha(kBlue, 0.5);
	line2_m850->SetLineWidth(2);
	line2_m850->SetLineStyle(2);
	line2_m850->Draw();
	eff_m850->Draw("p");
	
	gPad->RedrawAxis();
	
	style_info(false, lum_string["16"]);
	style_cut("ht850");
	
	save(tc_m850);
}
