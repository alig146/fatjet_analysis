#include "/home/tote/decortication/macros/common.cc"

void compare(TString cut_sig="sig15", TString cut_sb="sb") {
	tf_input = TFile::Open("theta_plots_" + cut_sig + "_" + cut_sb + ".root");
	tf_post = TFile::Open("theta_plots_" + cut_sig + "_" + cut_sb + "_prediction.root");
	tf_mle = TFile::Open("theta_plots_" + cut_sig + "_" + cut_sb + "_mle.root");
	TString name = "compare_posteriors_" + cut_sig + "_" + cut_sb;
	TCanvas* tc = new TCanvas(name, name);
	
	TH1* mle_qcd = (TH1*) tf_mle->Get("mAvgPSel__QCD");
	TH1* mle_ttbar = (TH1*) tf_mle->Get("mAvgPSel__TTbar");
	TH1* post = (TH1*) tf_post->Get("Ms100__mAvgPSel__0");
	TH1* mle_total = (TH1*) mle_qcd->Clone("mle_total");
	mle_total->Add(mle_ttbar);
//	TH1* mle_totale = (TH1*) mle_qcd->Clone("mle_totale");
	TH1* mle_totale = (TH1*) post->Clone("mle_totale");
	THStack* ths = new THStack();
	ths->Add(mle_qcd);
	ths->Add(mle_ttbar);
	TH1* data = (TH1*) tf_input->Get("mAvgPSel__DATA");
	
	mle_qcd->SetFillColorAlpha(kBlue-10, 0.5);
	mle_qcd->SetLineStyle(2);
	
	mle_ttbar->SetFillColor(kRed - 4);
	mle_ttbar->SetFillStyle(3003);
	
	mle_total->SetMarkerSize(0);
	mle_total->SetFillStyle(0);
	
	mle_totale->SetMarkerSize(0);
	mle_totale->SetFillColorAlpha(kBlack, 0.2);
	
	data->SetFillStyle(0);
	
	ths->Draw("hist");
	mle_total->Draw("same hist");
	mle_totale->Draw("same e2");
	
	ths->GetXaxis()->SetNdivisions(405);
	ths->GetXaxis()->SetTitle("Average jet mass [GeV]");
	style_ylabel(ths);
//	if (cut_sig == "sig15") ths->SetMaximum(13);
//	else ths->SetMaximum(75);
	ths->SetMaximum(data->GetMaximum()*1.2);
	
	data->Draw("same e");
//	
//	
//	tf_post->cd();
//	TH1* post = (TH1*) gDirectory->Get("Ms100__mAvgPSel__0");
//	post->SetLineWidth(2);
//	post->SetLineColor(kGreen);
//	post->Draw("e same");
////	TH1* total = (TH1*) post->Clone("post_total");
////	for (int ibin = 1; ibin < total->GetNbinsX() + 1; ++ibin) {
////		total->SetBinContent(ibin, mle_qcd->GetBinContent(ibin) + mle_ttbar->GetBinContent(ibin));
////		total->SetBinError(ibin, post->GetBinError(ibin));
////	}
////	total->Draw("e same");
//	
//	tf_input->cd();
//	data->SetMarkerColor(kRed);
//	data->SetLineColor(kRed);
//	data->Draw("e same");

	TLegend* leg = new TLegend(0.50, 0.65, 0.80, 0.80);
	leg->AddEntry(mle_totale, "Bkg. posterior uncertainty", "lf");
	leg->AddEntry(mle_qcd, "QCD component", "f");
	leg->AddEntry(mle_ttbar, "t#bar{t} component", "f");
	if (cut_sig == "sig15") leg->AddEntry(data, "JetHT 2015", "ple");
	else leg->AddEntry(data, "JetHT '15 + '16", "ple");
	leg->Draw();
//	
	if (cut_sig == "sig15") style_info(false, "2.3");
	else style_info(false);
	

	cout << "QCD events: " << mle_qcd->Integral() << endl;
	cout << "TTBar events: " << mle_ttbar->Integral() << endl;
	
	tc->SaveAs(name + ".pdf");
}
