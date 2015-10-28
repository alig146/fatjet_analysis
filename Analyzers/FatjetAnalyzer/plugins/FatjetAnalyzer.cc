/*#######################################################
# CMSSW EDAnalyzer                                      #
# Name: FatjetAnalyzer.cc                               #
# Author: Elliot Hughes                                 #
#                                                       #
# Description: To analyze fatjets or B2G ntuples:       #
# select jets, create ntuples.                          #
#######################################################*/

// INCLUDES:
// System includes
#include <iostream>
#include <typeinfo>
#include <cmath>

/// User includes:
//// Basic includes:
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/FileBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

//// Important class includes:
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//// Meta includes:
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//// ROOT includes:
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
//#include "TFileDirectory.h"
#include "TTree.h"
#include "TBranch.h"
#include "TNtuple.h"
// \INCLUDES

// NAMESPACES:
using namespace std;
using namespace reco;
using namespace edm;
// \NAMESPACES

// STRUCTURES:
//struct sort_by_m {
//	bool operator() (PseudoJet jet1, PseudoJet jet2) {
//		return (jet1.m() > jet2.m());
//	}
//};
// \STRUCTURES

// CLASS DEFINITIONS:
class FatjetAnalyzer : public edm::EDAnalyzer {
	public:
		explicit FatjetAnalyzer(const edm::ParameterSet&);		// Set the class argument to be (a reference to) a parameter set (?)
		~FatjetAnalyzer();		// Create the destructor.
		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	private:
		virtual void beginJob();
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void endJob();
		virtual void beginRun(const edm::Run&, const edm::EventSetup&);
		virtual void endRun(const edm::Run&, const edm::EventSetup&);
		virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
		virtual void endLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
		virtual void respondToOpenInputFile(const edm::FileBlock&);

	// Member data
	//// Configuration variables (filled by setting the python configuration file)
	int in_type_;		// Input type (0: B2G, 1: fatjets)
	bool v_;		// Verbose control
	double cut_pt_, cut_m_, sigma_, nevents_;
	double R_;		// Fatjet distance parameter
	bool make_gen_, make_pf_;		// Controls to make gen fatjets or pf fatjets
	bool do_true_;		// Control to look only at squark products (DOESN'T WORK)
	bool do_prune_, do_trim_;		// Controls for fatjet transformers
	double trim_R_, trim_ptf_;		// Trimmer transformer parameters
	// Basic fatjet variables
//	vector<string> jet_strings;
	vector<string> fat_vars;
//	map<string, vector<double>> fat_gen;		// This is a container for the branches I eventually save as gen output.
//	map<string, vector<double>> fat_pf;
	int n_selected_pf, n_selected_pf_untouched, n_selected_gn;
//	double b_pt_hat;
//	vector<double> b_phi_gn, b_eta_gn, b_y_gn, b_pt_gn, b_m_gn;
//	vector<double> b_phi_pf, b_eta_pf, b_y_pf, b_pt_pf, b_m_pf;
//	vector<double> b_phi_pf_untouched, b_eta_pf_untouched, b_y_pf_untouched, b_pt_pf_untouched, b_m_pf_untouched;
//	vector<int> b_n_gn, b_n_pf, b_n_pf_untouched;
	// Algorithm variables
	vector<string> kinds;
	int sq_close, first_squark;
	int n_event, n_event_sel, n_sel_lead, counter, n_error_g, n_error_q, n_error_sq, n_error_sq_match, n_error_m, n_error_sort;
//	map<string, JetDefinition> jet_defs;
	vector<int> outliers;
	
	// Input collections:
	map<string, vector<string>> collections;
	
	// Ntuple information:
	vector<string> variables, variables_all, variables_total;
	map<string, map<string, vector<double>>> branches;
	map<string, double> branches_total;
	map<string, TTree*> ttrees;
	map<string, map<string, TBranch*>> tbranches;
	map<string, TBranch*> tbranches_total;
	TTree* tt;
};
// \CLASS DEFINITIONS

// Constants, enums and typedefs
	// Simple algorithm variables:
	/// Custom iterators:
	typedef map<string, vector<string>>::iterator collection_it;
	typedef map<string, vector<double>>::iterator branch_it_inner;
	typedef map<string, map<string, vector<double>>>::iterator branch_it_outer;
	
	
	// DEFINE CUTS
//	float cut_dm = 25;

// static data member definitions

// constructors and destructors
FatjetAnalyzer::FatjetAnalyzer(const edm::ParameterSet& iConfig) :
	in_type_(iConfig.getParameter<int>("in_type")),
	v_(iConfig.getParameter<bool>("v")),
	cut_pt_(iConfig.getParameter<double>("cut_pt")),
	cut_m_(iConfig.getParameter<double>("cut_m")),
	sigma_(iConfig.getParameter<double>("sigma")),
	nevents_(iConfig.getParameter<double>("nevents"))
//	R_(iConfig.getParameter<double>("R")),
//	make_gen_(iConfig.getParameter<bool>("make_gen")),
//	make_pf_(iConfig.getParameter<bool>("make_pf")),
//	do_true_(iConfig.getParameter<bool>("do_true")),
//	do_prune_(iConfig.getParameter<bool>("do_prune")),
//	do_trim_(iConfig.getParameter<bool>("do_trim")),
//	trim_R_(iConfig.getParameter<double>("trim_R")),
//	trim_ptf_(iConfig.getParameter<double>("trim_ptf"))
{
//do what ever initialization is needed
	
	// Collections variables setup:
	collections["ak8_pf"] = {"pf", "selectedPatJetsAK8PFCHS", "", "ak8PFJetsCHSTrimmedMass", "NjettinessAK8CHS"};
	collections["ca8_pf"] = {"pf", "selectedPatJetsCA8PFCHS", "", "ca8PFJetsCHSTrimmedMass", "NjettinessCA8CHS"};
	collections["ak10_pf"] = {"pf", "selectedPatJetsAK10PFCHS", "", "ak10PFJetsCHSTrimmedMass", "NjettinessAK10CHS"};
	collections["ca10_pf"] = {"pf", "selectedPatJetsCA10PFCHS", "", "ca10PFJetsCHSTrimmedMass", "NjettinessCA10CHS"};
	collections["ak12_pf"] = {"pf", "selectedPatJetsCA12PFCHS", "", "ca12PFJetsCHSTrimmedMass", "NjettinessAK12CHS"};
	collections["ca12_pf"] = {"pf", "selectedPatJetsCA12PFCHS", "", "ca12PFJetsCHSTrimmedMass", "NjettinessCA12CHS"};
	collections["ak14_pf"] = {"pf", "selectedPatJetsCA12PFCHS", "", "ca12PFJetsCHSTrimmedMass", "NjettinessAK14CHS"};
	collections["ca14_pf"] = {"pf", "selectedPatJetsCA12PFCHS", "", "ca12PFJetsCHSTrimmedMass", "NjettinessCA14CHS"};
	collections["ak8_gn"] = {"gn", "selectedPatJetsAK8PFCHS", "genJets", ""};
	collections["ca8_gn"] = {"gn", "selectedPatJetsCA8PFCHS", "genJets", ""};
	collections["ak10_gn"] = {"gn", "selectedPatJetsAK10PFCHS", "genJets", ""};
	collections["ca10_gn"] = {"gn", "selectedPatJetsCA10PFCHS", "genJets", ""};
	collections["ak12_gn"] = {"gn", "selectedPatJetsCA12PFCHS", "genJets", ""};
	collections["ca12_gn"] = {"gn", "selectedPatJetsCA12PFCHS", "genJets", ""};
	collections["ak14_gn"] = {"gn", "selectedPatJetsCA12PFCHS", "genJets", ""};
	collections["ca14_gn"] = {"gn", "selectedPatJetsCA12PFCHS", "genJets", ""};
	
	// Ntuple setup:
	edm::Service<TFileService> fs;		// Open output services
	
	/// Event-by-event variables:
	ttrees["events"] = fs->make<TTree>();
	ttrees["events"]->SetName("events");
	//// Per collection:
	variables = {"phi", "eta", "y", "pt",
		"M",		// Untrimmed mass
		"m",		// Trimmed mass
		"tau2",		// Nsubjettiness 2
		"tau4",		// Nsubjettiness 4
		"tau42",		// The raio of nsubjettiness 4 to 2. (The value -1 means that the denominator was 0).
		"m_avg", "dm", "pt_hat", "sigma", "nevents"};		// List of event branch variables for each collection.
	for (collection_it i = collections.begin(); i != collections.end(); i++) {
		string name = i->first;
		for (vector<string>::iterator j = variables.begin(); j != variables.end(); j++) {
			string branch_name = name + "_" + *j;
			tbranches[name][*j] = ttrees["events"]->Branch(branch_name.c_str(), &(branches[name][*j]), 64000, 0);
		}
	}
//	//// Per event:
//	variables_all = {"pt_hat"};
//	for (vector<string>::iterator i = variables_all.begin(); i != variables_all.end(); i++) {
//		string branch_name = "all_" + *i;
//		tbranches["all"][*i] = ttrees["events"]->Branch(branch_name.c_str(), &(branches["all"][*i]), 64000, 0);
//	}
	
	
	/// Total variables:
	ttrees["totals"] = fs->make<TTree>();
	ttrees["totals"]->SetName("totals");
	variables_total = {"sigma", "n"};		// List of dataset branch variables for all collections.
	for (collection_it i = collections.begin(); i != collections.end(); i++) {
		variables_total.push_back("n_" + i->first);
	}
	for (vector<string>::iterator i = variables_total.begin(); i != variables_total.end(); i++) {
		branches_total[*i] = 0;
		tbranches_total[*i] = ttrees["totals"]->Branch(i->c_str(), &(branches_total[*i]), 64000, 0);
	}
	
	// Debug:
	cout << endl;
	cout << "Starting the Fatjet analyzer..." << endl;
	cout << "in_type = " << in_type_ << endl;
	cout << "v = " << v_ << ", cut_pt = " << cut_pt_ << ", and cut_m = " << cut_m_ << "." << endl;
}

// DEFINE THE DESTRUCTOR
FatjetAnalyzer::~FatjetAnalyzer()
{
	branches_total["sigma"] = sigma_;
	ttrees["totals"]->Fill();
	cout << "======================================================" << endl;
	cout << "Error records:" << endl;
	cout << "This hasn't been implemented, yet." << endl;
//	cout << "There were " << n_event << " events." << endl;
//	cout << "There were " << n_event_sel << " selected events." << endl;
//	cout << "There were " << n_sel_lead << " (" << n_sel_lead / (float)n_event_sel * 100 << "%) events in which the first two jets were selected." << endl;
//	cout << "* Squark number errors: " << n_error_sq << endl;
//	cout << "* Quark number errors: " << n_error_q << endl;
//	cout << "* Squark matching errors: " << n_error_sq_match << endl;
	cout << "======================================================" << endl;
}

// ------------ called once each job just before starting event loop  ------------
void FatjetAnalyzer::beginJob()
{
}

// CLASS METHODS ("method" = "member function")

// ------------ called for each event  ------------
void FatjetAnalyzer::analyze(
	const edm::Event& iEvent,
	const edm::EventSetup& iSetup
){
//	cout << "HERE 274" << endl;
	branches_total["n"] ++;
	n_event ++;
	
	// Event variables:
	double pt_hat = 0;
	
	// Get objects from event:
	if (in_type_ == 0) {
		if (v_) {cout << "Running over a B2G ntuple ..." << endl;}
		Handle<vector<float>> pf_ak8_e;
		Handle<vector<float>> pf_ak8_m;
		Handle<vector<float>> pf_ak8_pt;
//		Handle<vector<float>> pf_ak8_px;
//		Handle<vector<float>> pf_ak8_py;
//		Handle<vector<float>> pf_ak8_pz;
		Handle<vector<float>> pf_ak8_phi;
		Handle<vector<float>> pf_ak8_eta;
		Handle<vector<float>> pf_ak8_y;
		Handle<vector<float>> gn_ak8_e;
		Handle<vector<float>> gn_ak8_m;
		Handle<vector<float>> gn_ak8_pt;
//		Handle<vector<float>> gn_ak8_px;
//		Handle<vector<float>> gn_ak8_py;
//		Handle<vector<float>> gn_ak8_pz;
		Handle<vector<float>> gn_ak8_phi;
		Handle<vector<float>> gn_ak8_eta;
		Handle<vector<float>> gn_ak8_y;
		iEvent.getByLabel("jetsAK8", "jetAK8E", pf_ak8_e);
		iEvent.getByLabel("jetsAK8", "jetAK8Mass", pf_ak8_m);
		iEvent.getByLabel("jetsAK4", "jetAK4Pt", pf_ak8_pt);
//		iEvent.getByLabel("jetsAK8", "jetAK8Px", pf_ak8_px);		// Doesn't exist?
//		iEvent.getByLabel("jetsAK8", "jetAK8Py", pf_ak8_py);		// Doesn't exist?
//		iEvent.getByLabel("jetsAK8", "jetAK8Pz", pf_ak8_pz);		// Doesn't exist?
		iEvent.getByLabel("jetsAK8", "jetAK8Phi", pf_ak8_phi);
		iEvent.getByLabel("jetsAK8", "jetAK8Eta", pf_ak8_eta);
		iEvent.getByLabel("jetsAK8", "jetAK8Y", pf_ak8_y);
		iEvent.getByLabel("genJetsAK8", "genJetsAK8E", gn_ak8_e);
		iEvent.getByLabel("genJetsAK8", "genJetsAK8Mass", gn_ak8_m);
		iEvent.getByLabel("genJetsAK8", "genJetsAK8Pt", gn_ak8_pt);
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Px", gn_ak8_px);		// Doesn't exist?
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Py", gn_ak8_py);		// Doesn't exist?
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Pz", gn_ak8_pz);		// Doesn't exist?
		iEvent.getByLabel("genJetsAK8", "genJetsAK8Phi", gn_ak8_phi);
		iEvent.getByLabel("genJetsAK8", "genJetsAK8Eta", gn_ak8_eta);
		iEvent.getByLabel("genJetsAK8", "genJetsAK8Y", gn_ak8_y);
		int gn_ak8_n = (*gn_ak8_pt).size();
		int pf_ak8_n = (*pf_ak8_pt).size();
		if (v_) {cout << gn_ak8_n << "    " << pf_ak8_n << endl;}
		if (pf_ak8_n >= 2) {
	//		double pf_ak8_m0 = (*pf_ak8_m)[0];
	//		double pf_ak8_m1 = (*pf_ak8_m)[1];
			double pf_ak8_pt0 = (*pf_ak8_pt)[0];
	//		double pf_ak8_pt1 = (*pf_ak8_pt)[1];
			cout << "There are " << (*pf_ak8_pt).size() << " PFAK8 jets." << endl;
		
			// Event selection:
			if (pf_ak8_pt0 > 150) {
				cout << "Event selected!" << endl;
			}
			else {
				cout << "Event not selected." << endl;
			}
		}
	}
	else if (in_type_ == 1) {
		if (v_) {cout << "Running over fatjet collections ..." << endl;}
		
		// Clear branches:
		for(collection_it collection = collections.begin(); collection != collections.end(); collection++) {
			string name = collection->first;
			for (vector<string>::iterator i = variables.begin(); i != variables.end(); i++) {
				branches[name][*i].clear();
			}
		}
		
		// Get event info:
		edm::Handle<GenEventInfoProduct> gn_event_info;
		iEvent.getByLabel("generator", gn_event_info);
		if (gn_event_info->hasBinningValues()) {
			pt_hat = gn_event_info->binningValues()[0];
		}
		
		// Collection loop:
		/// Perform selection for each jet collection in the input sample:
		for(collection_it collection = collections.begin(); collection != collections.end(); collection++) {
			// Get the jets from the input:
			string name = collection->first;
			string collection_type = collection->second[0];
			if (v_) {cout << ">> Selecting from collection " << name << " ..." << endl;}
			if (collection_type == "pf"){
//				cout << name << endl;
				Handle<vector<pat::Jet>> jets_edm;
				iEvent.getByLabel(collection->second[1], collection->second[2], jets_edm);
			
				// Selection:
				vector<pat::Jet> jets;
				vector<float> jets_m;
				vector<vector<float>> jets_taus;
			
				/// Form candidate collections:
				for (vector<pat::Jet>::const_iterator jet_i = jets_edm->begin(); jet_i != jets_edm->end(); ++ jet_i) {
//					cout << jet_i->userFloat("NjettinessAK8CHS:tau1") << endl;
//					cout << jet_i->userFloat("NjettinessAK10CHS:tau1") << endl;
//					cout << jet_i->userFloat("NjettinessCA8CHS:tau1") << endl;
//					cout << jet_i->userFloat("NjettinessCA12CHS:tau1") << endl;
//					cout << jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("1")) << endl;
					if (jet_i->pt() > cut_pt_) {
						jets.push_back(*jet_i);
						jets_m.push_back(jet_i->userFloat(collection->second[3]));
						vector<float> taus;
//						cout << jet_i->userFloat("NjettinessCA12CHS:tau1") << endl;
//						cout << string(collection->second[4]) + string(":tau") + string("2") << endl;
//						cout << jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("1")) << endl;
						taus.push_back(jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("1")));
						taus.push_back(jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("2")));
						taus.push_back(jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("3")));
						taus.push_back(jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("4")));
						jets_taus.push_back(taus);
//						cout << jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("1")) << endl;
//						cout << jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("2")) << endl;
//						cout << jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("3")) << endl;
//						cout << jet_i->userFloat(string(collection->second[4]) + string(":tau") + string("4")) << endl;
					}
				}
	//			for(unsigned int i=0; i < jets.size(); i++){
	//				cout << i << "\t" << jets[i].pt() << "\t" << jets_m[i] << endl;
	//			}
				if (v_) {cout << ">> There were " << jets.size() << " jets with pT > " << cut_pt_ << " GeV." << endl;}
			
				/// Select dijet pair:
				if (jets.size() >= 2) {
					int jet0 = -1;
					int jet1 = -1;
					double dm_min = -1;
					for(unsigned int i=0; i < jets_m.size(); i++){
						for (unsigned int j=0; j < jets_m.size(); j++) {
							if (j > i) {
	//							cout << i << "\t" << jets_m[i] << "\t" << j << "\t" << jets_m[j] << endl;
								double dm = abs(jets_m[i] - jets_m[j]);
								if ((i == 0 && j == 1) || (dm < dm_min)) {
									jet0 = i;
									jet1 = j;
									dm_min = dm;
								}
							}
						}
					}
					if (v_) {cout << ">> Selected jet " << jet0 << " and jet " << jet1 << ", which have a mass difference of " << dm_min << " GeV." << endl;}
				
					// Add the selected dijet to the ntuple:
//					cout << jets_m[jet0] << "  " << jets_m[jet1] << "  " << cut_m_ << endl;
					if ((jets_m[jet0] > cut_m_) || (jets_m[jet1] > cut_m_)) {
						branches_total["n_" + name] ++;
					
						// Define some useful event variables:
						double M0 = jets[jet0].mass();
						double M1 = jets[jet1].mass();
						double m0 = jets_m[jet0];
						double m1 = jets_m[jet1];
//						cout << name << endl;
//						cout << m0 << "   " << m1 << endl;
//						cout << jets_m[jet0] << "   " << jets_m[jet1] << endl;
						double m_avg = (m0 + m1)/2;
						double dm = m0 - m1;
						double pt0 = jets[jet0].pt();
						double pt1 = jets[jet1].pt();
						double phi0 = jets[jet0].phi();
						double phi1 = jets[jet1].phi();
						double eta0 = jets[jet0].eta();
						double eta1 = jets[jet1].eta();
						double y0 = jets[jet0].y();
						double y1 = jets[jet1].y();
//						double tau10 = jets_taus[jet0][0];
//						double tau11 = jets_taus[jet1][0];
//						cout << tau10 << "   " << tau11 << endl;
						double tau20 = jets_taus[jet0][1];
						double tau21 = jets_taus[jet1][1];
//						cout << tau20 << "   " << tau21 << endl;
//						double tau30 = jets_taus[jet0][2];
//						double tau31 = jets_taus[jet1][2];
						double tau40 = jets_taus[jet0][3];
						double tau41 = jets_taus[jet1][3];
						double tau420 = -1;
						double tau421 = -1;
						if (tau20 != 0) {
							tau420 = tau40/tau20;
						}
						if (tau21 != 0) {
							tau421 = tau41/tau21;
						}
					
						// Fill branches:
						branches[name]["phi"].push_back(phi0);
						branches[name]["phi"].push_back(phi1);
						branches[name]["eta"].push_back(eta0);
						branches[name]["eta"].push_back(eta1);
						branches[name]["y"].push_back(y0);
						branches[name]["y"].push_back(y1);
						branches[name]["pt"].push_back(pt0);
						branches[name]["pt"].push_back(pt1);
						branches[name]["M"].push_back(M0);
						branches[name]["M"].push_back(M1);
						branches[name]["m"].push_back(m0);
						branches[name]["m"].push_back(m1);
						branches[name]["tau2"].push_back(tau20);
						branches[name]["tau2"].push_back(tau21);
						branches[name]["tau4"].push_back(tau40);
						branches[name]["tau4"].push_back(tau41);
						branches[name]["tau42"].push_back(tau420);
						branches[name]["tau42"].push_back(tau421);
						branches[name]["dm"].push_back(dm);
						branches[name]["m_avg"].push_back(m_avg);
						branches[name]["pt_hat"].push_back(pt_hat);
						branches[name]["sigma"].push_back(sigma_);
						branches[name]["nevents"].push_back(nevents_);
					
	//					cout << branches[name]["phi"].size() << endl;
					}
				}
				else {
					if (v_) {cout << ">> Not enough jet candidates around to make a selection!" << endl;}
				}
			}
			else if (collection_type == "gn") {
				Handle<vector<reco::GenJet>> jets_edm;
				iEvent.getByLabel(collection->second[1], collection->second[2], jets_edm);
			
				// Selection:
				vector<reco::GenJet> jets;
				vector<float> jets_m;
			
				/// Form candidate collections:
				for (vector<reco::GenJet>::const_iterator jet_i = jets_edm->begin(); jet_i != jets_edm->end(); ++ jet_i) {
					if (jet_i->pt() > cut_pt_) {
						jets.push_back(*jet_i);
						jets_m.push_back(jet_i->mass());
					}
				}
	//			for(unsigned int i=0; i < jets.size(); i++){
	//				cout << i << "\t" << jets[i].pt() << "\t" << jets_m[i] << endl;
	//			}
				if (v_) {cout << ">> There were " << jets.size() << " jets with pT > " << cut_pt_ << " GeV." << endl;}
			
				/// Select dijet pair:
				if (jets.size() >= 2) {
					int jet0 = -1;
					int jet1 = -1;
					double dm_min = -1;
					for(unsigned int i=0; i < jets_m.size(); i++){
						for (unsigned int j=0; j < jets_m.size(); j++) {
							if (j > i) {
	//							cout << i << "\t" << jets_m[i] << "\t" << j << "\t" << jets_m[j] << endl;
								double dm = abs(jets_m[i] - jets_m[j]);
								if ((i == 0 && j == 1) || (dm < dm_min)) {
									jet0 = i;
									jet1 = j;
									dm_min = dm;
								}
							}
						}
					}
					if (v_) {cout << ">> Selected jet " << jet0 << " and jet " << jet1 << ", which have a mass difference of " << dm_min << " GeV." << endl;}
				
					// Add the selected dijet to the ntuple:
					if ((jets_m[jet0] > cut_m_) || (jets_m[jet1] > cut_m_)) {
						branches_total["n_" + name] ++;
					
						// Define some useful event variables:
						double M0 = jets[jet0].mass();
						double M1 = jets[jet1].mass();
						double m0 = jets[jet0].mass();
						double m1 = jets[jet1].mass();
						double m_avg = (m0 + m1)/2;
						double dm = m0 - m1;
						double pt0 = jets[jet0].pt();
						double pt1 = jets[jet1].pt();
						double phi0 = jets[jet0].phi();
						double phi1 = jets[jet1].phi();
						double eta0 = jets[jet0].eta();
						double eta1 = jets[jet1].eta();
						double y0 = jets[jet0].y();
						double y1 = jets[jet1].y();
					
						// Fill branches:
						branches[name]["phi"].push_back(phi0);
						branches[name]["phi"].push_back(phi1);
						branches[name]["eta"].push_back(eta0);
						branches[name]["eta"].push_back(eta1);
						branches[name]["y"].push_back(y0);
						branches[name]["y"].push_back(y1);
						branches[name]["pt"].push_back(pt0);
						branches[name]["pt"].push_back(pt1);
						branches[name]["M"].push_back(M0);
						branches[name]["M"].push_back(M1);
						branches[name]["m"].push_back(m0);
						branches[name]["m"].push_back(m1);
						branches[name]["dm"].push_back(dm);
						branches[name]["m_avg"].push_back(m_avg);
						branches[name]["pt_hat"].push_back(pt_hat);
						branches[name]["sigma"].push_back(sigma_);
						branches[name]["nevents"].push_back(nevents_);
					
	//					cout << branches[name]["phi"].size() << endl;
					}
				}
				else {
					if (v_) {cout << ">> Not enough jet candidates around to make a selection!" << endl;}
				}
			}
			else {
				cout << "ERROR: The collection type " << collection_type << " isn't recognized." << endl;
			}
		}
		// END collection loop
		
//		branches["all"]["pt_hat"].push_back(pt_hat);
		// Fill ntuple:
		ttrees["events"]->Fill();		// Fills all defined branches.
	}
	else if (in_type_ == 2) {
		if (v_) {cout << "Running over old fatjet collections ..." << endl;}
		
		// Clear branches:
		for(collection_it collection = collections.begin(); collection != collections.end(); collection++) {
			string name = collection->first;
			for (vector<string>::iterator i = variables.begin(); i != variables.end(); i++) {
				branches[name][*i].clear();
			}
		}
		
		// Get event info:
		edm::Handle<GenEventInfoProduct> gn_event_info;
		iEvent.getByLabel("generator", gn_event_info);
		if (gn_event_info->hasBinningValues()) {
			pt_hat = gn_event_info->binningValues()[0];
		}
		
		// Collection section:
		collections["ak8_pf"] = {"FatjetProducer", "PF", ""};
		collections["ca8_pf"] = {"FatjetProducer", "PF", ""};
//		collections["ak10_pf"] = {"FatjetProducer", "PF", ""};
		collections["ca10_pf"] = {"FatjetProducer", "PF", ""};
		collections["ca12_pf"] = {"FatjetProducer", "PF", ""};
		/// Perform selection for each jet collection in the input sample:
		for(collection_it collection = collections.begin(); collection != collections.end(); collection++) {			
			// Get the jets from the input:
			string name = collection->first;
			if (v_) {cout << ">> Selecting from collection " << name << " ..." << endl;}
//			Handle<vector<pat::Jet>> jets_edm;
			Handle<vector<reco::BasicJet>> jets_edm;
			iEvent.getByLabel(collection->second[1], collection->second[2], jets_edm);
			
			// Selection:
//			vector<pat::Jet> jets;
			vector<reco::BasicJet> jets;
			vector<float> jets_m;
			
			/// Form candidate collections:
			for (vector<reco::BasicJet>::const_iterator jet_i = jets_edm->begin(); jet_i != jets_edm->end(); ++ jet_i) {
				if (jet_i->pt() > cut_pt_) {
					jets.push_back(*jet_i);
//					jets_m.push_back(jet_i->userFloat(collection->second[3]));
					jets_m.push_back(jet_i->mass());
				}
			}
//			for(unsigned int i=0; i < jets.size(); i++){
//				cout << i << "\t" << jets[i].pt() << "\t" << jets_m[i] << endl;
//			}
			if (v_) {cout << ">> There were " << jets.size() << " jets with pT > " << cut_pt_ << " GeV." << endl;}
			
			/// Select dijet pair:
			if (jets.size() >= 2) {
				int jet0 = -1;
				int jet1 = -1;
				double dm_min = -1;
				for(unsigned int i=0; i < jets_m.size(); i++){
					for (unsigned int j=0; j < jets_m.size(); j++) {
						if (j > i) {
//							cout << i << "\t" << jets_m[i] << "\t" << j << "\t" << jets_m[j] << endl;
							double dm = abs(jets_m[i] - jets_m[j]);
							if ((i == 0 && j == 1) || (dm < dm_min)) {
								jet0 = i;
								jet1 = j;
								dm_min = dm;
							}
						}
					}
				}
				if (v_) {cout << ">> Selected jet " << jet0 << " and jet " << jet1 << ", which have a mass difference of " << dm_min << " GeV." << endl;}
				
				// Add the selected dijet to the ntuple:
				if ((jets_m[jet0] > cut_m_) || (jets_m[jet1] > cut_m_)) {
					branches_total["n_" + name] ++;
					
					// Define some useful event variables:
					double m0 = jets[jet0].mass();
					double m1 = jets[jet1].mass();
					double m_avg = (m0 + m1)/2;
					double dm = m0 - m1;
					double pt0 = jets[jet0].pt();
					double pt1 = jets[jet1].pt();
					double phi0 = jets[jet0].phi();
					double phi1 = jets[jet1].phi();
					double eta0 = jets[jet0].eta();
					double eta1 = jets[jet1].eta();
					double y0 = jets[jet0].y();
					double y1 = jets[jet1].y();
					
					// Fill branches:
					branches[name]["phi"].push_back(phi0);
					branches[name]["phi"].push_back(phi1);
					branches[name]["eta"].push_back(eta0);
					branches[name]["eta"].push_back(eta1);
					branches[name]["y"].push_back(y0);
					branches[name]["y"].push_back(y1);
					branches[name]["pt"].push_back(pt0);
					branches[name]["pt"].push_back(pt1);
					branches[name]["m"].push_back(m0);
					branches[name]["m"].push_back(m1);
					branches[name]["dm"].push_back(dm);
					branches[name]["m_avg"].push_back(m_avg);
					branches[name]["pt_hat"].push_back(pt_hat);
					branches[name]["sigma"].push_back(sigma_);
					branches[name]["nevents"].push_back(nevents_);
					
//					cout << branches[name]["phi"].size() << endl;
				}
			}
			else {
				if (v_) {cout << ">> Not enough jet candidates around to make a selection!" << endl;}
			}
		}
		// END collection loop
		
//		branches["all"]["pt_hat"].push_back(pt_hat);
		// Fill ntuple:
		ttrees["events"]->Fill();		// Fills all defined branches.
	}
	else {
		cout << "You input an unknown \"in type\": " << in_type_ << endl;
	}
}

// ------------  called once each job just after ending the event loop  ------------
void FatjetAnalyzer::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
void 
FatjetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
FatjetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
FatjetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
FatjetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void
FatjetAnalyzer::respondToOpenInputFile(edm::FileBlock const& fb)
{
//	cout << "NEW FILE" << endl;
//	cout << fb.fileName() << endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FatjetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FatjetAnalyzer);
