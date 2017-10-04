#include "/home/tote/decortication/macros/common.cc"

void be_3dplot(TString cut="sb") {
	TFile* tf_in = TFile::Open("~/decortication/macros/background_tools/template_plots.root");
	TH3* h3 = (TH3*) tf_in->Get("fj_jetht_" + cut + "_p");
	
	TString name = "3dplot";
	TCanvas* tc = new TCanvas(name, name);
	
	gStyle->SetOptStat(0);
	h3->SetTitle("");
	h3->SetMarkerSize(0.1);
	h3->GetXaxis()->SetRangeUser(0, 900);
	h3->GetXaxis()->SetNdivisions(405);
	h3->GetXaxis()->SetTitle(get_xtitle("m0"));
	h3->GetXaxis()->SetTitleOffset(1.75);
	h3->GetXaxis()->SetTitleSize(40);
	h3->GetYaxis()->SetNdivisions(405);
	h3->GetYaxis()->SetTitle(get_xtitle("eta0"));
	h3->GetYaxis()->SetTitleOffset(1.75);
	h3->GetYaxis()->SetTitleSize(40);
	h3->GetZaxis()->SetRangeUser(900, 2000);
//	h3->GetZaxis()->SetNdivisions(505);
	h3->GetZaxis()->SetTitle(get_xtitle("ht"));
	h3->GetZaxis()->SetTitleOffset(1.75);
	h3->GetZaxis()->SetTitleSize(40);
	h3->Draw();
	
	tc->SaveAs(name + ".pdf");
}
