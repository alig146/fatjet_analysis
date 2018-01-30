#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void plot(TFile* tf_out, TTree* tt) {
	vector<TString> cuts = {"sig", "sb", "sbb"};
	TString name = tt->GetName();
	for(unsigned icut = 0; icut < cuts.size(); ++icut) {
		TString cut = cuts[icut];
		cout << tt->GetName() << " " << cut << endl;
		TString hname = name + "_" + cut;
		tt->Draw("mavg_p>>" + hname + "(1200, 0, 1200)", get_cut("fjp_" + cut));
		TH1* h = (TH1*) gDirectory->Get(hname);
		tf_out->WriteTObject(h);
	}
}

void vboson_plotter() {
//	vector<TString> names = {"wwto4q", "wwto2l2q", "zzto4q", "zzto2l2q", "zjets", "wjets"};
	gROOT->SetBatch();
	
	TFile* tf_in_wz = get_ana("wzjets");
	TFile* tf_in_other = get_ana("bosons");
	TFile* tf_out = new TFile("vboson_plots.root", "recreate");
	
	plot(tf_out, (TTree*) tf_in_wz->Get("wjets"));
	plot(tf_out, (TTree*) tf_in_wz->Get("zjets"));
	plot(tf_out, (TTree*) tf_in_other->Get("wwto4q"));
	plot(tf_out, (TTree*) tf_in_other->Get("wwto2l2q"));
	plot(tf_out, (TTree*) tf_in_other->Get("zzto4q"));
	plot(tf_out, (TTree*) tf_in_other->Get("zzto2l2q"));
}



