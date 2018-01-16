#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void jec_styler() {
	gStyle->SetOptStat(0);
	TFile* tf_in = TFile::Open("jets_plots.root");
	
	vector<TString> masses = {"100", "200", "300", "400", "500"};
//	vector<TString> masses = {"100"};
	
	for (unsigned i = 0; i < masses.size(); ++i) {
		TString m = masses[i];
		TString name = "jec0ak8_jec0_sq" + m + "to4j";
		
		// No pT cut:
		TH2* h_jec = (TH2*) tf_in->Get(TString("jec0ak8_jec0_sq") + m + "to4j");
		TCanvas* tc = new TCanvas(name, name);
		
		h_jec->Rebin2D(30, 30);
		h_jec->GetXaxis()->SetTitle("Leading AK8 jet JEC");
		h_jec->GetYaxis()->SetTitle("Leading CA12 jet JEC");
		h_jec->GetZaxis()->SetTitle("Events");
		h_jec->Draw("colz");
	
		TLine* line = new TLine(0.8, 0.8, 1.2, 1.2);
		line->SetLineColorAlpha(kRed, 0.5);
		line->SetLineStyle(2);
		line->SetLineWidth(2);
		line->Draw("same");
	
		gPad->RedrawAxis();
	
		style_info(true, lum_string["all"], 0);
	//	style_cut("#it{H}_{T} > 900 GeV");
		style_write(name_proper["sq" + m + "to4j"], 0.21, 0.78);
		save(tc);
		
		// pT cut:
		TH2* h_jec_cutpt = (TH2*) tf_in->Get(TString("jec0ak8_jec0_pt300to500_sq") + m + "to4j");
		name += "_cutpt";
		TCanvas* tc_cutpt = new TCanvas(name, name);
		
		h_jec_cutpt->Rebin2D(30, 30);
		h_jec_cutpt->GetXaxis()->SetTitle("Leading AK8 jet JEC");
		h_jec_cutpt->GetYaxis()->SetTitle("Leading CA12 jet JEC");
		h_jec_cutpt->GetZaxis()->SetTitle("Events");
		h_jec_cutpt->Draw("colz");
	
		TLine* line_cutpt = new TLine(0.8, 0.8, 1.2, 1.2);
		line_cutpt->SetLineColorAlpha(kRed, 0.5);
		line_cutpt->SetLineStyle(2);
		line_cutpt->SetLineWidth(2);
		line_cutpt->Draw("same");
	
		gPad->RedrawAxis();
	
		style_info(true, lum_string["all"], 0);
		style_write(name_proper["sq" + m + "to4j"], 0.21, 0.78);
		style_cut("300 GeV < p_{T} < 500 GeV");
		save(tc_cutpt);
	}
}
