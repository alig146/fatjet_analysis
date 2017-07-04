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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "TH1.h"

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

class SampleWeightAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit SampleWeightAnalyzer(const edm::ParameterSet&);
      ~SampleWeightAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      int nevents;
      EDGetTokenT<GenEventInfoProduct> genInfo_;
      TH1F* h_w;
      TH1F* h_wabs;
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
SampleWeightAnalyzer::SampleWeightAnalyzer(const edm::ParameterSet& iConfig):
	genInfo_(consumes<GenEventInfoProduct>(iConfig.getParameter<InputTag>("genInfo")))
{
   //now do what ever initialization is needed
   edm::Service<TFileService> fs;
   nevents = 0;
   h_w = fs->make<TH1F>("w", "", 100, 0, 0);
   h_wabs = fs->make<TH1F>("wabs", "", 100, 0, 0);
}


SampleWeightAnalyzer::~SampleWeightAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
SampleWeightAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	Handle<GenEventInfoProduct> genInfo;
	iEvent.getByToken(genInfo_, genInfo);
//	cout << genInfo->weight() << endl;
	h_w->Fill(genInfo->weight());
	h_wabs->Fill(abs(genInfo->weight()));
	nevents ++;
}


// ------------ method called once each job just before starting event loop  ------------
void 
SampleWeightAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SampleWeightAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SampleWeightAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SampleWeightAnalyzer);
