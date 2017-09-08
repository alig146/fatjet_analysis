#include "/home/tote/decortication/macros/common.cc"

void data_styler() {
	gROOT->SetBatch();
	
	TFile* tf = TFile::Open("data_plots.root");
	vector<TH1*> hs;
	vector<TCanvas*> canvases;
	
	vector<TString> variables = {"ht", "mavg", "masy", "deta", "tau21", "tau42", "tau43", "eta0", "phi0", "pt0", "tau210", "tau420", "tau430"};
	
	for (int ivar = 0; ivar < variables.size(); ++ivar) {
		TString variable = variables[ivar];
		TH1* h15 = (TH1*) tf->Get(variable + "_jetht_" + "fjp_sb15");
		TH1* h16 = (TH1*) tf->Get(variable + "_jetht_" + "fjp_sb16");
		
		h15->SetLineColor(kBlue);
		h15->SetMarkerStyle(21);
		h15->SetMarkerColor(kBlue);
		h16->SetLineColor(kBlack);
		h16->SetMarkerColor(kBlack);
		
		hs = {h15, h16};

		for (int i = 0; i < hs.size(); ++i) {
			hs[i]->SetFillColor(0);
	//		hs[i]->SetMarkerSize(0);
			hs[i]->GetXaxis()->SetNdivisions(405);
			TString xtitle = variable_proper[variable];
			if (unit_proper[variable] != "") xtitle += " [" + unit_proper[variable] + "]";
			hs[i]->GetXaxis()->SetTitle(xtitle);
	//		hs[i]->GetYaxis()->SetTitle("Normalized events");
			hs[i]->Rebin(60);
			
		}
		
		int leg_pos = 1;
		// Particulars:
		if (variable == "deta") {
			leg_pos = 3;
		}
		else if (variable == "masy") {
			leg_pos = 3;
		}
		else if (variable == "tau21") {
			leg_pos = 0;
		}
		else if (variable == "tau42") {
			leg_pos = 0;
		}
		else if (variable == "tau43") {
			leg_pos = 0;
		}
		else if (variable == "eta0") {
			leg_pos = 0;
		}
		else if (variable == "phi0") {
			leg_pos = 3;
		}
		else if (variable == "tau210") {
			leg_pos = 0;
		}
		else if (variable == "tau420") {
			leg_pos = 0;
		}
		else if (variable == "tau430") {
			leg_pos = 0;
		}
		
		vector<TLegend*> legs = {
			new TLegend(0.20, 0.80, 0.40, 0.92),		// 0: Top-left
			new TLegend(0.59, 0.66, 0.80, 0.83),		// 1: Top-right
			new TLegend(0.59, 0.66, 0.80, 0.83),		// 2: Bottom-right
			new TLegend(0.20, 0.32, 0.40, 0.47),		// 3: Bottom-left
		};
		TLegend* leg = legs[leg_pos];
		leg->AddEntry(h15, name_proper["jetht15"], "lep");
		leg->AddEntry(h16, name_proper["jetht16"], "lep");
		
		canvases = same_set(hs, "data_" + variable);
		for (int i = 0; i < 4; ++i) {		// Order: nom, norm, logy, norm_logy
			canvases[i]->cd();
			style_info(false, lum_string["sum"], 1, false, 0.49);
			leg->Draw();
			style_write(TString("Selection: #bf{") + cut_proper["sb"] + "}",  0.18, 0.94, 0.018);
			canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
			canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
		}
	}
}
