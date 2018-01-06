#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void basic_styler() {
	gROOT->SetBatch();
	
	TFile* tf = TFile::Open("basic_plots.root");
	vector<TH1*> hs;
	vector<TCanvas*> canvases;
	
	vector<TString> cuts = {"sig", "sb", "sbb"};
	vector<TString> variables = {"mavg", "masy", "deta", "tau21", "tau42", "tau43", "ht"};
	
	for (int icut = 0; icut < cuts.size(); ++icut) {
		for (int ivar = 0; ivar < variables.size(); ++ivar) {
			TString cut = cuts[icut];
			TString variable = variables[ivar];
			TH1* qcdp = (TH1*) tf->Get(variable + "_qcdp_" + cut);
			TH1* qcdmg = (TH1*) tf->Get(variable + "_qcdmg_" + cut);
			TH1* ttbar = (TH1*) tf->Get(variable + "_ttbar_" + cut);
			TH1* jetht = (TH1*) tf->Get(variable + "_jetht_" + cut);
			TH1* sq200to4j = (TH1*) tf->Get(variable + "_sq200to4j_" + cut);
		//	TH1* sq400to4j_deta = (TH1*) tf->Get("sq400to4j_sigxdeta");

			qcdmg->SetLineColor(kBlue);
			qcdmg->SetMarkerColor(kBlue);
			ttbar->SetLineColor(kViolet);
			ttbar->SetMarkerColor(kViolet);
			sq200to4j->SetLineColor(kRed);
			sq200to4j->SetMarkerColor(kRed);
			jetht->SetLineColor(kBlack);
			jetht->SetMarkerColor(kBlack);
	
		//	cout << "deta integral = " << qcdmg_deta->Integral(0, qcdmg_deta->GetXaxis()->FindBin(1.0)) << endl;
	
			hs = {qcdmg, ttbar, sq200to4j};
			if (cut != "sig" and cut != "sigl") hs.push_back(jetht);
	
			for (int i = 0; i < hs.size(); ++i) {
				
//				if (cut == "sig") hs[i]->Rebin(100);
//				else hs[i]->Rebin(30);
				hs[i]->SetFillColor(0);
		//		hs[i]->SetMarkerSize(0);
				if (variable == "mavg") hs[i]->GetXaxis()->SetRangeUser(0, 900);
				if (variable == "deta") hs[i]->GetXaxis()->SetRangeUser(0, 1.0);
				if (variable == "masy") hs[i]->GetXaxis()->SetRangeUser(0, 0.1);
				if (variable == "tau21") hs[i]->GetXaxis()->SetRangeUser(0, 1.0);
				if (variable == "tau42") hs[i]->GetXaxis()->SetRangeUser(0.2, 0.6);
				if (variable == "tau43") hs[i]->GetXaxis()->SetRangeUser(0.4, 1.0);
				hs[i]->GetXaxis()->SetNdivisions(405);
				set_xtitle(hs[i], variable);
		//		hs[i]->GetYaxis()->SetTitle("Normalized events");
				
				// Particulars:
//				if (cut == "sb") hs[i]->Rebin(60);
//				else hs[i]->Rebin(100);
				hs[i]->Rebin(30);
				
				if (variable == "deta") {
					if (cut == "sb") hs[i]->SetMaximum(900);
					else if (cut == "sbb") hs[i]->SetMaximum(180);
					else if (cut == "sig") hs[i]->SetMaximum(150);
				}
				else if (variable == "masy") {
					if (cut == "sb") hs[i]->SetMaximum(900);
					else if (cut == "sbb") hs[i]->SetMaximum(200);
					else if (cut == "sig") hs[i]->SetMaximum(100);
				}
//				else if (variable == "tau21") {
//					if (cut == "sb") hs[i]->SetMaximum(1300);
//					else if (cut == "sbb") hs[i]->SetMaximum(200);
//					else if (cut == "sig") hs[i]->SetMaximum(100);
//				}
			}
			
			int leg_pos = 1;
			
			// More particulars:
			if (variable == "deta") {
				leg_pos = 0;
			}
			else if (variable == "masy") {
				leg_pos = 0;
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
			
			vector<TLegend*> legs = {
				new TLegend(0.20, 0.80, 0.40, 0.92),		// 0: Top-left
				new TLegend(0.59, 0.66, 0.80, 0.83),		// 1: Top-right
				new TLegend(0.59, 0.66, 0.80, 0.83),		// 2: Bottom-right
				new TLegend(0.59, 0.66, 0.80, 0.83),		// 3: Bottom-left
			};
			TLegend* leg = legs[leg_pos];
		//	leg->AddEntry(qcdp, name_proper["qcdp"], "le");
			leg->AddEntry(qcdmg, name_proper["qcdmg"], "lpe");
			leg->AddEntry(ttbar, name_proper["ttbar"], "lpe");
			leg->AddEntry(sq200to4j, name_proper["sq200to4j"], "lpe");
			if (cut != "sig" and  cut != "sigl") leg->AddEntry(jetht, name_proper["jetht"], "lpe");
		//	leg->AddEntry(sq400to4j, name_proper["sq400to4j"], "le");
	
			canvases = same_set(hs, "basic_" + variable + "_" + cut);
			for (int i = 0; i < 4; ++i) {		// Order: nom, norm, logy, norm-logy
				canvases[i]->cd();
				style_info();
				leg->Draw();
				style_cut(cut);
				save(canvases[i]);
			}
		}
	}
}
