#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void template_compare(TString cut0="sb", TString cut1="sb", TString ds="qcdmg", bool dofit=false) {
	
	
//	TFile* tf0 = TFile::Open("/home/tote/decortication/macros/background_tools/template_plots.root.bkp");
	TFile* tf0 = TFile::Open("/home/tote/decortication/macros/background_tools/templates/template_plots_" + cut0 + ".root");
	TFile* tf1 = TFile::Open("template_plots_" + cut1 + ".root");
	
//	TH1* temp0 = (TH1*) tf0->Get(TString("temp_") + ds + "_" + cut0 + "_p_f1");
	TH1* temp0 = (TH1*) tf0->Get(TString("temp_") + ds + "_" + cut0 + "_p_f1_temp");
//	TH1* temp1 = (TH1*) tf1->Get(TString("temp_") + ds + "_" + cut1 + "_p_f1_temp");
	TH1* temp1 = (TH1*) tf1->Get(TString("temp_inj_") + cut1 + "_p_f1_temp");
	
	temp0->SetLineColor(kBlue);
	temp0->SetFillStyle(0);
	temp1->SetLineColor(kRed);
	temp1->SetFillStyle(0);
	temp0->Rebin(10);
	temp1->Rebin(10);
	
//	temp0->DrawNormalized("hist");
//	temp1->DrawNormalized("hist same");
	temp0->Draw("hist");
	temp1->Draw("hist same");
	
//	if (dofit) {
//		temp0->Rebin(10);
//		temp1->Rebin(10);
//	
//	
//		TH1* cdf0 = make_cdf(temp0, "cdf0");
//		double rescale = temp1->Integral(1, temp1->GetNbinsX())/temp0->Integral(1, temp0->GetNbinsX());
//		double amp = rescale;
//		double shift = 0.0;
//		double stretch = 1.0;
//		double ampe, shifte, stretche;
//		TH1* params = new TH1D("params", "", 3, 0, 3);
//		params->SetBinContent(1, amp);
//		params->SetBinError(1, ampe);
//		params->SetBinContent(2, shift);
//		params->SetBinError(2, shifte);
//		params->SetBinContent(3, stretch);
//		params->SetBinError(3, stretche);
//		vector<double> bins = {100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650};
//		perform_fit(temp1, cdf0, bins, params);
//		print_params(params);
//	
//		/// Make a new template from the CDF using the fitted values
//		TH1* fit = (TH1*) temp0->Clone("fit");
//		differentiate_cdf(cdf0, fit, median_from_cdf(cdf0), params->GetBinContent(1), params->GetBinContent(2), params->GetBinContent(3));
//	
//		temp0->Scale(rescale);
//		fit->SetFillStyle(0);
//		fit->SetLineColor(kBlue);
//	//	fit->Rebin(10);
//	
//		TCanvas* tc = new TCanvas("tc", "tc");
//	
//		temp1->Draw("hist");
//		temp0->Draw("hist same");
//		fit->Draw("hist same");
//	}
}
