#include "/home/tote/decortication/macros/common.cc"

void draw_plot(TString name, TString ds, TH1* data, TH1* temp, TH1* fit, TH1* params, int logy=0) {
	TCanvas* tc = draw_pull(name, data, fit, 0, 1200);
	TPad* pad = (TPad*) tc->GetPad(1);
	pad->cd();
	tc->SetLogy(logy);
	
	temp->Draw("hist");
	fit->Draw("same");
	data->Draw("same e");
	if (logy == 0) {
		temp->SetMinimum(0);
		temp->SetMaximum(data->GetMaximum()*1.2);
	}
	else if (logy == 1) {
		temp->SetMinimum(0.1);
		temp->SetMaximum(data->GetMaximum()*4);
	}
	pad->SetLogy(logy);
	
	TLegend* leg;
	if (logy == 0) leg = new TLegend(0.55, 0.42, 0.80, 0.57);
	else if (logy == 1) leg = new TLegend(0.22, 0.1, 0.48, 0.25);
	leg->AddEntry(data, "Jet pair selection", "ple");
	leg->AddEntry(temp, "Derived template", "f");
	leg->AddEntry(fit, "Post-fit template", "f");
	leg->Draw();
	style_info(false, lum_string["all"], 1, true);
	
	style_write(name_proper[ds], 0.61, 0.74, 0.04);
	
	// Write params:
	std::ostringstream oss1;
	oss1 << "#theta_{shift} = " << std::fixed << std::setprecision(1) << params->GetBinContent(2) << " #pm " << params->GetBinError(2) << " GeV";
	std::ostringstream oss2;
	oss2 << "#theta_{stretch} = (" << std::fixed << std::setprecision(1);
	if (params->GetBinContent(3) > 1) oss2 << "+";
	oss2 << 100*(params->GetBinContent(3) - 1) << " #pm " << 100*params->GetBinError(3) << ")\%";
	vector<TString> texts_par;
	texts_par.push_back("#bf{Fit parameters:}");
	texts_par.push_back(oss1.str());
	texts_par.push_back(oss2.str());
	style_write(texts_par, 0.55, 0.67, 0.035);
	
	tc->SaveAs(name + ".pdf");
}

void closure_plotter(TString cut_name="sb", TString ds="jetht", int nrebin=30, bool ht=true, TString dir="", int f=1) {
//	TFile* tf_in = TFile::Open("~/anatuples/anatuple_dalitz_predeta.root");
	TFile* tf_in = get_ana();
	
	TTree *tt = (TTree*) tf_in->Get(ds);
	
	tt->Draw("mavg_p>>" + ds + "_fjp(1200,0,1200)", get_cut("fjp_" + cut_name, get_weight(ds)));
	TH1 *h_fjp = (TH1*) gDirectory->Get(ds + "_fjp");
	TH1* h_temp = fetch_template(ds, cut_name, dir, f, ht);
	TH1 *h_cdf = make_cdf(h_temp, ds + "_cdf");
	
	/// best guesses for the amplitude, shift, and stretch:
	double amp = h_fjp->Integral(1, h_fjp->GetNbinsX())/h_temp->Integral(1, h_temp->GetNbinsX());
	double shift = 0.0;
	double stretch = 1.0;
	double ampe, shifte, stretche;
	vector<double> bins {0, 100, 150, 200, 250, 300, 350, 400, 500, 600, 650, 800};
	if (ds == "jetht" && cut_name == "sb") {
		amp = 2e-6;
		shift = -10;
		stretch = 0.90;
		bins = {0, 50, 150, 200, 250, 300, 400, 500, 600};
	}
	else if (ds == "qcdmg" && cut_name == "sb") {
		amp = 2e-6;
		shift = -10;
		stretch = 0.90;
		bins = {0, 100, 150, 200, 250, 300, 350, 400, 500, 600, 650};
	}
	else if (ds == "qcdp" && cut_name == "sb") {
		amp = 1e-6;
		shift = -20;
		stretch = 0.90;
//		bins = {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 600, 800};
		bins = {0, 100, 150, 200, 250, 300, 350, 400, 500, 600};
	}
	else if (ds == "qcdmg" && cut_name == "sbb") {
		amp = 5.0e-7;
		shift = -10;
		stretch = 0.95;
		bins = {0, 50, 100, 150, 200, 250, 300, 400, 500};
	}
	else if (ds == "qcdp" && cut_name == "sbb") {
		amp = 5.0e-6;
		shift = -10;
		stretch = 0.95;
		bins = {0, 50, 100, 150, 200, 250, 300, 400, 500};
	}
	else if (ds == "qcdmg" && cut_name == "sig") {
		amp = 2.0e-6;
		shift = 1.0;
		stretch = 1.0;
		bins = {0, 50, 100, 150, 200, 250, 300, 400, 500, 600, 800};
	}
	else if (ds == "qcdp" && cut_name == "sig") {
		amp = 2.0e-6;
		shift = 1.0;
		stretch = 1.0;
		bins = {50, 150, 250, 300, 400, 450, 500, 550, 600, 800};
	}
	else if (ds == "qcdmg" && cut_name == "sigl" && ht) {		// This works
		amp = 2.0e-6;
		shift = 1.0;
		stretch = 1.0;
		bins = {50, 100, 150, 250, 300, 350, 400, 450, 500, 550, 600, 800};
	}
//	else if (ds == "qcdmg" && cut_name == "sigl" && ht) {		// Experimental
//		amp = 2.0e-6;
//		shift = 1.0;
//		stretch = 1.0;
//		bins = {100, 250, 350, 550, 800};
//	}
	else if (ds == "qcdmg" && cut_name == "sigl" && !ht) {
		amp = 2.0e-6;
		shift = 1.0;
		stretch = 1.0;
		bins = {0, 50, 100, 150, 200, 250, 300, 400};
	}
	else if (ds == "qcdp" && cut_name == "sigl" && ht) {
		amp = 4.0e-7;
		shift = -10.0;
		stretch = 1.0;
		bins = {0, 200, 300, 400, 500, 600, 700, 800, 900, 1100};
	}
	else if (cut_name == "sbt") {
		amp = 2e-6;
		shift = -10;
		stretch = 0.90;
		bins = {0, 100, 150, 200, 250, 300, 350, 400, 500, 600, 650};
	}
	else if (cut_name == "sbtb") {
		amp = 5.0e-6;
		shift = -10;
		stretch = 0.95;
		bins = {0, 100, 150, 200, 300, 450};
	}
	else if (cut_name == "sbs") {
		amp = 2e-6;
		shift = -10;
		stretch = 0.90;
		bins = {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650};
	}
	else if (cut_name == "sbsb") {
		amp = 5.0e-6;
		shift = -10;
		stretch = 0.95;
		bins = {0, 100, 150, 200, 250, 300, 400};
	}
	else if (cut_name == "sigs") {
		amp = 2.0e-6;
		shift = 1.0;
		stretch = 1.0;
		bins = {0, 100, 150, 200, 250, 300, 400, 500, 600};
	}
	
	TH1* params = new TH1D("params", "", 3, 0, 3);
	params->SetBinContent(1, amp);
	params->SetBinError(1, ampe);
	params->SetBinContent(2, shift);
	params->SetBinError(2, shifte);
	params->SetBinContent(3, stretch);
	params->SetBinError(3, stretche);
	
	/// Fit:
	perform_fit(h_fjp, h_cdf, bins, params);
	
	print_params(params);
	
	/// Make a new template from the CDF using the fitted values
	TH1* h_fit = (TH1*) h_temp->Clone(ds + "_fit");
	differentiate_cdf(h_cdf, h_fit, median_from_cdf(h_cdf), params->GetBinContent(1), params->GetBinContent(2), params->GetBinContent(3));
	
	// Normalize unfitted stuff: (Don't normalize it before fitting!)
	h_temp->Scale(h_fjp->Integral(1, h_fjp->GetNbinsX())/h_temp->Integral(1, h_temp->GetNbinsX()));
	
	// Histogram styling:
	gStyle->SetOptStat(0);
	
	h_fjp->Rebin(nrebin);
	h_fjp->GetXaxis()->SetNdivisions(406);
	h_fjp->SetLineWidth(2);
//	h_fjp->SetMarkerSize(0.8);
	h_fjp->SetTitle("");
	h_temp->Rebin(nrebin);
	h_temp->SetFillColorAlpha(kBlue-10, 0.5);
	h_temp->SetLineWidth(2);
	h_temp->SetLineStyle(2);
	h_temp->SetMaximum(200);
	h_temp->SetMinimum(0.01);
	h_temp->GetXaxis()->SetNdivisions(406);
//		h_fj->Rebin(50);
//		h_fj->SetFillColor(kRed-4);
//		h_fj->SetFillStyle(3003);
	h_fit->Rebin(nrebin);
	h_fit->SetLineWidth(2);
	h_fit->SetFillStyle(0);
	
	// Histogram drawing:
	style_ylabel_th1(h_temp);
	h_fjp->GetXaxis()->SetTitle("Average jet mass [GeV]");
	for (int i=0; i < 2; ++ i) {
		TString name = "closure_" + ds + "_" + cut_name + "_f" + to_string(f);
		if (!ht) name = name + "_xht";
		if (dir != "") name + "_" + dir;
		if (i == 1) name = name + "_logy";
		draw_plot(name, ds, h_fjp, h_temp, h_fit, params, i);
	}
}










