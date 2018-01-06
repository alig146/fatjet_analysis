#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void extension_styler() {
	tf = get_ana();
	tf_ext = get_ana("qcdmgext");
	
	TTree* qcdmg = (TTree*) tf->Get("qcdmg");
	TTree* qcdmg_ext = (TTree*) tf_ext->Get("qcdmg");
	
	qcdmg->Draw("mavg_p>>h(1200,0,1200)", get_cut("fjp_sig"));
	qcdmg_ext->Draw("mavg_p>>h_ext(1200,0,1200)", get_cut("fjp_sig"));
	
	TH1* h = (TH1*) gDirectory->Get("h");
	TH1* h_ext = (TH1*) gDirectory->Get("h_ext");
	
	h->SetFillStyle(0);
	h->Rebin(30);
	h_ext->SetFillStyle(0);
	h_ext->Rebin(30);
	h_ext->SetLineColor(kRed);
	h_ext->SetMarkerColor(kRed);
	
	h->Draw("histe");
	h_ext->Draw("histe same");
}
