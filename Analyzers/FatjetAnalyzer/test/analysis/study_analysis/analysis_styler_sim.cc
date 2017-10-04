#include <iostream>
#include <sstream>
#include <string>
#include <Deracination/Straphanger/test/decortication/macros/common.cc>

bool VERBOSE = true;

void analysis_styler_sim(TString cut_name="sb", int f=1, int nrebin=30, double xmax=900) {
//	if (cut_name == "sbtb") assert(nrebin == 50);
//	if (cut_name == "sbtb" || cut_name == "sbideb") assert(nrebin == 30);
	if (cut_name != "sig15") assert(nrebin == 30);
	else assert(nrebin == 50);
	
	// Define input and output:
	TString name = cut_name + "_f" + to_string(f);
	TFile* tf_in = TFile::Open("analysis_plots_" + name + ".root");
	TFile* tf_out = new TFile("analysis_plots_fitsim_" + name + ".root", "RECREATE");
	
	// Extract plots:
	TH1* h_fjp_jetht = (TH1*) tf_in->Get("fjp_jetht");
	TH1* h_fjp_ttbar = (TH1*) tf_in->Get("fjp_ttbar");
	TH1* h_temp_jetht = (TH1*) tf_in->Get("temp_jetht");
	TH1* h_cdf_ttbar = (TH1*) tf_in->Get("cdf_ttbar");
	TH1* h_cdf_jetht = (TH1*) tf_in->Get("cdf_jetht");
	
	// Perform fits:
	vector<double> bins;
//	/// Best guesses for the amplitude, shift, and stretch:
	double newAmp1 = 8.0e-7;       // Floats
	double newShift1 = 0.0;        // Floats
	double newStretch1 = 1.0;      // Floats
	double newAmp2 = 1.0;          // Floats
	double newShift2 = 0.0;        // Floats
	double newStretch2 = 1.0;      // Floats
//	double newAmp1, newShift1, newStretch1, newAmp2, newShift2, newStretch2;
	double newAmpe1, newShifte1, newStretche1, newAmpe2, newShifte2, newStretche2;
//	if (cut_name == "sbb") {
//		bins = {0, 100, 150, 200, 250, 300, 350, 400, 450, 500, 600};
//		newAmp1 = 2.0e-6;
//		newShift1 = -10;
//		newStretch1 = 1.00;
//		newAmp2 = 0.95;
//		newShift2 = 0.0;
//		newStretch2 = 1.0;
//	}
	if (cut_name == "sbb") {
		if (f == 0) {
	//		bins = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600};
			bins = {0, 60, 80, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 280, 300, 310, 320, 330, 340, 350, 360, 400, 420, 440, 480, 500, 540, 600};
			newAmp1 = 2.0e-6;
			newShift1 = -11;
			newStretch1 = 1.00;
			newAmp2 = 0.90;
			newShift2 = 2.0;
			newStretch2 = 1.0;
		}
		else if (f == 1) {
			bins = {90, 120, 150, 210, 240, 300, 360, 420, 480, 540, 600};
//			newAmp1 = 4e-7;
			newAmp1 = 0.5;
			newShift1 = -15;
			newStretch1 = 1.0;
			newAmp2 = 0.95;
			newShift2 = -10;
			newStretch2 = 1.1;
		}
	}
	else if (cut_name == "sb") {
		if (f == 0) {
	//		bins = {0, 100, 150, 200, 250, 300, 350, 400, 500};
			bins = {0, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 280, 300, 320, 340, 360, 400, 440, 480, 500, 540, 600};
			newAmp1 = 2.0e-6;
			newShift1 = -20;
			newStretch1 = 0.95;
			newAmp2 = 0.95;
			newShift2 = 0.0;
			newStretch2 = 1.24;
		}
		else if (f == 1) {
			bins = {0, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 420, 480, 540, 600, 660, 810};
//			newAmp1 = 4e-7;
			newAmp1 = 0.8;
			newShift1 = -20;
			newStretch1 = 0.95;
			newAmp2 = 0.90;
			newShift2 = 5.0;
			newStretch2 = 1.1;
		}
	}
	else if (cut_name == "sblb") {
		if (f == 0) {
	//		bins = {0, 100, 150, 200, 250, 300, 400, 500, 600, 700};
	//		bins = {40, 80, 120, 160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700};
			bins = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 600};
			newAmp1 = 4e-7;
			newShift1 = -20;
			newStretch1 = 1.0;
			newAmp2 = 0.90;
			newShift2 = 0.0;
			newStretch2 = 1.0;
		}
		else if (f == 1) {
//			bins = {20, 80, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700};	// worked before lum change
			bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 360, 420, 480, 600};
//			newAmp1 = 4e-7;
			newAmp1 = 0.85;
			newShift1 = -20.0;
			newStretch1 = 0.9;
			newAmp2 = 1.00;
			newShift2 = 0.0;
			newStretch2 = 1.0;
		}
	}
	else if (cut_name == "sbl") {
		if (f == 0) {
	//		bins = {0, 100, 150, 200, 250, 300, 350, 400, 500};
			bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 480, 510, 540, 600};
			newAmp1 = 4e-7;
			newShift1 = -20;
			newStretch1 = 0.90;
			newAmp2 = 0.9;
			newShift2 = 1.0;
			newStretch2 = 1.0;
		}
		else if (f == 1) {
//			bins = {90, 120, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 270, 300, 330, 360, 390, 420, 450};
			bins = {90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 260, 270, 300, 330, 360, 390, 420, 480, 540, 600};
//			bins = {60, 90, 120, 150, 180, 210, 240, 270, 330, 360, 390, 420, 450};
			newAmp1 = 1.0;
			newShift1 = -20;
			newStretch1 = 0.90;
			newAmp2 = 1.0;
			newShift2 = 1.0;
			newStretch2 = 1.0;
		}
	}
	else if (cut_name == "sbl42b") {
		bins = {0, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 420, 480, 540};
//			newAmp1 = 4e-7;
		newAmp1 = 0.8;
		newShift1 = -20;
		newStretch1 = 0.95;
		newAmp2 = 0.90;
		newShift2 = 5.0;
		newStretch2 = 1.1;
	}
	else if (cut_name == "sbl42") {
//		bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 420, 480, 540, 600, 660};
		bins = {0, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};		// Converges: 0.97 =/- 0.8
//		bins = {0, 60, 90, 120, 150, 180, 210, 240, 270, 330, 450, 600};		// Converges: 2.05 +/- 0.6 (?)
//		bins = {60, 90, 120, 150, 180, 210, 240, 270, 300, 360, 420, 480, 540, 600};
//			newAmp1 = 4e-7;
		newAmp1 = 0.8;
		newShift1 = -20;
		newStretch1 = 0.95;
		newAmp2 = 0.90;
		newShift2 = 5.0;
		newStretch2 = 1.1;
	}
	else if (cut_name == "sbl43b") {		// chi2
		bins = {90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 420, 450};
//			newAmp1 = 4e-7;
		newAmp1 = 0.8;
		newShift1 = -20;
		newStretch1 = 0.95;
		newAmp2 = 0.90;
		newShift2 = 5.0;
		newStretch2 = 1.1;
	}
	else if (cut_name == "sbl43") {
		bins = {0, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 420, 480, 540, 600, 660};
//			newAmp1 = 4e-7;
		newAmp1 = 0.8;
		newShift1 = -20;
		newStretch1 = 0.95;
		newAmp2 = 0.90;
		newShift2 = 5.0;
		newStretch2 = 1.1;
	}
	else if (cut_name == "sbtb") {
		if (f == 0) {
	//		bins = {0, 100, 150, 200, 250, 300, 350, 400, 450, 700};
			bins = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500};		// chi2
			newAmp1 = 2e-6;
			newShift1 = 30;
			newStretch1 = 0.75;
			newAmp2 = 1.3;
			newShift2 = -20.0;
			newStretch2 = 1.1;
		}
		else if (f == 1) {
			bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 420, 480, 540, 660}; // chi2
			newAmp1 = 4e-7;
			newShift1 = 0;
			newStretch1 = 1.0;
			newAmp2 = 1.0;
			newShift2 = -20.0;
			newStretch2 = 1.0;
		}
	}
	else if (cut_name == "sbt") {
		if (f == 0) {
//			bins = {80, 100, 120, 130, 140, 150, 160, 180, 200, 250, 300, 350, 400, 420, 440, 460};
			bins = {0, 60, 90, 120, 150, 180, 210, 240, 300, 360, 420, 480, 540, 660};
			newAmp1 = 1.2e-6;
			newShift1 = -20;
			newStretch1 = 1.00;
			newAmp2 = 1.0;
			newShift2 = -10.0;
			newStretch2 = 1.0;
		}
		else if (f == 1) {
			bins = {60, 90, 120, 150, 180, 210, 240, 270, 300, 360, 390, 420, 450, 600};
			newAmp1 = 4e-7;
			newShift1 = -20;
			newStretch1 = 1.00;
			newAmp2 = 1.0;
			newShift2 = -10.0;
			newStretch2 = 1.0;
		}
	}
	else if (cut_name == "sbideb") {
		bins = {90, 120, 150, 180, 210, 240, 300, 330, 360, 390, 420, 450, 480};
		newAmp1 = 0.8;
		newShift1 = 20.0;
		newStretch1 = 1.00;
		newAmp2 = 1.0;
		newShift2 = -20.0;
		newStretch2 = 1.0;
	}
	else if (cut_name == "sbide") {
//		bins = {60, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 450, 480, 510, 540, 570};
		bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 390, 510, 540, 570, 600, 630};
		newAmp1 = 1.2;
		newShift1 = -20;
		newStretch1 = 1.00;
		newAmp2 = 1.0;
		newShift2 = -10.0;
		newStretch2 = 1.0;
	}
	else if (cut_name == "sig") {
//		bins = {60, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 450, 480, 510, 540, 570};
		bins = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 390, 510, 540, 570, 600, 630};
		newAmp1 = 1.2;
		newShift1 = -20;
		newStretch1 = 1.00;
		newAmp2 = 1.0;
		newShift2 = -10.0;
		newStretch2 = 1.0;
	}
	else if (cut_name == "sig15") {
//		bins = {60, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 450, 480, 510, 540, 570};
		bins = {0, 50, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600};
		newAmp1 = 1.0;
		newShift1 = 0.0;
		newStretch1 = 1.00;
		newAmp2 = 1.0;
		newShift2 = -10.0;
		newStretch2 = 1.0;
	}
	if (cut_name != "sbtb" && cut_name != "sbideb" && cut_name != "sbl43b" && cut_name != "sig15") perform_stacked_fit(h_fjp_jetht, h_cdf_jetht, h_cdf_ttbar, bins, newAmp1, newAmpe1, newShift1, newShifte1, newStretch1, newStretche1, newAmp2, newAmpe2, newShift2, newShifte2, newStretch2, newStretche2);
	else perform_stacked_fit_chi(h_fjp_jetht, h_cdf_jetht, h_cdf_ttbar, bins, newAmp1, newAmpe1, newShift1, newShifte1, newStretch1, newStretche1, newAmp2, newAmpe2, newShift2, newShifte2, newStretch2, newStretche2);
	
	if (VERBOSE) {
		std::cout << "Amp1=" << newAmp1 << " +/- " << newAmpe1 << std::endl;
		std::cout << "Shift1=" << newShift1 << " +/- " << newShifte1 << std::endl;
		std::cout << "Stretch1=" << newStretch1 << " +/- " << newStretche1 << std::endl;
		std::cout << "Amp2=" << newAmp2 << " +/- " << newAmpe2 << std::endl;
		std::cout << "Shift2=" << newShift2 << " +/- " << newShifte2 << std::endl;
		std::cout << "Stretch2=" << newStretch2 << " +/- " << newStretche2 << std::endl;
	}
	
	// Save fit parameters into a histogram:
	TH1D* h_fit_params = new TH1D("params", "", 6, 0, 6);
	h_fit_params->SetBinContent(1, newAmp1);
	h_fit_params->SetBinError(1, newAmpe1);
	h_fit_params->SetBinContent(2, newShift1);
	h_fit_params->SetBinError(2, newShifte1);
	h_fit_params->SetBinContent(3, newStretch1);
	h_fit_params->SetBinError(3, newStretche1);
	h_fit_params->SetBinContent(4, newAmp2);
	h_fit_params->SetBinError(4, newAmpe2);
	h_fit_params->SetBinContent(5, newShift2);
	h_fit_params->SetBinError(5, newShifte2);
	h_fit_params->SetBinContent(6, newStretch2);
	h_fit_params->SetBinError(6, newStretche2);
	
	// Make fit plots:
	TH1* h_fit_jetht = (TH1*) h_temp_jetht->Clone("fit_jetht");
	differentiate_cdf(h_cdf_jetht, h_fit_jetht, median_from_cdf(h_cdf_jetht), newAmp1, newShift1, newStretch1);
	TH1* h_fit_ttbar = (TH1*) h_fjp_ttbar->Clone("fit_ttbar");
	differentiate_cdf(h_cdf_ttbar, h_fit_ttbar, median_from_cdf(h_cdf_ttbar), newAmp2, newShift2, newStretch2);
	
	// Styling:
	h_fit_jetht->Rebin(nrebin);
	h_fit_jetht->SetFillColorAlpha(kBlue-10, 0.5);
	h_fit_jetht->SetFillStyle(1001);
	h_fit_jetht->SetLineStyle(2);
	h_fit_jetht->SetLineWidth(2);
//	if (cut_name == "sbb" || cut_name == "sbtb" || cut_name == "sbideb") h_fit_ttbar->Rebin(1);
//	else h_fit_ttbar->Rebin(nrebin/10);
//	if (cut_name == "sbl") h_fit_ttbar->Rebin(nrebin/10);
	if (cut_name == "sbl") h_fit_ttbar->Rebin(nrebin/5);
	h_fit_ttbar->SetFillColor(kRed - 4);
	h_fit_ttbar->SetFillStyle(3003);
	h_fit_ttbar->SetMarkerSize(0);
	h_fit_ttbar->SetLineWidth(2);
	h_fjp_jetht->Rebin(nrebin);
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
	name = "analysis_" + name + "_sim_" + to_string(nrebin);
	for (int i=0; i < 2; ++ i) {
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
		h_fjp_draw->Draw("same e");
//		h_bkg->SetLineColor(kRed);
//		h_bkg->Draw("same");
		
		/// Legend:
		TLegend* leg;
		if (i == 0 || cut_name == "sbtb") leg = new TLegend(0.50, 0.60, 0.80, 0.75);
		else if (i == 1) leg = new TLegend(0.22, 0.10, 0.53, 0.25);
		leg->AddEntry(h_fjp_jetht, name_proper["jetht"], "ple");
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
		style_cut(cut_name, true);
		
		
		std::ostringstream oss1;
		oss1 << "#theta_{t#bar{t} norm} = " << std::fixed << std::setprecision(2) << newAmp2 << " #pm " << newAmpe2;
		std::ostringstream oss2;
		oss2 << "#theta_{t#bar{t} shift} = " << std::fixed << std::setprecision(1) << newShift2 << " #pm " << newShifte2 << " GeV";
		std::ostringstream oss3;
		oss3 << "#theta_{t#bar{t} stretch} = (" << std::fixed << std::setprecision(1);
		if (newStretch2 > 1) oss3 << "+";
		oss3 << 100*(newStretch2 - 1) << " #pm " << 100*newStretche2 << ") \%";
		std::ostringstream oss4;
		oss4 << "#theta_{QCD shift} = " << std::fixed << std::setprecision(1) << newShift1 << " #pm " << newShifte1 << " GeV";
		std::ostringstream oss5;
		oss5 << "#theta_{QCD stretch} = (" << std::fixed << std::setprecision(1);
		if (newStretch1 > 1) oss5 << "+";
		oss5 << 100*(newStretch1 - 1) << " #pm " << 100*newStretche1 << ") \%";
		
		vector<TString> texts_par;
		texts_par.push_back("#bf{Fit parameters:}");
		texts_par.push_back(oss1.str());
		texts_par.push_back(oss2.str());
		texts_par.push_back(oss3.str());
		texts_par.push_back(oss4.str());
		texts_par.push_back(oss5.str());
		
		if (i == 0 || cut_name == "sbtb") style_write(texts_par, 0.56, 0.55, 0.027);
		else if (i == 1) style_write(texts_par, 0.56, 0.72, 0.027);
	
		// Save:
		tc->SaveAs(name + ".pdf");
	}		// End logy loop.
	
	// Write results to file:
	tf_out->cd();
	h_fjp_jetht->Write();
	h_fjp_ttbar->Write();
	h_temp_jetht->Write();
	h_cdf_jetht->Write();
	h_cdf_ttbar->Write();
	h_fit_jetht->Write();
	h_fit_ttbar->Write();
	h_fit_params->Write();
	
	
}
