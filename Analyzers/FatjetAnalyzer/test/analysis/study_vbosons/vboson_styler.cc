#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void style_and_save(TH1* H, int nrebin=50) {
	TH1* h = (TH1*) H->Clone(TString(H->GetName()) + "temp");
	
	TCanvas* tc = new TCanvas(H->GetName(), H->GetName());
	h->Rebin(nrebin);
	h->Draw();
	h->SetTitle("");
	h->GetXaxis()->SetTitle("Average pruned mass [GeV]");
	style_ylabel(h);
	style_info(true);
	
	double Ie;
	double I = h->IntegralAndError(0, h->GetNbinsX() + 1, Ie);
	std::ostringstream oss1;
	oss1 << std::fixed << std::setprecision(1) << I << " #pm " << Ie << " events";
	style_write(oss1.str(), 0.55, 0.78);
	
	tc->SaveAs(TString(H->GetName()) + ".pdf");
	cout << H->GetName() << " = " << I << " +/- " << Ie << endl;
}

void make_stack(TH1* hwjets, TH1* hzjets, TH1* hother, TString cut, int nrebin=100) {
	TH1* sum = (TH1*) hwjets->Clone("sum_" + cut);
//	sum->Sumw2();
	sum->Add(hzjets);
	sum->Add(hother);
	sum->Rebin(nrebin);
	TH1* sume = (TH1*) sum->Clone("sume_" + cut);
	
	THStack* stack = new THStack();
	TCanvas* tc = new TCanvas(TString("stack") + sum->GetName(), TString("stack") + sum->GetName());
	
	hwjets->Rebin(nrebin);
	hzjets->Rebin(nrebin);
	hother->Rebin(nrebin);
	hwjets->SetFillStyle(1001);
	hwjets->SetFillColorAlpha(kBlue, 0.1);
	hzjets->SetFillStyle(1001);
	hzjets->SetFillColorAlpha(kRed, 0.4);
	hother->SetFillStyle(1001);
	hother->SetFillColorAlpha(kGreen-5, 0.5);
//	hwjets->SetLineColorAlpha(kBlue, 0.1);
//	hzjets->SetLineColorAlpha(kRed, 0.4);
//	hother->SetLineColorAlpha(kGreen-5, 0.5);
	sum->SetFillStyle(0);
	sume->SetMarkerSize(0);
	sume->SetFillStyle(1001);
	sume->SetFillColorAlpha(kBlack, 0.2);
	
	stack->Add(hother);
	stack->Add(hzjets);
	stack->Add(hwjets);
	
	stack->Draw("hist");
	stack->GetXaxis()->SetRangeUser(0, 900);
	sum->Draw("same hist");
	sume->Draw("same e2");
	stack->SetTitle("");
	set_xtitle(stack, "mavg");
	style_ylabel(stack);
	style_info(true);
	style_cut(cut);
	
	/// Legend:
	TLegend* leg = get_legend(1, 4);
	leg->AddEntry(hwjets, "W^{#pm}+jets", "f");
	leg->AddEntry(hzjets, "Z+jets", "f");
	leg->AddEntry(hother, "Diboson", "f");
	leg->AddEntry(sume, "Total", "lf");
	leg->Draw();
	
	double Ie;
	double I = sum->IntegralAndError(0, sum->GetNbinsX() + 1, Ie);
	std::ostringstream oss1;
	oss1 << std::fixed << std::setprecision(1) << I << " #pm " << Ie << " events";
	style_write(oss1.str(), 0.55, 0.58);
	
	for (int ilog = 0; ilog < 2; ++ilog) {
		if (cut == "sbb") stack->SetMaximum(stack->GetMaximum()*2.0);
		else stack->SetMaximum(stack->GetMaximum()*1.4);
		stack->GetXaxis()->SetNdivisions(406);
		tc->SetLogy(ilog);
		TString fname = tc->GetName();
		if (ilog == 1) fname += "_logy";
		tc->SaveAs(fname + ".pdf");
	}
//	style_and_save(sum, 1);
}

void vboson_styler() {
	vector<TString> cuts = {"sig", "sb", "sbb"};
	vector<TString> names = {"wwto4q", "wwto2l2q", "zzto4q", "zzto2l2q", "zjets", "wjets"};
	map<TString, TH1*> hs;
	gROOT->SetBatch();
	gStyle->SetOptStat(0);
	
	TFile* tf_in = TFile::Open("vboson_plots.root");
	
	for(unsigned icut = 0; icut < cuts.size(); ++icut) {
		TString cut = cuts[icut];
		cout << cut << endl;
		for(unsigned iname = 0; iname < names.size(); ++iname) {
			TString name = names[iname];
			TString hname = name + "_" + cut;
			TH1* h = (TH1*) tf_in->Get(name + "_" + cut);
			hs[hname] = h;
			style_and_save(h);
		}
		
//		TH1* sum = (TH1*) hs[names[0] + "_" + cut]->Clone("sum_" + cut);
		TH1* sum_other = (TH1*) hs[names[0] + "_" + cut]->Clone("sum_other_" + cut);
		for(unsigned iname = 0; iname < names.size(); ++iname) {
			TString name = names[iname];
			if (name != "wjets" && name != "zjets") sum_other->Add(hs[name + "_" + cut]);
		}
//		style_and_save(sum);
		make_stack(hs["wjets_" + cut], hs["zjets_" + cut], sum_other, cut);
		cout << endl;
	}
	
}
