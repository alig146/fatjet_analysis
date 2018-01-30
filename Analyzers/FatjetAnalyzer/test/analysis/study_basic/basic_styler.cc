#include <Deracination/Straphanger/test/decortication/macros/common.cc>

int leg_position(TString cut, TString variable, int draw) {
	
	// Notes:
	/// draw: nominal, norm, logy, norm-logy
	
	int leg_pos = 1;
	// deta:
	if (variable == "deta" && draw == 0 && cut == "sig") leg_pos = 1;
	if (variable == "deta" && draw == 1 && cut == "sig") leg_pos = 3;
	if (variable == "deta" && draw == 2 && cut == "sig") leg_pos = 2;
	else if (variable == "deta" && draw == 0 && cut == "sb") leg_pos = 3;
	else if (variable == "deta" && draw == 2 && cut == "sb") leg_pos = 3;
	else if (variable == "deta" && draw == 0 && cut == "sbb") leg_pos = 0;
	else if (variable == "deta" && draw == 2 && cut == "sbb") leg_pos = 2;
	// masy:
	else if (variable == "masy" && draw == 0 && cut == "sig") leg_pos = 0;
	else if (variable == "masy" && draw == 2 && cut == "sig") leg_pos = 2;
	else if (variable == "masy" && draw == 0 && cut == "sb") leg_pos = 0;
	else if (variable == "masy" && draw == 2 && cut == "sb") leg_pos = 3;
	else if (variable == "masy" && draw == 0 && cut == "sbb") leg_pos = 0;
	else if (variable == "masy" && draw == 2 && cut == "sbb") leg_pos = 3;
	// tau21:
	else if (variable == "tau21") leg_pos = 0;
	// tau42:
	else if (variable == "tau42") leg_pos = 0;
	// tau43:
	else if (variable == "tau43") leg_pos = 0;
	
	
	return leg_pos;
}


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
				
				if (variable == "deta") hs[i]->Rebin(30);
				else if (variable == "masy") hs[i]->Rebin(15);
				else if (variable == "tau21") hs[i]->Rebin(30);
				else if (variable == "tau42") hs[i]->Rebin(30);
				else if (variable == "tau43") hs[i]->Rebin(30);
				else if (variable == "ht") hs[i]->Rebin(24);
				else hs[i]->Rebin(30);
//				if (cut == "sig") hs[i]->Rebin(100);
//				else hs[i]->Rebin(30);
				hs[i]->SetFillColor(0);
		//		hs[i]->SetMarkerSize(0);
//				cout << "============" << variable << endl;
				if (variable == "mavg") hs[i]->GetXaxis()->SetRangeUser(0, 900);
				else if (variable == "deta") hs[i]->GetXaxis()->SetRangeUser(0, 1.0);
				else if (variable == "masy") hs[i]->GetXaxis()->SetRangeUser(0, 0.1);
				else if (variable == "tau21") hs[i]->GetXaxis()->SetRangeUser(0, 1.0);
				else if (variable == "tau42") hs[i]->GetXaxis()->SetRangeUser(0.2, 0.6);
				else if (variable == "tau43") hs[i]->GetXaxis()->SetRangeUser(0.4, 1.0);
				else if (variable == "ht") hs[i]->GetXaxis()->SetRangeUser(800, 2400);
				hs[i]->GetXaxis()->SetNdivisions(405);
				set_xtitle(hs[i], variable);
		//		hs[i]->GetYaxis()->SetTitle("Normalized events");
				
				// Particulars:
//				if (cut == "sb") hs[i]->Rebin(60);
//				else hs[i]->Rebin(100);
				
//				if (variable == "deta") {
//					if (cut == "sb") hs[i]->SetMaximum(900);
//					else if (cut == "sbb") hs[i]->SetMaximum(180);
//					else if (cut == "sig") hs[i]->SetMaximum(150);
//				}
//				else if (variable == "masy") {
//					if (cut == "sb") hs[i]->SetMaximum(900);
//					else if (cut == "sbb") hs[i]->SetMaximum(200);
//					else if (cut == "sig") hs[i]->SetMaximum(100);
//				}
//				else if (variable == "tau21") {
//					if (cut == "sb") hs[i]->SetMaximum(1300);
//					else if (cut == "sbb") hs[i]->SetMaximum(200);
//					else if (cut == "sig") hs[i]->SetMaximum(100);
//				}
			}
			
			
			// More particulars:
			
//			vector<TLegend*> legs = {
//				new TLegend(0.20, 0.80, 0.40, 0.92),		// 0: Top-left
//				new TLegend(0.59, 0.66, 0.80, 0.83),		// 1: Top-right
//				new TLegend(0.59, 0.66, 0.80, 0.83),		// 2: Bottom-right
//				new TLegend(0.59, 0.66, 0.80, 0.83),		// 3: Bottom-left
//			};
//			TLegend* leg = legs[leg_pos];
			
			
		//	leg->AddEntry(qcdp, name_proper["qcdp"], "le");
		//	leg->AddEntry(sq400to4j, name_proper["sq400to4j"], "le");
	
			canvases = same_set(hs, "basic_" + variable + "_" + cut);
			for (int i = 0; i < 4; ++i) {		// Order: nom, norm, logy, norm-logy
				canvases[i]->cd();
				
				int leg_pos = leg_position(cut, variable, i);
				int nlegentries = 4;
				if (cut == "sig") nlegentries = 3;
				double legyscale = 1.0;
				double legxscale = 1.0;
				if (leg_pos == 0) {
					legyscale = 0.8;
					legxscale = 0.95;
				}
				TLegend* leg = get_legend(leg_pos, nlegentries, legyscale, legxscale);
				leg->AddEntry(qcdmg, name_proper["qcdmg"], "lpe");
				leg->AddEntry(ttbar, name_proper["ttbar"], "lpe");
				leg->AddEntry(sq200to4j, name_proper["sq200to4j"], "lpe");
				if (cut != "sig" and  cut != "sigl") leg->AddEntry(jetht, name_proper["jetht"], "lpe");
				if (cut != "sig") style_info(false);
				else style_info(true);
				leg->Draw();
				style_cut(cut);
				save(canvases[i]);
			}
		}
	}
}
