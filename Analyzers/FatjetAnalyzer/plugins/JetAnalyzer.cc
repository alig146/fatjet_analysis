/*#######################################################
# CMSSW EDAnalyzer                                      #
# Name: JetAnalyzer.cc                                  #
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
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//// Meta includes:
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//// Tools:
#include <boost/algorithm/string.hpp>

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
class JetAnalyzer : public edm::EDAnalyzer {
	public:
		explicit JetAnalyzer(const edm::ParameterSet&);		// Set the class argument to be (a reference to) a parameter set (?)
		~JetAnalyzer();		// Create the destructor.
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
	/// Configuration variables (filled by setting the python configuration file)
	int in_type_;               // Input type (0: B2G, 1: fatjets)
	bool v_;                    // Verbose control
	double sigma_, weight_, cut_pt_;
	bool make_gen_, make_pf_;		// Controls to make gen fatjets or pf fatjets
	/// Parameters
	double L;
	// Basic fatjet variables
	// Algorithm variables
	int n_event, n_event_sel, n_sel_lead, counter, n_error_g, n_error_q, n_error_sq, n_error_sq_match, n_error_m, n_error_sort;
	
	// Input collections:
	vector<string> jet_names, jet_types;
	vector<string> lep_names, lep_types;
	
	// Ntuple information:
	vector<string> jet_variables, lep_variables, event_variables;
	map<string, map<string, vector<double>>> branches;
	map<string, TTree*> ttrees;
	map<string, map<string, TBranch*>> tbranches;
	TTree* tt;
};
// \CLASS DEFINITIONS

// Constants, enums and typedefs
	// Simple algorithm variables:
	/// Custom iterators:
	typedef map<string, vector<double>>::iterator branch_it_inner;
	typedef map<string, map<string, vector<double>>>::iterator branch_it_outer;
	
	
	// DEFINE CUTS
//	float cut_dm = 25;

// static data member definitions

// constructors and destructors
JetAnalyzer::JetAnalyzer(const edm::ParameterSet& iConfig) :
	in_type_(iConfig.getParameter<int>("in_type")),
//	v_(iConfig.getParameter<bool>("v")),
	sigma_(iConfig.getParameter<double>("sigma")),
	weight_(iConfig.getParameter<double>("weight")),
	cut_pt_(iConfig.getParameter<double>("cut_pt"))
//	make_gen_(iConfig.getParameter<bool>("make_gen")),
//	make_pf_(iConfig.getParameter<bool>("make_pf")),
{
//do what ever initialization is needed
	// Variables:
	v_ = false;               // Verbose mode.
	
	// Parameters:
	L = 10000;                // Luminosity (in inverse pb)
	
	// Collections setup:
	/// "jet"
	jet_names = {"ak4", "ak8", "ca8", "ak10", "ca10", "ak12", "ca12"};
	jet_types = {"pf", "gn", "maod"};
	jet_variables = {		// List of event branch variables for each collection.
		"phi", "eta", "y", "px", "py", "pz", "e", "pt",
		"M",          // Ungroomed mass
		"m_t",        // Trimmed mass
		"m_p",        // Pruned mass
		"m_s",        // Soft Drop mass
		"m_f",        // Filtered mass
		"tau1",       // Nsubjettiness 1
		"tau2",       // Nsubjettiness 2
		"tau3",       // Nsubjettiness 3
		"tau4",       // Nsubjettiness 4
		"tau5",       // Nsubjettiness 5
		"ht",         // Sum of jet pTs. In the case of AK8, it's the sum of the jet pTs with pT > 150 GeV
		"bd_te",
		"bd_tp",
		"bd_csv",
		"bd_cisv",
	};
	
	/// "lep"
	lep_names = {"le", "lm", "lt", "lp"};
	lep_types = {"pf"};
	lep_variables = {		// List of event branch variables for each collection.
		"phi", "eta", "y", "px", "py", "pz", "e", "pt", "m",
	};
	
	/// "event"
	event_variables = {
		"pt_hat",
		"sigma",      // Cross section of the event
		"nevent",     // The unique event number
//		"nevents",    // The total number of events that were run over
		"w",          // Event weight
	};
	
	// Ntuple setup:
	edm::Service<TFileService> fs;		// Open output services
	
	/// Event-by-event variables:
	ttrees["events"] = fs->make<TTree>();
	ttrees["events"]->SetName("events");
	
	//// "jet":
	for (vector<string>::const_iterator i = jet_names.begin(); i != jet_names.end(); i++) {
		string name = *i;
		for (vector<string>::const_iterator j = jet_types.begin(); j != jet_types.end(); j++) {
			string type = *j;
			string name_type = name + "_" + type;
			for (vector<string>::const_iterator k = jet_variables.begin(); k != jet_variables.end(); k++) {
				string variable = *k;
				string branch_name = name + "_" + type + "_" + variable;
				tbranches[name_type][variable] = ttrees["events"]->Branch(branch_name.c_str(), &(branches[name_type][variable]), 64000, 0);
			}
		}
	}
	//// "lep":
	for (vector<string>::const_iterator i = lep_names.begin(); i != lep_names.end(); i++) {
		string name = *i;
		for (vector<string>::const_iterator j = lep_types.begin(); j != lep_types.end(); j++) {
			string type = *j;
			string name_type = name + "_" + type;
			for (vector<string>::const_iterator k = lep_variables.begin(); k != lep_variables.end(); k++) {
				string variable = *k;
				string branch_name = name + "_" + type + "_" + variable;
				tbranches[name_type][variable] = ttrees["events"]->Branch(branch_name.c_str(), &(branches[name_type][variable]), 64000, 0);
			}
		}
	}
	//// "event":
	for (vector<string>::iterator i = event_variables.begin(); i != event_variables.end(); i++) {
		string branch_name = *i;
		tbranches["event"][*i] = ttrees["events"]->Branch(branch_name.c_str(), &(branches["event"][*i]), 64000, 0);
	}
	
	// Debug:
	cout << endl;
	cout << "Starting the Jet Analyzer..." << endl;
	cout << "in_type = " << in_type_ << endl;
	cout << "v = " << v_ << "." << endl;
}

// DEFINE THE DESTRUCTOR
JetAnalyzer::~JetAnalyzer()
{
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
void JetAnalyzer::beginJob()
{
	n_event = 0;
//	cout << "maxEvents = " << nevents_ << endl;
//	cout << "Running over " << nevents_ << " events ..." << endl;
}

// CLASS METHODS ("method" = "member function")

// ------------ called for each event  ------------
void JetAnalyzer::analyze(
	const edm::Event& iEvent,
	const edm::EventSetup& iSetup
){
//	cout << "HERE 274" << endl;
	n_event ++;		// Increment the event counter by one. For the first event, n_event = 1.
	
	// Get objects from event:
	if (in_type_ == 0) {
		if (n_event == 1) {
			cout << "You wanted to run over a B2G ntuple. This isn't implemented, yet ..." << endl;
		}
//		if (v_) {cout << "Running over a B2G ntuple ..." << endl;}
//		Handle<vector<float>> pf_ak8_e;
//		Handle<vector<float>> pf_ak8_m;
//		Handle<vector<float>> pf_ak8_pt;
////		Handle<vector<float>> pf_ak8_px;
////		Handle<vector<float>> pf_ak8_py;
////		Handle<vector<float>> pf_ak8_pz;
//		Handle<vector<float>> pf_ak8_phi;
//		Handle<vector<float>> pf_ak8_eta;
//		Handle<vector<float>> pf_ak8_y;
//		Handle<vector<float>> gn_ak8_e;
//		Handle<vector<float>> gn_ak8_m;
//		Handle<vector<float>> gn_ak8_pt;
////		Handle<vector<float>> gn_ak8_px;
////		Handle<vector<float>> gn_ak8_py;
////		Handle<vector<float>> gn_ak8_pz;
//		Handle<vector<float>> gn_ak8_phi;
//		Handle<vector<float>> gn_ak8_eta;
//		Handle<vector<float>> gn_ak8_y;
//		iEvent.getByLabel("jetsAK8", "jetAK8E", pf_ak8_e);
//		iEvent.getByLabel("jetsAK8", "jetAK8Mass", pf_ak8_m);
//		iEvent.getByLabel("jetsAK4", "jetAK4Pt", pf_ak8_pt);
////		iEvent.getByLabel("jetsAK8", "jetAK8Px", pf_ak8_px);		// Doesn't exist?
////		iEvent.getByLabel("jetsAK8", "jetAK8Py", pf_ak8_py);		// Doesn't exist?
////		iEvent.getByLabel("jetsAK8", "jetAK8Pz", pf_ak8_pz);		// Doesn't exist?
//		iEvent.getByLabel("jetsAK8", "jetAK8Phi", pf_ak8_phi);
//		iEvent.getByLabel("jetsAK8", "jetAK8Eta", pf_ak8_eta);
//		iEvent.getByLabel("jetsAK8", "jetAK8Y", pf_ak8_y);
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8E", gn_ak8_e);
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Mass", gn_ak8_m);
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Pt", gn_ak8_pt);
////		iEvent.getByLabel("genJetsAK8", "genJetsAK8Px", gn_ak8_px);		// Doesn't exist?
////		iEvent.getByLabel("genJetsAK8", "genJetsAK8Py", gn_ak8_py);		// Doesn't exist?
////		iEvent.getByLabel("genJetsAK8", "genJetsAK8Pz", gn_ak8_pz);		// Doesn't exist?
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Phi", gn_ak8_phi);
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Eta", gn_ak8_eta);
//		iEvent.getByLabel("genJetsAK8", "genJetsAK8Y", gn_ak8_y);
//		int gn_ak8_n = (*gn_ak8_pt).size();
//		int pf_ak8_n = (*pf_ak8_pt).size();
//		if (v_) {cout << gn_ak8_n << "    " << pf_ak8_n << endl;}
//		if (pf_ak8_n >= 2) {
//	//		double pf_ak8_m0 = (*pf_ak8_m)[0];
//	//		double pf_ak8_m1 = (*pf_ak8_m)[1];
//			double pf_ak8_pt0 = (*pf_ak8_pt)[0];
//	//		double pf_ak8_pt1 = (*pf_ak8_pt)[1];
//			cout << "There are " << (*pf_ak8_pt).size() << " PFAK8 jets." << endl;
//		
//			// Event selection:
//			if (pf_ak8_pt0 > 150) {
//				cout << "Event selected!" << endl;
//			}
//			else {
//				cout << "Event not selected." << endl;
//			}
//		}
	}
	else if (in_type_ == 1) {
		if (v_) {cout << "Running over JetToolbox collections ..." << endl;}
		
		// Clear branches:
		/// "jet":
		for (vector<string>::const_iterator i = jet_names.begin(); i != jet_names.end(); i++) {
			string name = *i;
			for (vector<string>::const_iterator j = jet_types.begin(); j != jet_types.end(); j++) {
				string type = *j;
				string name_type = name + "_" + type;
				for (vector<string>::iterator k = jet_variables.begin(); k != jet_variables.end(); k++) {
					branches[name_type][*k].clear();
				}
			}
		}
		/// "lep":
		for (vector<string>::const_iterator i = lep_names.begin(); i != lep_names.end(); i++) {
			string name = *i;
			for (vector<string>::const_iterator j = lep_types.begin(); j != lep_types.end(); j++) {
				string type = *j;
				string name_type = name + "_" + type;
				for (vector<string>::iterator k = lep_variables.begin(); k != lep_variables.end(); k++) {
					branches[name_type][*k].clear();
				}
			}
		}
		/// "event":
		for (vector<string>::iterator i = event_variables.begin(); i != event_variables.end(); i++) {
			branches["event"][*i].clear();
		}
		
		// Get event-wide variables:
		double pt_hat = -1;
		edm::Handle<GenEventInfoProduct> gn_event_info;
		iEvent.getByLabel("generator", gn_event_info);
		if (gn_event_info->hasBinningValues()) {
			pt_hat = gn_event_info->binningValues()[0];
		}
		/// Save event-wide variables:
		branches["event"]["sigma"].push_back(sigma_);             // Provided in the configuration file
//		cout << n_event << endl;
		branches["event"]["nevent"].push_back(n_event);           // Event counter
//		branches["event"]["nevents"].push_back(nevents_);         // Provided in the configuration file
		branches["event"]["w"].push_back(weight_);                // The event weight
		branches["event"]["pt_hat"].push_back(pt_hat);            // Maybe I should take this out of "PF"
		
		// Get jet-specific variables:
		/// Save variables for each jet collection in the input sample:
		
		for (vector<string>::const_iterator i = jet_names.begin(); i != jet_names.end(); i++) {
			string name = *i;
			for (vector<string>::const_iterator j = jet_types.begin(); j != jet_types.end(); j++) {
				string type = *j;
				string name_type = name + "_" + type;
				int n_saved_jets = 0;
				
				// Print some info:
				if (v_) {cout << ">> Reading collection " << name_type << " ..." << endl;}
				
				// Grab the collection:
				if (type == "pf") {
					Handle<vector<pat::Jet>> jets_edm;
					string module = string("selectedPatJets") + boost::to_upper_copy<string>(name) + string("PFCHS");
					string label = "";
					iEvent.getByLabel(module, label, jets_edm);
				
					// Print some info:
					if (v_) {cout << ">> There are " << jets_edm->size() << " jets in the " << name_type << " collection." << endl;}
				
					// Loop over the collection:
					double ht = 0;
					for (vector<pat::Jet>::const_iterator jet = jets_edm->begin(); jet != jets_edm->end(); ++ jet) {
						// Define some useful event variables:
						double M = jet->mass();
						double m_t = jet->userFloat(name + string("PFJetsCHSTrimmedMass"));
						double m_p = jet->userFloat(name + string("PFJetsCHSPrunedMass"));
						double m_s = jet->userFloat(name + string("PFJetsCHSSoftDropMass"));
						double m_f = jet->userFloat(name + string("PFJetsCHSFilteredMass"));
						double tau1 = jet->userFloat(string("Njettiness") + boost::to_upper_copy<string>(name) + string("CHS:tau1"));
						double tau2 = jet->userFloat(string("Njettiness") + boost::to_upper_copy<string>(name) + string("CHS:tau2"));
						double tau3 = jet->userFloat(string("Njettiness") + boost::to_upper_copy<string>(name) + string("CHS:tau3"));
						double tau4 = jet->userFloat(string("Njettiness") + boost::to_upper_copy<string>(name) + string("CHS:tau4"));
						double tau5 = jet->userFloat(string("Njettiness") + boost::to_upper_copy<string>(name) + string("CHS:tau5"));
						double px = jet->px();
						double py = jet->py();
						double pz = jet->pz();
						double e = jet->energy();
						double pt = jet->pt();
						double phi = jet->phi();
						double eta = jet->eta();
						double y = jet->y();
						if (name_type == "ak8_pf") {
							if (pt > 150) {
								ht += pt;
							}
						}
						else {
							ht += pt;
						}
						
						if (pt > cut_pt_) {
							n_saved_jets ++;
						
							// Fill branches:
							branches[name_type]["phi"].push_back(phi);
							branches[name_type]["eta"].push_back(eta);
							branches[name_type]["y"].push_back(y);
							branches[name_type]["px"].push_back(px);
							branches[name_type]["py"].push_back(py);
							branches[name_type]["pz"].push_back(pz);
							branches[name_type]["e"].push_back(e);
							branches[name_type]["pt"].push_back(pt);
							branches[name_type]["M"].push_back(M);
							branches[name_type]["m_t"].push_back(m_t);
							branches[name_type]["m_p"].push_back(m_p);
							branches[name_type]["m_s"].push_back(m_s);
							branches[name_type]["m_f"].push_back(m_f);
							branches[name_type]["tau1"].push_back(tau1);
							branches[name_type]["tau2"].push_back(tau2);
							branches[name_type]["tau3"].push_back(tau3);
							branches[name_type]["tau4"].push_back(tau4);
							branches[name_type]["tau5"].push_back(tau5);
						}
					}		// :End collection loop
					
					branches[name_type]["ht"].push_back(ht);
				}		// :End type == "PF"
				else if (type == "gn") {
					Handle<vector<reco::GenJet>> jets_edm;
					string module = "selectedPatJets" + boost::to_upper_copy<string>(name) + "PFCHS";
					string label = "genJets";
					iEvent.getByLabel(module, label, jets_edm);
				
					// Print some info:
					if (v_) {cout << ">> There are " << jets_edm->size() << " jets in the " << name_type << " collection." << endl;}
				
					// Loop over the collection:
					double ht = 0;
					for (vector<reco::GenJet>::const_iterator jet = jets_edm->begin(); jet != jets_edm->end(); ++ jet) {
						// Define some useful event variables:
						double M = jet->mass();
						double m_t = -1;          // There is no groomed mass for GN jets.
						double m_p = -1;          // There is no groomed mass for GN jets.
						double m_s = -1;          // There is no groomed mass for GN jets.
						double m_f = -1;          // There is no groomed mass for GN jets.
						double tau1 = -1;         // There is no nsubjettiness for GN jets.
						double tau2 = -1;         // There is no nsubjettiness for GN jets.
						double tau3 = -1;         // There is no nsubjettiness for GN jets.
						double tau4 = -1;         // There is no nsubjettiness for GN jets.
						double tau5 = -1;         // There is no nsubjettiness for GN jets.
						double px = jet->px();
						double py = jet->py();
						double pz = jet->pz();
						double e = jet->energy();
						double pt = jet->pt();
						double phi = jet->phi();
						double eta = jet->eta();
						double y = jet->y();
						if (name_type == "ak8_gn") {
							if (pt > 150) {
								ht += pt;
							}
						}
						else {
							ht += pt;
						}
					
						if (pt > cut_pt_) {
							n_saved_jets ++;
						
							// Fill branches:
							branches[name_type]["phi"].push_back(phi);
							branches[name_type]["eta"].push_back(eta);
							branches[name_type]["y"].push_back(y);
							branches[name_type]["px"].push_back(px);
							branches[name_type]["py"].push_back(py);
							branches[name_type]["pz"].push_back(pz);
							branches[name_type]["e"].push_back(e);
							branches[name_type]["pt"].push_back(pt);
							branches[name_type]["M"].push_back(M);
							branches[name_type]["m_t"].push_back(m_t);
							branches[name_type]["m_p"].push_back(m_p);
							branches[name_type]["m_s"].push_back(m_s);
							branches[name_type]["m_f"].push_back(m_f);
							branches[name_type]["tau1"].push_back(tau1);
							branches[name_type]["tau2"].push_back(tau2);
							branches[name_type]["tau3"].push_back(tau3);
							branches[name_type]["tau4"].push_back(tau4);
							branches[name_type]["tau5"].push_back(tau5);
						}
					}
				
					branches[name_type]["ht"].push_back(ht);
				}         // :End type == "GN"
				else if (type == "maod") {
					if (name =="ak4" || name == "ak8") {
						Handle<vector<pat::Jet>> jets_edm;
						string module = "slimmedJets";
						if (name == "ak8") {
							module += "AK8";
						}
						string label = "";
						iEvent.getByLabel(module, label, jets_edm);
						
						// Print some info:
						if (v_) {cout << ">> There are " << jets_edm->size() << " jets in the " << name_type << " collection." << endl;}
						
						// Loop over the collection:
						double ht = 0;
						for (vector<pat::Jet>::const_iterator jet = jets_edm->begin(); jet != jets_edm->end(); ++ jet) {
							// Define some useful event variables:
							double M = jet->mass();
							double m_t = -1;          // There is no groomed mass for maod jets.
							double m_p = -1;          // There is no groomed mass for maod jets.
							double m_s = -1;          // There is no groomed mass for maod jets.
							double m_f = -1;          // There is no groomed mass for maod jets.
							double tau1 = -1;         // There is no nsubjettiness for maod jets.
							double tau2 = -1;         // There is no nsubjettiness for maod jets.
							double tau3 = -1;         // There is no nsubjettiness for maod jets.
							double tau4 = -1;         // There is no nsubjettiness for maod jets.
							double tau5 = -1;         // There is no nsubjettiness for maod jets.
							double px = jet->px();
							double py = jet->py();
							double pz = jet->pz();
							double e = jet->energy();
							double pt = jet->pt();
							double phi = jet->phi();
							double eta = jet->eta();
							double y = jet->y();
							if (name_type == "ak8_maod") {
								if (pt > 150) {
									ht += pt;
								}
							}
							else {
								ht += pt;
							}
							
							if (pt > cut_pt_) {
								n_saved_jets ++;
								
								// Fill branches:
								branches[name_type]["phi"].push_back(phi);
								branches[name_type]["eta"].push_back(eta);
								branches[name_type]["y"].push_back(y);
								branches[name_type]["px"].push_back(px);
								branches[name_type]["py"].push_back(py);
								branches[name_type]["pz"].push_back(pz);
								branches[name_type]["e"].push_back(e);
								branches[name_type]["pt"].push_back(pt);
								branches[name_type]["M"].push_back(M);
								branches[name_type]["m_t"].push_back(m_t);
								branches[name_type]["m_p"].push_back(m_p);
								branches[name_type]["m_s"].push_back(m_s);
								branches[name_type]["m_f"].push_back(m_f);
								branches[name_type]["tau1"].push_back(tau1);
								branches[name_type]["tau2"].push_back(tau2);
								branches[name_type]["tau3"].push_back(tau3);
								branches[name_type]["tau4"].push_back(tau4);
								branches[name_type]["tau5"].push_back(tau5);
								branches[name_type]["bd_te"].push_back(jet->bDiscriminator("pfTrackCountingHighEffBJetTags"));
								branches[name_type]["bd_tp"].push_back(jet->bDiscriminator("pfTtrackCountingHighPurBJetTags"));
								branches[name_type]["bd_csv"].push_back(jet->bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"));
								branches[name_type]["bd_cisv"].push_back(jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
							}
						}
						branches[name_type]["ht"].push_back(ht);
					}
				}         // :End type == "maod"
			}             // :End jet_type loop
		}                 // :End jet_name loop
		// :End "jet" loop
		
		// "lep" loop:
		for (vector<string>::const_iterator i = lep_names.begin(); i != lep_names.end(); i++) {
			string name = *i;
			for (vector<string>::const_iterator j = lep_types.begin(); j != lep_types.end(); j++) {
				string type = *j;
				string name_type = name + "_" + type;
				
				// Print some info:
				if (v_) {cout << ">> Reading collection " << name_type << " ..." << endl;}
				
				// Grab the collection:
				if (name == "le" && type == "pf") {
					Handle<vector<pat::Electron>> leps_edm;
					string module = string("slimmedElectrons");
					string label = "";
					iEvent.getByLabel(module, label, leps_edm);
				
					// Print some info:
					if (v_) {cout << ">> There are " << leps_edm->size() << " leptons in the " << name_type << " collection." << endl;}
				
					// Loop over the collection:
					for (vector<pat::Electron>::const_iterator lep = leps_edm->begin(); lep != leps_edm->end(); ++ lep) {
						// Define some useful event variables:
						double m = lep->mass();
						double px = lep->px();
						double py = lep->py();
						double pz = lep->pz();
						double e = lep->energy();
						double pt = lep->pt();
						double phi = lep->phi();
						double eta = lep->eta();
						double y = lep->y();
						
						if (pt > 5) {
							// Fill branches:
							branches[name_type]["phi"].push_back(phi);
							branches[name_type]["eta"].push_back(eta);
							branches[name_type]["y"].push_back(y);
							branches[name_type]["px"].push_back(px);
							branches[name_type]["py"].push_back(py);
							branches[name_type]["pz"].push_back(pz);
							branches[name_type]["e"].push_back(e);
							branches[name_type]["pt"].push_back(pt);
							branches[name_type]["m"].push_back(m);
						}
					}
				}         // :End "le_pf" (in "lep" loop)
				if (name == "lm" && type == "pf") {
					Handle<vector<pat::Muon>> leps_edm;
					string module = string("slimmedMuons");
					string label = "";
					iEvent.getByLabel(module, label, leps_edm);
				
					// Print some info:
					if (v_) {cout << ">> There are " << leps_edm->size() << " leptons in the " << name_type << " collection." << endl;}
				
					// Loop over the collection:
					for (vector<pat::Muon>::const_iterator lep = leps_edm->begin(); lep != leps_edm->end(); ++ lep) {
						// Define some useful event variables:
						double m = lep->mass();
						double px = lep->px();
						double py = lep->py();
						double pz = lep->pz();
						double e = lep->energy();
						double pt = lep->pt();
						double phi = lep->phi();
						double eta = lep->eta();
						double y = lep->y();
						
						if (pt > 50) {
							// Fill branches:
							branches[name_type]["phi"].push_back(phi);
							branches[name_type]["eta"].push_back(eta);
							branches[name_type]["y"].push_back(y);
							branches[name_type]["px"].push_back(px);
							branches[name_type]["py"].push_back(py);
							branches[name_type]["pz"].push_back(pz);
							branches[name_type]["e"].push_back(e);
							branches[name_type]["pt"].push_back(pt);
							branches[name_type]["m"].push_back(m);
						}
					}
				}         // :End "lm_pf" (in "lep" loop)
				if (name == "lt" && type == "pf") {
					Handle<vector<pat::Tau>> leps_edm;
					string module = string("slimmedTaus");
					string label = "";
					iEvent.getByLabel(module, label, leps_edm);
				
					// Print some info:
					if (v_) {cout << ">> There are " << leps_edm->size() << " leptons in the " << name_type << " collection." << endl;}
				
					// Loop over the collection:
					for (vector<pat::Tau>::const_iterator lep = leps_edm->begin(); lep != leps_edm->end(); ++ lep) {
						// Define some useful event variables:
						double m = lep->mass();
						double px = lep->px();
						double py = lep->py();
						double pz = lep->pz();
						double e = lep->energy();
						double pt = lep->pt();
						double phi = lep->phi();
						double eta = lep->eta();
						double y = lep->y();
						
						if (pt > 50) {
							// Fill branches:
							branches[name_type]["phi"].push_back(phi);
							branches[name_type]["eta"].push_back(eta);
							branches[name_type]["y"].push_back(y);
							branches[name_type]["px"].push_back(px);
							branches[name_type]["py"].push_back(py);
							branches[name_type]["pz"].push_back(pz);
							branches[name_type]["e"].push_back(e);
							branches[name_type]["pt"].push_back(pt);
							branches[name_type]["m"].push_back(m);
						}
					}
				}         // :End "lt_pf" (in "lep" loop)
				if (name == "lp" && type == "pf") {
					Handle<vector<pat::Photon>> leps_edm;
					string module = string("slimmedPhotons");
					string label = "";
					iEvent.getByLabel(module, label, leps_edm);
				
					// Print some info:
					if (v_) {cout << ">> There are " << leps_edm->size() << " leptons in the " << name_type << " collection." << endl;}
				
					// Loop over the collection:
					for (vector<pat::Photon>::const_iterator lep = leps_edm->begin(); lep != leps_edm->end(); ++ lep) {
						// Define some useful event variables:
						double m = lep->mass();
						double px = lep->px();
						double py = lep->py();
						double pz = lep->pz();
						double e = lep->energy();
						double pt = lep->pt();
						double phi = lep->phi();
						double eta = lep->eta();
						double y = lep->y();
						
						if (pt > 50) {
							// Fill branches:
							branches[name_type]["phi"].push_back(phi);
							branches[name_type]["eta"].push_back(eta);
							branches[name_type]["y"].push_back(y);
							branches[name_type]["px"].push_back(px);
							branches[name_type]["py"].push_back(py);
							branches[name_type]["pz"].push_back(pz);
							branches[name_type]["e"].push_back(e);
							branches[name_type]["pt"].push_back(pt);
							branches[name_type]["m"].push_back(m);
						}
					}
				}         // :End "lp_pf" (in "lep" loop)
			}             // :End lep_type loop
		}                 // :End lep_name loop
		// :End "lep" loop
		
		// Fill ntuple:
		ttrees["events"]->Fill();		// Fills all defined branches.
	}                 // :End in_type == 1
	else {
		cout << "You input an unknown \"in type\": " << in_type_ << endl;
	}
}

// ------------  called once each job just after ending the event loop  ------------
void JetAnalyzer::endJob()
{
//	cout << "END!" << endl;
}

// ------------ method called when starting to processes a run  ------------
void 
JetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
JetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
JetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
JetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void
JetAnalyzer::respondToOpenInputFile(edm::FileBlock const& fb)
{
//	cout << "NEW FILE" << endl;
//	cout << fb.fileName() << endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetAnalyzer);
