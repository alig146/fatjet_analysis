#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void htreweight_compare(TString ds="qcdmg", TString cut="sigl") {
	TFile* tf_in = get_ana();
	TTree *tt = (TTree*) tf_in->Get(ds);
	tt->Draw("mavg_p>>" + ds + "_fjp(1200,0,1200)", get_cut("fjp_" + cut));
	TH1 *h_fjp = (TH1*) gDirectory->Get(ds + "_fjp");
	
	TH1* h_temp = fetch_template(ds, cut, "", 1, true);
	TH1* h_temp_xht = fetch_template(ds, cut, "", 1, false);
	
	gStyle->SetOptStat(0);
	
	h_fjp->Rebin(30);
	h_fjp->SetTitle("");
	set_xtitle(h_fjp, "mavg");
	style_ylabel(h_fjp);
	h_temp->Scale(h_fjp->Integral()/h_temp->Integral());
	h_temp->Rebin(30);
	h_temp->SetFillColor(0);
	h_temp->SetLineStyle(2);
	h_temp->SetFillColorAlpha(kBlue-10, 0.5);
//	h_temp->SetFillColor(kAzure-9);
	h_temp_xht->Scale(h_fjp->Integral()/h_temp_xht->Integral());
	h_temp_xht->Rebin(30);
	h_temp_xht->SetFillColor(0);
	h_temp_xht->SetLineColor(kRed);
	
	for (int logy=0; logy < 2; ++logy) {
		TString name = "htreweight_" + ds + "_" + cut;
		if (logy == 1) name += "_logy";
		TCanvas* tc = new TCanvas(name, name);
		
		if (logy == 1) h_fjp->SetMinimum(0.0001);
		else h_fjp->SetMinimum(0.0);
		
		h_fjp->Draw("e");
		h_temp->Draw("hist same");
		h_temp_xht->Draw("hist same");
		h_fjp->Draw("e same");
		gPad->RedrawAxis();
		
		tc->SetLogy(logy);
		
		TLegend* leg;
		if (logy == 0) leg = get_legend(1, 3, 1.0, 0.8);
		else if (logy == 1) leg = get_legend(3, 3, 1.0, 1.2);
		leg->AddEntry(h_fjp, "Fatjet pair selection", "ple");
		leg->AddEntry(h_temp, "Derived template", "f");
		leg->AddEntry(h_temp_xht, "Template w/out #it{H}_{T} re-weight", "f");
		leg->Draw();
		bool mc = true;
		if (ds == "jetht") mc = false;
		style_info(mc, lum_string["all"], 1);
		style_cut(cut);
		
		save(tc);
	}
}










