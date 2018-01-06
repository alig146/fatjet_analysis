#include <iostream>
#include <sstream>
#include <string>
#include <Deracination/Straphanger/test/decortication/macros/common.cc>

bool VERBOSE = true;

TH3* fill_fj_plot(TTree* tt, TString name, TString cut, TString era, TString groomer="p") {
	// Set up HT bins:
//	vector<double> bins_ht = {900, 1000, 2000, 3000};
//	vector<double> bins_ht = {900, 910, 920, 930, 940, 950, 970, 1000};
//	for (double i = 1050; i <= 3200; i += 50) bins_ht.push_back(i);
	vector<double> bins_ht;
	for (double i = 900; i <= 3200; i += 92) bins_ht.push_back(i);
	// Set up m bins:
	vector<double> bins_m;
	for (double i = 0; i <= 1200; i += 1) bins_m.push_back(i);
	// Set up eta bins:
	vector<double> bins_eta;
//	for (double i = -2.0; i < 2.1; i += 0.1) bins_eta.push_back(i);
	for (double i = -2.0; i < 2.1; i += 0.2) bins_eta.push_back(i);
	// Fill histogram:
	TH3D* h3 = new TH3D(name, "", bins_m.size()-1, &bins_m[0], bins_eta.size()-1, &bins_eta[0], bins_ht.size()-1, &bins_ht[0]);
	tt->Draw("htak8:eta[0]:m_" + groomer + "[0]>>" + name, get_cut("fj_" + cut, era, get_weight(tt->GetName(), era)));
	return h3;
}

TH3* make_fj_plot(TFile* tf_in, TFile* tf_out, TString ds, TString cut, TString groomer="p") {
	TString name = "fj_" + ds + "_" + cut + "_" + groomer;
	
	TString era = "";
	if (cut == "sig15") {
		cut = "sig";
		era = "15";
	}
	
	vector<TTree*> tts;
	if (ds == "inj") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq150to4j"));
	}
	else if (ds == "inj100") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq100to4j"));
	}
	else if (ds == "inj150") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq150to4j"));
	}
	else if (ds == "inj200") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq200to4j"));
	}
	else if (ds == "inj250") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq250to4j"));
	}
	else if (ds == "inj300") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq300to4j"));
	}
	else if (ds == "inj400") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq400to4j"));
	}
	else if (ds == "inj500") {
		tts.push_back((TTree*) tf_in->Get("jetht"));
		tts.push_back((TTree*) tf_in->Get("sq500to4j"));
	}
	else if (ds == "all") {
		tts.push_back((TTree*) tf_in->Get("qcdmg"));
		tts.push_back((TTree*) tf_in->Get("ttbar"));
		tts.push_back((TTree*) tf_in->Get("sq150to4j"));
	}
	else tts.push_back((TTree*) tf_in->Get(ds));
	
	TH3* h = fill_fj_plot(tts[0], name, cut, era, groomer);
	if (ds == "all" || ds == "inj") {
		for (unsigned i = 1; i < tts.size() ; ++i) {
			TH3* h_temp = fill_fj_plot(tts[i], "h", cut, era, groomer);
			h->Add(h_temp);
		}
	}
	// Write out plot:
	tf_out->WriteTObject(h);
	return h;
}

TH1* make_temp_plot(TFile* tf_in, TFile* tf_out, TString ds, TString cut, TString dir="", int f=1, TString groomer="p", bool ht=true){
	TString name = "temp_" + ds + "_" + cut + "_" + groomer + "_f" + to_string(f);
	if (dir != "") name = name + "_" + dir;
	if (!ht) name = name + "_xht";
	cout << "[..] Making template for " << name << "." << endl;
	
	cout << "[..] Making single jet distribution for " << ds << " with " << cut << "." << endl;
	TH3* h = make_fj_plot(tf_in, tf_out, ds, cut, groomer);
	cout << "[OK] Made the single jet distribution." << endl;
//	cout << "[OK] The single jet distribution has the following binning:" << endl;
//	cout << "HT: " << h->GetNbinsZ() << " bins." << endl;
//	cout << "eta: " << h->GetNbinsY() << " bins." << endl;
//	cout << "m: " << h->GetNbinsX() << " bins." << endl;
	cout << "[..] Constructing template." << endl;
	if (ds == "inj" || ds == "inj100" || ds == "inj150" || ds == "inj200" || ds == "inj250" || ds == "inj300" || ds == "inj400" || ds == "inj500") ds = "jetht";
	TH1* temp = make_template(tf_out, name, h, ds, cut, dir, f, 0.1, 1.0, ht);
//	cout << "here" << endl;
	temp->SetName(name);
	cout << "[OK] Template constructed." << endl;
	return temp;
}

void template_plotter(TString cut="sb") {
	// Options:
	gROOT->SetBatch();
	
	// Define input and output:
	TFile *tf_in;
//	tf_in = TFile::Open("~/anatuples/anatuple_dalitz_predeta.root");
	tf_in = get_ana();
	tf_in_ext = get_ana("qcdmgext");
	TFile* tf_out = new TFile("template_plots_" + cut + ".root", "RECREATE");
//	vector<TString> dss = {"jetht", "qcdmg", "qcdp", "inj"};
	vector<TString> dss = {"jetht", "inj100", "inj150", "inj200", "inj250", "inj300", "inj400", "inj500"};
//	vector<TString> dss = {"inj"};
	
	// Make plots:
	for (unsigned i = 0; i < dss.size(); ++i) {
		make_temp_plot(tf_in, tf_out, dss[i], cut);
//		if (dss[i] != "inj") make_temp_plot(tf_in, tf_out, dss[i], cut, "", 1, "p", false);
	}
//	make_temp_plot(tf_in_ext, tf_out, "qcdmg", cut);
//	make_temp_plot(tf_in_ext, tf_out, "qcdmg", cut, "", 1, "p", false);
}


