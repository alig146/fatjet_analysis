#include "/uscms/home/tote/truculence/macros/styling_tools/styling_tools.cc"
#include "/uscms/home/tote/decortication/macros/info.cc"

void conesize_styler() {
	gROOT->SetBatch();
	gStyle->SetOptStat(0);
	
	TFile* tf = TFile::Open("conesize_plots.root");
	
	TH1* ak8 = (TH1*) tf->Get("ak8_sq200to4j_m0");
	TH1* ca12 = (TH1*) tf->Get("ca12_sq200to4j_m0");
	
	TCanvas* tc = new TCanvas("conesize_m0", "conesize_m0");
	
	ak8->SetTitle("");
	ak8->Rebin(10);
	ak8->SetFillColor(0);
	ak8->SetLineColor(kBlue);
	ak8->SetLineStyle(2);
	ca12->SetTitle("");
	ca12->Rebin(10);
	ca12->SetFillColor(0);
	ca12->SetMaximum(ca12->GetMaximum()*1.2);
	ca12->GetXaxis()->SetRangeUser(0, 600);
	ca12->GetXaxis()->SetTitle("Leading fatjet pruned mass [GeV]");
	style_ylabel(ca12);
	
	ca12->Draw("hist");
	ak8->Draw("hist same");
//	ca12->Draw("hist same");
	
	style_info(true);
	style_write(name_proper["sq200to4j"], 0.60, 0.78, 0.035);
	style_write("Selection: #bf{#it{H}_{T} > 900 GeV}",  0.18, 0.94, 0.023);
	
	TLegend* leg = new TLegend(0.59, 0.65, 0.80, 0.75);
	leg->AddEntry(ak8, "AK8 jets", "f");
	leg->AddEntry(ca12, "CA12 jets", "f");
	leg->Draw();
	
	
	tc->SaveAs(TString(tc->GetName()) + ".pdf");
	tc->SaveAs(TString(tc->GetName()) + ".png");
}

