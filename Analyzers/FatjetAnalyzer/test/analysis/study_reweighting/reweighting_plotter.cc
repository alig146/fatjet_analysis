#include <Deracination/Straphanger/test/decortication/macros/common.cc>
vector<TString> function_defs = {"[0]*x^[1]", "exp([0]*x + [1])"};
vector<TString> function_pretty = {"#it{p}_{0}#it{x}^{#it{p}_{1}}", "exp(#it{p}_{0}#it{x} + #it{p}_{1})"};
// Third one: actual points

TF1* fit_function(TH1* h, int f=0, double htmin=900) {
	TString name = h->GetName();
	TH1* hclone = (TH1*) h->Clone(name + "_clone" + to_string(f));
	TString fname = "f_" + name + "_f" + to_string(f);
	
	TF1* f1 = new TF1(fname, function_defs[f], htmin, 3200);
	if (f == 0) f1->SetParameters(4e17, -5);
	else if (f == 1) f1->SetParameters(-4e-3, 10);
//	if (ds == "qcdp" && cut == "sbb") f1->SetParameters(6e17, -5);
//	if (ds == "qcdp" && cut == "sbtb") f1->SetParameters(7e08, -2);
//	if (ds == "qcdmg" && cut == "sbtb") f1->SetParameters(5e08, -3);
//	hclone->Draw("e");
	hclone->Fit(fname, "", "", htmin, 3200);
	return f1;
}


void get_reweighting_functions(TString ds, TString cut, TFile* tf_out, TString ana_option="") {
	cout << "[..] Getting the HT re-weighting function for " << ds << " and " << cut << endl;
	
	tf = get_ana(ana_option);
	TString name = ds + "_" + cut;
	vector<TTree*> tts;
	TString era = "";
	if (ds == "inj") {
//		if (cut == "sig" || cut == "sigl") era = "15";
		tts.push_back((TTree*) tf->Get("jetht"));
		tts.push_back((TTree*) tf->Get("sq150to4j"));
	} 
	else if (ds == "all") {
		tts.push_back((TTree*) tf->Get("qcdmg"));
		tts.push_back((TTree*) tf->Get("ttbar"));
		tts.push_back((TTree*) tf->Get("sq150to4j"));
	}
	else tts.push_back((TTree*) tf->Get(ds));
	double weight_factor = get_weight(tts[0]->GetName(), era);
	cout << "[..] Making the HT plot with a weight factor of " << weight_factor << "." << endl;
	tts[0]->Draw("htak8>>" + name + "(24, 800, 3200)", get_cut("fjp_" + cut, era, weight_factor));
	TH1* h = (TH1*) gDirectory->Get(name);
	if (ds == "all" || ds == "inj") {
		for (unsigned i = 1; i < tts.size() ; ++i) {
			TTree* tt = tts[i];
			tt->Draw("htak8>>h(24, 800, 3200)", get_cut("fjp_" + cut, "", get_weight(tt->GetName(), era)));
			TH1* h_temp = (TH1*) gDirectory->Get("h");
			h->Add(h_temp);
		}
	}
	cout << "[OK] HT distribution integral = " << h->Integral() << endl;
//	if (ds == "qcdp" && cut == "sig") h->Rebin(2);
	
	for (int i=0; i < function_defs.size(); ++i) {
		TString cname = name + "_f" + to_string(i);
		TCanvas* tc = new TCanvas(cname, cname);
		
		double htmin = 900;
//		if (cut == "pretsbl" || cut == "pretsblb") htmin = 1100;
		
		TF1* f1 = fit_function(h, i, htmin);
		TF1* f1_plus = new TF1(TString(f1->GetName()) + "_plus", function_defs[i]);
		TF1* f1_minus = new TF1(TString(f1->GetName()) + "_minus", function_defs[i]);
//		f1_plus->SetParameters(f1->GetParameter(0), f1->GetParameter(1) + f1->GetParError(1));
//		f1_minus->SetParameters(f1->GetParameter(0), f1->GetParameter(1) - f1->GetParError(1));
		if (i == 0) {
			f1_plus->SetParameters(f1->GetParameter(0), f1->GetParameter(1) + f1->GetParError(1));
			f1_minus->SetParameters(f1->GetParameter(0), f1->GetParameter(1) - f1->GetParError(1));
		}
		else if (i == 1) {
			f1_plus->SetParameters(f1->GetParameter(0) + f1->GetParError(0), f1->GetParameter(1));
			f1_minus->SetParameters(f1->GetParameter(0) - f1->GetParError(0), f1->GetParameter(1));
		}
	
//		TF1* f1_fixup = new TF1("f1_fixup_" + fullname, "[0]*x^[1]");
//		f1_fixup->SetParameters(f1_plus->Eval(900)/pow(900, log(f1_plus->Eval(900)/f1_minus->Eval(3200))/log(900.0/3200.0)), log(f1_plus->Eval(900)/f1_minus->Eval(3200))/log(900.0/3200.0));
	
	//	f1_fix->SetParameters(f1->Eval(900)/pow(900, log(f1->Eval(900)/f1_minus->Eval(3200))/log(900.0/3200.0)), log(f1->Eval(900)/f1_minus->Eval(3200))/log(900.0/3200.0));
	
	//	cout << log(f1_plus->Eval(900)/f1_minus->Eval(3200))/log(900.0/3200.0) << endl;
	//	cout << f1_fix->Eval(900) << endl;
	//	cout << f1_fix->Eval(3200) << endl;


//		TF1* f1_fix = new TF1("f1_fix_" + fullname, "[0]*x^[1]");
//		f1_fix->SetParameters(f1_plus->Eval(900)/pow(900, log(f1_plus->Eval(900)/f1_minus->Eval(3200))/log(900.0/3200.0)), log(f1_plus->Eval(900)/f1_minus->Eval(3200))/log(900.0/3200.0));
	
		// Style and draw:
		gStyle->SetOptStat(0);
		h->SetLineWidth(2);
		h->SetFillStyle(0);
	//	h->SetLineColor(kRed);
	//	h->SetMarkerColor(kRed);
		h->SetNdivisions(506);
		h->SetTitle("");
		h->GetXaxis()->SetTitle("#it{H}_{T} [GeV]");
		if (i == 0) h->SetMaximum(h->GetMaximum()*4.0);
		
		f1->SetLineColorAlpha(kRed, 0.5);
		
		style_ylabel_th1(h);
		f1_plus->SetRange(htmin, 3200);
		f1_plus->SetLineStyle(2);
		f1_plus->SetLineColorAlpha(kRed, 0.5);
		f1_minus->SetRange(htmin, 3200);
		f1_minus->SetLineStyle(2);
		f1_minus->SetLineColorAlpha(kRed, 0.5);
//		f1_fix->SetRange(900,3200);
//		f1_fix->SetLineColor(kBlue);
//		f1_fix->SetLineStyle(5);
	
	
		h->Draw("e");
		f1->Draw("same");
		f1_plus->Draw("same");
		f1_minus->Draw("same");
	//	f1_fixup->Draw("same");
	
//		TLegend* leg = new TLegend(0.52, 0.67, 0.80, 0.80);
		TLegend* leg = get_legend(1, 3, 1.0, 0.9);
		leg->AddEntry(h, name_proper[ds], "ple");
		leg->AddEntry(f1, "Fit function", "l");
		leg->AddEntry(f1_plus, "Fit function #pm 1#sigma", "l");
	//	leg->AddEntry(f1_fix, "Adjusted function", "l");
		leg->Draw();
		style_info(true, lum_string["all"]);
		style_cut(cut);
	
		std::ostringstream oss1;
		oss1 << "#it{p}_{0} = " << std::scientific << std::setprecision(1) << f1->GetParameter(0) << " #pm " << std::setprecision(0) << f1->GetParError(0);
		std::ostringstream oss2;
		oss2 << "#it{p}_{1} = " << std::fixed << std::setprecision(1) << f1->GetParameter(1) << " #pm " << f1->GetParError(1);
	
//		std::ostringstream oss1_fix;
//		oss1_fix << "#color[4]{#it{p}_{0} = " << std::scientific << std::setprecision(1) << f1_fix->GetParameter(0) << "}";
//		std::ostringstream oss2_fix;
//		oss2_fix << "#color[4]{#it{p}_{1} = " << std::fixed << std::setprecision(1) << f1_fix->GetParameter(1) << "}";
	
		vector<TString> texts_par;
		texts_par.push_back("#bf{Fitting to " + function_pretty[i] + "}");
		texts_par.push_back(oss1.str());
		texts_par.push_back(oss2.str());
	//	texts_par.push_back(oss1_fix.str());
	//	texts_par.push_back(oss2_fix.str());
	
		style_write(texts_par, 0.21, 0.42);
	
		tc->SetLogy();
		save(tc);
	
		tf_out->WriteTObject(f1);
		tf_out->WriteTObject(f1_plus);
		tf_out->WriteTObject(f1_minus);
//		tf_out->WriteTObject(f1_fix);
	//	
	//	vector<TF1*> functions = {f1, f1_plus, f1_minus};
	//	return functions;
	}
	tf_out->WriteTObject(h);
}

void reweighting_plotter(TString cut="sb") {
	gROOT->SetBatch();
	
//	TFile* tf_in = TFile::Open("~/anatuples/anatuple_dalitz_predeta.root");
	TFile* tf_out = new TFile("reweight_functions_" + cut + ".root", "RECREATE");
	vector<TString> dss = {"jetht", "inj", "qcdmg", "qcdp"};
//	vector<TString> dss = {"jetht"};
	
	for (unsigned i = 0; i < dss.size(); ++ i) {
		TString ana_option = "";
		TString ds = dss[i];
		if (ds == "qcdp") ana_option = ds;
		get_reweighting_functions(dss[i], cut, tf_out, ana_option);
		cout << endl;
	}
}
