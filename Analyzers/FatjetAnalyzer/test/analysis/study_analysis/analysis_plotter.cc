#include <Deracination/Straphanger/test/decortication/macros/common.cc>

bool VERBOSE = true;

void analysis_plotter(TString cut="sbb", TString ds="jetht", int f=1, TString groom_name="p") {
	// Arguments:
	vector<int> masses = {100, 150, 175, 200, 250, 300, 400, 500, 600};
	TString name = cut;
	if (groom_name != "p") {
		name += "_" + groom_name;
		cut += "_" + groom_name;
	}
	if (ds != "jetht") {
		name += "_" + ds;
	}
	name += "_f" + to_string(f);
	
	// Options:
	gROOT->SetBatch();
	
	// Define input and output:
	TFile *tfile = get_ana();
	TFile *tfile_sq100 = get_ana("sq100");
	TFile* tf_out = new TFile("analysis_plots_" + name + ".root", "RECREATE");
	
	// Define TTrees:
	TTree* tt_ttbar = (TTree*) tfile->Get("ttbar");
	TTree* tt_jetht = (TTree*) tfile->Get("jetht");
	
	// Make plots:
	/// Make FJP plots (jet pair selection):
//	double weight = get_weight();
	double weight = 1;
	TString run = "";
	TString cut_practical = cut;
	if (cut == "sig15") {
		weight = get_weight("", "15");
		cut_practical = "sig";
		run = "15";
	}
	cout << "here 41" << endl;
	tt_ttbar->Draw("mavg_" + groom_name + ">>fjp_ttbar(1200,0,1200)", get_cut("fjp_" + cut_practical, "", weight));
	TH1* h_fjp_ttbar = (TH1*) gDirectory->Get("fjp_ttbar");
	tt_jetht->Draw("mavg_" + groom_name + ">>fjp_jetht(1200,0,1200)", get_cut("fjp_" + cut_practical, run));
	TH1* h_fjp_jetht = (TH1*) gDirectory->Get("fjp_jetht");
	
	cout << "here 47" << endl;
	for (int i = 0; i < masses.size(); ++i) {
		TString sig_name = "sq" + to_string(masses[i]) + "to4j";
		TTree* tt;
		if (i == 0) tt = (TTree*) tfile_sq100->Get(sig_name);		// KLUDGE
		else tt = (TTree*) tfile->Get(sig_name);
		tt->Draw("mavg_" + groom_name + ">>fjp_" + sig_name + "(1200,0,1200)", get_cut("fjp_" + cut_practical, "xwtt", weight));
		TH1* fjp = (TH1*) gDirectory->Get("fjp_" + sig_name);
		if (masses[i] == 150) fjp->Scale(0.3937);		// KLUDGE!
//		if (i == 0) fjp->Scale(0.10);		// KLUDGE
		TH1* cdf = make_cdf(fjp, "cdf_" + sig_name);
		tf_out->WriteTObject(fjp);
		tf_out->WriteTObject(cdf);
	}
	cout << "here 58" << endl;
	if (ds == "inj") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq150to4j"));
	else if (ds == "inj100") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq100to4j"));
	else if (ds == "inj150") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq150to4j"));
	else if (ds == "inj200") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq200to4j"));
	else if (ds == "inj250") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq250to4j"));
	else if (ds == "inj300") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq300to4j"));
	else if (ds == "inj400") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq400to4j"));
	else if (ds == "inj500") h_fjp_jetht->Add((TH1*) gDirectory->Get("fjp_sq500to4j"));
	
	cout << "here 60" << endl;
	/// Template:
	if (ds == "inj" || ds == "inj100" || ds == "inj150" || ds == "inj200" || ds == "inj250" || ds == "inj300" || ds == "inj400" || ds == "inj500") ds = "jetht";
	TH1* h_temp_jetht = fetch_template(ds, cut, "", f);		// WARNING: This doesn't use a different groomer.
	h_temp_jetht->SetName("temp_jetht");
//	if (cut == "sig15" || cut == "sig") h_temp_jetht->Scale(h_fjp_jetht->Integral()/h_temp_jetht->Integral());
	h_temp_jetht->Scale(h_fjp_jetht->Integral()/h_temp_jetht->Integral());
	
	cout << "here 70" << endl;
	// Make CDFs:
//	if (cut == "sbl" || cut == "sbb") h_fjp_ttbar->Rebin(10);
//	if (cut == "sig") h_fjp_ttbar->Rebin(30);
	if (cut == "sig") h_fjp_ttbar->Rebin(30);
	else if (cut == "sig15") h_fjp_ttbar->Rebin(50);
	else if (cut == "sbl") h_fjp_ttbar->Rebin(5);
	else {
		h_fjp_ttbar->Rebin(30);
//		h_temp_jetht->Rebin(30);
	}
//	h_fjp_ttbar->Rebin(10);
//	if (cut == "sbb") h_fjp_ttbar->Rebin(3);
//	else if (cut == "sbtb") h_fjp_ttbar->Rebin(3);
//	else if (cut == "sbideb") h_fjp_ttbar->Rebin(3);
	cout << "here 85" << endl;
	TH1 *h_cdf_ttbar = make_cdf(h_fjp_ttbar, "cdf_ttbar");
	TH1 *h_cdf_jetht = make_cdf(h_temp_jetht, "cdf_jetht");
	
	// Write results to file:
	tf_out->cd();
	h_fjp_jetht->Write();
	h_fjp_ttbar->Write();
	h_temp_jetht->Write();
	h_cdf_jetht->Write();
	h_cdf_ttbar->Write();
	
	cout << "FJP integral = " << h_fjp_jetht->Integral() << endl;
	cout << "QCD template integral = " << h_temp_jetht->Integral() << endl;
}
