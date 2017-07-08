#include "/home/tote/decortication/macros/common.cc"

void get_pileup_distributions_mc() {
	vector<TString> generations = {"moriond17", "moriond17ext", "spring16"};
	
	for (unsigned i = 0; i < generations.size(); ++i) {
		TString generation = generations[i];
		TFile* tf_out = TFile::Open(TString("pileup_distribution_") + generation + ".root", "recreate");
		TH1* h = make_pileup_histogram(generation);
		tf_out->WriteTObject(h);
	}
}
