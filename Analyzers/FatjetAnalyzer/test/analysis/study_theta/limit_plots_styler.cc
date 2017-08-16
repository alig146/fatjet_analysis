#include "/home/tote/decortication/macros/common.cc"

void limit_plots_styler(TString cut="sig15") {
	TFile* tf_in = TFile::Open("theta_plots_" + cut + "_sb_output.root");
	
	TGraph* cross = (TGraph*) tf_in->Get("cross");
	TGraph* cross_1 = (TGraph*) tf_in->Get("cross_sigma1");
	TGraph* exp = (TGraph*) tf_in->Get("expected_graph");
	TGraph* exp_1 = (TGraph*) tf_in->Get("expected_sigma1");
	TGraph* exp_2 = (TGraph*) tf_in->Get("expected_sigma2");
	TGraph* obs = (TGraph*) tf_in->Get("observed_graph");
	
	exp_1->SetFillColor(kGreen);
	exp_2->SetFillColor(kYellow);
	exp->SetLineStyle(2);
	cross->SetLineColor(kBlue);
	cross_1->SetLineColor(kBlue);
	cross_1->SetFillColorAlpha(kBlue-10, 0.7);
	cross->SetLineStyle(5);
	cross_1->SetLineStyle(5);
	
	TCanvas* tc = new TCanvas("tc", "tc");
	tc->SetLogy(1);
	
	cross_1->Draw("a3");
	exp_2->Draw("3");
	exp_1->Draw("3");
	exp->Draw("l");
	obs->Draw("l");
	cross_1->Draw("3");
	cross->Draw("l");
	
	cross_1->SetMaximum(3000);
	cross_1->SetMinimum(0.06);
	if (cut == "sig15") {
		cross_1->SetMaximum(7000);
		cross_1->SetMinimum(0.20);
	}
	
	cross_1->GetXaxis()->SetNdivisions(405);
//	cross_1->GetXaxis()->SetRangeUser(100, 500);
	cross_1->GetXaxis()->SetLimits(100., 500.);
//	cross_1->GetXaxis()->SetRange(100, 500);
	cross_1->GetXaxis()->SetTitle("#it{m}_{#tilde{q}}  [GeV]");
	cross_1->GetYaxis()->SetTitle("Upper limit on #sigma_{#tilde{q}} #times #it{B}");
	
//	TLegend* leg = new TLegend(0.20, 0.65, 0.50, 0.80);
	TLegend* leg = new TLegend(0.50, 0.65, 0.80, 0.80);
	leg->AddEntry(cross_1, "Theoretical cross section", "fl");
	leg->AddEntry(obs, "Observed limit", "l");
	leg->AddEntry(exp, "Expected limit", "l");
	leg->AddEntry(exp_1, "Expected limit #pm 1#sigma", "f");
	leg->AddEntry(exp_2, "Expected limit #pm 2#sigma", "f");
	leg->Draw();
	
	if (cut == "sig15") style_info(false, "2.3");
	else style_info(false);
	
	gPad->RedrawAxis();
	TLine *tl = new TLine(500, cross_1->GetMinimum(), 500, cross_1->GetMaximum());
//	tl->SetLineWidth(2);
//	tl->SetLineColor(kRed);
	tl->Draw();
	tc->SaveAs(TString("limits_") + cut + ".pdf");
	
	return;
}
