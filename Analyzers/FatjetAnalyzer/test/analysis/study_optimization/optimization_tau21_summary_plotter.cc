#include "/home/tote/decortication/macros/optimization_tools/optimization_tools.cc"

vector<double> get_significances(int m, TH1* signal, TH1* qcd, TH1* ttbar) {
	int ibin_low = qcd->GetXaxis()->FindBin(m - 25);
	int ibin_high = qcd->GetXaxis()->FindBin(m + 25);
	double s = signal->Integral(ibin_low, ibin_high);
	double b = qcd->Integral(ibin_low, ibin_high) + ttbar->Integral(ibin_low, ibin_high);
	
	vector<double> significances;
	for (int kind = 1; kind < 5; ++ kind) {
		if (s > 0 && b > 0) {
			significances.push_back(significance(kind, s, b));
		}
		else {
			significances.push_back(0);
		}
	}
	return significances;
}


void optimization_tau21_summary_plotter() {
	gROOT->SetBatch();
	vector<int> ms = {100, 150, 200, 250, 300, 400, 500};
	
	TFile *tf_in = TFile::Open("optimization_plots_43tau800_42tau450.root");
	TFile* tf_out = new TFile("optimization_summary_plots_43tau800_42tau450.root", "RECREATE");
	
	TH1* bins = (TH1*) tf_in->Get("bins");
	
	gStyle->SetOptStat(0);
	int tau21nbins = bins->GetXaxis()->GetNbins();
	double tau21min = bins->GetXaxis()->GetBinLowEdge(1);
	double tau21max = bins->GetXaxis()->GetBinUpEdge(tau21nbins);
	
	// Prepare "summary" histograms:
	map<TString, TH1*> summaries;
	for (int i = 0; i < ms.size(); ++ i) {
		for (int kind = 1; kind < 5; ++ kind) {
			TString name_summary = "sq" + to_string(ms[i]) + "to4j_s" + to_string(kind);
			summaries[name_summary] = new TH1F(name_summary, "", tau21nbins, tau21min, tau21max);
		}
	}
	
	// Fill "summary" histograms:
	for (int i = 1; i < tau21nbins + 1; i++) {
		double value_tau21 = bins->GetXaxis()->GetBinCenter(i);
		TString string_tau21 = to_string((int) round(value_tau21*1000));
		TString suffix = "_21tau" + string_tau21;
		
		TH1* h_qcdmg = (TH1*) tf_in->Get("qcdmg" + suffix);
		TH1* h_ttbar = (TH1*) tf_in->Get("ttbar" + suffix);
		for (int k = 0; k < ms.size(); ++ k) {
			int m = ms[k];
			TH1* h_s = (TH1*) tf_in->Get("sq" + to_string(m) + "to4j" + suffix);
			vector<double> significances = get_significances(m, h_s, h_qcdmg, h_ttbar);
			for (int kind = 1; kind < 5; ++ kind) {
				TString name_summary = "sq" + to_string(m) + "to4j_s" + to_string(kind);
				summaries[name_summary]->SetBinContent(i, significances[kind - 1]);
			}
		}
	}
	
	// Write out "summary" histograms:
	tf_out->cd();
	for (int i = 0; i < ms.size(); ++ i) {
		for (int kind = 1; kind < 5; ++ kind) {
			TString name_summary = "sq" + to_string(ms[i]) + "to4j_s" + to_string(kind);
			summaries[name_summary]->Write();
		}
	}
	
	return;
}
