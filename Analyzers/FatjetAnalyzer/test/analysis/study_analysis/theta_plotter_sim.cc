#include <iostream>
#include <sstream>
#include <string>
#include <Deracination/Straphanger/test/decortication/macros/common.cc>

bool VERBOSE = true;

void make_plots(TString region, TString name, TFile* tf_out, TH1* h_fjp, TH1* h_cdf, int nrebin, bool signal=false) {
	cout << name << endl;
	
	TString fullname = "mavg" + region + "__" + name;
	if (signal) name = "TTbar";
	
	
	double amp = 1.0;
	double shift = 0.0;
	double stretch = 1.0;
	double shifte, stretche;
	stretche = 0.5;
	shifte = 100;
	
	TH1F* h_null = (TH1F*) h_fjp->Clone(fullname);
	differentiate_cdf(h_cdf, h_null, median_from_cdf(h_cdf), amp, shift, stretch);
	h_null->Rebin(nrebin);
	
	// Shift:
	TString parname = "shift" + name;
//	if (name == "QCD") parname += region;
	TH1F* h_shift_plus = (TH1F*) h_fjp->Clone(fullname + "__" + parname + "__plus");
	differentiate_cdf(h_cdf, h_shift_plus, median_from_cdf(h_cdf), amp, shift + shifte, stretch);
	h_shift_plus->Rebin(nrebin);
	TH1F* h_shift_minus = (TH1F*) h_fjp->Clone(fullname + "__" + parname + "__minus");
	differentiate_cdf(h_cdf, h_shift_minus, median_from_cdf(h_cdf), amp, shift - shifte, stretch);
	h_shift_minus->Rebin(nrebin);
	
	// Stretch:
	parname = "stretch" + name;
	TH1F* h_stretch_plus = (TH1F*) h_fjp->Clone(fullname + "__" + parname + "__plus");
	differentiate_cdf(h_cdf, h_stretch_plus, median_from_cdf(h_cdf), amp, shift, stretch + stretche);
	h_stretch_plus->Rebin(nrebin);
	TH1F* h_stretch_minus = (TH1F*) h_fjp->Clone(fullname + "__" + parname + "__minus");
	differentiate_cdf(h_cdf, h_stretch_minus, median_from_cdf(h_cdf), amp, shift, stretch - stretche);
	h_stretch_minus->Rebin(nrebin);
	
	tf_out->WriteTObject(h_null);
	tf_out->WriteTObject(h_shift_plus);
	tf_out->WriteTObject(h_shift_minus);
	tf_out->WriteTObject(h_stretch_plus);
	tf_out->WriteTObject(h_stretch_minus);
}



void theta_plotter_sim(TString cut_sig="sig", TString cut_sb="sb", TString signal_type="sq", TString inj="") {
	int nrebin = 30;
	if (cut_sig == "sig") nrebin = 30;
	
	// Organize input and output:
	TString out_prefix = "theta_plots_sim_";
	if (signal_type != "sq") out_prefix += signal_type + "_";
	if (inj != "") out_prefix += "inj_";
	TFile* tf_out = new TFile(out_prefix + cut_sig + "_" + cut_sb + ".root", "RECREATE");
	
	TString prefix = "analysis_plots_";
	TString suffix = "";
	if (inj == "inj") suffix += "_inj";
	suffix += "_f1.root";
	
	vector<TString> regions = {"SR", "SB"};
	vector<TFile*> tfs = {TFile::Open(prefix + cut_sig + suffix), TFile::Open(prefix + cut_sb + suffix)};
	
	for (unsigned i = 0; i < regions.size(); ++ i) {
		TFile* tf = tfs[i];
		
		// Data:
		TH1* h_data = (TH1*) tf->Get("fjp_jetht");
		h_data->SetName("mavg" + regions[i] + "__DATA");
		h_data->Rebin(nrebin);
		tf_out->WriteTObject(h_data);
		
		// QCD:
		make_plots(
			regions[i],
			"QCD",
			tf_out,
			(TH1*) tfs[i]->Get("fjp_jetht"),
			(TH1*) tfs[i]->Get("cdf_jetht"),
			nrebin
		);
		
		// TTbar:
		make_plots(
			regions[i],
			"TTbar",
			tf_out,
			(TH1*) tfs[i]->Get("fjp_ttbar"),
			(TH1*) tfs[i]->Get("cdf_ttbar"),
			1
		);
		
		// Signals:
		vector<int> masses = {100, 150, 175, 200, 250, 300, 400, 500, 600, 700};
		if (signal_type == "sg") masses = {100, 150, 175, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650};
		for (int im = 0; im < masses.size(); ++im) {
			TString signal_name = "sq" + to_string(masses[im]) + "to4j";
			if (signal_type == "sg") signal_name = "sg" + to_string(masses[im]) + "to5j";
			TH1* h_fjp = (TH1*) tfs[i]->Get(TString("fjp_") + signal_name);
			TH1* h_cdf = (TH1*) tfs[i]->Get(TString("cdf_") + signal_name);
			make_plots(
				regions[i],
				"Ms" + to_string(masses[im]),
				tf_out,
				h_fjp,
				h_cdf,
				nrebin,
				true		// Signal
			);
		}
	}
}
