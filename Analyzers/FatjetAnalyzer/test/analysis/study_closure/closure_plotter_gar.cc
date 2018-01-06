#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void fix_zeros(TH1* h) {
	for (unsigned i = 0; i <= h->GetNbinsX(); ++i) {
		if (h->GetBinContent(i) == 0) h->SetBinContent(i, 0.0001);
		cout << i << " " << h->GetBinContent(i) << endl;
	}
}

void draw_plot(TString name, TString ds, TString cut, TH1* data, TH1* temp, TH1* fit, TH1* params, vector<Double_t> stats, int logy=0) {
	// Make pull:
	TCanvas* tc = draw_pull(name, data, fit, 0, 1200);
	TPad* pad = (TPad*) tc->GetPad(1);
	pad->cd();
	tc->SetLogy(logy);
	
	// Draw upper pad:
	temp->Draw("hist");
	fit->Draw("same");
	if (logy == 0) {
		temp->GetYaxis()->SetRangeUser(0, data->GetMaximum()*1.2);
//		temp->SetMinimum(0);
//		temp->SetMaximum(data->GetMaximum()*1.2);
	}
	else if (logy == 1) {
		temp->GetYaxis()->SetRangeUser(0.1, data->GetMaximum()*10);
//		temp->SetMinimum(0.1);
//		temp->SetMaximum(data->GetMaximum()*10);
	}
	data->Draw("same e0 p");
	temp->GetXaxis()->SetLabelOffset(100);
	pad->SetLogy(logy);
	
	TLegend* leg;
	if (logy == 0) leg = new TLegend(0.55, 0.57, 0.80, 0.72);
	else if (logy == 1) leg = new TLegend(0.22, 0.1, 0.48, 0.25);
	leg->AddEntry(data, "Jet pair selection", "ple");
	leg->AddEntry(temp, "Derived template", "f");
	leg->AddEntry(fit, "Post-fit template", "f");
	leg->Draw();
	style_info(true, lum_string["all"], 1, true);
	style_write(TString("Selection: #bf{") + cut_proper[cut] + "}",  0.18, 0.94, 0.032);
	
	style_write(name_proper[ds], 0.61, 0.74, 0.04);
	
	// Statistical text:
	TFile* tf_stats = TFile::Open(name + ".root");
	TH1* hstats = (TH1*) tf_stats->Get("stats");
//	cout << "Pull Chi2/ndf = " << stats->GetBinContent(3) << endl;
//	cout << "KS probability = " << data->KolmogorovTest(fit) << endl;
	vector<TString> texts_stats;
	texts_stats.push_back("#bf{Statistics:}");
	std::ostringstream oss_stats1;
//	oss_stats1 << "KS prob. = " << std::fixed << std::setprecision(3) << stats[0];
	std::ostringstream oss_stats2;
	oss_stats2 << "pull #chi^{2}/NDF = " << std::fixed << std::setprecision(2) << hstats->GetBinContent(3);
	texts_stats.push_back(oss_stats2.str());
	texts_stats.push_back(oss_stats1.str());
	if (logy == 1) style_write(texts_stats, 0.21, 0.87, 0.035);
	else if (logy == 0) style_write(texts_stats, 0.55, 0.35, 0.035);
	
	// Write params:
	std::ostringstream oss1;
	oss1 << "shift = " << std::fixed << std::setprecision(1) << params->GetBinContent(2) << " #pm " << params->GetBinError(2) << " GeV";
	std::ostringstream oss2;
	oss2 << "stretch = (" << std::fixed << std::setprecision(1);
	if (params->GetBinContent(3) > 1) oss2 << "+";
	oss2 << 100*(params->GetBinContent(3) - 1) << " #pm " << 100*params->GetBinError(3) << ")\%";
	vector<TString> texts_par;
	texts_par.push_back("#bf{Fit parameters:}");
	texts_par.push_back(oss1.str());
	texts_par.push_back(oss2.str());
	if (logy == 1) style_write(texts_par, 0.54, 0.67, 0.035);
	else if (logy == 0) style_write(texts_par, 0.54, 0.50, 0.035);
	
	tc->SaveAs(name + ".pdf");
	tc->SaveAs(name + ".png");
}

void closure_plotter_gar(TString cut_name="sb", TString ds="qcdmg", int nrebin=30, bool ht=true, TString dir="", int f=1) {
//	TFile* tf_in = TFile::Open("~/anatuples/anatuple_dalitz_predeta.root");
	TFile* tf_in = get_ana();
	if (ds == "qcdmg") tf_in = get_ana("qcdmgext");
	
	TTree *tt = (TTree*) tf_in->Get(ds);
	
//	tt->Draw("mavg_p>>" + ds + "_fjp(1200,0,1200)", get_cut("fjp_" + cut_name, get_weight(ds)));
//	TH1 *h_fjp = (TH1*) gDirectory->Get(ds + "_fjp");
	TH1D* h_fjp = make_qcd_garwood(tf_in, ds, cut_name, nrebin);
	cout << "here" << endl;
	TH1* h_temp = fetch_template(ds, cut_name, dir, f, ht);
	TH1* h_cdf = make_cdf(h_temp, ds + "_cdf");
	
	
	/// Save untouched plots for use later:
	TH1* h_fjp_original = (TH1*) h_fjp->Clone("fjp");
	
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
		amp = 1.0;
		shift = -10;
		stretch = 0.90;
//		bins = {0, 100, 150, 200, 250, 300, 350, 400, 500, 600, 650};
		bins = {0, 90, 150, 210, 240, 300, 360, 420, 480, 600, 660};
	}
	else if (ds == "qcdp" && cut_name == "sb") {
		amp = 1.0;
		shift = -20;
		stretch = 0.90;
//		bins = {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 600, 800};
//		bins = {0, 100, 150, 200, 250, 300, 350, 400, 500, 600};
		bins = {0, 90, 150, 210, 240, 300, 360, 420, 480, 600, 660};
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
		amp = 2.0;
		shift = -20.0;
		stretch = 1.0;
		bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 450, 480, 510, 540, 570, 600};
	}
	else if (ds == "qcdp" && cut_name == "sig") {
		amp = 1.0;
		shift = -20.0;
		stretch = 1.0;
		bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 330, 390, 450, 510, 570, 630};
	}
	else if (ds == "qcdmg" && cut_name == "sigl" && ht) {		// This works
		amp = 100.0;
		shift = -10;
		stretch = 1.0;
//		bins = {50, 100, 150, 250, 300, 350, 400, 450, 500, 550, 600, 800};
		bins = {60, 90, 120, 150, 180, 210, 240, 300, 360, 390, 450, 510, 540, 600, 810};
		bins = {60, 120, 150, 240, 300, 360, 390, 450, 510, 540, 600, 810};
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
		amp = 1.0;
		shift = -10.0;
		stretch = 1.0;
//		bins = {0, 200, 300, 400, 500, 600, 700, 800, 900, 1100};
//		bins = {0, 30, 60, 90, 120, 150, 240, 300, 360, 390, 450, 510, 540, 600, 660, 720, 780, 840, 1200};
//		bins = {0, 30, 60, 90, 120, 150, 240, 300, 360, 390, 450, 510, 540, 600};
//		bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 360, 390, 450, 510, 540, 600};
		bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 360, 390, 450, 510, 540, 600, 660, 720, 780, 840, 900, 960, 1020, 1080, 1200};
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
	perform_fit_chi2(h_fjp, h_cdf, bins, params);
	
	print_params(params);
	
	/// Make a new template from the CDF using the fitted values
	TH1* h_fit = (TH1*) h_temp->Clone(ds + "_fit");
	differentiate_cdf(h_cdf, h_fit, median_from_cdf(h_cdf), params->GetBinContent(1), params->GetBinContent(2), params->GetBinContent(3));
	
	// Normalize unfitted stuff: (Don't normalize it before fitting!)
	h_temp->Scale(h_fjp->Integral(1, h_fjp->GetNbinsX())/h_temp->Integral(1, h_temp->GetNbinsX()));
	
	// Calculate some stats:
	vector<Double_t> stats;
	h_fit->Rebin(nrebin);
	stats.push_back(h_fjp->KolmogorovTest(h_fit));
	
	// Histogram styling:
	gStyle->SetOptStat(0);
	
	h_fjp->GetXaxis()->SetNdivisions(406);
	h_fjp->SetLineWidth(2);
//	h_fjp->SetMarkerSize(0.8);
	h_fjp->SetTitle("");
	h_fjp->GetXaxis()->SetTitle(get_xtitle("mavg"));
//	fix_zeros(h_fjp);
	h_temp->Rebin(nrebin);
	h_temp->SetFillColorAlpha(kBlue-10, 0.5);
	h_temp->SetLineWidth(2);
	h_temp->SetLineStyle(2);
//	h_temp->SetMaximum(200);
//	h_temp->SetMinimum(0.01);
	h_temp->GetXaxis()->SetNdivisions(406);
	h_fit->SetLineWidth(2);
	h_fit->SetFillStyle(0);
	
	// Histogram drawing:
	style_ylabel_th1(h_temp);
	for (int i=0; i < 2; ++ i) {
		TString name = "closure_gar_" + ds + "_" + cut_name + "_f" + to_string(f);
		if (!ht) name = name + "_xht";
		if (dir != "") name + "_" + dir;
		if (i == 1) name = name + "_logy";
		draw_plot(name, ds, cut_name, h_fjp, h_temp, h_fit, params, stats, i);
	}
	
}










