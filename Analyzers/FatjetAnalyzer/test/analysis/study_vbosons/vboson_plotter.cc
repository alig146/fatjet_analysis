#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void vboson_plotter() {
	vector<TString> cuts = {"sig", "sb", "sbb"};
	vector<TString> names = {"wwto4q", "wwto2l2q", "zzto4q", "zzto2l2q", "zjets", "wjets"};
	gROOT->SetBatch();
	
	TFile* tf_in = get_ana("bosons");
	TFile* tf_out = new TFile("vboson_plots.root", "recreate");
	
	for(unsigned iname = 0; iname < names.size(); ++iname) {
		TString name = names[iname];
		TTree* tt = (TTree*) tf_in->Get(name);
		for(unsigned icut = 0; icut < cuts.size(); ++icut) {
			TString cut = cuts[icut];
			cout << tt->GetName() << " " << cut << endl;
			TString hname = name + "_" + cut;
			tt->Draw("mavg_p>>" + hname + "(1200, 0, 1200)", get_cut("fjp_" + cut));
			TH1* h = (TH1*) gDirectory->Get(hname);
			tf_out->WriteTObject(h);
		}
	}
}



