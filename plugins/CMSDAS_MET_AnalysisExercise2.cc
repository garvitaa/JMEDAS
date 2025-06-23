// -*- C++ -*-
//
// Package:    CMSDAS_MET/CMSDAS_MET_AnalysisExercise2
// Class:      CMSDAS_MET_AnalysisExercise2
//
/**\class CMSDAS_MET_AnalysisExercise2 CMSDAS_MET_AnalysisExercise2.cc CMSDAS_MET/CMSDAS_MET_Analysis/plugins/CMSDAS_MET_AnalysisExercise2.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Saranya Samik Ghosh
//         Created:  Fri, 28 Aug 2020 12:33:08 GMT
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
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

//ROOT includes                                                                                                                                               
#include "TTree.h"
#include <TFile.h>
#include <TROOT.h>
#include "TBranch.h"
#include <string>
#include <vector>
#include "TSystem.h"
#include "TVector3.h"
#include "TLorentzVector.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


class CMSDAS_MET_AnalysisExercise2 : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit CMSDAS_MET_AnalysisExercise2(const edm::ParameterSet&);
      ~CMSDAS_MET_AnalysisExercise2();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      bool doprints_;
      edm::EDGetTokenT<pat::METCollection> metToken_;

  // various met corrections                                                                                                                                                                                
  TH1F *h_metpt;
  TH1F *h_metrawpt;
  TH1F *h_metsmearpt;

  // various met uncertainties                                                                                                                                                                              
  TH1F *h_metjesuppt;
  TH1F *h_metjesdnpt;
  TH1F *h_metjeruppt;
  TH1F *h_metjerdnpt;
  TH1F *h_metuncuppt;
  TH1F *h_metuncdnpt;

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
CMSDAS_MET_AnalysisExercise2::CMSDAS_MET_AnalysisExercise2(const edm::ParameterSet& iConfig)
 :
  doprints_(iConfig.getParameter<bool>("doprints")),
  metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets")))
{
   //now do what ever initialization is needed
   edm::Service<TFileService> file;

   h_metpt = file->make<TH1F>("h_metpt" , "p_{T}^{miss}" , 20 , 0 , 200 );
   h_metrawpt = file->make<TH1F>("h_metrawpt" , "p_{T}^{miss}" , 20 , 0 , 200 );
   h_metsmearpt = file->make<TH1F>("h_metsmearpt" , "p_{T}^{miss}" , 20 , 0 , 200 );

   h_metjesuppt = file->make<TH1F>("h_metjesuppt" , "p_{T}^{miss}" , 20 , 0 , 200 );
   h_metjesdnpt = file->make<TH1F>("h_metjesdnpt" , "p_{T}^{miss}" , 20 , 0 , 200 );
   h_metjeruppt = file->make<TH1F>("h_metjeruppt" , "p_{T}^{miss}" , 20 , 0 , 200 );
   h_metjerdnpt = file->make<TH1F>("h_metjerdnpt" , "p_{T}^{miss}" , 20 , 0 , 200 );
   h_metuncuppt = file->make<TH1F>("h_metuncuppt" , "p_{T}^{miss}" , 20 , 0 , 200 );
   h_metuncdnpt = file->make<TH1F>("h_metuncdnpt" , "p_{T}^{miss}" , 20 , 0 , 200 );

}


CMSDAS_MET_AnalysisExercise2::~CMSDAS_MET_AnalysisExercise2()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
CMSDAS_MET_AnalysisExercise2::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   typedef math::XYZTLorentzVector LorentzVector;

   edm::Handle<pat::METCollection> mets;
   iEvent.getByToken(metToken_, mets);
   const pat::MET &met = mets->front();

   // MET unceetainties and different levels of corrections                                                                                                                                                 
   if (doprints_) {
     std::cout << " MET : \n";
     std::cout << "  pt [GeV] = " << met.pt() << "\n";
     std::cout << "  phi [rad] = " << met.phi() << "\n";
     std::cout << " MET uncertainties : \n";
     std::cout << "  JES up/down [GeV] = " << met.shiftedPt(pat::MET::JetEnUp) << "/" << met.shiftedPt(pat::MET::JetEnDown) << "\n";
     std::cout << "  JER up/down [GeV] = " << met.shiftedPt(pat::MET::JetResUp) << "/" << met.shiftedPt(pat::MET::JetResDown) << "\n";
     std::cout << "  Unc up/down [GeV] = " << met.shiftedPt(pat::MET::UnclusteredEnUp) << "/" << met.shiftedPt(pat::MET::UnclusteredEnDown) << "\n";
     std::cout << " MET corrections : \n";
     std::cout << "  Raw PFMET pt [GeV] = " << met.uncorPt() << "\n";
     std::cout << "  PFMET-type1 pt [GeV] = " << met.pt() << "\n";
     std::cout << "  Smeared PFMET-type1 pt [GeV] = " << met.corPt(pat::MET::Type1Smear) << "\n";
     std::cout << "\n";
   }

   // store vars to the tree                                                                                                                                                                                
   h_metpt->Fill(met.pt());
   h_metrawpt->Fill(met.uncorPt());
   h_metsmearpt->Fill(met.corPt(pat::MET::Type1Smear));

   h_metjesuppt->Fill(met.shiftedPt(pat::MET::JetEnUp));
   h_metjesdnpt->Fill(met.shiftedPt(pat::MET::JetEnDown));
   h_metjeruppt->Fill(met.shiftedPt(pat::MET::JetResUp));
   h_metjerdnpt->Fill(met.shiftedPt(pat::MET::JetResDown));
   h_metuncuppt->Fill(met.shiftedPt(pat::MET::UnclusteredEnUp));
   h_metuncdnpt->Fill(met.shiftedPt(pat::MET::UnclusteredEnDown));

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
CMSDAS_MET_AnalysisExercise2::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
CMSDAS_MET_AnalysisExercise2::endJob()
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CMSDAS_MET_AnalysisExercise2::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(CMSDAS_MET_AnalysisExercise2);
