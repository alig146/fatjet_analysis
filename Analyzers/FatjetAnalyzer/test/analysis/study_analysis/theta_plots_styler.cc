#include <iostream>
#include <sstream>
#include <string>
#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void style_plot(TH1* h) {
	h->SetTitle("");
	h->GetXaxis()->SetNdivisions(405);
	h->GetXaxis()->SetTitle("#bar{#it{m}}_{pruned} (GeV)");
	style_ylabel_th1(h);
}

void make_triplet(TFile* tf, TString name, int first_param, bool signal=false, bool mc=true) {
	cout << "[..] Making triplet for " << name << endl;
	TString fullname = "mAvgPSel__" + name;
	TH1F* h = (TH1F*) tf->Get(fullname);
	TH1F* h_p = (TH1F*) tf->Get("params");
	style_plot(h);
	TString pname = name;
	if (signal) pname = "TTbar";
	TH1F* h_shift_plus = (TH1F*) tf->Get(fullname + "__shift" + pname + "__plus");
	style_plot(h_shift_plus);
	TH1F* h_shift_minus = (TH1F*) tf->Get(fullname + "__shift" + pname + "__minus");
	style_plot(h_shift_minus);
	TH1F* h_stretch_plus = (TH1F*) tf->Get(fullname + "__stretch" + pname + "__plus");
	style_plot(h_stretch_plus);
	TH1F* h_stretch_minus = (TH1F*) tf->Get(fullname + "__stretch" + pname + "__minus");
	style_plot(h_stretch_minus);
	TCanvas* tc = new TCanvas(name, name);
	tc->SetCanvasSize(2100, 1400);
	tc->SetWindowSize(2150, 1450);
	tc->Divide(3,2);
	tc->cd(1);
	h_shift_minus->Draw();
	style_info(mc);
	double shift = h_p->GetBinContent(first_param + 1);
	double shifte = h_p->GetBinError(first_param + 1);
	double stretch = h_p->GetBinContent(first_param + 2);
	double stretche = h_p->GetBinError(first_param + 2);
//	if (name == "QCD") {
//		shifte = abs(shift);
//		shift = 0.0;
//		stretche = abs(1.0 - stretch);
//		stretch = 1.0;
//	}
	
	std::ostringstream oss1; oss1 << "#scale[1.1]{#splitline{shift = " << std::fixed << std::setprecision(2) << shift - shifte << "}{stretch = " << stretch << "}}";
	TLatex* txt1 = new TLatex(0.62, 0.23, oss1.str().c_str());
	txt1->SetNDC(); txt1->SetX(0.52); txt1->SetY(0.57); txt1->SetTextSize(0.04); txt1->SetTextFont(42);
	txt1->Draw();
	tc->cd(2);
	h->Draw();
	style_info(mc);
	std::ostringstream oss2; oss2 << "#scale[1.1]{#splitline{shift = " << std::fixed << std::setprecision(2) << shift << "}{stretch = " << stretch << "}}";
	TLatex* txt2 = new TLatex(0.62, 0.23, oss2.str().c_str());
	txt2->SetNDC(); txt2->SetX(0.52); txt2->SetY(0.57); txt2->SetTextSize(0.04); txt2->SetTextFont(42);
	txt2->Draw();
	tc->cd(3);
	h_shift_plus->Draw();
	style_info(mc);
	std::ostringstream oss3; oss3 << "#scale[1.1]{#splitline{shift = " << std::fixed << std::setprecision(2) << shift + shifte << "}{stretch = " << stretch << "}}";
	TLatex* txt3 = new TLatex(0.62, 0.23, oss3.str().c_str());
	txt3->SetNDC(); txt3->SetX(0.52); txt3->SetY(0.57); txt3->SetTextSize(0.04); txt3->SetTextFont(42);
	txt3->Draw();
	tc->cd(4);
	h_stretch_minus->Draw();
	style_info(mc);
	std::ostringstream oss4; oss4 << "#scale[1.1]{#splitline{shift = " << std::fixed << std::setprecision(2) << shift << "}{stretch = " << stretch - stretche << "}}";
	TLatex* txt4 = new TLatex(0.62, 0.23, oss4.str().c_str());
	txt4->SetNDC(); txt4->SetX(0.52); txt4->SetY(0.57); txt4->SetTextSize(0.04); txt4->SetTextFont(42);
	txt4->Draw();
	tc->cd(5);
	h->Draw();
	style_info(mc);
	std::ostringstream oss5; oss5 << "#scale[1.1]{#splitline{shift = " << std::fixed << std::setprecision(2) << shift << "}{stretch = " << stretch << "}}";
	TLatex* txt5 = new TLatex(0.62, 0.23, oss5.str().c_str());
	txt5->SetNDC(); txt5->SetX(0.52); txt5->SetY(0.57); txt5->SetTextSize(0.04); txt5->SetTextFont(42);
	txt5->Draw();
	tc->cd(6);
	h_stretch_plus->Draw();
	style_info(mc);
	std::ostringstream oss6; oss6 << "#scale[1.1]{#splitline{shift = " << std::fixed << std::setprecision(2) << shift << "}{stretch = " << stretch + stretche << "}}";
	TLatex* txt6 = new TLatex(0.62, 0.23, oss6.str().c_str());
	txt6->SetNDC(); txt6->SetX(0.52); txt6->SetY(0.57); txt6->SetTextSize(0.04); txt6->SetTextFont(42);
	txt6->Draw();
}

void theta_plots_styler(TString sig="sig15") {
	TFile* tf_plots = TFile::Open("theta_plots_" + sig + "_sb.root");
	gStyle->SetOptStat(1001110);		// right-to-left: "ksiourmen"
	make_triplet(tf_plots, "QCD", 1, false, false);
	make_triplet(tf_plots, "TTbar", 4);
	make_triplet(tf_plots, "Ms100", 7, true);
//	make_triplet(tf_plots, "Ms150", 10);
//	make_triplet(tf_plots, "Ms200", 13);
//	make_triplet(tf_plots, "Ms250", 16);
//	make_triplet(tf_plots, "Ms300", 19);
//	make_triplet(tf_plots, "Ms400", 21);
//	make_triplet(tf_plots, "Ms500", 24);
	
	TString name = "mAvgPSel__DATA";
	TCanvas* tc = new TCanvas(name, name);
	TH1F* h_data = (TH1F*) tf_plots->Get(name);
	style_plot(h_data);
	h_data->Draw();
	style_info(false);
}
