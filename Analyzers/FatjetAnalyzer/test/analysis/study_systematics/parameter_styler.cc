#include <Deracination/Straphanger/test/decortication/macros/common.cc>

void parameter_styler() {
	TFile* tf_in = TFile::Open("parameter_plots.root");
	vector<TString> names_par = {"", "", "QCD shift #pm 1#sigma", "QCD stretch #pm 1#sigma", "t#bar{t} rate #pm 1#sigma", "t#bar{t} shift #pm 1#sigma", "t#bar{t} stretch #pm 1#sigma"};
	vector<int> ips = {2, 3, 4, 5, 6};
	for (int iip = 0; iip < ips.size(); ++ iip) {
		int ip = ips[iip];
		for (int b = 0; b < 2; ++ b) {
			TString name = "b" + to_string(b) + "_p" + to_string(ip);
			int corner = 0;
			if (ip == 3 && b == 0) corner = 0;
			if (ip == 3 && b == 1) corner = 0;
			if (ip == 4 && b == 1) corner = 3;
			if (ip == 5 && b == 0) corner = 1;
			if (ip == 5 && b == 0) corner = 3;
			if (ip == 5 && b == 1) corner = 3;
			if (ip == 6 && b == 0) corner = 3;
			if (ip == 6 && b == 1) corner = 3;
			TCanvas* tc = new TCanvas(name, name);
			TH1* plot = (TH1*) tf_in->Get(name);
			
			gStyle->SetOptStat(0);
			if (b < 1) plot->GetXaxis()->SetTitle("Control region");
			else plot->GetXaxis()->SetTitle("b-tagged control region");
			plot->GetXaxis()->SetTitleOffset(2.00);
			plot->GetXaxis()->CenterTitle();
			if (ip == 2 || ip == 5) plot->GetYaxis()->SetTitle("Parameter value [GeV]");
			else plot->GetYaxis()->SetTitle("Parameter value");
			plot->Draw("e");
			style_info(false, "38.2", corner);
			if (ip == 5 && b == 0) plot->SetMaximum(8.0);
			
			TLegend* leg;
			if (corner == 0) leg = new TLegend(0.20, 0.75, 0.45, 0.80);
			else if (corner == 1) leg = new TLegend(0.50, 0.75, 0.80, 0.80);
			else if (corner == 3) leg = new TLegend(0.20, 0.45, 0.45, 0.50);
			leg->AddEntry(plot, names_par[ip], "lpe");
			leg->Draw();
			
			tc->SaveAs(name + ".pdf");
		}
	}
}
