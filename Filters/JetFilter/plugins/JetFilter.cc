/*#######################################################
# CMSSW EDFilter                                        #
# Name: JetFilter.cc                                    #
# Author: Elliot Hughes                                 #
#                                                       #
# Description: Filters events based on the jet          #
# collections produced by the (Fat)jetProducer.         #
#######################################################*/


// system include files
#include <memory>
#include <typeinfo>
#include <algorithm>    // std::min

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/// Custom includes:
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"


// NAMESPACES:
using namespace std;
using namespace edm;
// \NAMESPACES

//
// class declaration
//

class JetFilter : public edm::EDFilter {
   public:
      explicit JetFilter(const edm::ParameterSet&);
      ~JetFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      double cut_pt_, cut_eta_;
      bool cut_smu_;
      int nevents, nevents_passed;
      
	// ?
	EDGetTokenT<vector<pat::Jet>> jetCollection_;
	EDGetTokenT<TriggerResults> triggerResults_;
	EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
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
JetFilter::JetFilter(const edm::ParameterSet& iConfig):
	cut_pt_(iConfig.getParameter<double>("cut_pt")),
	cut_eta_(iConfig.getParameter<double>("cut_eta")),
	cut_smu_(iConfig.getParameter<bool>("cut_smu")),
	
	//?
	jetCollection_(consumes<vector<pat::Jet>>(iConfig.getParameter<InputTag>("jetCollection"))),
	triggerResults_(consumes<TriggerResults>(iConfig.getParameter<InputTag>("triggerResults"))),
	triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<InputTag>("triggerPrescales")))
{
//	cout << "=================================" << endl;
//	cout << "FILTER START" << endl;
//	cout << "=================================" << endl;
	nevents = 0;
	nevents_passed = 0;
}


JetFilter::~JetFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
JetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
//	cout << "here 97" << endl;
	nevents ++;
	
	// Optional single-muon trigger cut:
	Handle<TriggerResults> results;
	iEvent.getByToken(triggerResults_, results);
	Handle<pat::PackedTriggerPrescales> prescales;
	iEvent.getByToken(triggerPrescales_, prescales);
	
	const TriggerNames& names = iEvent.triggerNames(*results);
	string trigger_name;
	for (unsigned int i=0; i < results->size(); ++i) {
		string full_name = names.triggerName(i);
		size_t found = full_name.find("HLT_Mu50_v");
		if (found != string::npos) {		// If contains
			trigger_name = full_name;
//			cout << trigger_name << endl;
			break;
		}
	}
	bool smu_result = results->accept(names.triggerIndex(trigger_name));
//	cout << trigger_name << "   " << smu_result << endl;
	
	if (cut_smu_){
		if (smu_result) {
			nevents_passed ++;
			return true;
		}
		else return false;
	}
	
	
	// Get CA12 jets:
	Handle<vector<pat::Jet>> jets_edm;
	iEvent.getByToken(jetCollection_, jets_edm);
	
	// Ensure the top two jets have pT greater than cut_pt_:
//	for (vector<pat::Jet>::const_iterator jet = jets_edm->begin(); jet != jets_edm->end(); ++ jet) {
//		cout << jet->pt() << endl;
//	}
	if (jets_edm.isValid()) {
//		cout << jets_edm->size() << endl;
		if (jets_edm->size() >= 2) {
			pat::Jet jet0 = jets_edm->at(0);
			pat::Jet jet1 = jets_edm->at(1);
			if (min(jet0.pt(), jet1.pt()) > cut_pt_ && ((max(abs(jet0.eta()), abs(jet1.eta())) < cut_eta_) || cut_eta_ < 0)) {
				nevents_passed ++;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

// ------------ method called once each job just before starting event loop  ------------
void 
JetFilter::beginJob() {
//	cout << "=================================" << endl;
//	cout << "FILTER BEGIN" << endl;
//	cout << "=================================" << endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetFilter::endJob() {
	cout << "=================================" << endl;
	cout << "JetFilter information:" << endl;
	cout << "pT cut = " << cut_pt_ << " GeV" << endl;
	cout << "abs(eta) cut = " << cut_eta_ << endl;
	cout << "total events: " << nevents << endl;
	cout << "accepted events: " << nevents_passed << endl;
	cout << "=================================" << endl;
}

// ------------ method called when starting to processes a run  ------------
/*
void
JetFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
JetFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
JetFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
JetFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(JetFilter);
