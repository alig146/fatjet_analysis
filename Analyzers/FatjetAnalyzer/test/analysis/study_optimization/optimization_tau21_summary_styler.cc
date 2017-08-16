void optimization_tau21_summary_styler() {
	TFile *tf = TFile::Open("optimization_summary_plots_43tau800_42tau450.root");
	
//	vector<int> masses = {100, 150, 200, 250, 300, 400, 500};
	vector<int> masses = {150, 250, 400};
	vector<int> sigs = {1, 2, 3, 4};
	
	gStyle->SetOptStat(0);
	
	for (int j = 0; j < sigs.size(); j ++) {
		TString tc_name = "optimization_tau21_s" + to_string(sigs[j]);
		TCanvas* tc = new TCanvas(tc_name, tc_name);
		tc->cd();
		for (int i = 0; i < masses.size(); i ++) {
			TString name = "sq" + to_string(masses[i]) + "to4j_s" + to_string(sigs[j]);
			TH1F* sig = (TH1F*) tf->Get(name);
			sig->Rebin(2);
			sig->GetXaxis()->SetTitle("Pruned #tau_{21} cut");
			sig->GetXaxis()->SetNdivisions(405);
			sig->GetYaxis()->SetTitle(TString("Significance measure ") + to_string(sigs[j]));
//			sig->GetYaxis()->SetTitle(TString("Normalized significance measure ") + to_string(sigs[j]));
			sig->SetFillStyle(0);
			sig->Draw("same hist");
//			sig->DrawNormalized("same hist");
		}
//		gPad->SetTickx();
//		gPad->SetTicky();
//		tc->SaveAs("optimization_tau21" + name + ".pdf");
//		tc->SaveAs("optimization_tau21" + name + ".png");
	}
}
