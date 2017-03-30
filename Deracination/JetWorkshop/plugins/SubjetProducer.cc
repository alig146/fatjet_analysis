// system include files
#include <memory>
#include <iostream>

/// CMSSW includes:
//// Defaults:
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
//// Custom:
//#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//#include "DataFormats/JetReco/interface/PFJet.h"
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include "fastjet/tools/Filter.hh"
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ClusterSequenceArea.hh>

// NAMESPACES:
using namespace std;
using namespace reco;
using namespace edm;
using namespace fastjet;
// \NAMESPACES

//
// class declaration
//

class SubjetProducer : public edm::stream::EDProducer<> {
   public:
      explicit SubjetProducer(const edm::ParameterSet&);
      ~SubjetProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
//      virtual vector<PseudoJet> get_subjets(pat::Jet, JetDefinition, int);
      virtual vector<PseudoJet> get_subjets(reco::Jet, JetDefinition, unsigned);
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // Member data:
      /// Arguments:
      unsigned nSubjets_;
      EDGetTokenT<View<reco::Jet>> src_;
      /// Variables:
      vector<string> subjet_variables;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
SubjetProducer::SubjetProducer(const edm::ParameterSet& iConfig) :
	nSubjets_(iConfig.getParameter<unsigned>("nSubjets")),
	// Consumes statements:
//	ca12Collection_(consumes<View<pat::Jet>>(iConfig.getParameter<InputTag>("ca12Collection")))
	src_(consumes<View<reco::Jet>>(iConfig.getParameter<InputTag>("src")))
{
	subjet_variables = {"px", "py", "pz", "e", "pt", "m", "eta", "phi"};
	for (vector<string>::const_iterator v = subjet_variables.begin(); v != subjet_variables.end(); v++) {
		for (unsigned isj=0; isj < nSubjets_; isj++) {
			string name = *v + to_string(isj);
			produces<ValueMap<float>>(name.c_str());
		}
	}
}


SubjetProducer::~SubjetProducer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


vector<PseudoJet> SubjetProducer::get_subjets(reco::Jet jet, JetDefinition algo, unsigned n=4){
	// Recover and convert constituent collection:
	vector<PseudoJet> constituents;
	Jet::Constituents daughters = jet.getJetConstituents();		// PFJet
	for (Jet::Constituents::const_iterator daughter = daughters.begin(); daughter != daughters.end(); daughter++) {
		constituents.push_back(PseudoJet((*daughter)->px(), (*daughter)->py(), (*daughter)->pz(), (*daughter)->energy()));
	}
	
	// Recluster the jet:
	ClusterSequence cs(constituents, algo);
	
	// Extract subjets:
	vector<fastjet::PseudoJet> inc = cs.inclusive_jets();
	vector<fastjet::PseudoJet> subjets = sorted_by_pt(cs.exclusive_jets_up_to(n));
//	if (inc.size() > 1) cout << inc.size() << "  " << subjets.size() << endl;
	
	return subjets;
}

// ------------ method called to produce the data  ------------
void
SubjetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
//	Handle<vector<pat::Jet>> jets;
//	Handle<View<pat::Jet>> jets;
	Handle<View<reco::Jet>> jets;
	iEvent.getByToken(src_, jets);
	unsigned nJets = jets->size();
	vector<float> values(nJets, 0);
	
//	vector<vector<float>> subjet_px(nSubjets_), subjet_py(nSubjets_)cd;
	
	map<string, vector<float>> subjet_values;
	for (vector<string>::const_iterator v = subjet_variables.begin(); v != subjet_variables.end(); v++) {
		for (unsigned isj=0; isj < nSubjets_; isj++) {
			string name = *v + to_string(isj);
			subjet_values.insert(pair<string, vector<float>>(name, vector<float>(nJets, 0)));
		}
	}
//	cout << subjet_values["px0"].size() << endl;
	
	JetDefinition algo_ca12(cambridge_algorithm, 1.2);
	JetDefinition algo_kt15(kt_algorithm, 1.5);
	JetDefinition algo_ak15(antikt_algorithm, 1.5);
	
	for (unsigned ijet=0; ijet < jets->size(); ijet++) {		// Only find subjets for two leading jets.
		if (ijet > 1) break;
//		pat::Jet jet = (*jets)[ijet];
		reco::Jet jet = (*jets)[ijet];
//		cout << "jet:" << ijet << endl;
		vector<PseudoJet> subjets = get_subjets(jet, algo_kt15, nSubjets_);
//		cout << subjets.size() << endl;
		for (unsigned isj=0; isj < nSubjets_; isj++) {
			subjet_values["px" + to_string(isj)][ijet] = subjets[isj].px();
			subjet_values["py" + to_string(isj)][ijet] = subjets[isj].py();
			subjet_values["pz" + to_string(isj)][ijet] = subjets[isj].pz();
			subjet_values["e" + to_string(isj)][ijet] = subjets[isj].e();
			subjet_values["pt" + to_string(isj)][ijet] = subjets[isj].pt();
			subjet_values["m" + to_string(isj)][ijet] = subjets[isj].m();
			subjet_values["eta" + to_string(isj)][ijet] = subjets[isj].eta();
			subjet_values["phi" + to_string(isj)][ijet] = subjets[isj].phi();
		}
	}
//	cout << subjets.size() << endl;
	for (vector<string>::const_iterator v = subjet_variables.begin(); v != subjet_variables.end(); v++) {
		for (unsigned isj=0; isj < nSubjets_; isj++) {
			string name = *v + to_string(isj);
			auto variable_out = make_unique<ValueMap<float>>();
			ValueMap<float>::Filler variable_filler(*variable_out);
			variable_filler.insert(jets, subjet_values[name].begin(), subjet_values[name].end());
			variable_filler.fill();
			iEvent.put(move(variable_out), name.c_str());
		}
	}

//	auto subjet_test_out = make_unique<ValueMap<float>>();
//	ValueMap<float>::Filler subjet_test_out_filler(*subjet_test_out);
//	subjet_test_out_filler.insert(jets, subjet_test.begin(), subjet_test.end());
//	subjet_test_out_filler.fill();
//	iEvent.put(move(subjet_test_out), "wdhewiofgh");
}



// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
SubjetProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
SubjetProducer::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
SubjetProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
SubjetProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
SubjetProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
SubjetProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SubjetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SubjetProducer);
