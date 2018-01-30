#include <iostream>
#include <sstream>
#include <string>
#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void manual_styler(TString cut, double normQCD, double shiftQCD, double stretchQCD, double normTTbar, double shiftTTbar, double stretchTTbar) {
	TFile* tf_in = TFile::Open("../analysis_plots_" + cut + "_f1.root");
	
	
	TH1D* h_fjp_jetht = (TH1D*) tf_in->Get("fjp_jetht");
//	h_fjp_jetht = GarwoodErrorBands_68CL(h_fjp_jetht);
	TH1* h_fjp_ttbar = (TH1*) tf_in->Get("fjp_ttbar");
	TH1* h_temp_jetht = (TH1*) tf_in->Get("temp_jetht");
	TH1* h_cdf_ttbar = (TH1*) tf_in->Get("cdf_ttbar");
	TH1* h_cdf_jetht = (TH1*) tf_in->Get("cdf_jetht");
	
	
	
	
//	std::cout << "Amp1=" << newAmp1 << " +/- " << newAmpe1 << std::endl;
//	std::cout << "Shift1=" << newShift1 << " +/- " << newShifte1 << std::endl;
//	std::cout << "Stretch1=" << newStretch1 << " +/- " << newStretche1 << std::endl;
//	std::cout << "Amp2=" << newAmp2 << " +/- " << newAmpe2 << std::endl;
//	std::cout << "Shift2=" << newShift2 << " +/- " << newShifte2 << std::endl;
//	std::cout << "Stretch2=" << newStretch2 << " +/- " << newStretche2 << std::endl;
	
	
	// Morph plots:
	TH1* h_fit_jetht = (TH1*) h_temp_jetht->Clone("fit_jetht");
	differentiate_cdf(h_cdf_jetht, h_fit_jetht, median_from_cdf(h_cdf_jetht), normQCD, shiftQCD, stretchQCD);
	TH1* h_fit_ttbar = (TH1*) h_fjp_ttbar->Clone("fit_ttbar");
	differentiate_cdf(h_cdf_ttbar, h_fit_ttbar, median_from_cdf(h_cdf_ttbar), normTTbar, shiftTTbar, stretchTTbar);
	
	// Styling:
	int nrebin = 30;
	double xmax = 900;
	h_fit_jetht->Rebin(nrebin);
	h_fit_jetht->SetFillColorAlpha(kBlue-10, 0.5);
	h_fit_jetht->SetFillStyle(1001);
	h_fit_jetht->SetLineStyle(2);
	h_fit_jetht->SetLineWidth(2);
//	if (cut_name == "sbb" || cut_name == "sbtb" || cut_name == "sbideb") h_fit_ttbar->Rebin(1);
//	else h_fit_ttbar->Rebin(nrebin/10);
//	if (cut_name == "sbl") h_fit_ttbar->Rebin(nrebin/10);
	if (cut == "sbl") h_fit_ttbar->Rebin(nrebin/5);
	h_fit_ttbar->SetFillColor(kRed - 4);
	h_fit_ttbar->SetFillStyle(3003);
	h_fit_ttbar->SetMarkerSize(0);
	h_fit_ttbar->SetLineWidth(2);
	h_fjp_jetht->Rebin(nrebin);
	h_fjp_jetht = GarwoodErrorBands_68CL(h_fjp_jetht);
	h_fjp_jetht->SetLineWidth(2);
	h_fjp_jetht->SetMarkerSize(1.2);
	h_fjp_jetht->GetXaxis()->SetRangeUser(0, xmax);
	h_fjp_jetht->GetXaxis()->SetNdivisions(406);
	h_fjp_jetht->GetXaxis()->SetTitle(get_xtitle("mavg"));
	TH1* h_fjp_draw = (TH1*) h_fjp_jetht->Clone("fjp_draw");
	h_fjp_draw->GetXaxis()->SetLabelSize(0);
	h_fjp_draw->GetXaxis()->SetTitle("");
	
	// Stack:
	THStack* ths = new THStack("hs", "");
	ths->Add(h_fit_jetht);
	ths->Add(h_fit_ttbar);
	
	// Total bkg distribution:
	TH1* h_bkg = (TH1*) h_fjp_draw->Clone("bkg");
	for (int i=1; i <= h_bkg->GetNbinsX(); ++ i) {
		h_bkg->SetBinContent(i, h_fit_jetht->GetBinContent(i) + h_fit_ttbar->GetBinContent(i));
		h_bkg->SetBinError(i, 0);
	}
	h_bkg->SetFillStyle(0);
	
	// Loop over logy:
	TString name = "test";
	for (int i=0; i < 1; ++ i) {
		if (i==1) name += "_logy";
		
//		cout << h_fjp_jetht->GetBinWidth(1) << endl;
//		cout << h_bkg->GetBinWidth(1) << endl;
		TCanvas* tc = draw_pull(name, h_fjp_jetht, h_bkg, 0, xmax);
//		TCanvas* tc = (TCanvas*) gDirectory->Get(name);
//		TCanvas* tc = new TCanvas(name, name);
		TPad* pad = (TPad*) tc->GetPad(1);
		pad->cd();
		h_fjp_draw->Draw("same e");
		pad->SetLogy(i);
//		tc->SetLogy(i);
		
		// Draw:
		THStack* hs = (THStack*) ths->Clone(TString("hs") + to_string(i));
		hs->Draw("hist e");
		hs->GetXaxis()->SetLabelOffset(1000);
		h_bkg->Draw("same hist");
		h_fjp_draw->Draw("same e0");
//		h_bkg->SetLineColor(kRed);
//		h_bkg->Draw("same");
		
		/// Legend:
//		TLegend* leg = get_legend(1, 3);
		TLegend* leg;
		if (i == 0) leg = new TLegend(0.50, 0.60, 0.80, 0.75);
		else if (i == 1) leg = new TLegend(0.22, 0.10, 0.53, 0.25);
		leg->AddEntry(h_fjp_jetht, name_proper["data"], "ple");
//		leg->AddEntry(h_fjp_jetht, name_proper["jetht"], "ple");
		leg->AddEntry(h_fit_jetht, "QCD template", "f");
		leg->AddEntry(h_fit_ttbar, "t#bar{t} MC", "f");
		leg->Draw();
		
		// Axes:
		hs->GetXaxis()->SetRangeUser(0, xmax);
		hs->GetXaxis()->SetNdivisions(h_fjp_jetht->GetXaxis()->GetNdivisions());
		hs->GetXaxis()->SetTitle("");
		hs->GetXaxis()->SetLabelSize(0);
		style_ylabel(hs);
		if (i == 0) hs->SetMaximum(h_fjp_jetht->GetMaximum()*1.1);
		else if (i == 1) {
			hs->SetMaximum(h_fjp_jetht->GetMaximum()*2.0);
			hs->SetMinimum(0.5);
		}
		
		// Info:
		style_info(false, lum_string["all"], 1, true);
		style_cut(cut, true);
		
		
//		std::ostringstream oss1;
//		oss1 << "t#bar{t} norm = " << std::fixed << std::setprecision(2) << newAmp2 << " #pm " << newAmpe2;
//		std::ostringstream oss2;
//		oss2 << "t#bar{t} shift = " << std::fixed << std::setprecision(1) << newShift2 << " #pm " << newShifte2 << " GeV";
//		std::ostringstream oss3;
//		oss3 << "t#bar{t} stretch = (" << std::fixed << std::setprecision(1);
//		if (newStretch2 > 1) oss3 << "+";
//		oss3 << 100*(newStretch2 - 1) << " #pm " << 100*newStretche2 << ")\%";
//		std::ostringstream oss4;
//		oss4 << "QCD shift = " << std::fixed << std::setprecision(1) << newShift1 << " #pm " << newShifte1 << " GeV                                               #bar{t}";
//		std::ostringstream oss5;
//		oss5 << "QCD stretch = (" << std::fixed << std::setprecision(1);
//		if (newStretch1 > 1) oss5 << "+";
//		oss5 << 100*(newStretch1 - 1) << " #pm " << 100*newStretche1 << ")\%                                               #bar{t}";
		
//		vector<TString> texts_par;
//		texts_par.push_back("#bf{Fit parameters:}");
//		texts_par.push_back(oss1.str());
//		texts_par.push_back(oss2.str());
//		texts_par.push_back(oss3.str());
//		texts_par.push_back(oss4.str());
//		texts_par.push_back(oss5.str());
		
//		if (i == 0 || cut_name == "sbtb") style_write(texts_par, 0.51, 0.55, 0.032);
//		else if (i == 1) style_write(texts_par, 0.56, 0.72, 0.027);
	
		// Save:
		save(tc);
	}		// End logy loop.
}
