#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void draw_first(TH1* fj) {
	TString name = "technique_first";
	TCanvas* tc = new TCanvas(name, name);
	
	fj->GetXaxis()->SetTitle("Jet fatjet mass [GeV]");
	
	fj->Draw("hist");
	
	TLegend* leg = new TLegend(0.47, 0.70, 0.77, 0.75);
	leg->AddEntry(fj, "Tagged fatjets", "f");
	leg->Draw();
	
	style_info();
	txt_ds = style_write(name_proper["qcdmg"],  0.63, 0.78);
//	txt_norm = style_write("#splitline{#bf{Normalization:}}{jet pair selection}",  0.55, 0.65);
	txt_norm = style_write("Normalized to analysis region",  0.18, 0.94, 0.025);
	
	tc->SaveAs(name + ".pdf");
}

void draw_second(TH1* fj, TH1* tempp, TH1* temp_xht) {
	TString name = "technique_second";
	TCanvas* tc = new TCanvas(name, name);
	TH1* temp = (TH1*) tempp->Clone("t");
	TH1* tempxht = (TH1*) temp_xht->Clone("txht");
	
	fj->GetXaxis()->SetTitle("(Average) fatjet mass [GeV]");
	tempxht->GetXaxis()->SetTitle("(Average) fatjet mass [GeV]");
	
	temp->Rebin(50);
	temp->Scale(fj->Integral()/temp->Integral());
	
	tempxht->Scale(fj->Integral()/tempxht->Integral());
//	temp->SetMaximum(130);
	
	tempxht->Draw("hist");
	fj->Draw("hist same");
	temp->Draw("same hist");
	tempxht->Draw("same hist");
	gPad->RedrawAxis();
	
	TLegend* leg = new TLegend(0.47, 0.60, 0.80, 0.75);
	leg->SetFillStyle(0);
	leg->AddEntry(fj, "Tagged fatjets", "f");
	leg->AddEntry(temp, "QCD template", "f");
	leg->AddEntry(tempxht, "QCD template w/out #it{H}_{T} re-weight", "f");
	leg->Draw();
	
	style_info();
	txt_ds = style_write(name_proper["qcdmg"],  0.63, 0.78);
//	txt_norm = style_write("#splitline{#bf{Normalization:}}{jet pair selection}",  0.55, 0.60);
	txt_norm = style_write("Normalized to analysis region",  0.18, 0.94, 0.025);
	
	tc->SaveAs(name + ".pdf");
}

void draw_third(TH1* temp, TH1* fjp) {
	TString name = "technique_third";
	TCanvas* tc = new TCanvas(name, name);
	TH1 *cdf = make_cdf(temp);
	
	double newAmp = 1.5e-6;
	double newShift = -10;
	double newStretch = 1.0;
	double newAmpe, newShifte, newStretche;
	vector<double> bins {0, 50, 100, 150, 200, 250, 300, 400, 500, 600, 800};
	perform_fit(fjp, cdf, bins, newAmp, newAmpe, newShift, newShifte, newStretch, newStretche);
	std::cout << "Amp=" << newAmp << " +/- " << newAmpe << std::endl;
	std::cout << "Shift=" << newShift << " +/- " << newShifte << std::endl;
	std::cout << "Stretch=" << newStretch << " +/- " << newStretche << std::endl;
	TH1* fit = (TH1*) temp->Clone("fit");
	differentiate_cdf(cdf, fit, median_from_cdf(cdf), newAmp, newShift, newStretch);
	
	temp->GetXaxis()->SetTitle("Average fatjet mass [GeV]");
	temp->Scale(fjp->Integral()/temp->Integral());
	temp->Rebin(50);
	style_ylabel(temp);
	
	fit->SetLineColor(kBlack);
	fit->Rebin(50);
	fit->SetFillStyle(0);
	fit->SetLineStyle(0);
	
	fjp->Rebin(50);
	
	temp->SetMaximum(fjp->GetMaximum()*1.2);
	temp->GetXaxis()->SetRangeUser(0, 1000);
	temp->Draw("hist");
	fit->Draw("same hist");
	fjp->Draw("same e");
	
	TLegend* leg = new TLegend(0.49, 0.60, 0.79, 0.75);
	leg->AddEntry(fjp, "Fatjet pairs", "lpe");
	leg->AddEntry(temp, "QCD template", "f");
	leg->AddEntry(fit, "Post-fit QCD template", "f");
	leg->Draw();
	
	style_info();
	txt_ds = style_write(name_proper["qcdmg"], 0.63, 0.78);
//	txt_norm = style_write("#splitline{#bf{Normalization:}}{jet pair selection}",  0.55, 0.60);
	txt_norm = style_write("Normalized to analysis region",  0.18, 0.94, 0.025);
	
	cout << temp->Integral() << endl;
	cout << fit->Integral() << endl;
	cout << fjp->Integral() << endl;
	
	tc->SaveAs(name + ".pdf");
}

void technique_styler(TString cut="sigl") {
	tf_in = TFile::Open(TString("technique_plots_") + cut + ".root");
	TH1* fjp = (TH1*) tf_in->Get("fjp");
	TH1* fj = (TH1*) tf_in->Get("fj");
	TH1* temp_xht = fetch_template("qcdmg", cut, "", 1, false);
	TH1* temp = fetch_template("qcdmg", cut);
	gStyle->SetOptStat(0);
	
	TH1* fjpp = (TH1*) fjp->Clone("throwaway");
	fjpp->Rebin(50);
	
	fj->SetTitle("");
	fj->SetFillColor(kRed);
	fj->SetFillStyle(3003);
	fj->Rebin(50);
	fj->Scale(fjp->Integral()/fj->Integral());
	fj->SetMaximum(fj->GetMaximum()*1.2);
	fj->GetXaxis()->SetRangeUser(0, 1000);
	fj->GetXaxis()->SetNdivisions(405);
	style_ylabel(fj);
	
	temp->SetTitle("");
	temp->SetFillStyle(1001);
	temp->SetFillColorAlpha(kBlue-7, 0.2);
	temp->SetLineStyle(2);
	temp->SetMaximum(fjpp->GetMaximum()*1.2);
	temp->GetXaxis()->SetRangeUser(0, 1000);
	temp->GetXaxis()->SetNdivisions(405);
	
	temp_xht->SetLineColor(kRed);
	temp_xht->Rebin(50);
	temp_xht->SetMaximum(temp_xht->GetMaximum()*1.2);
	temp_xht->GetXaxis()->SetRangeUser(0, 1000);
	temp_xht->GetXaxis()->SetNdivisions(405);
	style_ylabel(temp_xht);
	
	draw_first(fj);
	draw_second(fj, temp, temp_xht);
//	draw_third(temp, fjp);
}
