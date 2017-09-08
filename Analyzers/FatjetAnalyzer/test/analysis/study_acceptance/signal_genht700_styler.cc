#include "/home/tote/decortication/macros/common.cc"

void signal_genht700_styler() {
	TFile* tf_in = TFile::Open("signal_genht700_plots.root");
	TGraph* accept = (TGraph*) tf_in->Get("acceptance_sqto4j_genht700");
	TCanvas* tc = new TCanvas("acceptance_sqto4j_genht700", "acceptance_sqto4j_genht700");
	
	gStyle->SetOptStat(0);
	accept->Draw("alp");
	accept->GetXaxis()->SetNdivisions(405);
	accept->GetXaxis()->SetRangeUser(0, 600);
	accept->GetXaxis()->SetTitle("Squark mass [GeV]");
	accept->GetYaxis()->SetTitle("Signal acceptance");
//	accept->GetYaxis()->SetTitleOffset(1.8);
//	TGaxis::SetMaxDigits(2);
//	cout << accept->GetMaximum() << endl;
	accept->SetMaximum(2);
	accept->SetMinimum(1.0e-3);
	
	style_info(true, lum_string["all"], 0);
	
	tc->SetLogy();
	
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc->SaveAs(TString(tc->GetName()) + ".png");
	
	// pre:
	TCanvas* tc_pre = new TCanvas("pre_sqto4j_genht700", "pre_sqto4j_genht700");
	tc_pre->SetLogy(1);
	TH1* pre = (TH1*) tf_in->Get("h_pre");
	pre->GetXaxis()->SetNdivisions(405);
	pre->GetXaxis()->SetTitle("Squark mass [GeV]");
	pre->GetYaxis()->SetTitle("Events");
	pre->Draw("hist");
	style_info(true, lum_string["all"], 1);
	tc_pre->SaveAs(TString(tc_pre->GetName()) + ".pdf");
	tc_pre->SaveAs(TString(tc_pre->GetName()) + ".png");
	
	// post:
	TCanvas* tc_post = new TCanvas("post_sqto4j_genht700", "post_sqto4j_genht700");
	tc_post->SetLogy(1);
	TH1* post = (TH1*) tf_in->Get("h_post");
	post->GetXaxis()->SetNdivisions(405);
	post->GetXaxis()->SetTitle("Squark mass [GeV]");
	post->GetYaxis()->SetTitle("Events after #it{H}_{T}^{8 quarks} > 700 GeV");
	post->Draw("hist");
	style_info(true, lum_string["all"], 1);
	tc_post->SaveAs(TString(tc_post->GetName()) + ".pdf");
	tc_post->SaveAs(TString(tc_post->GetName()) + ".png");
}
