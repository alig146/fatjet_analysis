// -*- C++ -*-
//
// Package:    Analyzers/MiniAODAnalyzer
// Class:      MiniAODAnalyzer
// 
/**\class MiniAODAnalyzer MiniAODAnalyzer.cc Analyzers/MiniAODAnalyzer/plugins/MiniAODAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Tote Hughes
//         Created:  Thu, 08 Dec 2016 13:38:21 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

/// Custom includes:
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

// NAMESPACES:
using namespace std;
using namespace edm;
// :NAMESPACES

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class MiniAODAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MiniAODAnalyzer(const edm::ParameterSet&);
      ~MiniAODAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      int nevents;
      EDGetTokenT<vector<reco::GenParticle>> genParticles_;
      EDGetTokenT<vector<PileupSummaryInfo>> pileupInfo_;
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
MiniAODAnalyzer::MiniAODAnalyzer(const edm::ParameterSet& iConfig):
//	genParticles_(consumes<vector<reco::GenParticle>>(iConfig.getParameter<InputTag>("genParticles")))
	pileupInfo_(consumes<vector<PileupSummaryInfo>>(iConfig.getParameter<InputTag>("pileupInfo"))),
	triggerResults_(consumes<TriggerResults>(iConfig.getParameter<InputTag>("triggerResults"))),
	triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<InputTag>("triggerPrescales")))

{
   //now do what ever initialization is needed
   usesResource("TFileService");
   nevents = 0;

}


MiniAODAnalyzer::~MiniAODAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MiniAODAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
//	Handle<vector<reco::GenParticle>> genParticles;
//	iEvent.getByToken(genParticles_, genParticles);
	Handle<TriggerResults> triggerResults;
	iEvent.getByToken(triggerResults_, triggerResults);
	Handle<pat::PackedTriggerPrescales> triggerPrescales;
	iEvent.getByToken(triggerPrescales_, triggerPrescales);
	Handle<vector<PileupSummaryInfo>> pileupInfo;
	iEvent.getByToken(pileupInfo_, pileupInfo);
	
//	const TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults); 
//	
//    for (unsigned int i=0, n=triggerResults->size(); i < n; ++i) {
//        std::cout << "Trigger " << triggerNames.triggerName(i) <<
//                ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
//                ": " << (triggerResults->accept(i) ? "PASS" : "fail (or not run)")
//                << std::endl;
//    }

	cout << pileupInfo->size() << endl;
	
	nevents ++;
	
//	for (vector<reco::GenParticle>::const_iterator genParticle = genParticles->begin(); genParticle != genParticles->end(); ++ genParticle) {
////		if (abs(genParticle->pdgId()) == 6 && genParticle->pt() > 400) {
//		if (abs(genParticle->pdgId()) > 1000000) {
//			cout << nevents << "   " << genParticle->pt() << "   " << genParticle->pdgId() << "   " << genParticle->status() << endl;
//		}
//	}
}


// ------------ method called once each job just before starting event loop  ------------
void 
MiniAODAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniAODAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MiniAODAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MiniAODAnalyzer);
