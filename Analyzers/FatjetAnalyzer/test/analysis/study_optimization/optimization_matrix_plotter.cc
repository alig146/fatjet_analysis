#include <Deracination/Straphanger/test/decortication/macros/optimization_tools/optimization_tools.cc>

vector<double> get_significances(int m, TH1* signal, TH1* qcd, TH1* ttbar, int window=50) {
	int ibin_low = qcd->GetXaxis()->FindBin(m - window/2);
	int ibin_high = qcd->GetXaxis()->FindBin(m + window/2);
	double s = signal->Integral(ibin_low, ibin_high);
	double b = qcd->Integral(ibin_low, ibin_high) + ttbar->Integral(ibin_low, ibin_high);
	if (qcd->Integral(ibin_low, ibin_high) == 0 && ttbar->Integral(ibin_low, ibin_high) < 15) b = 0;
//	if (qcd->Integral(ibin_low, ibin_high) == 0) b = 0;
	cout << s << "  " << b << endl;
	
	vector<double> significances;
	for (int kind = 1; kind < 5; ++ kind) {
		if (s > 0 && b > 0) {
			significances.push_back(significance(kind, s, b, false));
		}
		else {
			significances.push_back(0);
		}
	}
	return significances;
}


void optimization_matrix_plotter() {
	gROOT->SetBatch();
	vector<int> ms = {100, 150, 175, 200, 250, 300, 400, 500};
	
	TFile *tf_in = TFile::Open("optimization_plots.root");
	TFile* tf_out = new TFile("optimization_matrices.root", "RECREATE");
//	TFile* tf_out = new TFile("optimization_matrices_window100.root", "RECREATE");
	
	TH2* bins = (TH2*) tf_in->Get("bins");
	
	gStyle->SetOptStat(0);
	int tau42nbins = bins->GetXaxis()->GetNbins();
	double tau42min = bins->GetXaxis()->GetBinLowEdge(1);
	double tau42max = bins->GetXaxis()->GetBinUpEdge(tau42nbins);
	int tau43nbins = bins->GetYaxis()->GetNbins();
	double tau43min = bins->GetYaxis()->GetBinLowEdge(1);
	double tau43max = bins->GetYaxis()->GetBinUpEdge(tau43nbins);
	
	// Prepare "matrix" histograms:
	map<TString, TH2*> matrices;
	for (int i = 0; i < ms.size(); ++ i) {
		for (int kind = 1; kind < 5; ++ kind) {
			TString name_matrix = "sq" + to_string(ms[i]) + "to4j_s" + to_string(kind);
			matrices[name_matrix] = new TH2F(name_matrix, "", tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max);
		}
	}
	
	// Fill "matrix" histograms:
	for (int i = 1; i < tau42nbins + 1; i++) {
		for (int j = 1; j < tau43nbins + 1; j++) {
			double value_tau42 = bins->GetXaxis()->GetBinCenter(i);
			double value_tau43 = bins->GetYaxis()->GetBinCenter(j);
			TString string_tau42 = to_string((int) round(value_tau42*1000));
			TString string_tau43 = to_string((int) round(value_tau43*1000));
			TString suffix = "_42tau" + string_tau42 + "_43tau" + string_tau43;
			
			TH1* h_qcdmg = (TH1*) tf_in->Get("qcdmg" + suffix);
			TH1* h_ttbar = (TH1*) tf_in->Get("ttbar" + suffix);
			for (int k = 0; k < ms.size(); ++ k) {
				int m = ms[k];
				TH1* h_s = (TH1*) tf_in->Get("sq" + to_string(m) + "to4j" + suffix);
				vector<double> significances = get_significances(m, h_s, h_qcdmg, h_ttbar, 50);
				cout << value_tau42 << "   " << value_tau43 << "   " << significances[0] << endl;
				for (int kind = 1; kind < 5; ++ kind) {
					TString name_matrix = "sq" + to_string(m) + "to4j_s" + to_string(kind);
					matrices[name_matrix]->SetBinContent(i, j, significances[kind - 1]);
				}
			}
		}
	}
	
	// Write out "matrix" histograms:
	tf_out->cd();
	for (int i = 0; i < ms.size(); ++ i) {
		for (int kind = 1; kind < 5; ++ kind) {
			TString name_matrix = "sq" + to_string(ms[i]) + "to4j_s" + to_string(kind);
			matrices[name_matrix]->Write();
		}
	}
//	
//	cout << "[..] Making plots for significance measure 1." << endl;
//	make_significance_matrix(100, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 1);
//	make_significance_matrix("sq150to4j_qcdmg_s1", 150, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 1);
//	make_significance_matrix("sq300to4j_qcdmg_s1", 300, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 1);
//	make_significance_matrix("sq400to4j_qcdmg_s1", 400, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 1);
//	
//		cout << "[..] Making plots for significance measure 2." << endl;
//	make_significance_matrix("sq100to4j_qcdmg_s2", 100, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 2);
////	make_significance_matrix("sq150to4j_qcdmg_s2", 150, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 2);
////	make_significance_matrix("sq300to4j_qcdmg_s2", 300, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 2);
//	make_significance_matrix("sq400to4j_qcdmg_s2", 400, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 2);
//////	
//	cout << "[..] Making plots for significance measure 3." << endl;
//	make_significance_matrix("sq100to4j_qcdmg_s3", 100, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 3);
////	make_significance_matrix("sq150to4j_qcdmg_s3", 150, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 3);
////	make_significance_matrix("sq300to4j_qcdmg_s3", 300, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 3);
//	make_significance_matrix("sq400to4j_qcdmg_s3", 400, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 3);
//////	
//	cout << "[..] Making plots for significance measure 4 (ZBI)." << endl;
//	make_significance_matrix("sq100to4j_qcdmg_s4", 100, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 4);
////	make_significance_matrix("sq150to4j_qcdmg_s4", 150, tf_out, tt_sq150to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 4);
////	make_significance_matrix("sq300to4j_qcdmg_s4", 300, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 4);
//	make_significance_matrix("sq400to4j_qcdmg_s4", 400, tf_out, tt_sq300to4j, tt_qcdmg, tt_ttbar, tau42nbins, tau42min, tau42max, tau43nbins, tau43min, tau43max, 4);
	
	
	return;
}
