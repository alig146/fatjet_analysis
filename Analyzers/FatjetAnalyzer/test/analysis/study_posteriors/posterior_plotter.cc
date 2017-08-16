#include "/home/tote/decortication/macros/common.cc"

TH1* save_post(TFile*tf, TString name) {
	TCanvas* tc = new TCanvas(name, name);
	gStyle->SetOptStat(0);
	TH1* h = (TH1*) tf->Get(name);
	h->GetXaxis()->SetTitle("Parameter change [pre-fit RMS]");
	h->GetYaxis()->SetTitle("Arbitrary");
	h->SetTitle("");
	h->Draw("hist");
	
	std::ostringstream oss;
	oss << "Mean = " << std::fixed << std::setprecision(2) << h->GetMean();
	style_write(oss.str(), 0.64);
	style_info(false, "2.3");
	std::ostringstream oss2;
	oss2 << "RMS = " << std::fixed << std::setprecision(2) << h->GetStdDev();
	style_write(oss2.str(), 0.649, 0.75);
	style_info(false, "2.3");
	tc->SaveAs(name + ".pdf");
	return h;
}

void posterior_plotter() {
	gROOT->SetBatch();
	TFile* tf_in = TFile::Open("theta_plots_sig15_sb_posteriors.root");
	save_post(tf_in, "Ms200__shiftTTbar__0");
	save_post(tf_in, "Ms200__stretchTTbar__0");
	save_post(tf_in, "Ms200__Normalization_ttbar__0");
	save_post(tf_in, "Ms200__shiftQCD__0");
	save_post(tf_in, "Ms200__stretchQCD__0");
	save_post(tf_in, "Ms200__Normalization_qcd__0");
}
