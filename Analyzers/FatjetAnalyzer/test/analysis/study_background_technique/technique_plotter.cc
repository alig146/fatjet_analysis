#include "/home/tote/decortication/macros/common.cc"

void technique_plotter(TString cut_sig="sig") {
	gROOT->SetBatch();
	TFile* ana = get_ana();
	weight = get_weight("qcdmg");
	TFile* tf_out = new TFile(TString("technique_plots_") + cut_sig + ".root", "recreate");
	TTree* jetht = (TTree*) ana->Get("jetht");
	TTree* qcdmg = (TTree*) ana->Get("qcdmg");
	qcdmg->Draw("mavg_p>>fjp(1200,0,1200)", get_cut(TString("fjp_") + cut_sig, weight));
	TH1* fjp = (TH1*) gDirectory->Get("fjp");
	tf_out->WriteTObject(fjp);
	qcdmg->Draw("mavg_p>>fj(1200,0,1200)", get_cut(TString("fj_") + cut_sig, weight));
	TH1* fj = (TH1*) gDirectory->Get("fj");
	tf_out->WriteTObject(fj);
}
