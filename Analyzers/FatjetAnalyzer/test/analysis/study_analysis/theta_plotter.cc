#include <iostream>
#include <sstream>
#include <string>
#include <Deracination/Straphanger/test/decortication/macros/common.cc>

bool VERBOSE = true;

void make_plots(TString name, TFile* tf_out, TH1* h_fjp, TH1* h_cdf, TH1* params, int first_param, int nrebin, TH1* params_out, int params_out_offset, bool signal=false) {
//	h->SetName(name);
	cout << name << endl;
	
	TString fullname = "mavg__" + name;
	if (signal) name = "TTbar";
////	double amp = params->GetBinContent(first_param);
////	double ampe = params->GetBinError(first_param);
//	amp = 1.0;
//	ampe = 0.0;
////	if (signal) {
////		amp = 1.0;
////		ampe = 0.0;
////	}
//	double shift = params->GetBinContent(first_param + 1);
//	double shifte = params->GetBinError(first_param + 1);
//	double stretch = params->GetBinContent(first_param + 2);
//	double stretche = params->GetBinError(first_param + 2);
//	
//	if (name == "QCD") {
//		shifte = abs(shift);
//		shift = 0;
//		stretche = abs(1 - stretch);		// 1 - 0.95
//		stretch = 1.00;
//	}
	
	double amp = 1.0;
	double shift = 0.0;
	double stretch = 1.0;
	double ampe, shifte, stretche;
	if (name == "TTbar") {
//		ampe = 0.20;
//		ampe = 0.22;		// v5 (new cut)
//		shifte = 5.0;		// v3-4
//		shifte = 6.0;		// v5 (new cut)
//		shifte = 8.0;		// method 4.1
//		shifte = 4.0;		// method 4.1 inj
//		shifte = 8.0;		// method 4.2
		shifte = 8.0;		// method 4.2 inj
//		stretche = 0.09;		// v3-4
//		stretche = 0.08;		// v5 (new cut)
//		stretche = 0.06;		// method 4.1
//		stretche = 0.13;		// method 4.1 inj
//		stretche = 0.19;		// method 4.2
		stretche = 0.24;		// method 4.2 inj
		
		// Injection:
//		ampe = 0.96;
//		shifte = 19.8;
//		stretche = 0.194;
	}
	else if (name == "QCD") {
		ampe = 5.0;
//		shifte = 20.0;		// v3-4
//		shifte = 22.0;		// v5 (new cut)
//		shifte = 21.0;		// method 4.1
//		shifte = 20.0;		// method 4.1 inj
//		shifte = 21.0;		// method 4.2
		shifte = 20.0;		// method 4.2 inj
//		stretche = 0.08;		// v3
//		stretche = 0.09;		// v4 (after btag wp change)
//		stretche = 0.13;		// v5 (new cut)
//		stretche = 0.03;		// method 4.1
//		stretche = 0.05;		// method 4.1 inj
//		stretche = 0.18;		// method 4.2
		stretche = 0.16;		// method 4.2 inj
		
		// Injection:
//		shifte = 33.1;
//		stretche = 0.055;
	}
	
	TH1F* h_null = (TH1F*) h_fjp->Clone(fullname);
	differentiate_cdf(h_cdf, h_null, median_from_cdf(h_cdf), amp, shift, stretch);
	h_null->Rebin(nrebin);
	
//	TH1F* h_amp_plus = (TH1F*) h_fjp->Clone(fullname + TString("__amp") + name + TString("__plus"));
//	differentiate_cdf(h_cdf, h_amp_plus, median_from_cdf(h_cdf), amp + ampe, shift, stretch);
//	h_amp_plus->Rebin(nrebin);
//	TH1F* h_amp_minus = (TH1F*) h_fjp->Clone(fullname + TString("__amp") + name + TString("__minus"));
//	differentiate_cdf(h_cdf, h_amp_minus, median_from_cdf(h_cdf), amp - ampe, shift, stretch);
//	h_amp_minus->Rebin(nrebin);
	TH1F* h_shift_plus = (TH1F*) h_fjp->Clone(fullname + TString("__shift") + name + TString("__plus"));
	differentiate_cdf(h_cdf, h_shift_plus, median_from_cdf(h_cdf), amp, shift + shifte, stretch);
	h_shift_plus->Rebin(nrebin);
	TH1F* h_shift_minus = (TH1F*) h_fjp->Clone(fullname + TString("__shift") + name + TString("__minus"));
	differentiate_cdf(h_cdf, h_shift_minus, median_from_cdf(h_cdf), amp, shift - shifte, stretch);
	h_shift_minus->Rebin(nrebin);
	TH1F* h_stretch_plus = (TH1F*) h_fjp->Clone(fullname + TString("__stretch") + name + TString("__plus"));
	differentiate_cdf(h_cdf, h_stretch_plus, median_from_cdf(h_cdf), amp, shift, stretch + stretche);
	h_stretch_plus->Rebin(nrebin);
	TH1F* h_stretch_minus = (TH1F*) h_fjp->Clone(fullname + TString("__stretch") + name + TString("__minus"));
	differentiate_cdf(h_cdf, h_stretch_minus, median_from_cdf(h_cdf), amp, shift, stretch - stretche);
	h_stretch_minus->Rebin(nrebin);
	
	params_out->SetBinContent(params_out_offset, amp);
	params_out->SetBinError(params_out_offset, ampe);
	params_out->SetBinContent(params_out_offset + 1, shift);
	params_out->SetBinError(params_out_offset + 1, shifte);
	params_out->SetBinContent(params_out_offset + 2, stretch);
	params_out->SetBinError(params_out_offset + 2, stretche);
	
	tf_out->WriteTObject(h_null);
	tf_out->WriteTObject(h_shift_plus);
	tf_out->WriteTObject(h_shift_minus);
	tf_out->WriteTObject(h_stretch_plus);
	tf_out->WriteTObject(h_stretch_minus);
}


void theta_plotter(TString cut_sig="sig", TString cut_sb="sb", TString inj="") {
	int nrebin = 50;
	if (cut_sig == "sig") nrebin = 30;
	
	TString in_name = "analysis_plots_" + cut_sig;
	if (inj == "inj") in_name += "_inj";
	in_name += "_f1.root";
	
	TFile* tf_sbb = TFile::Open("analysis_plots_fitsim_" + cut_sb + "b_f1.root");
	TFile* tf_sb = TFile::Open("analysis_plots_fitsim_" + cut_sb + "_f1.root");
	TFile* tf_sig = TFile::Open(in_name);
	TString out_name = "theta_plots_";
	if (inj == "inj") out_name += "inj_";
	out_name += cut_sig + "_" + cut_sb + ".root";
	TFile* tf_out = new TFile(out_name, "RECREATE");
	
	TH1* params_sb = (TH1*) tf_sb->Get("params");
	params_sb->SetName("params_sb");
	TH1* params_sbb = (TH1*) tf_sbb->Get("params");
	params_sbb->SetName("params_sbb");
	TH1D* params_out = new TH1D("params", "", 27, 0, 27);
	
	// Data:
	TH1F* h_data_original = (TH1F*) tf_sig->Get("fjp_jetht");
//	h_data_original->Add((TH1F*) tf_sig->Get("fjp_sq150to4j"));
	TH1* h_data = (TH1*) h_data_original->Clone("mavg__DATA");
	h_data->Rebin(nrebin);
	tf_out->WriteTObject(h_data);
	// QCD:
	make_plots(
		"QCD",
		tf_out,
		(TH1*) tf_sig->Get("fjp_jetht"),
		(TH1*) tf_sig->Get("cdf_jetht"),
		params_sb,
		1,
		nrebin,
		params_out,
		1
	);
	// TTbar:
	make_plots(
		"TTbar",
		tf_out,
		(TH1*) tf_sig->Get("fjp_ttbar"),
		(TH1*) tf_sig->Get("cdf_ttbar"),
		params_sbb,
		4,
		1,
		params_out,
		4
	);
	// Signals:
	vector<int> masses = {100, 150, 175, 200, 250, 300, 400, 500, 600};
	for (int i = 0; i < masses.size(); ++i) {
		make_plots(
			"Ms" + to_string(masses[i]),
			tf_out,
			(TH1*) tf_sig->Get(TString("fjp_sq") + to_string(masses[i]) + "to4j"),
			(TH1*) tf_sig->Get(TString("cdf_sq") + to_string(masses[i]) + "to4j"),
			params_sbb,
			4,
			nrebin,
			params_out,
			7 + (3*i),
			true		// Signal
		);
	}
	
	tf_out->WriteTObject(params_out);
}
