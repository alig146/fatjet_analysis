#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void njets_styler(TString cut="") {
	gStyle->SetOptStat(0);
	TString name = "njets";
	if (cut != "") name += "_fj" + cut;
	TFile* tf_in = TFile::Open("jets_plots.root");
	TH1* h_njets_sq100to4j = (TH1*) tf_in->Get(name + "_sq100to4j");
	TH1* h_njets_sq200to4j = (TH1*) tf_in->Get(name + "_sq200to4j");
	TH1* h_njets_sq500to4j = (TH1*) tf_in->Get(name + "_sq500to4j");
	
	TCanvas* tc = new TCanvas(name, name);
	
	h_njets_sq100to4j->Scale(100/h_njets_sq100to4j->Integral());
	h_njets_sq100to4j->GetXaxis()->SetTitle("CA12 jets with #it{p}_{T} > 400 GeV and |#eta| < 2.0");
	h_njets_sq100to4j->GetXaxis()->SetTitleSize(50*0.7);
	h_njets_sq100to4j->GetXaxis()->SetTitleOffset(1.6);
	h_njets_sq100to4j->GetYaxis()->SetTitle("Events  [%]");
	h_njets_sq100to4j->SetFillStyle(0);
	h_njets_sq100to4j->SetLineColor(kBlack);
	h_njets_sq100to4j->Draw("hist");
	
	h_njets_sq200to4j->Scale(100/h_njets_sq200to4j->Integral());
	h_njets_sq200to4j->SetFillStyle(0);
	h_njets_sq200to4j->SetFillColor(0);
	h_njets_sq200to4j->SetLineColor(kBlue);
	h_njets_sq200to4j->SetLineStyle(2);
	h_njets_sq200to4j->Draw("hist same");
	
	
	h_njets_sq500to4j->Scale(100/h_njets_sq500to4j->Integral());
	h_njets_sq500to4j->SetFillStyle(0);
	h_njets_sq500to4j->SetFillColor(0);
	h_njets_sq500to4j->SetLineColor(kRed);
	h_njets_sq500to4j->SetLineStyle(6);
	h_njets_sq500to4j->Draw("hist same");
	
	TLegend* tl = get_legend(1, 3);
	tl->AddEntry(h_njets_sq100to4j, name_proper["sq100to4j"], "f");
	tl->AddEntry(h_njets_sq200to4j, name_proper["sq200to4j"], "f");
	tl->AddEntry(h_njets_sq500to4j, name_proper["sq500to4j"], "f");
	tl->Draw();
	
	gPad->RedrawAxis();
	
	style_info(true, "", 1, false, 3.0);
	if (cut == "") style_cut("#it{H}_{T} > 900 GeV");
	else style_cut(cut);
	save(tc);
}
