#include "../study_basic/basicstack_styler.cc"

void nminusonestack_styler() {
	gROOT->SetBatch();
	
	TFile* tf = TFile::Open("nminusone_plots.root");
	
	vector<TString> variables = {"masyp", "deta", "tau21", "tau42", "tau43"};
	
	for (int ivar = 0; ivar < variables.size(); ++ivar) {
		TString variable = variables[ivar];
		TString cut = "sigx" + variable;
		int nrebin = 30;
		int corner = 1;
		if (cut == "sigxtau42") {
			corner = 0;
		}
		else if (cut == "sigxtau43") {
			nrebin = 50;
			corner = 0;
		}
		else if (cut == "sigxtau21") {
			nrebin = 60;
			corner = 0;
		}
		else if (cut == "sigxdeta") {
			nrebin = 60;
		}
		else if (cut == "sigxmasyp") {
			nrebin = 60;
			corner = 1;
		}
		
		double line = -1;
		if (variable == "deta") line = 1.0;
		else if (variable == "masyp") line = 0.1;
		else if (variable == "tau21") line = 0.75;
		else if (variable == "tau42") line = 0.45;
		else if (variable == "tau43") line = 0.80;
		draw_stack(variable, cut, "200", nrebin, corner, line);
	}
}
