#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void m_styler() {
	gStyle->SetOptStat(0);
	TFile* tf_in = TFile::Open("jets_plots.root");
	
	vector<TString> cuts = {"pre", "sig"};
	
	for (unsigned i = 0; i < cuts.size(); ++ i) {
		TString cut = cuts[i];
		TH1* h_mavgp_sq100to4j = (TH1*) tf_in->Get(TString("mavgp_") + cut + "_sq100to4j");
		TH1* h_mavgs_sq100to4j = (TH1*) tf_in->Get(TString("mavgs_") + cut + "_sq100to4j");
		TH1* h_mp0_sq100to4j = (TH1*) tf_in->Get(TString("mp0_") + cut + "_sq100to4j");
		TH1* h_ms0_sq100to4j = (TH1*) tf_in->Get(TString("ms0_") + cut + "_sq100to4j");
		TH1* h_mavgp_sq200to4j = (TH1*) tf_in->Get(TString("mavgp_") + cut + "_sq200to4j");
		TH1* h_mavgs_sq200to4j = (TH1*) tf_in->Get(TString("mavgs_") + cut + "_sq200to4j");
		TH1* h_mp0_sq200to4j = (TH1*) tf_in->Get(TString("mp0_") + cut + "_sq200to4j");
		TH1* h_ms0_sq200to4j = (TH1*) tf_in->Get(TString("ms0_") + cut + "_sq200to4j");
		TH1* h_mavgp_sq400to4j = (TH1*) tf_in->Get(TString("mavgp_") + cut + "_sq400to4j");
		TH1* h_mavgs_sq400to4j = (TH1*) tf_in->Get(TString("mavgs_") + cut + "_sq400to4j");
		TH1* h_mp0_sq400to4j = (TH1*) tf_in->Get(TString("mp0_") + cut + "_sq400to4j");
		TH1* h_ms0_sq400to4j = (TH1*) tf_in->Get(TString("ms0_") + cut + "_sq400to4j");

		cout << h_mavgp_sq100to4j->Integral() << "   " << h_mavgs_sq100to4j->Integral() << endl;
	
	
		// mavg:
		TString name = "mavg_" + cut;
		TCanvas* tc = new TCanvas(name, name);
		
		int nrebin = 20;
		double mmax = 500;
		int ndiv = 405;
		
		h_mavgp_sq100to4j->Rebin(nrebin);
		h_mavgp_sq100to4j->GetXaxis()->SetRangeUser(0, mmax);
		h_mavgp_sq100to4j->GetXaxis()->SetNdivisions(ndiv);
		h_mavgp_sq100to4j->SetMaximum(h_mavgp_sq100to4j->GetMaximum()*1.4);
		set_xtitle(h_mavgp_sq100to4j, "mavg");
		h_mavgp_sq100to4j->Draw("hist same");
	
		h_mavgs_sq100to4j->Rebin(nrebin);
		h_mavgs_sq100to4j->SetLineStyle(2);
		
		h_mavgp_sq200to4j->Rebin(nrebin);
		h_mavgp_sq200to4j->GetXaxis()->SetRangeUser(0, mmax);
		h_mavgp_sq200to4j->GetXaxis()->SetNdivisions(ndiv);
		h_mavgp_sq200to4j->SetMaximum(h_mavgp_sq200to4j->GetMaximum()*1.4);
		h_mavgp_sq200to4j->SetLineColor(kBlue);
		set_xtitle(h_mavgp_sq200to4j, "mavg");
		style_ylabel(h_mavgp_sq200to4j);
	
		h_mavgs_sq200to4j->Rebin(nrebin);
		h_mavgs_sq200to4j->SetLineColor(kBlue);
		h_mavgs_sq200to4j->SetLineStyle(2);
		h_mavgs_sq200to4j->Draw("hist same");
		
		h_mavgp_sq400to4j->Rebin(nrebin);
		h_mavgp_sq400to4j->GetXaxis()->SetRangeUser(0, mmax);
		h_mavgp_sq400to4j->GetXaxis()->SetNdivisions(ndiv);
		h_mavgp_sq400to4j->SetMaximum(h_mavgp_sq400to4j->GetMaximum()*1.4);
		h_mavgp_sq400to4j->SetLineColor(kRed);
		set_xtitle(h_mavgp_sq400to4j, "mavg");
		style_ylabel(h_mavgp_sq400to4j);
	
		h_mavgs_sq400to4j->Rebin(nrebin);
		h_mavgs_sq400to4j->SetLineColor(kRed);
		h_mavgs_sq400to4j->SetLineStyle(2);
		h_mavgs_sq400to4j->Draw("hist same");
		
		
		if (cut == "pre") {
			h_mavgp_sq100to4j->Draw("hist");
			h_mavgs_sq100to4j->Draw("hist same");
			h_mavgp_sq200to4j->Draw("hist same");
			h_mavgs_sq200to4j->Draw("hist same");
			h_mavgp_sq400to4j->Draw("hist same");
			h_mavgs_sq400to4j->Draw("hist same");
		}
		else {
			h_mavgp_sq200to4j->Draw("hist");
			h_mavgs_sq200to4j->Draw("hist same");
			h_mavgp_sq100to4j->Draw("hist same");
			h_mavgs_sq100to4j->Draw("hist same");
			h_mavgp_sq400to4j->Draw("hist same");
			h_mavgs_sq400to4j->Draw("hist same");
		}
		
		TLegend* tl = get_legend(1, 6, 1.0, 0.9);
		tl->AddEntry(h_mavgp_sq100to4j, name_proper["sq100to4j"] + ", pruned", "f");
		tl->AddEntry(h_mavgs_sq100to4j, name_proper["sq100to4j"] + ", SoftDrop", "f");
		tl->AddEntry(h_mavgp_sq200to4j, name_proper["sq200to4j"] + ", pruned", "f");
		tl->AddEntry(h_mavgs_sq200to4j, name_proper["sq200to4j"] + ", SoftDrop", "f");
		tl->AddEntry(h_mavgp_sq400to4j, name_proper["sq400to4j"] + ", pruned", "f");
		tl->AddEntry(h_mavgs_sq400to4j, name_proper["sq400to4j"] + ", SoftDrop", "f");
		tl->Draw();
		
		gPad->RedrawAxis();
		
		style_info();
		style_cut(cut);
		save(tc);
	}
	
	
	
//	h_njets_sq100to4j->Scale(100/h_njets_sq100to4j->Integral());
//	h_njets_sq100to4j->GetXaxis()->SetTitle("CA12 jets with #it{p}_{T} > 400 GeV and |#eta| < 2.0");
//	h_njets_sq100to4j->GetXaxis()->SetTitleSize(50*0.7);
//	h_njets_sq100to4j->GetXaxis()->SetTitleOffset(1.6);
//	h_njets_sq100to4j->GetYaxis()->SetTitle("Events  [%]");
//	h_njets_sq100to4j->SetFillStyle(0);
//	h_njets_sq100to4j->SetLineColor(kBlack);
//	h_njets_sq100to4j->Draw("hist");
//	
//	h_njets_sq200to4j->Scale(100/h_njets_sq200to4j->Integral());
//	h_njets_sq200to4j->SetFillStyle(0);
//	h_njets_sq200to4j->SetFillColor(0);
//	h_njets_sq200to4j->SetLineColor(kRed);
//	h_njets_sq200to4j->SetLineStyle(2);
//	h_njets_sq200to4j->Draw("hist same");
//	
//	
//	h_njets_sq500to4j->Scale(100/h_njets_sq500to4j->Integral());
//	h_njets_sq500to4j->SetFillStyle(0);
//	h_njets_sq500to4j->SetFillColor(0);
//	h_njets_sq500to4j->SetLineColor(kBlue);
//	h_njets_sq500to4j->SetLineStyle(6);
//	h_njets_sq500to4j->Draw("hist same");
//	
}
