#include "/home/tote/truculence/macros/styling_tools/styling_tools.cc"
#include "/home/tote/decortication/macros/info.cc"

void cutflow_styler() {
	tfile = TFile::Open("cutflow_plots.root");
	vector<TString> tt_names = {"sq100to4j","sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j", "qcdmg", "ttbar", "wjets"};
	vector<TString> versions = {"n", "w"};
	
	gStyle->SetOptStat(0);
	TCanvas* tc = new TCanvas();
	
	for (vector<TString>::iterator tt_name = tt_names.begin(); tt_name != tt_names.end(); ++tt_name) {
		for (vector<TString>::iterator version = versions.begin(); version != versions.end(); ++version) {
			TString name = *tt_name + "_sig_" + *version;
			TH1* h = (TH1*) tfile->Get(name);
			h->Draw("hist");
			style_write("#scale[0.8]{" + name_proper[*tt_name] + "}", 0.62, 0.78);
			h->SetTitleOffset(1.55, "x");
			h->GetXaxis()->SetTitle("Cut (cumulative)");
			if (*version == "n"){h->GetYaxis()->SetTitle("Entries");}
			else if (*version == "w"){
				h->GetYaxis()->SetTitle("Events");
				style_info(true);
			}
			tc->SetLogy(0);
			tc->SaveAs("cutflow_results/cutflow_" + name + ".pdf");
			tc->SetLogy(1);
			tc->SaveAs("cutflow_results/cutflow_" + name + "_logy.pdf");
		}
	}
}

