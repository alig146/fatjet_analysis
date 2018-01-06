#include <Deracination/Straphanger/test/decortication/macros/common.cc>

vector<TH1*> make_plot_set(TString cut, int nbins=1200) {
	vector<TString> names = {"jetht", "qcdmg", "qcdp", "ttbar"};
	vector<TString> names_sig = {"sq100to4j", "sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j"};
	
	TFile* tf_in = get_ana();
//	vector<TString> names = list_tfile(tf_in);
	vector<TH1*> plots;
	
	cout << "[..] Making plots for the " << cut << " cut." << endl;
	
	for (int i = 0; i < names.size(); ++ i) {
		TString name = names[i] + "_" + cut;
		TTree* tt = (TTree*) tf_in->Get(names[i]);
		tt->Draw("mavg_p>>" + name + "(" + to_string(nbins) + ", 0, 1200)", get_cut("fjp_" + cut));
		TH1* h = (TH1*) gDirectory->Get(name);
		h->SetTitle("");
		plots.push_back(h);
	}
	// Special treatment for signals:
	for (int i = 0; i < names_sig.size(); ++ i) {
		TString name = names_sig[i] + "_" + cut;
		if (names_sig[i] == "sq100to4j" && cut == "sig") {		// KLUDGE
			TFile* tf_temp = get_ana("sq100");
			TTree* tt = (TTree*) tf_temp->Get(names_sig[i]);
			tt->Draw("mavg_p>>" + name + "(" + to_string(nbins) + ", 0, 1200)", get_cut("fjp_" + cut, "xwtt"));
			TH1* h = (TH1*) gDirectory->Get(name);
			h->SetTitle("");
//			h->Scale(0.1);
			plots.push_back(h);
		}
		else {
			TTree* tt = (TTree*) tf_in->Get(names_sig[i]);
			tt->Draw("mavg_p>>" + name + "(" + to_string(nbins) + ", 0, 1200)", get_cut("fjp_" + cut, "xwtt"));
			TH1* h = (TH1*) gDirectory->Get(name);
			h->SetTitle("");
			plots.push_back(h);
		}
	}
	return plots;
}


void contamination_plotter(TString option="") {
	// Options:
	gROOT->SetBatch();
	vector<TString> cuts = {"pre", "sig", "sb", "sbb", "sbide", "sbideb", "sbl", "sblb", "sigl", "sbl42", "sbl42b", "sbl43", "sbl43b"};
	if (option == "lmasy") cuts = {"siglmasy", "sblmasy", "sblmasyb"};
	
	// Output:
	TString output_name = "contamination";
	if (option != "") output_name += "_" + option;
	TFile* tf_out = new TFile(output_name + "_plots.root", "RECREATE");
	
	// Loop over cuts:
	for (int i = 0; i < cuts.size(); ++ i) {
		vector<TH1*> plots = make_plot_set(cuts[i]);
		
		// Write out plots:
		for (int j = 0; j < plots.size(); ++ j) {tf_out->WriteTObject(plots[j]);}
	}
}
