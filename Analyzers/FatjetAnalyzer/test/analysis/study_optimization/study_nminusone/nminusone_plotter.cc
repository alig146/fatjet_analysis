#include <Deracination/Straphanger/test/decortication/macros/common.cc>

vector<TString> nminusone = {"sigxdeta", "sigxmasyp", "sigxtau21", "sigxtau42", "sigxtau43", "sigxtau4", "sigxtau"};

void nminusone_plotter() {
	gROOT->SetBatch();
	
	TFile* tf_in = get_ana();
	TFile* tf_in_qcdp = get_ana("qcdp");
	
	TFile* tf_out = TFile::Open("nminusone_plots.root", "recreate");
	TTree* qcdp = (TTree*) tf_in_qcdp->Get("qcdp");
	TTree* qcdmg = (TTree*) tf_in->Get("qcdmg");
	TTree* ttbar = (TTree*) tf_in->Get("ttbar");
	TTree* sq200to4j = (TTree*) tf_in->Get("sq200to4j");
	TTree* sq400to4j = (TTree*) tf_in->Get("sq400to4j");
	
	tf_out->cd();
	
//	for (int i = 0; i < nminusone.size(); ++i) {
	
	// deta:
	qcdp->Draw("deta>>deta_qcdp_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* qcdp_sigxdeta = (TH1*) gDirectory->Get("deta_qcdp_sigxdeta");
	qcdp_sigxdeta->Write();
	
	qcdmg->Draw("deta>>deta_qcdmg_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* qcdmg_sigxdeta = (TH1*) gDirectory->Get("deta_qcdmg_sigxdeta");
	qcdmg_sigxdeta->Write();
	
	ttbar->Draw("deta>>deta_ttbar_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta", "wtt"));
	TH1* ttbar_sigxdeta = (TH1*) gDirectory->Get("deta_ttbar_sigxdeta");
	ttbar_sigxdeta->Write();
	
	sq200to4j->Draw("deta>>deta_sq200to4j_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* sq200to4j_sigxdeta = (TH1*) gDirectory->Get("deta_sq200to4j_sigxdeta");
	sq200to4j_sigxdeta->Write();
	
	sq400to4j->Draw("deta>>deta_sq400to4j_sigxdeta(1200,0,4)", get_cut("fjp_sigxdeta"));
	TH1* sq400to4j_sigxdeta = (TH1*) gDirectory->Get("deta_sq400to4j_sigxdeta");
	sq400to4j_sigxdeta->Write();
	
	// masy_p:
	qcdp->Draw("masy_p>>masyp_qcdp_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* qcdp_sigxmasyp = (TH1*) gDirectory->Get("masyp_qcdp_sigxmasyp");
	qcdp_sigxmasyp->Write();
	
	qcdmg->Draw("masy_p>>masyp_qcdmg_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* qcdmg_sigxmasyp = (TH1*) gDirectory->Get("masyp_qcdmg_sigxmasyp");
	qcdmg_sigxmasyp->Write();
	
	ttbar->Draw("masy_p>>masyp_ttbar_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp", "wtt"));
	TH1* ttbar_sigxmasyp = (TH1*) gDirectory->Get("masyp_ttbar_sigxmasyp");
	ttbar_sigxmasyp->Write();
	
	sq200to4j->Draw("masy_p>>masyp_sq200to4j_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* sq200to4j_sigxmasyp = (TH1*) gDirectory->Get("masyp_sq200to4j_sigxmasyp");
	sq200to4j_sigxmasyp->Write();
	
	sq400to4j->Draw("masy_p>>masyp_sq400to4j_sigxmasyp(1200,0,1)", get_cut("fjp_sigxmasyp"));
	TH1* sq400to4j_sigxmasyp = (TH1*) gDirectory->Get("masyp_sq400to4j_sigxmasyp");
	sq400to4j_sigxmasyp->Write();
	
	// tau21:
	qcdp->Draw("Max$(tau21)>>tau21_qcdp_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* qcdp_sigxtau21 = (TH1*) gDirectory->Get("tau21_qcdp_sigxtau21");
	qcdp_sigxtau21->Write();
	
	qcdmg->Draw("Max$(tau21)>>tau21_qcdmg_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* qcdmg_sigxtau21 = (TH1*) gDirectory->Get("tau21_qcdmg_sigxtau21");
	qcdmg_sigxtau21->Write();
	
	ttbar->Draw("Max$(tau21)>>tau21_ttbar_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21", "wtt"));
	TH1* ttbar_sigxtau21 = (TH1*) gDirectory->Get("tau21_ttbar_sigxtau21");
	ttbar_sigxtau21->Write();
	
	sq200to4j->Draw("Max$(tau21)>>tau21_sq200to4j_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* sq200to4j_sigxtau21 = (TH1*) gDirectory->Get("tau21_sq200to4j_sigxtau21");
	sq200to4j_sigxtau21->Write();
	
	sq400to4j->Draw("Max$(tau21)>>tau21_sq400to4j_sigxtau21(1200,0,1)", get_cut("fjp_sigxtau21"));
	TH1* sq400to4j_sigxtau21 = (TH1*) gDirectory->Get("tau21_sq400to4j_sigxtau21");
	sq400to4j_sigxtau21->Write();
	
	// tau42:
	qcdp->Draw("Max$(tau42)>>tau42_qcdp_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* qcdp_sigxtau42 = (TH1*) gDirectory->Get("tau42_qcdp_sigxtau42");
	qcdp_sigxtau42->Write();
	
	qcdmg->Draw("Max$(tau42)>>tau42_qcdmg_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* qcdmg_sigxtau42 = (TH1*) gDirectory->Get("tau42_qcdmg_sigxtau42");
	qcdmg_sigxtau42->Write();
	
	ttbar->Draw("Max$(tau42)>>tau42_ttbar_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42", "wtt"));
	TH1* ttbar_sigxtau42 = (TH1*) gDirectory->Get("tau42_ttbar_sigxtau42");
	ttbar_sigxtau42->Write();
	
	sq200to4j->Draw("Max$(tau42)>>tau42_sq200to4j_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* sq200to4j_sigxtau42 = (TH1*) gDirectory->Get("tau42_sq200to4j_sigxtau42");
	sq200to4j_sigxtau42->Write();
	
	sq400to4j->Draw("Max$(tau42)>>tau42_sq400to4j_sigxtau42(1200,0,1)", get_cut("fjp_sigxtau42"));
	TH1* sq400to4j_sigxtau42 = (TH1*) gDirectory->Get("tau42_sq400to4j_sigxtau42");
	sq400to4j_sigxtau42->Write();
	
	// tau43:
	qcdp->Draw("Max$(tau43)>>tau43_qcdp_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* qcdp_sigxtau43 = (TH1*) gDirectory->Get("tau43_qcdp_sigxtau43");
	qcdp_sigxtau43->Write();
	
	qcdmg->Draw("Max$(tau43)>>tau43_qcdmg_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* qcdmg_sigxtau43 = (TH1*) gDirectory->Get("tau43_qcdmg_sigxtau43");
	qcdmg_sigxtau43->Write();
	
	ttbar->Draw("Max$(tau43)>>tau43_ttbar_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43", "wtt"));
	TH1* ttbar_sigxtau43 = (TH1*) gDirectory->Get("tau43_ttbar_sigxtau43");
	ttbar_sigxtau43->Write();
	
	sq200to4j->Draw("Max$(tau43)>>tau43_sq200to4j_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* sq200to4j_sigxtau43 = (TH1*) gDirectory->Get("tau43_sq200to4j_sigxtau43");
	sq200to4j_sigxtau43->Write();
	
	sq400to4j->Draw("Max$(tau43)>>tau43_sq400to4j_sigxtau43(1200,0,1)", get_cut("fjp_sigxtau43"));
	TH1* sq400to4j_sigxtau43 = (TH1*) gDirectory->Get("tau43_sq400to4j_sigxtau43");
	sq400to4j_sigxtau43->Write();
}
