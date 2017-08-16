#include "/home/tote/decortication/macros/common.cc"

vector<TH1*> make_plot_set(TString cut, int nbins=1200) {
//	vector<TString> names = {"jetht", "qcdmg", "qcdp", "ttbar", "sq100to4j", "sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j"};
	
	TFile* tf_in = get_ana();
	vector<TString> names = list_tfile(tf_in);
	vector<TH1*> plots;
	
	cout << "[..] Making plots for the " << cut << " cut." << endl;
	
	for (int i = 0; i < names.size(); ++ i) {
		TString name = names[i] + "_" + cut;
		TTree* tt = (TTree*) tf_in->Get(names[i]);
		tt->Draw("mavg_p>>" + name + "(" + to_string(nbins) + ", 0, 1200)", get_cut("fjp_" + cut, get_weight(names[i])));
		TH1* h = (TH1*) gDirectory->Get(name);
		h->SetTitle("");
		plots.push_back(h);
	}
	return plots;
}


void contamination_plotter() {
	// Options:
	gROOT->SetBatch();
	vector<TString> cuts = {"sig", "sb", "sbb", "sbide", "sbideb", "sbl", "sblb", "sbt", "sbtb", "sigl", "sbl42", "sbl42b", "sbl43", "sbl43b"};
	
	// Output:
	TFile* tf_out = new TFile("contamination_plots.root", "RECREATE");
	
	// Loop over cuts:
	for (int i = 0; i < cuts.size(); ++ i) {
		vector<TH1*> plots = make_plot_set(cuts[i]);
		
		// Write out plots:
		for (int j = 0; j < plots.size(); ++ j) {tf_out->WriteTObject(plots[j]);}
	}
}
