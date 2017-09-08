#include "/home/tote/decortication/macros/common.cc"

void masy_styler() {
	TFile* tf = TFile::Open("masy_plots.root");
	
	// pre:
	TCanvas* pre = new TCanvas("masy_pre", "masy_pre");
	TH1* qcdp_pre = (TH1*) tf->Get("qcdp_pre");
	TH1* qcdmg_pre = (TH1*) tf->Get("qcdmg_pre");
	TH1* sq200to4j_pre = (TH1*) tf->Get("sq200to4j_pre");
	TH1* sq400to4j_pre = (TH1*) tf->Get("sq400to4j_pre");
	
	qcdp_pre->Rebin(20);
	qcdp_pre->SetLineColor(kBlue);
	qcdp_pre->SetFillColor(0);
	qcdp_pre->SetMarkerSize(0);
	qcdmg_pre->Rebin(20);
	qcdmg_pre->SetLineColor(kViolet);
	qcdmg_pre->SetFillColor(0);
	qcdmg_pre->SetMarkerSize(0);
	sq200to4j_pre->Rebin(20);
	sq200to4j_pre->GetXaxis()->SetRangeUser(0, 1.2);
	sq200to4j_pre->GetXaxis()->SetTitle("#it{A}_{#it{m}}");
	sq200to4j_pre->GetYaxis()->SetTitle("Normalized events");
	sq200to4j_pre->SetTitle("");
	sq200to4j_pre->SetLineColor(kRed);
	sq200to4j_pre->SetFillColor(0);
	sq200to4j_pre->SetMarkerSize(0);
	sq400to4j_pre->Rebin(20);
	sq400to4j_pre->SetLineColor(kOrange);
	sq400to4j_pre->SetFillColor(0);
	sq400to4j_pre->SetMarkerSize(0);
	
	gStyle->SetOptStat(0);
	
	sq200to4j_pre->DrawNormalized("histe");
	qcdp_pre->DrawNormalized("histe same");
	sq400to4j_pre->DrawNormalized("histe same");
	qcdmg_pre->DrawNormalized("histe same");
	sq200to4j_pre->DrawNormalized("histe same");
	
	TLegend* leg = new TLegend(0.55, 0.60, 0.80, 0.80);
	leg->AddEntry(qcdp_pre, name_proper["qcdp"], "le");
	leg->AddEntry(qcdmg_pre, name_proper["qcdmg"], "le");
	leg->AddEntry(sq200to4j_pre, name_proper["sq200to4j"], "le");
	leg->AddEntry(sq400to4j_pre, name_proper["sq400to4j"], "le");
	leg->Draw();
	
	style_info();
	
	pre->SaveAs("masy_pre.pdf");
	pre->SaveAs("masy_pre.png");
	
	// deta:
	TCanvas* deta = new TCanvas("masy_deta", "masy_deta");
	TH1* qcdp_deta = (TH1*) tf->Get("qcdp_deta");
	TH1* qcdmg_deta = (TH1*) tf->Get("qcdmg_deta");
	TH1* sq200to4j_deta = (TH1*) tf->Get("sq200to4j_deta");
	TH1* sq400to4j_deta = (TH1*) tf->Get("sq400to4j_deta");
	
	qcdp_deta->Rebin(20);
	qcdp_deta->SetLineColor(kBlue);
	qcdp_deta->SetFillColor(0);
	qcdp_deta->SetMarkerSize(0);
	qcdmg_deta->Rebin(20);
	qcdmg_deta->SetLineColor(kViolet);
	qcdmg_deta->SetFillColor(0);
	qcdmg_deta->SetMarkerSize(0);
	sq200to4j_deta->Rebin(20);
	sq200to4j_deta->GetXaxis()->SetRangeUser(0, 1.2);
	sq200to4j_deta->GetXaxis()->SetTitle("#it{A}_{#it{m}}");
	sq200to4j_deta->GetYaxis()->SetTitle("Normalized events");
	sq200to4j_deta->SetTitle("");
	sq200to4j_deta->SetLineColor(kRed);
	sq200to4j_deta->SetFillColor(0);
	sq200to4j_deta->SetMarkerSize(0);
	sq400to4j_deta->Rebin(20);
	sq400to4j_deta->SetLineColor(kOrange);
	sq400to4j_deta->SetFillColor(0);
	sq400to4j_deta->SetMarkerSize(0);
	
	gStyle->SetOptStat(0);
	
	sq200to4j_deta->DrawNormalized("histe");
	qcdp_deta->DrawNormalized("histe same");
	sq400to4j_deta->DrawNormalized("histe same");
	qcdmg_deta->DrawNormalized("histe same");
	sq200to4j_deta->DrawNormalized("histe same");
	
	leg = new TLegend(0.55, 0.60, 0.80, 0.80);
	leg->AddEntry(qcdp_deta, name_proper["qcdp"], "le");
	leg->AddEntry(qcdmg_deta, name_proper["qcdmg"], "le");
	leg->AddEntry(sq200to4j_deta, name_proper["sq200to4j"], "le");
	leg->AddEntry(sq400to4j_deta, name_proper["sq400to4j"], "le");
	leg->Draw();
	
	style_info();
	
	deta->SaveAs("masy_deta.pdf");
	deta->SaveAs("masy_deta.png");
}
