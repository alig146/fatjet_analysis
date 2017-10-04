#include "/home/tote/decortication/macros/common.cc"

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
				hs[i]->GetXaxis()->SetNdivisions(405);
				TString xtitle = variable_proper[variable];
				if (unit_proper[variable] != "") xtitle += " [" + unit_proper[variable] + "]";
				hs[i]->GetXaxis()->SetTitle(xtitle);
		//		hs[i]->GetYaxis()->SetTitle("Normalized events");
				
				// Particulars:
//				if (cut == "sb") hs[i]->Rebin(60);
//				else hs[i]->Rebin(100);
				hs[i]->Rebin(60);
				
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
				style_write(TString("Selection: #bf{") + cut_proper[cut] + "}",  0.18, 0.94, 0.018);
				canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
				canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
			}
		}
	}
	
//	// masyp:
//	TH1* qcdp_masyp = (TH1*) tf->Get("qcdp_sigxmasyp");
//	TH1* qcdmg_masyp = (TH1*) tf->Get("qcdmg_sigxmasyp");
//	TH1* sq200to4j_masyp = (TH1*) tf->Get("sq200to4j_sigxmasyp");
//	TH1* sq400to4j_masyp = (TH1*) tf->Get("sq400to4j_sigxmasyp");
//	qcdp_masyp->SetLineColor(kBlue);
//	qcdmg_masyp->SetLineColor(kViolet);
//	sq200to4j_masyp->SetLineColor(kRed);
//	sq400to4j_masyp->SetLineColor(kOrange);
//	
//	cout << "masyp integral = " << qcdmg_masyp->Integral(0, qcdmg_masyp->GetXaxis()->FindBin(0.1)) << endl;
//	
//	hs = {qcdp_masyp, qcdmg_masyp, sq200to4j_masyp, sq400to4j_masyp};
//	
//	for (int i = 0; i < hs.size(); ++i) {
//		hs[i]->Rebin(60);
//		hs[i]->SetFillColor(0);
//		hs[i]->SetMarkerSize(0);
//		hs[i]->GetXaxis()->SetNdivisions(405);
//		hs[i]->GetXaxis()->SetTitle("#it{A}_{#it{m}}");
//	}
//	leg = new TLegend(0.59, 0.66, 0.80, 0.83);
//	leg->AddEntry(qcdp_masyp, name_proper["qcdp"], "le");
//	leg->AddEntry(qcdmg_masyp, name_proper["qcdmg"], "le");
//	leg->AddEntry(sq200to4j_masyp, name_proper["sq200to4j"], "le");
//	leg->AddEntry(sq400to4j_masyp, name_proper["sq400to4j"], "le");
//	leg->AddEntry(l, "cut value", "e");
//	
//	canvases = same_set(hs, "nminusone_sigxmasyp");
//	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
//		canvases[i]->cd();
//		style_info();
//		leg->Draw();
//		double x0, y0, x1, y1;
//		x0 = 0.1;
//		x1 = 0.1;
//		if (i == 0) {
//			y0 = 0;
//			y1 = 493.215;
//		}
//		else if (i == 1) {
//			y0 = 0;
//			y1 = 0.308707;
//		}
//		else if (i == 2) {
//			y0 = 0.5;
//			y1 = 1409.19;
//		}
//		TLine* line = new TLine(x0, y0, x1, y1);
//		line->SetLineColorAlpha(kBlack, 0.3);
//		line->SetLineWidth(2);
//		line->SetLineStyle(2);
//		line->Draw();
//		gPad->RedrawAxis();
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
//	}
//	
//	// tau21:
//	TH1* qcdp_tau21 = (TH1*) tf->Get("qcdp_sigxtau21");
//	TH1* qcdmg_tau21 = (TH1*) tf->Get("qcdmg_sigxtau21");
//	TH1* sq200to4j_tau21 = (TH1*) tf->Get("sq200to4j_sigxtau21");
//	TH1* sq400to4j_tau21 = (TH1*) tf->Get("sq400to4j_sigxtau21");
//	qcdp_tau21->SetLineColor(kBlue);
//	qcdmg_tau21->SetLineColor(kViolet);
//	sq200to4j_tau21->SetLineColor(kRed);
//	sq400to4j_tau21->SetLineColor(kOrange);
//	
//	cout << "tau21 integral = " << qcdmg_tau21->Integral(0, qcdmg_tau21->GetXaxis()->FindBin(0.75)) << endl;
//	
//	hs = {qcdp_tau21, qcdmg_tau21, sq200to4j_tau21, sq400to4j_tau21};
//	
//	for (int i = 0; i < hs.size(); ++i) {
//		hs[i]->Rebin(60);
//		hs[i]->SetFillColor(0);
//		hs[i]->SetMarkerSize(0);
//		hs[i]->GetXaxis()->SetNdivisions(405);
//		hs[i]->GetXaxis()->SetTitle("max#left(#tau_{21}#right)");
//	}
//	leg = new TLegend(0.2, 0.73, 0.40, 0.90);
//	leg->AddEntry(qcdp_tau21, name_proper["qcdp"], "le");
//	leg->AddEntry(qcdmg_tau21, name_proper["qcdmg"], "le");
//	leg->AddEntry(sq200to4j_tau21, name_proper["sq200to4j"], "le");
//	leg->AddEntry(sq400to4j_tau21, name_proper["sq400to4j"], "le");
//	leg->AddEntry(l, "cut value", "e");
//	
//	canvases = same_set(hs, "nminusone_sigxtau21");
//	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
//		canvases[i]->cd();
//		style_info();
//		leg->Draw();
//		double x0, y0, x1, y1;
//		x0 = 0.75;
//		x1 = 0.75;
//		if (i == 0) {
//			y0 = 0;
//			y1 = 242.736;
//		}
//		else if (i == 1) {
//			y0 = 0;
//			y1 = 0.34232;
//		}
//		else if (i == 2) {
//			y0 = 0.5;
//			y1 = 693.532;
//		}
//		TLine* line = new TLine(x0, y0, x1, y1);
//		line->SetLineColorAlpha(kBlack, 0.3);
//		line->SetLineWidth(2);
//		line->SetLineStyle(2);
//		line->Draw();
//		gPad->RedrawAxis();
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
//	}
//	
//	// tau42:
//	TH1* qcdp_tau42 = (TH1*) tf->Get("qcdp_sigxtau42");
//	TH1* qcdmg_tau42 = (TH1*) tf->Get("qcdmg_sigxtau42");
//	TH1* sq200to4j_tau42 = (TH1*) tf->Get("sq200to4j_sigxtau42");
//	TH1* sq400to4j_tau42 = (TH1*) tf->Get("sq400to4j_sigxtau42");
//	qcdp_tau42->SetLineColor(kBlue);
//	qcdmg_tau42->SetLineColor(kViolet);
//	sq200to4j_tau42->SetLineColor(kRed);
//	sq400to4j_tau42->SetLineColor(kOrange);
//	
//	cout << "tau42 integral = " << qcdmg_tau42->Integral(0, qcdmg_tau42->GetXaxis()->FindBin(0.45)) << endl;
//	
//	hs = {qcdp_tau42, qcdmg_tau42, sq200to4j_tau42, sq400to4j_tau42};
//	
//	for (int i = 0; i < hs.size(); ++i) {
//		hs[i]->Rebin(60);
//		hs[i]->SetFillColor(0);
//		hs[i]->SetMarkerSize(0);
//		hs[i]->GetXaxis()->SetNdivisions(405);
//		hs[i]->GetXaxis()->SetTitle("max#left(#tau_{42}#right)");
//	}
//	leg = new TLegend(0.2, 0.73, 0.40, 0.90);
//	leg->AddEntry(qcdp_tau42, name_proper["qcdp"], "le");
//	leg->AddEntry(qcdmg_tau42, name_proper["qcdmg"], "le");
//	leg->AddEntry(sq200to4j_tau42, name_proper["sq200to4j"], "le");
//	leg->AddEntry(sq400to4j_tau42, name_proper["sq400to4j"], "le");
//	leg->AddEntry(l, "cut value", "e");
//	
//	canvases = same_set(hs, "nminusone_sigxtau42");
//	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
//		canvases[i]->cd();
//		style_info();
//		leg->Draw();
//		double x0, y0, x1, y1;
//		x0 = 0.45;
//		x1 = 0.45;
//		if (i == 0) {
//			y0 = 0;
//			y1 = 10282.3;
//		}
//		else if (i == 1) {
//			y0 = 0;
//			y1 = 0.357036;
//		}
//		else if (i == 2) {
//			y0 = 0.5;
//			y1 = 29378;
//		}
//		TLine* line = new TLine(x0, y0, x1, y1);
//		line->SetLineColorAlpha(kBlack, 0.3);
//		line->SetLineWidth(2);
//		line->SetLineStyle(2);
//		line->Draw();
//		gPad->RedrawAxis();
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
//	}
//	
//	// tau43:
//	TH1* qcdp_tau43 = (TH1*) tf->Get("qcdp_sigxtau43");
//	TH1* qcdmg_tau43 = (TH1*) tf->Get("qcdmg_sigxtau43");
//	TH1* sq200to4j_tau43 = (TH1*) tf->Get("sq200to4j_sigxtau43");
//	TH1* sq400to4j_tau43 = (TH1*) tf->Get("sq400to4j_sigxtau43");
//	qcdp_tau43->SetLineColor(kBlue);
//	qcdmg_tau43->SetLineColor(kViolet);
//	sq200to4j_tau43->SetLineColor(kRed);
//	sq400to4j_tau43->SetLineColor(kOrange);
//	
//	cout << "tau43 integral = " << qcdmg_tau43->Integral(0, qcdmg_tau43->GetXaxis()->FindBin(0.80)) << endl;
//	
//	hs = {qcdp_tau43, qcdmg_tau43, sq200to4j_tau43, sq400to4j_tau43};
//	
//	for (int i = 0; i < hs.size(); ++i) {
//		hs[i]->Rebin(60);
//		hs[i]->SetFillColor(0);
//		hs[i]->SetMarkerSize(0);
//		hs[i]->GetXaxis()->SetNdivisions(405);
//		hs[i]->GetXaxis()->SetTitle("max#left(#tau_{43}#right)");
//	}
//	leg = new TLegend(0.2, 0.66, 0.40, 0.83);
//	leg->AddEntry(qcdp_tau43, name_proper["qcdp"], "le");
//	leg->AddEntry(qcdmg_tau43, name_proper["qcdmg"], "le");
//	leg->AddEntry(sq200to4j_tau43, name_proper["sq200to4j"], "le");
//	leg->AddEntry(sq400to4j_tau43, name_proper["sq400to4j"], "le");
//	leg->AddEntry(l, "cut value", "e");
//	
//	canvases = same_set(hs, "nminusone_sigxtau43");
//	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
//		canvases[i]->cd();
//		style_info(true, "38.2", 0);
//		leg->Draw();
//		double x0, y0, x1, y1;
//		x0 = 0.80;
//		x1 = 0.80;
//		if (i == 0) {
//			y0 = 0;
//			y1 = 816.461;
//		}
//		else if (i == 1) {
//			y0 = 0;
//			y1 = 0.404319;
//		}
//		else if (i == 2) {
//			y0 = 0.5;
//			y1 = 2332.74;
//		}
//		TLine* line = new TLine(x0, y0, x1, y1);
//		line->SetLineColorAlpha(kBlack, 0.3);
//		line->SetLineWidth(2);
//		line->SetLineStyle(2);
//		line->Draw();
//		gPad->RedrawAxis();
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
//		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
//	}
}
