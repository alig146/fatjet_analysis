/*#######################################################
# CMSSW EDAnalyzer                                      #
# Name: JetTuplizer.cc                                  #
# Author: Elliot Hughes                                 #
#                                                       #
# Description: To tuplize fatjets or B2G ntuples:       #
# select jets, create tuples.                           #
#######################################################*/

// INCLUDES:
// System includes
#include <iostream>
#include <sstream>
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
//#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
///// JEC:
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
///// b-tag scale factors:
#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

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
class JetTuplizer : public edm::EDAnalyzer {
	public:
		explicit JetTuplizer(const edm::ParameterSet&);		// Set the class argument to be (a reference to) a parameter set (?)
		~JetTuplizer();		// Create the destructor.
		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	private:
		virtual void beginJob();
		virtual void process_jets_pf(const edm::Event&, string, string, EDGetTokenT<vector<pat::Jet>>);
		virtual void process_jets_gn(const edm::Event&, string, EDGetTokenT<vector<reco::GenJet>>);
		virtual void process_jets_maod(const edm::Event&, string, EDGetTokenT<vector<pat::Jet>>);
		virtual void process_electrons_pf(const edm::Event&, EDGetTokenT<vector<pat::Electron>>);
		virtual void process_muons_pf(const edm::Event&, EDGetTokenT<vector<pat::Muon>>);
		virtual void process_tauons_pf(const edm::Event&, EDGetTokenT<vector<pat::Tau>>);
		virtual void process_photons_pf(const edm::Event&, EDGetTokenT<vector<pat::Photon>>);
//		virtual void process_quarks_gn(const edm::Event&, EDGetTokenT<vector<pat::PackedGenParticle>>);
		virtual void process_quarks_gn(const edm::Event&, EDGetTokenT<vector<reco::GenParticle>>);
		virtual void match_bjets();
		virtual void find_btagsf();
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void endJob();
		virtual void beginRun(const edm::Run&, const edm::EventSetup&);
		virtual void endRun(const edm::Run&, const edm::EventSetup&);
		virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
		virtual void endLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
		virtual void respondToOpenInputFile(const edm::FileBlock&);

	// Member data
	/// Configuration variables (filled by setting the python configuration file)
	bool v_;                    // Verbose control
	bool is_data_;
	int in_type_;               // Input type (0: B2G, 1: fatjets)
	double sigma_, weight_, cut_pt_;
	bool make_gen_, make_pf_;		// Controls to make gen fatjets or pf fatjets
	string jec_version_;
	// Basic fatjet variables
	// Algorithm variables
	int n_event, n_event_sel, n_sel_lead, counter, n_error_g, n_error_q, n_error_sq, n_error_sq_match, n_error_m, n_error_sort;
	
	// Input collections:
	vector<string> jet_names, jet_types, jet_collections;
	vector<string> lep_names, lep_types;
	vector<string> gen_names, gen_types;
	
	// JEC info:
	string jec_prefix;
	vector<string> jec_ak4_files, jmc_ak4_files, jec_ak8_files, jmc_ak8_files;
	FactorizedJetCorrector *jec_corrector_ak4, *jmc_corrector_ak4, *jec_corrector_ak8, *jmc_corrector_ak8;
	
	// b-tag scale factor things:
	BTagCalibration btagsf_calib;
	BTagCalibrationReader btagsf_reader;
	
	// Ntuple information:
	vector<string> jet_variables, lep_variables, gen_variables, event_variables;
	map<string, map<string, vector<double>>> branches;
	map<string, TTree*> ttrees;
	map<string, map<string, TBranch*>> tbranches;
	TTree* tt;
	
	// Event variables:
	double pt_hat;
	double rho;
	double npv;
	
	// Tokens:
	EDGetTokenT<GenEventInfoProduct> genInfo_;
	EDGetTokenT<double> rhoInfo_;
	EDGetTokenT<reco::VertexCollection> vertexCollection_;
	EDGetTokenT<vector<pat::Jet>> ak4PFCollection_;
	EDGetTokenT<vector<pat::Jet>> ak4PFFilteredCollection_;
	EDGetTokenT<vector<pat::Jet>> ak4PFPrunedCollection_;
	EDGetTokenT<vector<pat::Jet>> ak4PFSoftDropCollection_;
	EDGetTokenT<vector<pat::Jet>> ak4PFTrimmedCollection_;
	EDGetTokenT<vector<pat::Jet>> ak8PFCollection_;
	EDGetTokenT<vector<pat::Jet>> ak8PFFilteredCollection_;
	EDGetTokenT<vector<pat::Jet>> ak8PFPrunedCollection_;
	EDGetTokenT<vector<pat::Jet>> ak8PFSoftDropCollection_;
	EDGetTokenT<vector<pat::Jet>> ak8PFTrimmedCollection_;
	EDGetTokenT<vector<pat::Jet>> ca12PFCollection_;
	EDGetTokenT<vector<pat::Jet>> ca12PFFilteredCollection_;
	EDGetTokenT<vector<pat::Jet>> ca12PFPrunedCollection_;
	EDGetTokenT<vector<pat::Jet>> ca12PFSoftDropCollection_;
	EDGetTokenT<vector<pat::Jet>> ca12PFTrimmedCollection_;
	EDGetTokenT<vector<reco::GenJet>> ak4GNCollection_;
	EDGetTokenT<vector<reco::GenJet>> ak8GNCollection_;
	EDGetTokenT<vector<reco::GenJet>> ca12GNCollection_;
	EDGetTokenT<vector<pat::Jet>> ak4MAODCollection_;
	EDGetTokenT<vector<pat::Jet>> ak8MAODCollection_;
	EDGetTokenT<vector<pat::Electron>> electronCollection_;
	EDGetTokenT<vector<pat::Muon>> muonCollection_;
	EDGetTokenT<vector<pat::Tau>> tauCollection_;
	EDGetTokenT<vector<pat::Photon>> photonCollection_;
//	EDGetTokenT<vector<pat::PackedGenParticle>> genCollection_;
	EDGetTokenT<vector<reco::GenParticle>> genCollection_;
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
JetTuplizer::JetTuplizer(const edm::ParameterSet& iConfig) :
	v_(iConfig.getParameter<bool>("v")),
	is_data_(iConfig.getParameter<bool>("is_data")),
	in_type_(iConfig.getParameter<int>("in_type")),
	sigma_(iConfig.getParameter<double>("sigma")),
	weight_(iConfig.getParameter<double>("weight")),
	cut_pt_(iConfig.getParameter<double>("cut_pt")),
	jec_version_(iConfig.getParameter<string>("jec_version")),
	// Consume statements:
	genInfo_(consumes<GenEventInfoProduct>(iConfig.getParameter<InputTag>("genInfo"))),
	rhoInfo_(consumes<double>(iConfig.getParameter<InputTag>("rhoInfo"))),
	vertexCollection_(consumes<reco::VertexCollection>(iConfig.getParameter<InputTag>("vertexCollection"))),
	ak4PFCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak4PFCollection"))),
	ak4PFFilteredCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak4PFFilteredCollection"))),
	ak4PFPrunedCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak4PFPrunedCollection"))),
	ak4PFSoftDropCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak4PFSoftDropCollection"))),
	ak4PFTrimmedCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak4PFTrimmedCollection"))),
	ak8PFCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak8PFCollection"))),
	ak8PFFilteredCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak8PFFilteredCollection"))),
	ak8PFPrunedCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak8PFPrunedCollection"))),
	ak8PFSoftDropCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak8PFSoftDropCollection"))),
	ak8PFTrimmedCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak8PFTrimmedCollection"))),
	ca12PFCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ca12PFCollection"))),
	ca12PFFilteredCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ca12PFFilteredCollection"))),
	ca12PFPrunedCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ca12PFPrunedCollection"))),
	ca12PFSoftDropCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ca12PFSoftDropCollection"))),
	ca12PFTrimmedCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ca12PFTrimmedCollection"))),
	ak4GNCollection_(consumes<vector<reco::GenJet>>(iConfig.getParameter<InputTag>("ak4GNCollection"))),
	ak8GNCollection_(consumes<vector<reco::GenJet>>(iConfig.getParameter<InputTag>("ak8GNCollection"))),
	ca12GNCollection_(consumes<vector<reco::GenJet>>(iConfig.getParameter<InputTag>("ca12GNCollection"))),
	ak4MAODCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak4MAODCollection"))),
	ak8MAODCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("ak8MAODCollection"))),
	electronCollection_(consumes<vector<pat::Electron>>(iConfig.getParameter<InputTag>("electronCollection"))),
	muonCollection_(consumes<vector<pat::Muon>>(iConfig.getParameter<InputTag>("muonCollection"))),
	tauCollection_(consumes<vector<pat::Tau>>(iConfig.getParameter<InputTag>("tauCollection"))),
	photonCollection_(consumes<vector<pat::Photon>>(iConfig.getParameter<InputTag>("photonCollection"))),
//	genCollection_(consumes<vector<pat::PackedGenParticle>>(iConfig.getParameter<InputTag>("genCollection")))
	genCollection_(consumes<vector<reco::GenParticle>>(iConfig.getParameter<InputTag>("genCollection")))
	
	
{
//do what ever initialization is needed
	// Collections setup:
	/// "jet"
	jet_names = {"ak4", "ak8", "ca12"};
	jet_types = {"pf", "pff", "pfp", "pfs", "pft", "gn", "maod"};
	jet_collections = {
		"ak4_maod", "ak4_gn", "ak4_pf",
		"ak8_maod", "ak8_gn", "ak8_pf",
		"ca12_gn", "ca12_pf", "ca12_pff", "ca12_pfp", "ca12_pfs", "ca12_pft"
	};
	jet_variables = {		// List of event branch variables for each collection.
		"phi", "eta", "y", "px", "py", "pz", "e", "pt",
		"m",          // Ungroomed mass
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
		"jec",        // Jet energy correction
		"jmc",        // Jet mass correction
		"bsf",        // b-tag scale factor
		"bsf_u",      // b-tag scale factor uncertainty up (?)
		"bsf_d",      // b-tag scale factor uncertainty down (?)
		"neef",       // Neutral EM energy fraction
		"ceef",       // Charged EM energy fraction
		"nhef",       // Neutral hadron energy fraction
		"chef",       // Charged hadron energy fraction
		"mef",        // Muon energy fraction
		"nm",         // Neutral multiplicity
		"cm",         // Charged multiplicity
		"n",          // Number of constituents
		"f"           // Hadron flavor
	};
	
	/// "lep"
	lep_names = {"le", "lm", "lt", "lp"};
	lep_types = {"pf"};
	lep_variables = {		// List of event branch variables for each collection.
		"phi", "eta", "y", "px", "py", "pz", "e", "pt", "m",
	};
	
	/// "gen"
	gen_names = {"q"};
	gen_types = {"gn"};
	gen_variables = {		// List of event branch variables for each collection.
		"phi", "eta", "y", "px", "py", "pz", "e", "pt", "m", "pid",
	};
	
	/// "event"
	event_variables = {
		"pt_hat",
		"sigma",      // Cross section of the event
		"nevent",     // The unique event number
//		"nevents",    // The total number of events that were run over
		"w",          // Event weight
		"rho",
		"npv",        // Number of primary vertices
		"event",
		"lumi",
		"run"
	};
	
	// Ntuple setup:
	edm::Service<TFileService> fs;		// Open output services
	
	/// Event-by-event variables:
	ttrees["events"] = fs->make<TTree>();
	ttrees["events"]->SetName("events");
	
	//// "jet":
//	for (vector<string>::const_iterator i = jet_names.begin(); i != jet_names.end(); i++) {
//		string name = *i;
//		for (vector<string>::const_iterator j = jet_types.begin(); j != jet_types.end(); j++) {
//			string type = *j;
//			string name_type = name + "_" + type;

	for (vector<string>::const_iterator i = jet_collections.begin(); i != jet_collections.end(); i++) {
		for (vector<string>::const_iterator k = jet_variables.begin(); k != jet_variables.end(); k++) {
			string name_type = *i;
			string variable = *k;
			string branch_name = name_type + "_" + variable;
			tbranches[name_type][variable] = ttrees["events"]->Branch(branch_name.c_str(), &(branches[name_type][variable]), 64000, 0);
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
	//// "gen":
	for (vector<string>::const_iterator i = gen_names.begin(); i != gen_names.end(); i++) {
		string name = *i;
		for (vector<string>::const_iterator j = gen_types.begin(); j != gen_types.end(); j++) {
			string type = *j;
			string name_type = name + "_" + type;
			for (vector<string>::const_iterator k = gen_variables.begin(); k != gen_variables.end(); k++) {
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
	
	// JEC setup:
	if (is_data_) jec_prefix = jec_version_ + "_DATA";
	else jec_prefix = jec_version_ + "_MC";
	
	/// AK4 JEC setup:
	jec_ak4_files.push_back(jec_prefix + "_L1FastJet_AK4PFchs.txt");
	jec_ak4_files.push_back(jec_prefix + "_L2Relative_AK4PFchs.txt");
	jec_ak4_files.push_back(jec_prefix + "_L3Absolute_AK4PFchs.txt");
	if (is_data_) jec_ak4_files.push_back(jec_prefix + "_L2L3Residual_AK4PFchs.txt");
	vector<JetCorrectorParameters> jec_parameters_ak4;
	for (vector<string>::const_iterator jec_file = jec_ak4_files.begin(); jec_file != jec_ak4_files.end(); ++jec_file) {
//		cout << *jec_file << endl;
		jec_parameters_ak4.push_back(*(new JetCorrectorParameters(*jec_file)));
	}
	jec_corrector_ak4 = new FactorizedJetCorrector(jec_parameters_ak4);
	
	/// AK4 JMC setup:
	jmc_ak4_files.push_back(jec_prefix + "_L2Relative_AK4PFchs.txt");
	jmc_ak4_files.push_back(jec_prefix + "_L3Absolute_AK4PFchs.txt");
	if (is_data_) jmc_ak4_files.push_back(jec_prefix + "_L2L3Residual_AK4PFchs.txt");
	vector<JetCorrectorParameters> jmc_parameters_ak4;
	for (vector<string>::const_iterator jmc_file = jmc_ak4_files.begin(); jmc_file != jmc_ak4_files.end(); ++jmc_file) {
//		cout << *jmc_file << endl;
		jmc_parameters_ak4.push_back(*(new JetCorrectorParameters(*jmc_file)));
	}
	jmc_corrector_ak4 = new FactorizedJetCorrector(jmc_parameters_ak4);
//	
	/// AK8 JEC setup:
	jec_ak8_files.push_back(jec_prefix + "_L1FastJet_AK8PFchs.txt");
	jec_ak8_files.push_back(jec_prefix + "_L2Relative_AK8PFchs.txt");
	jec_ak8_files.push_back(jec_prefix + "_L3Absolute_AK8PFchs.txt");
	if (is_data_) jec_ak8_files.push_back(jec_prefix + "_L2L3Residual_AK8PFchs.txt");
	vector<JetCorrectorParameters> jec_parameters_ak8;
	for (vector<string>::const_iterator jec_file = jec_ak8_files.begin(); jec_file != jec_ak8_files.end(); ++jec_file) {
//		cout << *jec_file << endl;
		jec_parameters_ak8.push_back(*(new JetCorrectorParameters(*jec_file)));
	}
	jec_corrector_ak8 = new FactorizedJetCorrector(jec_parameters_ak8);
	
	/// AK8 JMC setup:
	jmc_ak8_files.push_back(jec_prefix + "_L2Relative_AK8PFchs.txt");
	jmc_ak8_files.push_back(jec_prefix + "_L3Absolute_AK8PFchs.txt");
	if (is_data_) jmc_ak8_files.push_back(jec_prefix + "_L2L3Residual_AK8PFchs.txt");
	vector<JetCorrectorParameters> jmc_parameters_ak8;
	for (vector<string>::const_iterator jmc_file = jmc_ak8_files.begin(); jmc_file != jmc_ak8_files.end(); ++jmc_file) {
//		cout << *jmc_file << endl;
		jmc_parameters_ak8.push_back(*(new JetCorrectorParameters(*jmc_file)));
	}
	jmc_corrector_ak8 = new FactorizedJetCorrector(jmc_parameters_ak8);
	
	// b-tag scale factor setup:
	BTagCalibration btagsf_calib("csvv2", "CSVv2_ichep.csv");		// "CSVv2_ichep.csv" must be in the directory cmsRun is run from.
//	BTagCalibration btagsf_calib("csvv2");		// What value should I enter here when I don't use a csv file?
	BTagCalibrationReader btagsf_reader(BTagEntry::OP_LOOSE, "central", {"up", "down"});
	btagsf_reader.load(btagsf_calib, BTagEntry::FLAV_B, "comb");
	btagsf_reader.load(btagsf_calib, BTagEntry::FLAV_C, "comb");
	btagsf_reader.load(btagsf_calib, BTagEntry::FLAV_UDSG, "comb");
	
	// Debug:
	cout << endl;
	cout << "Starting the Jet Analyzer..." << endl;
	cout << "in_type = " << in_type_ << endl;
	cout << "v = " << v_ << endl;
	cout << "jec_prefix = " << jec_prefix << endl;
	cout << "is_data = " << is_data_ << endl;
}

// DEFINE THE DESTRUCTOR
JetTuplizer::~JetTuplizer()
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
void JetTuplizer::beginJob()
{
	n_event = 0;
//	cout << "maxEvents = " << nevents_ << endl;
//	cout << "Running over " << nevents_ << " events ..." << endl;
}

// CLASS METHODS ("method" = "member function")

/// PF jets method:
void JetTuplizer::process_jets_pf(const edm::Event& iEvent, string algo, string groom, EDGetTokenT<vector<pat::Jet>> token) {
	// Arguments:
	string type = "pf";
	if (!groom.empty()) {
		stringstream ss;
		string s;
		ss << groom.at(0);
		ss >> s;
		type += boost::to_lower_copy<string>(s);
	}
	string algo_type = algo + "_" + type;
	
	Handle<vector<pat::Jet>> jets;
	iEvent.getByToken(token, jets);

	// Print some info:
//	if (v_) {cout << ">> There are " << jets->size() << " jets in the " << algo_type << " collection." << endl;}
	
	// Loop over the collection:
	double ht = 0;
	int njet = 0;
	for (vector<pat::Jet>::const_iterator jet = jets->begin(); jet != jets->end(); ++ jet) {
		njet ++;
		if (!groom.empty() && njet > 4) break;
		// Define some useful event variables:
		double m = jet->mass();
		string tau_tag = string("taus") + boost::to_upper_copy<string>(algo) + string("CHS") + groom + string(":tau");
		double tau1 = jet->userFloat(tau_tag + string("1"));
		double tau2 = jet->userFloat(tau_tag + string("2"));
		double tau3 = jet->userFloat(tau_tag + string("3"));
		double tau4 = jet->userFloat(tau_tag + string("4"));
		double tau5 = jet->userFloat(tau_tag + string("5"));
		double px = jet->px();
		double py = jet->py();
		double pz = jet->pz();
		double e = jet->energy();
		double pt = jet->pt();
		double phi = jet->phi();
		double eta = jet->eta();
		double y = jet->y();
		double A = jet->jetArea();
		double neef = jet->neutralEmEnergyFraction();
		double ceef = jet->chargedEmEnergyFraction();
		double nhef = jet->neutralHadronEnergyFraction();
		double chef = jet->chargedHadronEnergyFraction();
		double mef = jet->muonEnergyFraction();
		double nm = jet->neutralMultiplicity();
		double cm = jet->chargedMultiplicity();
		double n = nm + cm;
		double f = jet->hadronFlavour();
		if (algo == "ak8") {
			if (pt > 150) {
				ht += pt;
			}
		}
		else {
			ht += pt;
		}
		
		// Get JECs:
		double jec = -1;
		double jmc = -1;
		if (algo == "ak4") {
			jec_corrector_ak4->setJetPt(pt);
			jec_corrector_ak4->setJetEta(eta);
			jec_corrector_ak4->setJetPhi(phi);
			jec_corrector_ak4->setJetE(e);
			jec_corrector_ak4->setJetA(A);
			jec_corrector_ak4->setRho(rho);
			jec_corrector_ak4->setNPV(npv);
			jec = jec_corrector_ak4->getCorrection();
			
			jmc_corrector_ak4->setJetPt(pt);
			jmc_corrector_ak4->setJetEta(eta);
			jmc_corrector_ak4->setJetPhi(phi);
			jmc_corrector_ak4->setJetE(e);
			jmc_corrector_ak4->setJetA(A);
			jmc_corrector_ak4->setRho(rho);
			jmc_corrector_ak4->setNPV(npv);
			jmc = jmc_corrector_ak4->getCorrection();
		}
		else {
			jec_corrector_ak8->setJetPt(pt);
			jec_corrector_ak8->setJetEta(eta);
			jec_corrector_ak8->setJetPhi(phi);
			jec_corrector_ak8->setJetE(e);
			jec_corrector_ak8->setJetA(A);
			jec_corrector_ak8->setRho(rho);
			jec_corrector_ak8->setNPV(npv);
			jec = jec_corrector_ak8->getCorrection();
			
			jmc_corrector_ak8->setJetPt(pt);
			jmc_corrector_ak8->setJetEta(eta);
			jmc_corrector_ak8->setJetPhi(phi);
			jmc_corrector_ak8->setJetE(e);
			jmc_corrector_ak8->setJetA(A);
			jmc_corrector_ak8->setRho(rho);
			jmc_corrector_ak8->setNPV(npv);
			jmc = jmc_corrector_ak8->getCorrection();
		}
		
		// Fill branches:
		if (pt > cut_pt_) {
			branches[algo_type]["phi"].push_back(phi);
			branches[algo_type]["eta"].push_back(eta);
			branches[algo_type]["y"].push_back(y);
			branches[algo_type]["px"].push_back(px);
			branches[algo_type]["py"].push_back(py);
			branches[algo_type]["pz"].push_back(pz);
			branches[algo_type]["e"].push_back(e);
			branches[algo_type]["pt"].push_back(pt);
			branches[algo_type]["m"].push_back(m);
			branches[algo_type]["tau1"].push_back(tau1);
			branches[algo_type]["tau2"].push_back(tau2);
			branches[algo_type]["tau3"].push_back(tau3);
			branches[algo_type]["tau4"].push_back(tau4);
			branches[algo_type]["tau5"].push_back(tau5);
			branches[algo_type]["jec"].push_back(jec);
			branches[algo_type]["jmc"].push_back(jmc);
			branches[algo_type]["neef"].push_back(neef);
			branches[algo_type]["ceef"].push_back(ceef);
			branches[algo_type]["nhef"].push_back(nhef);
			branches[algo_type]["chef"].push_back(chef);
			branches[algo_type]["mef"].push_back(mef);
			branches[algo_type]["nm"].push_back(nm);
			branches[algo_type]["cm"].push_back(cm);
			branches[algo_type]["n"].push_back(n);
			branches[algo_type]["f"].push_back(f);
		}
	}		// :End collection loop
	
	branches[algo_type]["ht"].push_back(ht);
}

/// GN jets method:
void JetTuplizer::process_jets_gn(const edm::Event& iEvent, string algo, EDGetTokenT<vector<reco::GenJet>> token) {
	// Arguments:
	string type = "gn";
	string algo_type = algo + "_" + type;
	
	Handle<vector<reco::GenJet>> jets;
	iEvent.getByToken(token, jets);

	// Print some info:
//	if (v_) {cout << ">> There are " << jets->size() << " jets in the " << algo_type << " collection." << endl;}
	
	// Loop over the collection:
	double ht = 0;
	for (vector<reco::GenJet>::const_iterator jet = jets->begin(); jet != jets->end(); ++ jet) {
		// Define some useful event variables:
		double m = jet->mass();
		double px = jet->px();
		double py = jet->py();
		double pz = jet->pz();
		double e = jet->energy();
		double pt = jet->pt();
		double phi = jet->phi();
		double eta = jet->eta();
		double y = jet->y();
		if (algo == "ak8") {
			if (pt > 150) {
				ht += pt;
			}
		}
		else {
			ht += pt;
		}
		
		// Fill branches:
		if (pt > cut_pt_) {
			branches[algo_type]["phi"].push_back(phi);
			branches[algo_type]["eta"].push_back(eta);
			branches[algo_type]["y"].push_back(y);
			branches[algo_type]["px"].push_back(px);
			branches[algo_type]["py"].push_back(py);
			branches[algo_type]["pz"].push_back(pz);
			branches[algo_type]["e"].push_back(e);
			branches[algo_type]["pt"].push_back(pt);
			branches[algo_type]["m"].push_back(m);
		}
	}		// :End collection loop
	
	branches[algo_type]["ht"].push_back(ht);
}

/// MAOD jets method:
void JetTuplizer::process_jets_maod(const edm::Event& iEvent, string algo, EDGetTokenT<vector<pat::Jet>> token) {
	// Arguments:
	string type = "maod";
	string algo_type = algo + "_" + type;
	
	Handle<vector<pat::Jet>> jets;
	iEvent.getByToken(token, jets);

	// Print some info:
//	if (v_) {cout << ">> There are " << jets->size() << " jets in the " << algo_type << " collection." << endl;}
	
	// Loop over the collection:
	double ht = 0;
	for (vector<pat::Jet>::const_iterator jet = jets->begin(); jet != jets->end(); ++ jet) {
		// Define some useful event variables:
		double m = jet->mass();
		double px = jet->px();
		double py = jet->py();
		double pz = jet->pz();
		double e = jet->energy();
		double pt = jet->pt();
		double phi = jet->phi();
		double eta = jet->eta();
		double y = jet->y();
		if (algo == "ak8") {
			if (pt > 150) {
				ht += pt;
			}
		}
		else {
			ht += pt;
		}
		
		
		// Fill branches:
		if (pt > cut_pt_) {
			branches[algo_type]["phi"].push_back(phi);
			branches[algo_type]["eta"].push_back(eta);
			branches[algo_type]["y"].push_back(y);
			branches[algo_type]["px"].push_back(px);
			branches[algo_type]["py"].push_back(py);
			branches[algo_type]["pz"].push_back(pz);
			branches[algo_type]["e"].push_back(e);
			branches[algo_type]["pt"].push_back(pt);
			branches[algo_type]["m"].push_back(m);
			branches[algo_type]["bd_te"].push_back(jet->bDiscriminator("pfTrackCountingHighEffBJetTags"));
			branches[algo_type]["bd_tp"].push_back(jet->bDiscriminator("pfTtrackCountingHighPurBJetTags"));
			branches[algo_type]["bd_csv"].push_back(jet->bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"));
			branches[algo_type]["bd_cisv"].push_back(jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
		}
	}		// :End collection loop
	
	branches[algo_type]["ht"].push_back(ht);
}

/// Electrons method:
void JetTuplizer::process_electrons_pf(const edm::Event& iEvent, EDGetTokenT<vector<pat::Electron>> token) {
	// Arguments:
	string name = "le";
	string type = "pf";
	string name_type = name + "_" + type;
	
	Handle<vector<pat::Electron>> leps;
	iEvent.getByToken(token, leps);
	
	// Loop over the collection:
	for (vector<pat::Electron>::const_iterator lep = leps->begin(); lep != leps->end(); ++ lep) {
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
		
		// Fill branches:
		if (pt > 5) {
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
	}		// :End collection loop
}

/// Muons method:
void JetTuplizer::process_muons_pf(const edm::Event& iEvent, EDGetTokenT<vector<pat::Muon>> token) {
	// Arguments:
	string name = "lm";
	string type = "pf";
	string name_type = name + "_" + type;
	
	Handle<vector<pat::Muon>> leps;
	iEvent.getByToken(token, leps);
	
	// Loop over the collection:
	for (vector<pat::Muon>::const_iterator lep = leps->begin(); lep != leps->end(); ++ lep) {
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
		
		// Fill branches:
		if (pt > 5) {
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
	}		// :End collection loop
}

/// Tauons method:
void JetTuplizer::process_tauons_pf(const edm::Event& iEvent, EDGetTokenT<vector<pat::Tau>> token) {
	// Arguments:
	string name = "lt";
	string type = "pf";
	string name_type = name + "_" + type;
	
	Handle<vector<pat::Tau>> leps;
	iEvent.getByToken(token, leps);
	
	// Loop over the collection:
	for (vector<pat::Tau>::const_iterator lep = leps->begin(); lep != leps->end(); ++ lep) {
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
		
		// Fill branches:
		if (pt > 5) {
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
	}		// :End collection loop
}

/// Photons method:
void JetTuplizer::process_photons_pf(const edm::Event& iEvent, EDGetTokenT<vector<pat::Photon>> token) {
	// Arguments:
	string name = "lp";
	string type = "pf";
	string name_type = name + "_" + type;
	
	Handle<vector<pat::Photon>> leps;
	iEvent.getByToken(token, leps);
	
	// Loop over the collection:
	for (vector<pat::Photon>::const_iterator lep = leps->begin(); lep != leps->end(); ++ lep) {
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
		
		// Fill branches:
		if (pt > 5) {
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
	}		// :End collection loop
}

/// Quarks method:
void JetTuplizer::process_quarks_gn(const edm::Event& iEvent, EDGetTokenT<vector<reco::GenParticle>> token) {
	// Arguments:
	string name = "q";
	string type = "gn";
	string name_type = name + "_" + type;
	
	Handle<vector<reco::GenParticle>> gens;
	iEvent.getByToken(token, gens);
	
	// Loop over the collection:
	for (vector<reco::GenParticle>::const_iterator gen = gens->begin(); gen != gens->end(); ++ gen) {
		// Define some useful event variables:
		double m = gen->mass();
		double px = gen->px();
		double py = gen->py();
		double pz = gen->pz();
		double e = gen->energy();
		double pt = gen->pt();
		double phi = gen->phi();
		double eta = gen->eta();
		double y = gen->y();
		double status = gen->status();
		double pdgid = gen->pdgId();
		
//		cout << status << "  " << pdgid << endl;
		
		// Fill branches:
		if ((abs(pdgid) == 6 || abs(pdgid) == 1000006) and status == 22) {
			branches[name_type]["phi"].push_back(phi);
			branches[name_type]["eta"].push_back(eta);
			branches[name_type]["y"].push_back(y);
			branches[name_type]["px"].push_back(px);
			branches[name_type]["py"].push_back(py);
			branches[name_type]["pz"].push_back(pz);
			branches[name_type]["e"].push_back(e);
			branches[name_type]["pt"].push_back(pt);
			branches[name_type]["m"].push_back(m);
			branches[name_type]["pid"].push_back(pdgid);
		}
	}		// :End collection loop
}

// B-jet matching method:
void JetTuplizer::match_bjets() {
	for (int ijet_ca12 = 0; ijet_ca12 < 2; ijet_ca12++) {
		double bd_te_max = 0;
		double bd_tp_max = 0;
		double bd_csv_max = 0;
		double bd_cisv_max = 0;
		for (unsigned int ijet_ak4 = 0; ijet_ak4 < branches["ak4_maod"]["pt"].size(); ijet_ak4++) {
//			double ca12_pt = branches["ca12_pf"]["pt"].at(ijet_ca12);
			double ca12_eta = branches["ca12_pf"]["eta"].at(ijet_ca12);
			double ca12_phi = branches["ca12_pf"]["phi"].at(ijet_ca12);
//			double ak4_pt = branches["ak4_pf"]["pt"].at(ijet_ak4);
			double ak4_eta = branches["ak4_maod"]["eta"].at(ijet_ak4);
			double ak4_phi = branches["ak4_maod"]["phi"].at(ijet_ak4);
			double bd_te = branches["ak4_maod"]["bd_te"].at(ijet_ak4);
			double bd_tp = branches["ak4_maod"]["bd_tp"].at(ijet_ak4);
			double bd_csv = branches["ak4_maod"]["bd_csv"].at(ijet_ak4);
			double bd_cisv = branches["ak4_maod"]["bd_cisv"].at(ijet_ak4);
			double dR = reco::deltaR(ca12_eta, ca12_phi, ak4_eta, ak4_phi);
//			double dR = sqrt(pow(ca12_eta - ak4_eta, 2) + pow(M_PI - abs(M_PI - abs(ca12_phi - ak4_phi)), 2));		// Same as above.
			
			if (dR < 0.6 && bd_te > bd_te_max) {bd_te_max = bd_te;}
			if (dR < 0.6 && bd_tp > bd_tp_max) {bd_tp_max = bd_tp;}
			if (dR < 0.6 && bd_csv > bd_csv_max) {bd_csv_max = bd_csv;}
			if (dR < 0.6 && bd_cisv > bd_cisv_max) {bd_cisv_max = bd_cisv;}
		}
		branches["ca12_pf"]["bd_te"].push_back(bd_te_max);
		branches["ca12_pf"]["bd_tp"].push_back(bd_tp_max);
		branches["ca12_pf"]["bd_csv"].push_back(bd_csv_max);
		branches["ca12_pf"]["bd_cisv"].push_back(bd_cisv_max);
	}
}

// B-jet scale factors:
/// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagCalibration#Example_code_in_C
void JetTuplizer::find_btagsf() {
	for (int ijet_ca12 = 0; ijet_ca12 < 2; ijet_ca12++) {
		double pt = branches["ca12_pf"]["pt"].at(ijet_ca12);
		double eta = branches["ca12_pf"]["eta"].at(ijet_ca12);
		double f = branches["ca12_pf"]["f"].at(ijet_ca12);
		double bsf = 1;
		double bsf_u = 1;
		double bsf_d = 1;
		
		if (f == 5) {
			bsf = btagsf_reader.eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt);
			bsf_u = btagsf_reader.eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt);
			bsf_d = btagsf_reader.eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt);
		}
		else if (f == 4) {
			bsf = btagsf_reader.eval_auto_bounds("central", BTagEntry::FLAV_C, eta, pt);
			bsf_u = btagsf_reader.eval_auto_bounds("up", BTagEntry::FLAV_C, eta, pt);
			bsf_d = btagsf_reader.eval_auto_bounds("down", BTagEntry::FLAV_C, eta, pt);
		}
		else if (f == 0) {
			bsf = btagsf_reader.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, eta, pt);
			bsf_u = btagsf_reader.eval_auto_bounds("up", BTagEntry::FLAV_UDSG, eta, pt);
			bsf_d = btagsf_reader.eval_auto_bounds("down", BTagEntry::FLAV_UDSG, eta, pt);
		}
		branches["ca12_pf"]["bsf"].push_back(bsf);
		branches["ca12_pf"]["bsf_u"].push_back(bsf_u);
		branches["ca12_pf"]["bsf_d"].push_back(bsf_d);
	}
}

// ------------ called for each event  ------------
void JetTuplizer::analyze(
	const edm::Event& iEvent,
	const edm::EventSetup& iSetup
){
	n_event ++;		// Increment the event counter by one. For the first event, n_event = 1.
	
	// Get objects from event:
	if (in_type_ == 0) {
		if (n_event == 1) {
			cout << "You wanted to run over a B2G ntuple. This isn't implemented, yet ..." << endl;
		}
	}
	else if (in_type_ == 1) {
		if (v_) {cout << "Running over JetWorkshop collections ..." << endl;}
		
		// Clear branches:
		/// "jet":
//		for (vector<string>::const_iterator i = jet_names.begin(); i != jet_names.end(); i++) {
//			string name = *i;
//			for (vector<string>::const_iterator j = jet_types.begin(); j != jet_types.end(); j++) {
//				string type = *j;
//				string name_type = name + "_" + type;
		for (vector<string>::const_iterator i = jet_collections.begin(); i != jet_collections.end(); i++) {
			for (vector<string>::iterator k = jet_variables.begin(); k != jet_variables.end(); k++) {
				branches[*i][*k].clear();
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
		/// "gen":
		for (vector<string>::const_iterator i = gen_names.begin(); i != gen_names.end(); i++) {
			string name = *i;
			for (vector<string>::const_iterator j = gen_types.begin(); j != gen_types.end(); j++) {
				string type = *j;
				string name_type = name + "_" + type;
				for (vector<string>::iterator k = gen_variables.begin(); k != gen_variables.end(); k++) {
					branches[name_type][*k].clear();
				}
			}
		}
		/// "event":
		for (vector<string>::iterator i = event_variables.begin(); i != event_variables.end(); i++) {
			branches["event"][*i].clear();
		}
		
		// Get event-wide variables:
		/// pT-hat:
		pt_hat = -1;
		if (!is_data_) {
			edm::Handle<GenEventInfoProduct> gn_event_info;
			iEvent.getByToken(genInfo_, gn_event_info);
			if (gn_event_info->hasBinningValues()) {
				pt_hat = gn_event_info->binningValues()[0];
			}
		}
		/// Rho:
		rho = -1;
		edm::Handle<double> rho_;
		iEvent.getByToken(rhoInfo_, rho_);
		rho = *rho_;
		/// Number of primary vertices (NPV):
		npv = 0;
		edm::Handle<reco::VertexCollection> pvs;
		iEvent.getByToken(vertexCollection_, pvs);
		for (vector<reco::Vertex>::const_iterator ipv = pvs->begin(); ipv != pvs->end(); ipv++) {
			if (ipv->ndof() > 4 && ipv->isFake() == false) {
				npv += 1;
			}
		}
		
		/// Save event-wide variables:
		branches["event"]["sigma"].push_back(sigma_);             // Provided in the configuration file
//		cout << n_event << endl;
		branches["event"]["nevent"].push_back(n_event);           // Event counter
//		branches["event"]["nevents"].push_back(nevents_);         // Provided in the configuration file
		branches["event"]["w"].push_back(weight_);                // The event weight
		branches["event"]["pt_hat"].push_back(pt_hat);            // Maybe I should take this out of "PF"
		branches["event"]["event"].push_back(iEvent.id().event());
		branches["event"]["lumi"].push_back(iEvent.id().luminosityBlock());
		branches["event"]["run"].push_back(iEvent.id().run());
		
		// Process each object collection:
		process_jets_pf(iEvent, "ak4", "", ak4PFCollection_);
		process_jets_pf(iEvent, "ak4", "Filtered", ak4PFFilteredCollection_);
		process_jets_pf(iEvent, "ak4", "Pruned", ak4PFPrunedCollection_);
		process_jets_pf(iEvent, "ak4", "SoftDrop", ak4PFSoftDropCollection_);
		process_jets_pf(iEvent, "ak4", "Trimmed", ak4PFTrimmedCollection_);
		process_jets_pf(iEvent, "ak8", "", ak8PFCollection_);
		process_jets_pf(iEvent, "ak8", "Filtered", ak8PFFilteredCollection_);
		process_jets_pf(iEvent, "ak8", "Pruned", ak8PFPrunedCollection_);
		process_jets_pf(iEvent, "ak8", "SoftDrop", ak8PFSoftDropCollection_);
		process_jets_pf(iEvent, "ak8", "Trimmed", ak8PFTrimmedCollection_);
		process_jets_pf(iEvent, "ca12", "", ca12PFCollection_);
		process_jets_pf(iEvent, "ca12", "Filtered", ca12PFFilteredCollection_);
		process_jets_pf(iEvent, "ca12", "Pruned", ca12PFPrunedCollection_);
		process_jets_pf(iEvent, "ca12", "SoftDrop", ca12PFSoftDropCollection_);
		process_jets_pf(iEvent, "ca12", "Trimmed", ca12PFTrimmedCollection_);
		process_jets_gn(iEvent, "ak4", ak4GNCollection_);
		process_jets_gn(iEvent, "ak8", ak8GNCollection_);
		process_jets_gn(iEvent, "ca12", ca12GNCollection_);
		process_jets_maod(iEvent, "ak4", ak4MAODCollection_);
		process_jets_maod(iEvent, "ak8", ak8MAODCollection_);
		process_electrons_pf(iEvent, electronCollection_);
		process_muons_pf(iEvent, muonCollection_);
		process_tauons_pf(iEvent, tauCollection_);
		process_photons_pf(iEvent, photonCollection_);
		if (!is_data_) {process_quarks_gn(iEvent, genCollection_);}
		match_bjets();
//		find_btagsf();		// Seg faults ... (170119)
		
		// Fill ntuple:
		ttrees["events"]->Fill();		// Fills all defined branches.
	}                 // :End in_type == 1
	else {
		cout << "You input an unknown \"in type\": " << in_type_ << endl;
	}
}

// ------------  called once each job just after ending the event loop  ------------
void JetTuplizer::endJob()
{
//	cout << "END!" << endl;
}

// ------------ method called when starting to processes a run  ------------
void 
JetTuplizer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
JetTuplizer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
JetTuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
JetTuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void
JetTuplizer::respondToOpenInputFile(edm::FileBlock const& fb)
{
//	cout << "NEW FILE" << endl;
//	cout << fb.fileName() << endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetTuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetTuplizer);
