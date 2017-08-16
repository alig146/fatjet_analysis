#include "/home/tote/decortication/macros/common.cc"

bool VERBOSE = true;

void analysis_plotter(TString cut="sbb", int f=1, TString groom_name="p") {
	// Arguments:
	vector<int> masses = {100, 150, 200, 250, 300, 400, 500};
	TString name = cut;
	if (groom_name != "p") {
		name += "_" + groom_name;
		cut += "_" + groom_name;
	}
	name += "_f" + to_string(f);
	
	// Options:
	gROOT->SetBatch();
	
	// Define input and output:
	TFile *tfile = get_ana(cut);
	TFile* tf_out = new TFile("analysis_plots_" + name + ".root", "RECREATE");
	
	// Define TTrees:
	TTree* tt_ttbar = (TTree*) tfile->Get("ttbar");
	TTree* tt_jetht = (TTree*) tfile->Get("jetht");
	
	// Make plots:
	/// Make FJP plots (jet pair selection):
//	double weight = get_weight();
	double weight = 1;
	if (cut == "sig15") weight = get_weight("", "15");
	tt_ttbar->Draw("mavg_" + groom_name + ">>fjp_ttbar(1200,0,1200)", get_cut("fjp_" + cut, weight));
	TH1* h_fjp_ttbar = (TH1*) gDirectory->Get("fjp_ttbar");
	tt_jetht->Draw("mavg_" + groom_name + ">>fjp_jetht(1200,0,1200)", get_cut("fjp_" + cut));
	TH1* h_fjp_jetht = (TH1*) gDirectory->Get("fjp_jetht");
	for (int i = 0; i < masses.size(); ++i) {
		TString sig_name = "sq" + to_string(masses[i]) + "to4j";
		TTree* tt = (TTree*) tfile->Get(sig_name);
		tt->Draw("mavg_" + groom_name + ">>fjp_" + sig_name + "(1200,0,1200)", get_cut("fjp_" + cut, weight));
		TH1* fjp = (TH1*) gDirectory->Get("fjp_" + sig_name);
		TH1* cdf = make_cdf(fjp, "cdf_" + sig_name);
		tf_out->WriteTObject(fjp);
		tf_out->WriteTObject(cdf);
	}
	
	/// Template:
	TH1* h_temp_jetht = fetch_template("jetht", cut, "", f);		// WARNING: This doesn't use a different groomer.
	h_temp_jetht->SetName("temp_jetht");
//	if (cut == "sig15" || cut == "sig") h_temp_jetht->Scale(h_fjp_jetht->Integral()/h_temp_jetht->Integral());
	h_temp_jetht->Scale(h_fjp_jetht->Integral()/h_temp_jetht->Integral());
	
	// Make CDFs:
//	if (cut == "sbl" || cut == "sbb") h_fjp_ttbar->Rebin(10);
	if (cut == "sig15" || cut == "sig") h_fjp_ttbar->Rebin(1);
	else if (cut == "sbl") h_fjp_ttbar->Rebin(5);
	else {
		h_fjp_ttbar->Rebin(30);
//		h_temp_jetht->Rebin(30);
	}
//	h_fjp_ttbar->Rebin(10);
//	if (cut == "sbb") h_fjp_ttbar->Rebin(3);
//	else if (cut == "sbtb") h_fjp_ttbar->Rebin(3);
//	else if (cut == "sbideb") h_fjp_ttbar->Rebin(3);
	TH1 *h_cdf_ttbar = make_cdf(h_fjp_ttbar, "cdf_ttbar");
	TH1 *h_cdf_jetht = make_cdf(h_temp_jetht, "cdf_jetht");
	
	// Write results to file:
	tf_out->cd();
	h_fjp_jetht->Write();
	h_fjp_ttbar->Write();
	h_temp_jetht->Write();
	h_cdf_jetht->Write();
	h_cdf_ttbar->Write();
	
	cout << h_fjp_jetht->Integral() << endl;
	cout << h_temp_jetht->Integral() << endl;
}
