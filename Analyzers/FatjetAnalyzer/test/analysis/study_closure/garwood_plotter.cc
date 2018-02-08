#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void garwood_plotter(int nrebin=30) {
	// Run this code to produce mavg_p distributions for the QCD samples with Garwood interval error bars.
	//
	// This code takes a while to run because there is some nuance to calculating Garwood intervals for distributions
	// that combine events with different weights.
	
	gROOT->SetBatch();
	
	vector<TString> dss = {"qcdmg", "qcdp"};
	vector<TString> cuts = {"sig", "sigl", "sb", "sbl"};
	
	for (int icut = 0; icut < cuts.size(); ++icut) {
		TString cut = cuts[icut];
//		TFile* tf_out = TFile::Open("garwood_plots_" + cut + ".root", "recreate");		// This isn't necessary; the function that makes the plot already saves it to a file.
	
		for (int ids = 0; ids < dss.size(); ++ids) {
			TString ds = dss[ids];
			TString ana_option = "";
//			if (ds == "qcdp") ana_option = ds;
			TFile* tf_in = get_ana(ana_option);
			
			// Make the plot:
			cout << "[..] Making the Garwoodified plot for " << ds << " with cut " << cut << "." << endl;
			TH1D* h = make_qcd_garwood(tf_in, ds, cut, nrebin);
			
			// Save the plot:
			// Note: The plot is automatically saved by the make_qcd_garwood function.
//			tf_out->WriteTObject(h);
		}
//		tf_out->Close();
	}
}










