#include "/home/tote/decortication/macros/common.cc"

TH2* smmijhat_plot(TString i="0", TString j="1", TString ds_name="qcdmg", TString cut_name="sig") {
	TString name = "smm" + i + j + "hat_" + ds_name + "_" + cut_name;
	gStyle->SetOptStat(0);
	TTree* tt = (TTree*) gDirectory->Get(ds_name);
	tt->Draw("smm" + i + "hat[0]:smm" + j + "hat[0]>>h(20,0,1.0,20,0,1.0)", *get_cut("fjp_" + cut_name));
	TH2* h = (TH2*) gDirectory->Get("h");
	h->SetName(name);
	h->SetTitle("");
	h->GetXaxis()->SetTitle("Leading jet #hat{#it{m}}_{" + j + "}^{2}");
	h->GetYaxis()->SetTitle("Leading jet #hat{#it{m}}_{" + i + "}^{2}");
	style_zlabel(h);
//	style_info(false, "31.3", 4);
	return h;
}

TCanvas* sd_styled(TString cut_name, TString sig="sq100to4j", TString bkg="qcdmg", bool logy=false, TString var="sd") {
	TString name = var + "_" + cut_name + "_" + sig + "_" + bkg;
	TString N = "4";
	if (var == "smd") N = "3";
	
	TCanvas* c = new TCanvas(name, name);
	gStyle->SetOptStat(0);
	TH1* h_jetht = (TH1*) gDirectory->Get(var + "_jetht_" + cut_name);
	TH1* h_bkg = (TH1*) gDirectory->Get(var + "_" + bkg + "_" + cut_name);
	TH1* h_sig = (TH1*) gDirectory->Get(var + "_" + sig + "_" + cut_name);
	
	h_sig->SetTitle("");
	h_sig->GetXaxis()->SetTitle("Dalitz distance squared (#it{N} = " + N + ")");
	h_sig->GetXaxis()->SetNdivisions(405);
	style_ylabel(h_sig, "", "Normalized events");
	h_sig->SetFillStyle(0);
	h_sig->SetLineWidth(2);
	h_sig->SetLineStyle(2);
	h_sig->SetLineColor(kRed);
	
	h_bkg->SetTitle("");
	h_bkg->GetXaxis()->SetTitle("Dalitz distance squared (#it{N} = " + N + ")");
	h_bkg->GetXaxis()->SetNdivisions(405);
	style_ylabel(h_bkg, "", "Normalized events");
	h_bkg->SetFillStyle(0);
	h_bkg->SetLineWidth(2);
	
	h_jetht->SetTitle("");
	h_jetht->SetFillStyle(0);
	h_jetht->GetXaxis()->SetTitle("Dalitz distance squared (#it{N} = " + N + ")");
	h_jetht->GetXaxis()->SetNdivisions(405);
	style_ylabel(h_jetht, "", "Normalized events");
	
//	h_qcdmg->Draw("hist");
//	h_ttbar->Draw("same hist");
//	h_sq200to4j->Draw("same hist");
	if (var == "sd") {
		h_sig->DrawNormalized("hist");
		h_bkg->DrawNormalized("same hist");
		h_jetht->DrawNormalized("same e");
	}
	else {
		h_jetht->DrawNormalized("e");
		h_sig->DrawNormalized("same hist");
		h_bkg->DrawNormalized("same hist");
	}
	style_info(false, "31.3");
	
	TLegend* leg = new TLegend(0.50, 0.65, 0.80, 0.80);
	leg->AddEntry(h_sig, name_proper[sig], "l");
	leg->AddEntry(h_bkg, name_proper[bkg], "l");
	leg->AddEntry(h_jetht, "JetHT 15-16", "pl");
	leg->Draw();
	
//	style_write("#bar{#it{D}} = ", );
	
	if (logy) c->SetLogy();
	gPad->RedrawAxis();
	return c;
}

void dalitz_styler() {
	TFile* tf_in = TFile::Open("dalitz_plots.root");
	
	TCanvas* tc0 = sd_styled("sigl");
	TCanvas* tc1 = sd_styled("sb");
	TCanvas* tc2 = sd_styled("sbb");
	tc0->SaveAs(TString(tc0->GetName()) + ".pdf");
	tc1->SaveAs(TString(tc1->GetName()) + ".pdf");
	tc2->SaveAs(TString(tc2->GetName()) + ".pdf");
	
	TCanvas* tc = sd_styled("sigl", "sq200to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sb", "sq200to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sbb", "sq200to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	
	tc = sd_styled("sigl", "sq300to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sb", "sq300to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sbb", "sq300to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	
	tc = sd_styled("sigl", "sq400to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sb", "sq400to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sbb", "sq400to4j");
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	
	tc = sd_styled("sigl", "ttbar", "qcdmg", true);
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sb", "ttbar", "qcdmg", true);
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc = sd_styled("sbb", "ttbar", "qcdmg", true);
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
}













