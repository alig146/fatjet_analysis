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

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/// Custom includes:
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


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
      double cut_pt_;
      int nevents, nevents_passed;
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
JetFilter::JetFilter(const edm::ParameterSet& iConfig) :
	cut_pt_(iConfig.getParameter<double>("cut_pt"))
{
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
	nevents ++;
	// Get CA12 jets:
	Handle<vector<pat::Jet>> jets_edm;
	iEvent.getByLabel("selectedPatJetsCA12PFCHS","", jets_edm);
	
	// Ensure the top two jets have pT greater than cut_pt_:
//	for (vector<pat::Jet>::const_iterator jet = jets_edm->begin(); jet != jets_edm->end(); ++ jet) {
//		cout << jet->pt() << endl;
//	}
	pat::Jet jet1 = jets_edm->at(1);
	if (jet1.pt() > cut_pt_) {
		nevents_passed ++;
		return true;
	}
	else {
		return false;
	}
}

// ------------ method called once each job just before starting event loop  ------------
void 
JetFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetFilter::endJob() {
	cout << nevents << endl;
	cout << nevents_passed << endl;
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
