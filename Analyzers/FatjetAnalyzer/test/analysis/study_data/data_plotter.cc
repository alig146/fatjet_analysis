#include <Deracination/Straphanger/test/decortication/macros/common.cc>


TH1* make_plot(TTree* tt, TString variable, TCut tcut, TString name, TString nbins, TString low, TString high) {
	tt->Draw(variable + ">>" + name + "(" + nbins + ", " + low + ", " + high + ")", tcut);
	TH1* h = (TH1*) gDirectory->Get(name);
	h->SetTitle("");
	h->SetFillColor(0);
	return h;
}

vector<TH1*> make_plot_set(TCut tcut, int nbins=1200) {
	TFile* tf_in = get_ana();
	TString ds = "jetht";
	vector<TH1*> plots;
	
	cout << "[..] Making plots for the " << tcut.GetName() << " cut." << endl;
	
	TString suffix = ds + "_" + tcut.GetName();
	TTree* tt = (TTree*) tf_in->Get(ds);
	
	plots.push_back(make_plot(tt, "mavg_p", tcut, "mavg_" + suffix, to_string(nbins), "0", "1200"));
	plots.push_back(make_plot(tt, "masy_p", tcut, "masy_" + suffix, to_string(nbins), "0", "0.1"));
	plots.push_back(make_plot(tt, "deta", tcut, "deta_" + suffix, to_string(nbins), "0", "1"));
	plots.push_back(make_plot(tt, "Max$(tau21)", tcut, "tau21_" + suffix, to_string(nbins), "0", "1"));
	plots.push_back(make_plot(tt, "Max$(tau42)", tcut, "tau42_" + suffix, to_string(nbins), "0", "0.8"));
	plots.push_back(make_plot(tt, "Max$(tau43)", tcut, "tau43_" + suffix, to_string(nbins), "0.4", "1"));
	plots.push_back(make_plot(tt, "eta[0]", tcut, "eta0_" + suffix, to_string(nbins), "-4", "4"));
	plots.push_back(make_plot(tt, "phi[0]", tcut, "phi0_" + suffix, to_string(nbins), "-4", "4"));
	plots.push_back(make_plot(tt, "pt[0]", tcut, "pt0_" + suffix, to_string(nbins), "0", "1200"));
	plots.push_back(make_plot(tt, "tau21[0]", tcut, "tau210_" + suffix, to_string(nbins), "0", "1"));
	plots.push_back(make_plot(tt, "tau42[0]", tcut, "tau420_" + suffix, to_string(nbins), "0", "0.8"));
	plots.push_back(make_plot(tt, "tau43[0]", tcut, "tau430_" + suffix, to_string(nbins), "0.4", "1"));
	plots.push_back(make_plot(tt, "htak8", tcut, "ht_" + suffix, to_string(nbins), "0", "2400"));
	return plots;
}


void data_plotter() {
	TFile* tf_in = get_ana();
	
	// Options:
	gROOT->SetBatch();
	vector<TCut> tcuts = {
		get_cut("fjp_sb", "15"),
		get_cut("fjp_sb", "16"),
	};
	
	// Output:
	TFile* tf_out = new TFile("data_plots.root", "RECREATE");
	
	// Loop over cuts:
	for (int icut = 0; icut < tcuts.size(); ++ icut) {
		TCut tcut = tcuts[icut];
		// Make plots for each dataset:
		vector<TH1*> plots = make_plot_set(tcut);
		
		// Write out plots:
		for (int j = 0; j < plots.size(); ++ j) {tf_out->WriteTObject(plots[j]);}
	}
}
