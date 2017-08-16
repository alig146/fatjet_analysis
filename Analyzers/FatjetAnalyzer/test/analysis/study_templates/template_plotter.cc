#include <iostream>
#include <sstream>
#include <string>
#include "/home/tote/decortication/macros/common.cc"

bool VERBOSE = true;

TH3* make_fj_plot(TFile* tf_in, TFile* tf_out, TString ds, TString cut, TString groomer="p") {
	TString name = "fj_" + ds + "_" + cut + "_" + groomer;
	TTree *tt = (TTree*) tf_in->Get(ds);
	tt->Draw("htak8:eta[0]:m_" + groomer + "[0]>>" + name + "(1200,0,1200,20,-2.5,2.5,25,900,3200)", get_cut("fj_" + cut, get_weight(ds)));
	TH3* h = (TH3*) gDirectory->Get(name);
	
	// Write out plot:
	tf_out->cd();
	h->Write();
	
	return h;
}

TH1* make_temp_plot(TFile* tf_in, TFile* tf_out, TString ds, TString cut, TString dir="", int f=0, TString groomer="p", bool ht=true){
	TString name = "temp_" + ds + "_" + cut + "_" + groomer + "_f" + to_string(f);
	if (dir != "") name = name + "_" + dir;
	if (!ht) name = name + "_xht";
	cout << "[..] Making template for " << name << "." << endl;
	
	TH3* h = make_fj_plot(tf_in, tf_out, ds, cut, groomer);
	TH1* temp = make_template(h, ds, cut, dir, f, 0.1, 1.0, ht);
	temp->SetName(name);
	
	// Write out plot:
	tf_out->cd();
	temp->Write();
	
	return temp;
}

void template_plotter() {
	// Options:
	gROOT->SetBatch();
	
	// Define input and output:
	TFile *tf_in;
//	tf_in = TFile::Open("~/anatuples/anatuple_dalitz_predeta.root");
	tf_in = get_ana();
	TFile* tf_out = new TFile("template_plots.root", "RECREATE");
	
	// Make plots:

//	make_temp_plot(tf_in, tf_out, "jetht", "sbide", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbide", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbide", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbide", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbide", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbide", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sbide", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbide", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbide", "", 1);
	
//	make_temp_plot(tf_in, tf_out, "jetht", "sbideb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbideb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbideb", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbideb", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbideb", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbideb", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sbideb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbideb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbideb", "", 1);
	
//	make_temp_plot(tf_in, tf_out, "jetht", "sig15", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sig", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sig", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sig", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sig15", "");
//	make_temp_plot(tf_in, tf_out, "jetht", "sig", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sig", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sig", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sig15", "", 1);
	make_temp_plot(tf_in, tf_out, "jetht", "sig", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sig", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sig", "", 1);

//	make_temp_plot(tf_in, tf_out, "jetht", "sigl", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sigl", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sigl", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sigl", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sigl", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sigl", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sigl", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sigl", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sigl", "", 1);
//	
//	make_temp_plot(tf_in, tf_out, "jetht", "sb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sb", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sb", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sb", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sb", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sb", "", 1);
//	
//	make_temp_plot(tf_in, tf_out, "jetht", "sbb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbb", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbb", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbb", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbb", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sbb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbb", "", 1);
//	
//	make_temp_plot(tf_in, tf_out, "jetht", "sbt", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbt", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbt", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbt", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbt", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbt", "");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbt", "", 1);
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbt", "", 1);
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbt", "", 1);
//	
//	make_temp_plot(tf_in, tf_out, "jetht", "sbtb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbtb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbtb", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbtb", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbtb", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbtb", "");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbtb", "", 1);
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbtb", "", 1);
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbtb", "", 1);

//	make_temp_plot(tf_in, tf_out, "jetht", "sbl", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbl", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbl", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sbl", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sbl", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sbl", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sbl", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbl", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbl", "", 1);
//	
//	make_temp_plot(tf_in, tf_out, "jetht", "sblb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sblb", "fix");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sblb", "fix");
//	make_temp_plot(tf_in, tf_out, "jetht", "sblb", "");
//	make_temp_plot(tf_in, tf_out, "qcdmg", "sblb", "");
//	make_temp_plot(tf_in, tf_out, "qcdp", "sblb", "");
	make_temp_plot(tf_in, tf_out, "jetht", "sblb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sblb", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sblb", "", 1);

	make_temp_plot(tf_in, tf_out, "jetht", "sbl42", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbl42", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbl42", "", 1);
	make_temp_plot(tf_in, tf_out, "jetht", "sbl42b", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbl42b", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbl42b", "", 1);

	make_temp_plot(tf_in, tf_out, "jetht", "sbl43", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbl43", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbl43", "", 1);
	make_temp_plot(tf_in, tf_out, "jetht", "sbl43b", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdmg", "sbl43b", "", 1);
	make_temp_plot(tf_in, tf_out, "qcdp", "sbl43b", "", 1);


////	make_temp_plot(tf_in, tf_out, "jetht", "sbls");
////	make_temp_plot(tf_in, tf_out, "qcdmg", "sbls");
////	make_temp_plot(tf_in, tf_out, "qcdp", "sbls");
////	
////	make_temp_plot(tf_in, tf_out, "jetht", "sblsb");
////	make_temp_plot(tf_in, tf_out, "qcdmg", "sblsb");
////	make_temp_plot(tf_in, tf_out, "qcdp", "sblsb");
////	
////	make_temp_plot(tf_in, tf_out, "jetht", "sigs");
////	make_temp_plot(tf_in, tf_out, "qcdmg", "sigs");
////	make_temp_plot(tf_in, tf_out, "qcdp", "sigs");
////	
////	make_temp_plot(tf_in, tf_out, "jetht", "sbs");
////	make_temp_plot(tf_in, tf_out, "qcdmg", "sbs");
////	make_temp_plot(tf_in, tf_out, "qcdp", "sbs");
////	
////	make_temp_plot(tf_in, tf_out, "jetht", "sbsb");
////	make_temp_plot(tf_in, tf_out, "qcdmg", "sbsb");
////	make_temp_plot(tf_in, tf_out, "qcdp", "sbsb");
}
