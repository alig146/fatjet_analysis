#include "/home/tote/decortication/macros/common.cc"

void masy_plotter() {
	gROOT->SetBatch();
	
	TFile* tf_in = get_ana();
	TFile* tf_out = TFile::Open("masy_plots.root", "recreate");
	TTree* qcdp = (TTree*) tf_in->Get("qcdp");
	TTree* qcdmg = (TTree*) tf_in->Get("qcdmg");
	TTree* sq200to4j = (TTree*) tf_in->Get("sq200to4j");
	TTree* sq400to4j = (TTree*) tf_in->Get("sq400to4j");
	
	tf_out->cd();
	
	// pre
	qcdp->Draw("masy_p>>qcdp_pre(1200,0,4)", get_cut("fjp_pre"));
	TH1* qcdp_pre = (TH1*) gDirectory->Get("qcdp_pre");
	qcdp_pre->Write();
	
	qcdmg->Draw("masy_p>>qcdmg_pre(1200,0,4)", get_cut("fjp_pre"));
	TH1* qcdmg_pre = (TH1*) gDirectory->Get("qcdmg_pre");
	qcdmg_pre->Write();
	
	sq200to4j->Draw("masy_p>>sq200to4j_pre(1200,0,4)", get_cut("fjp_pre"));
	TH1* sq200to4j_pre = (TH1*) gDirectory->Get("sq200to4j_pre");
	sq200to4j_pre->Write();
	
	sq400to4j->Draw("masy_p>>sq400to4j_pre(1200,0,4)", get_cut("fjp_pre"));
	TH1* sq400to4j_pre = (TH1*) gDirectory->Get("sq400to4j_pre");
	sq400to4j_pre->Write();
	
	// deta
	qcdp->Draw("masy_p>>qcdp_deta(1200,0,4)", get_cut("fjp_deta"));
	TH1* qcdp_deta = (TH1*) gDirectory->Get("qcdp_deta");
	qcdp_deta->Write();
	
	qcdmg->Draw("masy_p>>qcdmg_deta(1200,0,4)", get_cut("fjp_deta"));
	TH1* qcdmg_deta = (TH1*) gDirectory->Get("qcdmg_deta");
	qcdmg_deta->Write();
	
	sq200to4j->Draw("masy_p>>sq200to4j_deta(1200,0,4)", get_cut("fjp_deta"));
	TH1* sq200to4j_deta = (TH1*) gDirectory->Get("sq200to4j_deta");
	sq200to4j_deta->Write();
	
	sq400to4j->Draw("masy_p>>sq400to4j_deta(1200,0,4)", get_cut("fjp_deta"));
	TH1* sq400to4j_deta = (TH1*) gDirectory->Get("sq400to4j_deta");
	sq400to4j_deta->Write();
}
