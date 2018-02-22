#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void nminusone_styler() {
	gROOT->SetBatch();
	
	TFile* tf = TFile::Open("nminusone_plots.root");
	vector<TString> vars = {"deta", "masyp", "tau21", "tau42", "tau43"};
	vector<double> values = {1.0, 0.1, 0.75, 0.50, 0.80};
	
	vector<TH1*> hs;
	vector<TCanvas*> canvases;
	TLine* l = new TLine(0, 0, 1, 1);
	l->SetLineColorAlpha(kBlack, 0.3);
	l->SetLineWidth(2);
	l->SetLineStyle(2);
	
	for (unsigned ivar = 0; ivar < vars.size(); ++ivar) {
		TString var = vars[ivar];
		double value = values[ivar];
		
		cout << "[..] Styling " + var + " plots." << endl;
		
		TH1* qcdp = (TH1*) tf->Get(var + "_qcdp_sigx" + var);
		TH1* qcdmg = (TH1*) tf->Get(var + "_qcdmg_sigx" + var);
		TH1* sq200to4j = (TH1*) tf->Get(var + "_sq200to4j_sigx" + var);
		TH1* sq400to4j = (TH1*) tf->Get(var + "_sq400to4j_sigx" + var);
		
		qcdp->SetLineColor(kBlue);
		qcdmg->SetLineColor(kViolet);
		sq200to4j->SetLineColor(kRed);
		sq400to4j->SetLineColor(kOrange);
	
//		cout << var + " qcdmg integral = " << qcdmg->Integral(0, qcdmg->GetXaxis()->FindBin(value)) << endl;
	
		hs = {qcdp, qcdmg, sq200to4j, sq400to4j};
	
		for (int i = 0; i < hs.size(); ++i) {
			hs[i]->Rebin(60);
			hs[i]->SetFillColor(0);
			hs[i]->SetMarkerSize(0);
			hs[i]->GetXaxis()->SetNdivisions(405);
			hs[i]->GetXaxis()->SetTitle(get_xtitle(var));
	//		hs[i]->GetYaxis()->SetTitle("Normalized events");
		}
		int corner_leg = 1;
		if (var == "tau21" || var == "tau42" || var == "tau43") corner_leg = 0; 
		TLegend* leg = get_legend(corner_leg, 5);
		leg->AddEntry(qcdp, name_proper["qcdp"], "le");
		leg->AddEntry(qcdmg, name_proper["qcdmg"], "le");
		leg->AddEntry(sq200to4j, name_proper["sq200to4j"], "le");
		leg->AddEntry(sq400to4j, name_proper["sq400to4j"], "le");
		leg->AddEntry(l, "cut value", "e");
	
		canvases = same_set(hs, "nminusone_sigx" + var);
		for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
			canvases[i]->cd();
			leg->Draw();
			
			int corner_info = 1;
			// Make line:
			double x0, y0, x1, y1;
			x0 = value;
			x1 = value;
			y0 = 0;
			if (i == 0) {
				if (var == "deta") {
					y1 = 240;
				}
				else if (var == "masyp") {
					y1 = 422;
				}
				else if (var == "tau21") {
					y1 = 242.736;
				}
				else if (var == "tau42") {
					y1 = 10282.3;
				}
				else if (var == "tau43") {
					y1 = 816.461;
				}
			}
			else if (i == 1) {
				if (var == "deta") {
					y1 = 0.261;
				}
				else if (var == "masyp") {
					y1 = 0.374;
				}
				else if (var == "tau21") {
					y1 = 0.4;
					corner_info = 3;
				}
				else if (var == "tau42") {
					y1 = 0.415;
//					corner_info = 3;
				}
				else if (var == "tau43") {
					y1 = 0.47;
					corner_info = 3;
				}
			}
			else if (i == 2) {
				if (var == "deta") {
					y0 = 0.5;
					y1 = 720.568;
				}
				else if (var == "masyp") {
					y0 = 0.5;
					y1 = 1409.19;
				}
				else if (var == "tau21") {
					y0 = 0.5;
					y1 = 693.532;
				}
				else if (var == "tau42") {
					y0 = 0.5;
					y1 = 29378;
				}
				else if (var == "tau43") {
					y0 = 0.5;
					y1 = 2332.74;
				}
			}
			TLine* line = new TLine(x0, y0, x1, y1);
			line->SetLineColorAlpha(kBlack, 0.3);
			line->SetLineWidth(2);
			line->SetLineStyle(2);
			line->Draw();
			style_info(true, lum_string["all"], corner_info);
			style_cut("sigx" + var);
			gPad->RedrawAxis();
			save(canvases[i]);
		}
	}
}
