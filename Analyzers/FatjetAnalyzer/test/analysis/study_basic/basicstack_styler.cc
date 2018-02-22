#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void draw_stack(TString variable, TString cut, TString mass, int nrebin=30, int corner=1, bool change_xaxis=true, double line=-1, TString line_label="Cut value") {
	TString signame = "sq" + mass + "to4j";
	bool plot_jetht = true;
//	if (cut == "sig" || cut == "sigxdeta" || cut == "sigxmasyp" || cut == "sigxtau21" || cut == "sigxtau42" || cut == "sigxtau43") plot_jetht = false;
	
	// Get plots from the file and set up:
//	cout << variable + "_qcdmg_" + cut << endl;
	TH1* h_qcdmg_orig = (TH1*) gDirectory->Get(variable + "_qcdmg_" + cut);
	TH1* h_ttbar_orig = (TH1*) gDirectory->Get(variable + "_ttbar_" + cut);
	TH1* h_sqto4j_orig = (TH1*) gDirectory->Get(variable + "_" + signame + "_" + cut);
	TH1* h_jetht_orig;
	if (plot_jetht) h_jetht_orig = (TH1*) gDirectory->Get(variable + "_jetht_" + cut);
	
	// Log loop:
	for (int logy = 0; logy <= 1; ++logy) {
		TString name = "stack_" + variable + "_" + cut;
		if (logy == 1) name += "_logy";
		TCanvas* tc = new TCanvas(name, name);
		tc->SetLogy(logy);
		THStack* hs = new THStack(TString("hs_") + name, "");		// Normal stack
	
		TH1* h_qcdmg = (TH1*) h_qcdmg_orig->Clone(name + "_qcd");
		TH1* h_ttbar = (TH1*) h_ttbar_orig->Clone(name + "_ttbar");
		TH1* h_sqto4j = (TH1*) h_sqto4j_orig->Clone(name + "_" + signame);
		TH1D* h_jetht;
		if (plot_jetht) h_jetht = (TH1D*) h_jetht_orig->Clone(name + "_jetht");
		if (plot_jetht) h_jetht->Rebin(nrebin);
		h_jetht = GarwoodErrorBands_68CL(h_jetht);
		h_qcdmg->Rebin(nrebin);
		h_ttbar->Rebin(nrebin);
		h_sqto4j->Rebin(nrebin);
	
		// Make summed background and background error plots:
		TH1* h_bkg = (TH1*) h_qcdmg->Clone(variable + "_bkg_" + cut);
		h_bkg->Add(h_ttbar);
		TH1* h_bkge = (TH1*) h_qcdmg->Clone(variable + "_bkge_" + cut);
		h_bkge->Add(h_ttbar);
		// Make signal (+bkg with 0 error) error plot:
		TH1* h_sige = (TH1*) h_sqto4j->Clone(variable + "_" + signame + "e_" + cut);
		for (int i = 1; i < h_qcdmg->GetXaxis()->GetNbins() + 1; ++i) h_qcdmg->SetBinError(i, 0);
		for (int i = 1; i < h_ttbar->GetXaxis()->GetNbins() + 1; ++i) h_ttbar->SetBinError(i, 0);
		h_sige->Add(h_qcdmg);
		h_sige->Add(h_ttbar);
	
		h_qcdmg->SetFillStyle(1001);
		h_qcdmg->SetFillColorAlpha(kBlue, 0.1);
		h_qcdmg->SetLineStyle(2);
		h_qcdmg->SetLineWidth(2);
		h_qcdmg->SetMarkerSize(0);
		hs->Add(h_qcdmg);
		h_ttbar->SetFillColor(kRed - 4);
		h_ttbar->SetFillStyle(3003);
		h_ttbar->SetMarkerSize(0);
		h_ttbar->SetLineWidth(2);
		hs->Add(h_ttbar);
		h_sqto4j->SetFillStyle(1001);
		h_sqto4j->SetFillColorAlpha(kGreen-5, 0.5);
		h_sqto4j->SetLineWidth(2);
		h_sqto4j->SetMarkerSize(0);
		hs->Add(h_sqto4j);
	
		h_bkge->SetMarkerSize(0);
		h_bkge->SetFillStyle(1001);
		h_bkge->SetFillColorAlpha(kBlack, 0.2);
		h_sige->SetMarkerSize(0);
		h_sige->SetFillStyle(3013);
		h_sige->SetFillColorAlpha(kBlack, 0.8);
	
		h_bkg->SetFillStyle(0);


		hs->Draw("hist");
		if (change_xaxis) {
			if (variable == "mavg") hs->GetXaxis()->SetRangeUser(0, 900);
			if (variable == "deta") hs->GetXaxis()->SetRangeUser(0, 1.0);
			if (variable == "masy") hs->GetXaxis()->SetRangeUser(0, 0.1);
			if (variable == "tau21") hs->GetXaxis()->SetRangeUser(0, 1.0);
			if (variable == "tau42") hs->GetXaxis()->SetRangeUser(0.2, 0.6);
			if (variable == "tau43") hs->GetXaxis()->SetRangeUser(0.4, 1.0);
		}
		if (logy == 0) hs->SetMaximum(hs->GetMaximum()*1.3);
		else {
			hs->SetMaximum(hs->GetMaximum()*10);
			hs->SetMinimum(hs->GetMaximum()*0.001);
//			cout << hs->GetMaximum() << " " << hs->GetMinimum() << endl;
		}
		hs->GetXaxis()->SetNdivisions(406);
		h_bkg->Draw("same hist");
		h_bkge->Draw("same e2");
		h_sige->Draw("same e2");
		if (plot_jetht) h_jetht->Draw("same e");
		TLine* tline;
		if (line >=0) {
//			double ymin = hs->GetMinimum();
			double ymin = 0;
			double ymax = hs->GetMaximum();
			if (logy) ymax = ymax/10;
			tline = new TLine(line, ymin, line, ymax);
			tline->SetLineColor(kRed-6);
			tline->SetLineWidth(2);
			tline->SetLineStyle(5);
			tline->Draw("same");
		}
		gPad->RedrawAxis();
	
		/// Legend:
		int corner_cms = 1;
		TLegend* leg;
		if (corner == 1) {
//			if (cut != "sig") leg = new TLegend(0.50, 0.71, 0.80, 0.91);		// When I was blinded.
//			else leg = new TLegend(0.50, 0.76, 0.80, 0.91);
			leg = new TLegend(0.50, 0.71, 0.80, 0.91);
			leg->SetFillStyle(0);
			corner_cms = 0;
		}
		else if (corner == 0) {
//			if (cut != "sig") leg = new TLegend(0.20, 0.71, 0.50, 0.91);
//			else leg = new TLegend(0.20, 0.76, 0.50, 0.91);
			leg = new TLegend(0.20, 0.71, 0.50, 0.91);
			leg->SetFillStyle(0);
		}
		else if (corner == 5) {
//			if (cut != "sig") leg = new TLegend(0.20, 0.80, 0.37, 0.91);
//			else leg = new TLegend(0.20, 0.81, 0.37, 0.91);
			leg = new TLegend(0.20, 0.80, 0.37, 0.91);
			leg->SetFillStyle(0);
		}
		else if (corner == 3) {
//			if (cut != "sig") leg = new TLegend(0.20, 0.32, 0.50, 0.52);
//			else leg = new TLegend(0.20, 0.32, 0.50, 0.47);
			leg = new TLegend(0.20, 0.32, 0.50, 0.52);
		}
		leg->AddEntry(h_qcdmg, "QCD (2 #rightarrow 4)", "f");
		leg->AddEntry(h_ttbar, "t#bar{t}", "f");
		leg->AddEntry(h_bkge, "Total background ", "lf");
		leg->AddEntry(h_sqto4j, TString("#it{m}_{#tilde{q}} = ") + mass + " GeV", "f");
		if (plot_jetht) leg->AddEntry(h_jetht, name_proper["jetht"], "lpe");
		if (line >= 0) leg->AddEntry(tline, line_label, "l");
		
		leg->Draw();
		/// Axes:
		set_xtitle(hs, variable);
		hs->GetYaxis()->SetTitle("Events");
		if (variable == "mavg") style_ylabel(hs);
	
		// Write info:
		style_info(!plot_jetht, lum_string["all"], corner_cms);
		style_cut(cut);
	
		tc->SaveAs(name + ".pdf");
	}
	
//	cout << "bkg integral = " << h_qcdmg->Integral() + h_ttbar->Integral() << endl;
}

void basicstack_styler() {
	gROOT->SetBatch();
	
	TFile* tf = TFile::Open("basic_plots.root");
	
	vector<TString> cuts = {"sig", "sb", "sbb"};
	vector<TString> variables = {"mavg", "masy", "deta", "tau21", "tau42", "tau43"};
	
	for (int icut = 0; icut < cuts.size(); ++icut) {
		for (int ivar = 0; ivar < variables.size(); ++ivar) {
			TString cut = cuts[icut];
			TString variable = variables[ivar];
			int nrebin = 30;
			int corner = 1;
			if (variable == "deta") nrebin = 20;
			if (variable == "masy") nrebin = 10;
			
			if (variable == "deta" || variable == "masy") {
				corner = 3;
				if (cut == "sbb" || cut == "sig") corner = 5;
			}
			else if (variable == "tau21") {
				corner = 0;
				if (cut == "sbb") nrebin = 40;
				if (cut == "sig") nrebin = 50;
			}
			else if (variable == "tau42") {
				corner = 0;
				if (cut == "sig") nrebin = 40;
			}
			else if (variable == "tau43") {
				corner = 0;
				if (cut == "sbb") nrebin = 40;
				if (cut == "sig") nrebin = 50;
			}
			draw_stack(variable, cut, "200", nrebin, corner);
		}
	}
//	vector<TH1*> hs;
//	vector<TCanvas*> canvases;
//	
//	vector<TString> cuts = {"sig", "sb", "sbb"};
//	vector<TString> variables = {"mavg", "masy", "deta", "tau21", "tau42", "tau43"};
//	
//	for (int icut = 0; icut < cuts.size(); ++icut) {
//		for (int ivar = 0; ivar < variables.size(); ++ivar) {
//			TString cut = cuts[icut];
//			TString variable = variables[ivar];
//			TH1* qcdp = (TH1*) tf->Get(variable + "_qcdp_" + cut);
//			TH1* qcdmg = (TH1*) tf->Get(variable + "_qcdmg_" + cut);
//			TH1* ttbar = (TH1*) tf->Get(variable + "_ttbar_" + cut);
//			TH1* jetht = (TH1*) tf->Get(variable + "_jetht_" + cut);
//			TH1* sq200to4j = (TH1*) tf->Get(variable + "_sq200to4j_" + cut);
//		//	TH1* sq400to4j_deta = (TH1*) tf->Get("sq400to4j_sigxdeta");

//			qcdmg->SetLineColor(kBlue);
//			qcdmg->SetMarkerColor(kBlue);
//			ttbar->SetLineColor(kViolet);
//			ttbar->SetMarkerColor(kViolet);
//			sq200to4j->SetLineColor(kRed);
//			sq200to4j->SetMarkerColor(kRed);
//			jetht->SetLineColor(kBlack);
//			jetht->SetMarkerColor(kBlack);
//	
//		//	cout << "deta integral = " << qcdmg_deta->Integral(0, qcdmg_deta->GetXaxis()->FindBin(1.0)) << endl;
//	
//			hs = {qcdmg, ttbar, sq200to4j};
//			if (cut != "sig" and cut != "sigl") hs.push_back(jetht);
//	
//			for (int i = 0; i < hs.size(); ++i) {
//				
////				if (cut == "sig") hs[i]->Rebin(100);
////				else hs[i]->Rebin(30);
//				hs[i]->SetFillColor(0);
//		//		hs[i]->SetMarkerSize(0);
//				hs[i]->GetXaxis()->SetNdivisions(405);
//				TString xtitle = variable_proper[variable];
//				if (unit_proper[variable] != "") xtitle += " [" + unit_proper[variable] + "]";
//				hs[i]->GetXaxis()->SetTitle(xtitle);
//		//		hs[i]->GetYaxis()->SetTitle("Normalized events");
//				
//				// Particulars:
////				if (cut == "sb") hs[i]->Rebin(60);
////				else hs[i]->Rebin(100);
//				hs[i]->Rebin(60);
//				
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
////				else if (variable == "tau21") {
////					if (cut == "sb") hs[i]->SetMaximum(1300);
////					else if (cut == "sbb") hs[i]->SetMaximum(200);
////					else if (cut == "sig") hs[i]->SetMaximum(100);
////				}
//			}
//			
//			int leg_pos = 1;
//			
//			// More particulars:
//			if (variable == "deta") {
//				leg_pos = 0;
//			}
//			else if (variable == "masy") {
//				leg_pos = 0;
//			}
//			else if (variable == "tau21") {
//				leg_pos = 0;
//			}
//			else if (variable == "tau42") {
//				leg_pos = 0;
//			}
//			else if (variable == "tau43") {
//				leg_pos = 0;
//			}
//			
//			vector<TLegend*> legs = {
//				new TLegend(0.20, 0.80, 0.40, 0.92),		// 0: Top-left
//				new TLegend(0.59, 0.66, 0.80, 0.83),		// 1: Top-right
//				new TLegend(0.59, 0.66, 0.80, 0.83),		// 2: Bottom-right
//				new TLegend(0.59, 0.66, 0.80, 0.83),		// 3: Bottom-left
//			};
//			TLegend* leg = legs[leg_pos];
//		//	leg->AddEntry(qcdp, name_proper["qcdp"], "le");
//			leg->AddEntry(qcdmg, name_proper["qcdmg"], "lpe");
//			leg->AddEntry(ttbar, name_proper["ttbar"], "lpe");
//			leg->AddEntry(sq200to4j, name_proper["sq200to4j"], "lpe");
//			if (cut != "sig" and  cut != "sigl") leg->AddEntry(jetht, name_proper["jetht"], "lpe");
//		//	leg->AddEntry(sq400to4j, name_proper["sq400to4j"], "le");
//	
//			canvases = same_set(hs, "basic_" + variable + "_" + cut);
//			for (int i = 0; i < 3; ++i) {		// Order: nom, norm, logy
//				canvases[i]->cd();
//				style_info();
//				leg->Draw();
//				style_write(TString("Selection: #bf{") + cut_proper[cut] + "}",  0.18, 0.94, 0.018);
//				canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".pdf");
//				canvases[i]->SaveAs(TString(canvases[i]->GetName()) + ".png");
//			}
//		}
//	}
}
