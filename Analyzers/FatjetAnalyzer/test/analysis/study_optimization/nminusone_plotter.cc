#include "/home/tote/decortication/macros/common.cc"

vector<TString> nminusone = {"sigxdeta", "sigxmasyp", "sigxtau21", "sigxtau42", "sigxtau43", "sigxtau4", "sigxtau"};

void nminusone_plotter() {
	gROOT->SetBatch();
	
	TFile* tf_in = get_ana();
	TFile* tf_out = TFile::Open("nminusone_plots.root", "recreate");
	TTree* qcdp = (TTree*) tf_in->Get("qcdp");
	TTree* qcdmg = (TTree*) tf_in->Get("qcdmg");
	TTree* sq200to4j = (TTree*) tf_in->Get("sq200to4j");
	TTree* sq400to4j = (TTree*) tf_in->Get("sq400to4j");
	
	tf_out->cd();
	
//	for (int i = 0; i < nminusone.size(); ++i) {
	
	// deta:
	qcdp->Draw("deta>>qcdp_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* qcdp_sigxdeta = (TH1*) gDirectory->Get("qcdp_sigxdeta");
	qcdp_sigxdeta->Write();
	
	qcdmg->Draw("deta>>qcdmg_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* qcdmg_sigxdeta = (TH1*) gDirectory->Get("qcdmg_sigxdeta");
	qcdmg_sigxdeta->Write();
	
	sq200to4j->Draw("deta>>sq200to4j_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* sq200to4j_sigxdeta = (TH1*) gDirectory->Get("sq200to4j_sigxdeta");
	sq200to4j_sigxdeta->Write();
	
	sq400to4j->Draw("deta>>sq400to4j_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* sq400to4j_sigxdeta = (TH1*) gDirectory->Get("sq400to4j_sigxdeta");
	sq400to4j_sigxdeta->Write();
	
	// masy_p:
	qcdp->Draw("masy_p>>qcdp_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* qcdp_sigxmasyp = (TH1*) gDirectory->Get("qcdp_sigxmasyp");
	qcdp_sigxmasyp->Write();
	
	qcdmg->Draw("masy_p>>qcdmg_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* qcdmg_sigxmasyp = (TH1*) gDirectory->Get("qcdmg_sigxmasyp");
	qcdmg_sigxmasyp->Write();
	
	sq200to4j->Draw("masy_p>>sq200to4j_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* sq200to4j_sigxmasyp = (TH1*) gDirectory->Get("sq200to4j_sigxmasyp");
	sq200to4j_sigxmasyp->Write();
	
	sq400to4j->Draw("masy_p>>sq400to4j_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* sq400to4j_sigxmasyp = (TH1*) gDirectory->Get("sq400to4j_sigxmasyp");
	sq400to4j_sigxmasyp->Write();
	
	// tau21:
	qcdp->Draw("Max$(tau21)>>qcdp_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* qcdp_sigxtau21 = (TH1*) gDirectory->Get("qcdp_sigxtau21");
	qcdp_sigxtau21->Write();
	
	qcdmg->Draw("Max$(tau21)>>qcdmg_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* qcdmg_sigxtau21 = (TH1*) gDirectory->Get("qcdmg_sigxtau21");
	qcdmg_sigxtau21->Write();
	
	sq200to4j->Draw("Max$(tau21)>>sq200to4j_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* sq200to4j_sigxtau21 = (TH1*) gDirectory->Get("sq200to4j_sigxtau21");
	sq200to4j_sigxtau21->Write();
	
	sq400to4j->Draw("Max$(tau21)>>sq400to4j_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* sq400to4j_sigxtau21 = (TH1*) gDirectory->Get("sq400to4j_sigxtau21");
	sq400to4j_sigxtau21->Write();
	
	// tau42:
	qcdp->Draw("Max$(tau42)>>qcdp_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* qcdp_sigxtau42 = (TH1*) gDirectory->Get("qcdp_sigxtau42");
	qcdp_sigxtau42->Write();
	
	qcdmg->Draw("Max$(tau42)>>qcdmg_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* qcdmg_sigxtau42 = (TH1*) gDirectory->Get("qcdmg_sigxtau42");
	qcdmg_sigxtau42->Write();
	
	sq200to4j->Draw("Max$(tau42)>>sq200to4j_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* sq200to4j_sigxtau42 = (TH1*) gDirectory->Get("sq200to4j_sigxtau42");
	sq200to4j_sigxtau42->Write();
	
	sq400to4j->Draw("Max$(tau42)>>sq400to4j_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* sq400to4j_sigxtau42 = (TH1*) gDirectory->Get("sq400to4j_sigxtau42");
	sq400to4j_sigxtau42->Write();
	
	// tau43:
	qcdp->Draw("Max$(tau43)>>qcdp_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* qcdp_sigxtau43 = (TH1*) gDirectory->Get("qcdp_sigxtau43");
	qcdp_sigxtau43->Write();
	
	qcdmg->Draw("Max$(tau43)>>qcdmg_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* qcdmg_sigxtau43 = (TH1*) gDirectory->Get("qcdmg_sigxtau43");
	qcdmg_sigxtau43->Write();
	
	sq200to4j->Draw("Max$(tau43)>>sq200to4j_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* sq200to4j_sigxtau43 = (TH1*) gDirectory->Get("sq200to4j_sigxtau43");
	sq200to4j_sigxtau43->Write();
	
	sq400to4j->Draw("Max$(tau43)>>sq400to4j_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* sq400to4j_sigxtau43 = (TH1*) gDirectory->Get("sq400to4j_sigxtau43");
	sq400to4j_sigxtau43->Write();
}
