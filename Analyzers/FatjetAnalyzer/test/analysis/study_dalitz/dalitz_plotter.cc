#include "/home/tote/decortication/macros/common.cc"

TH2* smmijhat_plot(TString i="0", TString j="1", TString ds_name="qcdmg", TString cut_name="sig") {
	TString name = "smm" + i + j + "hat_" + ds_name + "_" + cut_name;
	gStyle->SetOptStat(0);
	TTree* tt = (TTree*) gDirectory->Get(ds_name);
	tt->Draw("smm" + i + "hat[0]:smm" + j + "hat[0]>>h(20,0,1.0,20,0,1.0)", *get_cut("fjp_" + cut_name));
	TH2* h = (TH2*) gDirectory->Get("h");
	h->SetName(name);
	h->SetTitle("");
	h->GetXaxis()->SetTitle("Leading jet #hat{#it{m}}_{" + j + "}^{2}");
	h->GetYaxis()->SetTitle("Leading jet #hat{#it{m}}_{" + i + "}^{2}");
	style_zlabel(h);
//	style_info(false, "31.3", 4);
	return h;
}

TH1* sd_plot(TString ds_name, TString cut_name, Double_t weight=1.0, TString var="sd") {
	TString name = var + "_" + ds_name + "_" + cut_name;
	TTree* tt = (TTree*) gDirectory->Get(ds_name);
	tt->Draw(var + ">>" + name + "(25,0,0.5)", *get_cut("fjp_" + cut_name, weight));
	TH1* h = (TH1*) gDirectory->Get(name);
	return h;
}

void dalitz_plotter() {
//	TFile* tf_in = TFile::Open("anatuple_dalitz.root");
	TFile* tf_in = TFile::Open("anatuple_dalitz_predeta.root");
	TFile* tf_out = new TFile("dalitz_plots.root", "RECREATE");
	vector<TH1*> plots;
	tf_in->cd();
	
	Double_t weight = (29.128 + 2.183)/2.183;
	vector<TString> sig_names = {"sq100to4j", "sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j"};
	vector<TString> bkg_names = {"qcdmg", "qcdp", "ttbar", "wjets"};
	
	// sd:
	/// sigl:
	plots.push_back(sd_plot("jetht", "sigl"));
	for (vector<TString>::iterator bkg_name = bkg_names.begin(); bkg_name != bkg_names.end(); ++bkg_name) {
		plots.push_back(sd_plot(*bkg_name, "sigl", weight));
	}
	for (vector<TString>::iterator sig_name = sig_names.begin(); sig_name != sig_names.end(); ++sig_name) {
		plots.push_back(sd_plot(*sig_name, "sigl", weight));
	}
	/// sb:
	plots.push_back(sd_plot("jetht", "sb"));
	for (vector<TString>::iterator bkg_name = bkg_names.begin(); bkg_name != bkg_names.end(); ++bkg_name) {
		plots.push_back(sd_plot(*bkg_name, "sb", weight));
	}
	for (vector<TString>::iterator sig_name = sig_names.begin(); sig_name != sig_names.end(); ++sig_name) {
		plots.push_back(sd_plot(*sig_name, "sb", weight));
	}
	/// sbb:
	plots.push_back(sd_plot("jetht", "sbb"));
	for (vector<TString>::iterator bkg_name = bkg_names.begin(); bkg_name != bkg_names.end(); ++bkg_name) {
		plots.push_back(sd_plot(*bkg_name, "sbb", weight));
	}
	for (vector<TString>::iterator sig_name = sig_names.begin(); sig_name != sig_names.end(); ++sig_name) {
		plots.push_back(sd_plot(*sig_name, "sbb", weight));
	}
	
	
	tf_out->cd();
	for(vector<TH1*>::iterator plot=plots.begin(); plot != plots.end(); ++plot) {
		(*plot)->Write();
	}
}
