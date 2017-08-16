#include "/home/tote/decortication/macros/optimization_tools/optimization_tools.cc"

TH1F* make_plot_set(TFile* tf_in, TFile* tf_out, int tau21nbins, double tau21min, double tau21max) {
	vector<TString> names = {"qcdmg", "qcdp", "ttbar", "sq100to4j", "sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j"};
	
	// Get TTrees:
	map<TString, TTree*> tts;
	for (int i = 0; i < names.size() ; ++ i) {
		tts[names[i]] = (TTree*) tf_in->Get(names[i]);
	}
	tf_out->cd();
	
	// Define cut bins:
	TH1F* bins = new TH1F("bins", "", tau21nbins, tau21min, tau21max);
	
	// Loop over cut bins:
	for (int i = 1; i < bins->GetNbinsX() + 1; i++) {
		double value_tau21 = bins->GetXaxis()->GetBinCenter(i);
		std::ostringstream cut;
		cut << "wtt*w*(deta<1.0&&masy_p<0.1&&Max$(tau43)<0.80&&Max$(tau42)<0.45&&Max$(tau21)<" << value_tau21 << ")";
		TString suffix = "_21tau" + to_string((int) round(value_tau21*1000));
		
		for (int j = 0; j < names.size() ; ++ j) {
			TString name = names[j] + suffix;
			TCut* tcut = new TCut("tcut", cut.str().c_str());
			tts[names[j]]->Draw("mavg_p>>h(1000, 0, 1000)", *tcut);
			TH1F* h = (TH1F*) tf_out->Get("h");
			h->SetName(name);
			tf_out->WriteTObject(h);
		}
	}
	
	tf_out->WriteTObject(bins);
	return bins;
}

void optimization_tau21_plotter() {
	gROOT->SetBatch();
	
//	TFile *tf_in = TFile::Open("~/anatuples/anatuple_cutpt400eta25_predeta.root");
	TFile *tf_in = TFile::Open("~/anatuples/anatuple_cutpt400eta25_prextau.root");
	
//	TFile* tf_out = new TFile("optimization_plots_150-300.root", "RECREATE");
	TFile* tf_out = new TFile("optimization_plots_43tau800_42tau450.root", "RECREATE");
	
	gStyle->SetOptStat(0);
	int tau21nbins = 50;
	double tau21min = 0.5;
	double tau21max = 1.0;
	
	cout << "[..] Making plots for tau21 optimization studies." << endl;
	make_plot_set(tf_in, tf_out, tau21nbins, tau21min, tau21max);
	
	return;
}
