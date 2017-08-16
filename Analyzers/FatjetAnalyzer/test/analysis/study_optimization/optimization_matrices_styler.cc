#include "/home/tote/decortication/macros/common.cc"

void optimization_matrices_styler() {
//	TFile *tf = TFile::Open("optimization_matrices.root");
	TFile *tf = TFile::Open("optimization_matrices_21tau750.root");
	
	vector<int> masses = {100, 150, 200, 250, 300, 400, 500};
	vector<int> sigs = {1, 2, 3, 4};
	
	TCanvas* tc = new TCanvas("tc", "tc");
	gStyle->SetOptStat(0);
	
	for (int i = 0; i < masses.size(); i ++) {
		for (int j = 0; j < sigs.size(); j ++) {
			TString name = "sq" + to_string(masses[i]) + "to4j_s" + to_string(sigs[j]);
			TH2F* sig = (TH2F*) tf->Get(name);
			sig->GetXaxis()->SetTitle("#tau_{42} cut");
			sig->GetYaxis()->SetTitle("#tau_{43} cut");
			sig->GetZaxis()->SetTitle(TString("Significance measure ") + to_string(sigs[j]) + " for #it{m}_{sq} = " + to_string(masses[i]) + " GeV");
			sig->Draw("colz");
			gPad->SetTickx();
			gPad->SetTicky();
			style_info(true, lum_string["all"], 4);
			tc->SaveAs("optimization_" + name + ".pdf");
			tc->SaveAs("optimization_" + name + ".png");
		}
	}
}
