#include "/home/tote/decortication/macros/common.cc"
vector<TString> function_defs = {"[0]*x^[1]", "exp([0]*x + [1])"};
vector<TString> function_pretty = {"#it{p}_{0}#it{x}^{#it{p}_{1}}", "exp(#it{p}_{0}#it{x} + #it{p}_{1})"};
// Third one: actual points

TF1* fit_function(TH1* h, int f=0) {
	TString name = h->GetName();
	TH1* hclone = (TH1*) h->Clone(name + "_clone" + to_string(f));
	TString fname = "f_" + name + "_f" + to_string(f);
	
	TF1* f1 = new TF1(fname, function_defs[f], 900, 3200);
	if (f == 0) f1->SetParameters(4e17, -5);
//	if (ds == "qcdp" && cut == "sbb") f1->SetParameters(6e17, -5);
//	if (ds == "qcdp" && cut == "sbtb") f1->SetParameters(7e08, -2);
//	if (ds == "qcdmg" && cut == "sbtb") f1->SetParameters(5e08, -3);
//	hclone->Draw("e");
	hclone->Fit(fname, "", "", 900, 3200);
	return f1;
}


void get_reweighting_functions(TTree* tt, TString cut, TFile* tf_out) {
	TString ds = tt->GetName();
	TString name = ds + "_" + cut;
	tt->Draw("htak8>>" + name + "(24, 800, 3200)", get_cut("fjp_" + cut, get_weight(ds)));
	TH1* h = (TH1*) gDirectory->Get(name);
	if (ds == "qcdp" && cut == "sig") h->Rebin(2);
	
	for (int i=0; i < function_defs.size(); ++i) {
		TString cname = name + "_f" + to_string(i);
		TCanvas* tc = new TCanvas(cname, cname);
		
		TF1* f1 = fit_function(h, i);
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
		f1_plus->SetRange(900,3200);
		f1_plus->SetLineStyle(2);
		f1_plus->SetLineColorAlpha(kRed, 0.5);
		f1_minus->SetRange(900,3200);
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
	
		TLegend* leg = new TLegend(0.52, 0.67, 0.80, 0.80);
		leg->AddEntry(h, name_proper[ds], "ple");
		leg->AddEntry(f1, "Fit function", "l");
		leg->AddEntry(f1_plus, "Fit function #pm 1#sigma", "l");
	//	leg->AddEntry(f1_fix, "Adjusted function", "l");
		leg->Draw();
		style_info(true, lum_string["all"]);
	
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
		tc->SaveAs(cname + ".pdf");
		tc->SaveAs(cname + ".png");
	
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

void reweighting_plotter() {
	gROOT->SetBatch();
	
//	TFile* tf_in = TFile::Open("~/anatuples/anatuple_dalitz_predeta.root");
	tf_in = get_ana();
	TFile* tf_out = new TFile("reweight_functions.root", "RECREATE");
	
	TTree* tt_qcdmg = (TTree*) tf_in->Get("qcdmg");
	TTree* tt_qcdp = (TTree*) tf_in->Get("qcdp");
	TTree* tt_ttbar = (TTree*) tf_in->Get("ttbar");
	TTree* tt_jetht = (TTree*) tf_in->Get("jetht");
	
	get_reweighting_functions(tt_qcdmg, "sigl", tf_out);
	get_reweighting_functions(tt_qcdp, "sigl", tf_out);
	get_reweighting_functions(tt_jetht, "sigl", tf_out);
	
	get_reweighting_functions(tt_qcdmg, "sig", tf_out);
	get_reweighting_functions(tt_qcdp, "sig", tf_out);
	get_reweighting_functions(tt_jetht, "sig", tf_out);
//	get_reweighting_functions(tt_jetht, "sig15", tf_out);
	
	get_reweighting_functions(tt_qcdmg, "sb", tf_out);
	get_reweighting_functions(tt_qcdp, "sb", tf_out);
	get_reweighting_functions(tt_jetht, "sb", tf_out);
	get_reweighting_functions(tt_qcdmg, "sbb", tf_out);
	get_reweighting_functions(tt_qcdp, "sbb", tf_out);
	get_reweighting_functions(tt_jetht, "sbb", tf_out);
	
	get_reweighting_functions(tt_qcdmg, "sbt", tf_out);
	get_reweighting_functions(tt_qcdp, "sbt", tf_out);
	get_reweighting_functions(tt_jetht, "sbt", tf_out);
	get_reweighting_functions(tt_qcdmg, "sbtb", tf_out);
	get_reweighting_functions(tt_qcdp, "sbtb", tf_out);
	get_reweighting_functions(tt_jetht, "sbtb", tf_out);
	
	get_reweighting_functions(tt_qcdmg, "sbl", tf_out);
	get_reweighting_functions(tt_qcdp, "sbl", tf_out);
	get_reweighting_functions(tt_jetht, "sbl", tf_out);
	get_reweighting_functions(tt_qcdmg, "sblb", tf_out);
	get_reweighting_functions(tt_qcdp, "sblb", tf_out);
	get_reweighting_functions(tt_jetht, "sblb", tf_out);
	
	get_reweighting_functions(tt_qcdmg, "sbl42", tf_out);
	get_reweighting_functions(tt_qcdp, "sbl42", tf_out);
	get_reweighting_functions(tt_jetht, "sbl42", tf_out);
	get_reweighting_functions(tt_qcdmg, "sbl42b", tf_out);
	get_reweighting_functions(tt_qcdp, "sbl42b", tf_out);
	get_reweighting_functions(tt_jetht, "sbl42b", tf_out);
	
	get_reweighting_functions(tt_qcdmg, "sbl43", tf_out);
	get_reweighting_functions(tt_qcdp, "sbl43", tf_out);
	get_reweighting_functions(tt_jetht, "sbl43", tf_out);
	get_reweighting_functions(tt_qcdmg, "sbl43b", tf_out);
	get_reweighting_functions(tt_qcdp, "sbl43b", tf_out);
	get_reweighting_functions(tt_jetht, "sbl43b", tf_out);
	
	get_reweighting_functions(tt_qcdmg, "sbide", tf_out);
	get_reweighting_functions(tt_qcdp, "sbide", tf_out);
	get_reweighting_functions(tt_jetht, "sbide", tf_out);
	get_reweighting_functions(tt_qcdmg, "sbideb", tf_out);
	get_reweighting_functions(tt_qcdp, "sbideb", tf_out);
	get_reweighting_functions(tt_jetht, "sbideb", tf_out);
	
////	get_reweighting_functions(tt_qcdmg, "sigs", tf_out);
////	get_reweighting_functions(tt_qcdp, "sigs", tf_out);
////	
////	get_reweighting_functions(tt_qcdmg, "sbs", tf_out);
////	get_reweighting_functions(tt_qcdp, "sbs", tf_out);
////	
////	get_reweighting_functions(tt_qcdmg, "sbsb", tf_out);
////	get_reweighting_functions(tt_qcdp, "sbsb", tf_out);
}
