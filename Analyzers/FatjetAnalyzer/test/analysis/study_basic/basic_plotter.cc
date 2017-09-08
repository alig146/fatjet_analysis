#include "/home/tote/decortication/macros/common.cc"


TH1* make_plot(TTree* tt, TString variable, TString cut, TString name, TString nbins, TString low, TString high) {
	tt->Draw(variable + ">>" + name + "(" + nbins + ", " + low + ", " + high + ")", get_cut("fjp_" + cut));
	TH1* h = (TH1*) gDirectory->Get(name);
	h->SetTitle("");
	h->SetFillColor(0);
	return h;
}

vector<TH1*> make_plot_set(TString cut, int nbins=1200) {
	TFile* tf_in = get_ana();
	vector<TString> dss = list_tfile(tf_in);
	vector<TH1*> plots;
	
	cout << "[..] Making plots for the " << cut << " cut." << endl;
	
	for (int ids = 0; ids < dss.size(); ++ ids) {
		TString ds = dss[ids];
		TString suffix = ds + "_" + cut;
		TTree* tt = (TTree*) tf_in->Get(ds);
		
		plots.push_back(make_plot(tt, "mavg_p", cut, "mavg_" + suffix, to_string(nbins), "0", "1200"));
		plots.push_back(make_plot(tt, "masy_p", cut, "masy_" + suffix, to_string(nbins), "0", "0.1"));
		plots.push_back(make_plot(tt, "deta", cut, "deta_" + suffix, to_string(nbins), "0", "1"));
		plots.push_back(make_plot(tt, "Max$(tau21)", cut, "tau21_" + suffix, to_string(nbins), "0", "1"));
		plots.push_back(make_plot(tt, "Max$(tau42)", cut, "tau42_" + suffix, to_string(nbins), "0", "0.8"));
		plots.push_back(make_plot(tt, "Max$(tau43)", cut, "tau43_" + suffix, to_string(nbins), "0.4", "1"));
	}
	return plots;
}


void basic_plotter() {
	TFile* tf_in = get_ana();
	
	// Options:
	gROOT->SetBatch();
	vector<TString> cuts = {"sig", "sb", "sbb", "sbide", "sbideb", "sbl", "sblb", "sbt", "sbtb", "sigl", "sbl42", "sbl42b", "sbl43", "sbl43b"};
	
	// Output:
	TFile* tf_out = new TFile("basic_plots.root", "RECREATE");
	
	// Loop over cuts:
	for (int icut = 0; icut < cuts.size(); ++ icut) {
		TString cut = cuts[icut];
		// Make plots for each dataset:
		vector<TH1*> plots = make_plot_set(cut);
		
		// Write out plots:
		for (int j = 0; j < plots.size(); ++ j) {tf_out->WriteTObject(plots[j]);}
	}
}
