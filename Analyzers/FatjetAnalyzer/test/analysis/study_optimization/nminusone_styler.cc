#include "/home/tote/decortication/macros/common.cc"

void nminusone_styler() {
	TFile* tf = TFile::Open("nminusone_plots.root");
	vector<TH1*> hs;
	vector<TCanvas*> canvases;
	TLine* l = new TLine(0, 0, 1, 1);
	l->SetLineColorAlpha(kBlack, 0.3);
	l->SetLineWidth(2);
	l->SetLineStyle(2);
	
	// deta:
	TH1* qcdp_deta = (TH1*) tf->Get("qcdp_sigxdeta");
	TH1* qcdmg_deta = (TH1*) tf->Get("qcdmg_sigxdeta");
	TH1* sq200to4j_deta = (TH1*) tf->Get("sq200to4j_sigxdeta");
	TH1* sq400to4j_deta = (TH1*) tf->Get("sq400to4j_sigxdeta");
	qcdp_deta->SetLineColor(kBlue);
	qcdmg_deta->SetLineColor(kViolet);
	sq200to4j_deta->SetLineColor(kRed);
	sq400to4j_deta->SetLineColor(kOrange);
	
	cout << "deta integral = " << qcdmg_deta->Integral(0, qcdmg_deta->GetXaxis()->FindBin(1.0)) << endl;
	
	hs = {qcdp_deta, qcdmg_deta, sq200to4j_deta, sq400to4j_deta};
	
	for (int i = 0; i < hs.size(); ++i) {
		hs[i]->Rebin(60);
		hs[i]->SetFillColor(0);
		hs[i]->SetMarkerSize(0);
		hs[i]->GetXaxis()->SetNdivisions(405);
		hs[i]->GetXaxis()->SetTitle("#left|#Delta#eta#right|");
//		hs[i]->GetYaxis()->SetTitle("Normalized events");
	}
	leg = new TLegend(0.59, 0.66, 0.80, 0.83);
	leg->AddEntry(qcdp_deta, name_proper["qcdp"], "le");
	leg->AddEntry(qcdmg_deta, name_proper["qcdmg"], "le");
	leg->AddEntry(sq200to4j_deta, name_proper["sq200to4j"], "le");
	leg->AddEntry(sq400to4j_deta, name_proper["sq400to4j"], "le");
	leg->AddEntry(l, "cut value", "e");
	
	canvases = same_set(hs, "nminusone_sigxdeta");
	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
		canvases[i]->cd();
		style_info();
		leg->Draw();
		double x0, y0, x1, y1;
		x0 = 1.0;
		x1 = 1.0;
		if (i == 0) {
			y0 = 0;
			y1 = 252.199;
		}
		else if (i == 1) {
			y0 = 0;
			y1 = 0.2556;
		}
		else if (i == 2) {
			y0 = 0.5;
			y1 = 720.568;
		}
		TLine* line = new TLine(x0, y0, x1, y1);
		line->SetLineColorAlpha(kBlack, 0.3);
		line->SetLineWidth(2);
		line->SetLineStyle(2);
		line->Draw();
		gPad->RedrawAxis();
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
	}
	
	// masyp:
	TH1* qcdp_masyp = (TH1*) tf->Get("qcdp_sigxmasyp");
	TH1* qcdmg_masyp = (TH1*) tf->Get("qcdmg_sigxmasyp");
	TH1* sq200to4j_masyp = (TH1*) tf->Get("sq200to4j_sigxmasyp");
	TH1* sq400to4j_masyp = (TH1*) tf->Get("sq400to4j_sigxmasyp");
	qcdp_masyp->SetLineColor(kBlue);
	qcdmg_masyp->SetLineColor(kViolet);
	sq200to4j_masyp->SetLineColor(kRed);
	sq400to4j_masyp->SetLineColor(kOrange);
	
	cout << "masyp integral = " << qcdmg_masyp->Integral(0, qcdmg_masyp->GetXaxis()->FindBin(0.1)) << endl;
	
	hs = {qcdp_masyp, qcdmg_masyp, sq200to4j_masyp, sq400to4j_masyp};
	
	for (int i = 0; i < hs.size(); ++i) {
		hs[i]->Rebin(60);
		hs[i]->SetFillColor(0);
		hs[i]->SetMarkerSize(0);
		hs[i]->GetXaxis()->SetNdivisions(405);
		hs[i]->GetXaxis()->SetTitle("#it{A}_{#it{m}}");
	}
	leg = new TLegend(0.59, 0.66, 0.80, 0.83);
	leg->AddEntry(qcdp_masyp, name_proper["qcdp"], "le");
	leg->AddEntry(qcdmg_masyp, name_proper["qcdmg"], "le");
	leg->AddEntry(sq200to4j_masyp, name_proper["sq200to4j"], "le");
	leg->AddEntry(sq400to4j_masyp, name_proper["sq400to4j"], "le");
	leg->AddEntry(l, "cut value", "e");
	
	canvases = same_set(hs, "nminusone_sigxmasyp");
	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
		canvases[i]->cd();
		style_info();
		leg->Draw();
		double x0, y0, x1, y1;
		x0 = 0.1;
		x1 = 0.1;
		if (i == 0) {
			y0 = 0;
			y1 = 493.215;
		}
		else if (i == 1) {
			y0 = 0;
			y1 = 0.308707;
		}
		else if (i == 2) {
			y0 = 0.5;
			y1 = 1409.19;
		}
		TLine* line = new TLine(x0, y0, x1, y1);
		line->SetLineColorAlpha(kBlack, 0.3);
		line->SetLineWidth(2);
		line->SetLineStyle(2);
		line->Draw();
		gPad->RedrawAxis();
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
	}
	
	// tau21:
	TH1* qcdp_tau21 = (TH1*) tf->Get("qcdp_sigxtau21");
	TH1* qcdmg_tau21 = (TH1*) tf->Get("qcdmg_sigxtau21");
	TH1* sq200to4j_tau21 = (TH1*) tf->Get("sq200to4j_sigxtau21");
	TH1* sq400to4j_tau21 = (TH1*) tf->Get("sq400to4j_sigxtau21");
	qcdp_tau21->SetLineColor(kBlue);
	qcdmg_tau21->SetLineColor(kViolet);
	sq200to4j_tau21->SetLineColor(kRed);
	sq400to4j_tau21->SetLineColor(kOrange);
	
	cout << "tau21 integral = " << qcdmg_tau21->Integral(0, qcdmg_tau21->GetXaxis()->FindBin(0.75)) << endl;
	
	hs = {qcdp_tau21, qcdmg_tau21, sq200to4j_tau21, sq400to4j_tau21};
	
	for (int i = 0; i < hs.size(); ++i) {
		hs[i]->Rebin(60);
		hs[i]->SetFillColor(0);
		hs[i]->SetMarkerSize(0);
		hs[i]->GetXaxis()->SetNdivisions(405);
		hs[i]->GetXaxis()->SetTitle("max#left(#tau_{21}#right)");
	}
	leg = new TLegend(0.2, 0.73, 0.40, 0.90);
	leg->AddEntry(qcdp_tau21, name_proper["qcdp"], "le");
	leg->AddEntry(qcdmg_tau21, name_proper["qcdmg"], "le");
	leg->AddEntry(sq200to4j_tau21, name_proper["sq200to4j"], "le");
	leg->AddEntry(sq400to4j_tau21, name_proper["sq400to4j"], "le");
	leg->AddEntry(l, "cut value", "e");
	
	canvases = same_set(hs, "nminusone_sigxtau21");
	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
		canvases[i]->cd();
		style_info();
		leg->Draw();
		double x0, y0, x1, y1;
		x0 = 0.75;
		x1 = 0.75;
		if (i == 0) {
			y0 = 0;
			y1 = 242.736;
		}
		else if (i == 1) {
			y0 = 0;
			y1 = 0.34232;
		}
		else if (i == 2) {
			y0 = 0.5;
			y1 = 693.532;
		}
		TLine* line = new TLine(x0, y0, x1, y1);
		line->SetLineColorAlpha(kBlack, 0.3);
		line->SetLineWidth(2);
		line->SetLineStyle(2);
		line->Draw();
		gPad->RedrawAxis();
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
	}
	
	// tau42:
	TH1* qcdp_tau42 = (TH1*) tf->Get("qcdp_sigxtau42");
	TH1* qcdmg_tau42 = (TH1*) tf->Get("qcdmg_sigxtau42");
	TH1* sq200to4j_tau42 = (TH1*) tf->Get("sq200to4j_sigxtau42");
	TH1* sq400to4j_tau42 = (TH1*) tf->Get("sq400to4j_sigxtau42");
	qcdp_tau42->SetLineColor(kBlue);
	qcdmg_tau42->SetLineColor(kViolet);
	sq200to4j_tau42->SetLineColor(kRed);
	sq400to4j_tau42->SetLineColor(kOrange);
	
	cout << "tau42 integral = " << qcdmg_tau42->Integral(0, qcdmg_tau42->GetXaxis()->FindBin(0.45)) << endl;
	
	hs = {qcdp_tau42, qcdmg_tau42, sq200to4j_tau42, sq400to4j_tau42};
	
	for (int i = 0; i < hs.size(); ++i) {
		hs[i]->Rebin(60);
		hs[i]->SetFillColor(0);
		hs[i]->SetMarkerSize(0);
		hs[i]->GetXaxis()->SetNdivisions(405);
		hs[i]->GetXaxis()->SetTitle("max#left(#tau_{42}#right)");
	}
	leg = new TLegend(0.2, 0.73, 0.40, 0.90);
	leg->AddEntry(qcdp_tau42, name_proper["qcdp"], "le");
	leg->AddEntry(qcdmg_tau42, name_proper["qcdmg"], "le");
	leg->AddEntry(sq200to4j_tau42, name_proper["sq200to4j"], "le");
	leg->AddEntry(sq400to4j_tau42, name_proper["sq400to4j"], "le");
	leg->AddEntry(l, "cut value", "e");
	
	canvases = same_set(hs, "nminusone_sigxtau42");
	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
		canvases[i]->cd();
		style_info();
		leg->Draw();
		double x0, y0, x1, y1;
		x0 = 0.45;
		x1 = 0.45;
		if (i == 0) {
			y0 = 0;
			y1 = 10282.3;
		}
		else if (i == 1) {
			y0 = 0;
			y1 = 0.357036;
		}
		else if (i == 2) {
			y0 = 0.5;
			y1 = 29378;
		}
		TLine* line = new TLine(x0, y0, x1, y1);
		line->SetLineColorAlpha(kBlack, 0.3);
		line->SetLineWidth(2);
		line->SetLineStyle(2);
		line->Draw();
		gPad->RedrawAxis();
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
	}
	
	// tau43:
	TH1* qcdp_tau43 = (TH1*) tf->Get("qcdp_sigxtau43");
	TH1* qcdmg_tau43 = (TH1*) tf->Get("qcdmg_sigxtau43");
	TH1* sq200to4j_tau43 = (TH1*) tf->Get("sq200to4j_sigxtau43");
	TH1* sq400to4j_tau43 = (TH1*) tf->Get("sq400to4j_sigxtau43");
	qcdp_tau43->SetLineColor(kBlue);
	qcdmg_tau43->SetLineColor(kViolet);
	sq200to4j_tau43->SetLineColor(kRed);
	sq400to4j_tau43->SetLineColor(kOrange);
	
	cout << "tau43 integral = " << qcdmg_tau43->Integral(0, qcdmg_tau43->GetXaxis()->FindBin(0.80)) << endl;
	
	hs = {qcdp_tau43, qcdmg_tau43, sq200to4j_tau43, sq400to4j_tau43};
	
	for (int i = 0; i < hs.size(); ++i) {
		hs[i]->Rebin(60);
		hs[i]->SetFillColor(0);
		hs[i]->SetMarkerSize(0);
		hs[i]->GetXaxis()->SetNdivisions(405);
		hs[i]->GetXaxis()->SetTitle("max#left(#tau_{43}#right)");
	}
	leg = new TLegend(0.2, 0.66, 0.40, 0.83);
	leg->AddEntry(qcdp_tau43, name_proper["qcdp"], "le");
	leg->AddEntry(qcdmg_tau43, name_proper["qcdmg"], "le");
	leg->AddEntry(sq200to4j_tau43, name_proper["sq200to4j"], "le");
	leg->AddEntry(sq400to4j_tau43, name_proper["sq400to4j"], "le");
	leg->AddEntry(l, "cut value", "e");
	
	canvases = same_set(hs, "nminusone_sigxtau43");
	for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
		canvases[i]->cd();
		style_info(true, "38.2", 0);
		leg->Draw();
		double x0, y0, x1, y1;
		x0 = 0.80;
		x1 = 0.80;
		if (i == 0) {
			y0 = 0;
			y1 = 816.461;
		}
		else if (i == 1) {
			y0 = 0;
			y1 = 0.404319;
		}
		else if (i == 2) {
			y0 = 0.5;
			y1 = 2332.74;
		}
		TLine* line = new TLine(x0, y0, x1, y1);
		line->SetLineColorAlpha(kBlack, 0.3);
		line->SetLineWidth(2);
		line->SetLineStyle(2);
		line->Draw();
		gPad->RedrawAxis();
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
		canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
	}
}
