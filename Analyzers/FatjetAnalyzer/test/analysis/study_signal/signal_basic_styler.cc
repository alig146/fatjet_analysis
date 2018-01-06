#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void signal_basic_styler() {
	// Configure:
	gStyle->SetOptStat(0);
	
	TFile* tf_in = TFile::Open("../study_basic/basic_plots.root");
	
	vector<TString> sqto4j = {"sq100to4j", "sq200to4j", "sq300to4j", "sq400to4j", "sq500to4j"};
	vector<TString> cuts = {"sig"};
	vector<TString> vars = {"eta"};
	vector<int> colors = {kRed-4, kOrange-1, kSpring+4, kBlue-5, kViolet-6};
	
	for (unsigned ivar = 0; ivar < vars.size() ; ++ ivar) {
		TString var = vars[ivar];
		for (unsigned icut = 0; icut < cuts.size() ; ++ icut) {
			TString cut = cuts[icut];
			TString name = "signal_basic_" + var + "_" + cut;
			TCanvas* tc = new TCanvas(name, name);
			
			
			// Set up legend:
			TLegend* leg = get_legend(0);
			
			for (unsigned i = 0; i < sqto4j.size() ; ++ i) {
				TH1* h = (TH1*) tf_in->Get(var + "_" + sqto4j[i] + "_" + cut);
				
				// Normalize:
				normalize_th1(h);
				
				// Style:
				h->Rebin(30);
				if (var == "eta") h->GetXaxis()->SetRangeUser(-2.3, 2.3);
				h->SetLineColor(colors[i]);
				h->SetMarkerColor(colors[i]);
				
				// Draw:
				if (i == 0) {
					h->SetMaximum(h->GetMaximum()*1.4);
					h->GetXaxis()->SetNdivisions(405);
					h->GetXaxis()->SetTitle(variable_proper[var]);
					h->GetYaxis()->SetTitle("Normalized events");
					h->Draw("e");
				}
				else h->Draw("e same");
				
				leg->AddEntry(h, name_proper[sqto4j[i]], "lep");
			}
			
			leg->Draw();
			
			style_info();
			style_cut(cut);
			
			tc->SaveAs(TString(tc->GetName()) + ".pdf");
		}
	}
}
