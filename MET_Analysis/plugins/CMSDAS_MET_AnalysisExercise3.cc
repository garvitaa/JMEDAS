// -*- C++ -*-
//
// Package:    CMSDAS_MET/CMSDAS_MET_AnalysisExercise3
// Class:      CMSDAS_MET_AnalysisExercise3
//
/**\class CMSDAS_MET_AnalysisExercise3 CMSDAS_MET_AnalysisExercise3.cc CMSDAS_MET/CMSDAS_MET_Analysis/plugins/CMSDAS_MET_AnalysisExercise3.cc

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


class CMSDAS_MET_AnalysisExercise3 : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit CMSDAS_MET_AnalysisExercise3(const edm::ParameterSet&);
      ~CMSDAS_MET_AnalysisExercise3();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
  edm::EDGetTokenT<pat::MuonCollection> muonToken_;
  edm::EDGetTokenT<pat::METCollection> metToken_;
  edm::EDGetTokenT<pat::METCollection> metpuppiToken_;

  TTree *tree = new TTree("tree","tree");

  edm::RunNumber_t irun;
  edm::EventNumber_t ievent;
  edm::LuminosityBlockNumber_t ilumiblock;
  edm::Timestamp itime;

  size_t run, event, lumi, time;

  unsigned int npv;

  Float_t mett1pt;
  Float_t mett1phi;
  Float_t metrawpt;
  Float_t metrawphi;
  Float_t metpuppipt;
  Float_t metpuppiphi;

  Float_t uparat1;
  Float_t uperpt1;
  Float_t upararaw;
  Float_t uperpraw;
  Float_t uparapuppi;
  Float_t uperppuppi;
  Float_t upara_illia;
  Float_t uperp_illia;

  Int_t ndimu;
  Float_t dimupt;
  Float_t dimueta;
  Float_t dimuphi;
  Float_t dimuq;
  Float_t dimumass;

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
CMSDAS_MET_AnalysisExercise3::CMSDAS_MET_AnalysisExercise3(const edm::ParameterSet& iConfig)
 :
  vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
  metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
  metpuppiToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("metspuppi")))
{
  //now do what ever initialization is needed                                                                                                                                                              
  usesResource("TFileService");
  edm::Service<TFileService> file;

  // h_metpt = file->make<TH1F>("h_metpt" , "p_{T}^{miss}" , 20 , 0 , 200 );                                                                                                                               

  tree = file->make<TTree>("tree","DAS 2019 MEtandPU");
  tree->Branch("npv",&npv,"npv/i");

  tree->Branch("mett1pt"     , &mett1pt     , "mett1pt/F"     );
  tree->Branch("mett1phi"    , &mett1phi    , "mett1phi/F"    );
  tree->Branch("metrawpt"    , &metrawpt    , "metrawpt/F"    );
  tree->Branch("metrawphi"   , &metrawphi   , "metrawphi/F"   );
  tree->Branch("metpuppipt"  , &metpuppipt  , "metpuppipt/F"  );
  tree->Branch("metpuppiphi" , &metpuppiphi , "metpuppiphi/F" );

  tree->Branch("uparat1"    , &uparat1    , "uparat1/F"    );
  tree->Branch("uperpt1"    , &uperpt1    , "uperpt1/F"    );
  tree->Branch("upararaw"   , &upararaw   , "upararaw/F"   );
  tree->Branch("uperpraw"   , &uperpraw   , "uperpraw/F"   );
  tree->Branch("uparapuppi" , &uparapuppi , "uparapuppi/F" );
  tree->Branch("uperppuppi" , &uperppuppi , "uperppuppi/F" );
  tree->Branch("upara_illia" , &upara_illia , "upara_illia/F" );
  tree->Branch("uperp_illia" , &uperp_illia , "uperp_illia/F" );

  tree->Branch("ndimu"    , &ndimu    , "ndimu/I"    );
  tree->Branch("dimupt"   , &dimupt   , "dimupt/F"   );
  tree->Branch("dimueta"  , &dimueta  , "dimueta/F"  );
  tree->Branch("dimuphi"  , &dimuphi  , "dimuphi/F"  );
  tree->Branch("dimuq"    , &dimuq    , "dimuq/F"    );
  tree->Branch("dimumass" , &dimumass , "dimumass/F" );

}


CMSDAS_MET_AnalysisExercise3::~CMSDAS_MET_AnalysisExercise3()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
CMSDAS_MET_AnalysisExercise3::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   typedef math::XYZTLorentzVector LorentzVector;

   edm::Handle<reco::VertexCollection> vertices;
   iEvent.getByToken(vtxToken_, vertices);
   if (vertices->empty()) return; // skip the event if no PV found                                                                                                                                          
   const reco::Vertex &PV = vertices->front();

   edm::Handle<pat::MuonCollection> muons;
   iEvent.getByToken(muonToken_, muons);

   edm::Handle<pat::METCollection> mets;
   iEvent.getByToken(metToken_, mets);
   const pat::MET &met = mets->front();

   edm::Handle<pat::METCollection> metspuppi;
   iEvent.getByToken(metpuppiToken_, metspuppi);
   const pat::MET &metpuppi = metspuppi->front();

   // event variables                                                                                                                                                                                       
   irun       = iEvent.id().run();
   ievent     = iEvent.id().event();
   ilumiblock = iEvent.id().luminosityBlock();
   itime      = iEvent.time();

   run   = (size_t)irun;
   event = (size_t)ievent;
   lumi  = (size_t)ilumiblock;
   time  = (size_t)((iEvent.time().value())>>32);


   // muon collection                                                                                                                                                                                       
   std::vector<pat::Muon> dimu;
   for (const pat::Muon &mu : *muons) {
     if (mu.pt() < 10 || fabs(mu.eta())>2.4) { continue; }
     if (!mu.isLooseMuon())                  { continue; }

     float isodbeta = (mu.pfIsolationR04().sumChargedHadronPt + std::max(0., mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt))/mu.pt();

     if (isodbeta>0.2)  { continue; }

     float d0 = mu.innerTrack().isNonnull() ? -1.*mu.innerTrack()->dxy(PV.position()) : 0 ;
     float dz = mu.innerTrack().isNonnull() ? mu.innerTrack()->dz(PV.position()) : 0 ;

     if (std::fabs(d0)>0.05) { continue; }
     if (std::fabs(dz)>0.10) { continue; }

     //di-mu events                                                                                                                                                                                         
     if (mu.pt()>=10) { dimu.push_back(mu); }

   }


   // skip the event if less than 2 muons                                                                                                                                                                   

   if (dimu.size()<1) { return; }


   // calculate boson recoil                                                                                                                                                                                
   TLorentzVector metT1p4;       metT1p4.SetPtEtaPhiM(met.pt(),0.,met.phi(),0.);
   TLorentzVector metRawp4;      metRawp4.SetPtEtaPhiM(met.uncorPt(),0.,met.uncorPhi(),0.);
   TLorentzVector metPuppip4;    metPuppip4.SetPtEtaPhiM(metpuppi.pt(),0.,metpuppi.phi(),0.);
   TLorentzVector recmetT1p4;    recmetT1p4.SetPtEtaPhiM(0.,0.,0.,0.);
   TLorentzVector recmetRawp4;   recmetRawp4.SetPtEtaPhiM(0.,0.,0.,0.);
   TLorentzVector recmetPuppip4; recmetPuppip4.SetPtEtaPhiM(0.,0.,0.,0.);
   TLorentzVector bosonp4;       bosonp4.SetPtEtaPhiM(0.,0.,0.,0.);
   float recmetT1_par_    = -9.;
   float recmetT1_per_    = -9.;
   float recmetRaw_par_   = -9.;
   float recmetRaw_per_   = -9.;
   float recmetPuppi_par_ = -9.;
   float recmetPuppi_per_ = -9.;
   float upara_illia_  = -9.;
   float uperp_illia_  = -9.;


   //di-mu events                                                                                                                                                                                           
   float dimupt_   = -9.;
   float dimueta_  = -9.;
   float dimuphi_  = -9.;
   float dimumass_ = -9.;
   float dimuq_    = 0.;
   if (dimu.size()>=2) {

     TLorentzVector mu1p4;      mu1p4.SetPtEtaPhiM(dimu[0].pt(),dimu[0].eta(),dimu[0].phi(),dimu[0].mass());
     TLorentzVector mu2p4;      mu2p4.SetPtEtaPhiM(dimu[1].pt(),dimu[1].eta(),dimu[1].phi(),dimu[1].mass());

     if (dimu[0].charge()*dimu[1].charge()<0.) {

       bosonp4 = mu1p4 + mu2p4;
       recmetT1p4    = metT1p4 + mu1p4 + mu2p4;
       recmetRawp4   = metRawp4 + mu1p4 + mu2p4;
       recmetPuppip4 = metPuppip4 + mu1p4 + mu2p4;
       float cosPhiT1     = (bosonp4.Px()*recmetT1p4.Px()+bosonp4.Py()*recmetT1p4.Py())/(bosonp4.Pt()*recmetT1p4.Pt());
       float sinPhiT1     = sqrt(1.-cosPhiT1*cosPhiT1);
       float cosPhiRaw    = (bosonp4.Px()*recmetRawp4.Px()+bosonp4.Py()*recmetRawp4.Py())/(bosonp4.Pt()*recmetRawp4.Pt());
       float sinPhiRaw    = sqrt(1.-cosPhiRaw*cosPhiRaw);
       float cosPhiPuppi  = (bosonp4.Px()*recmetPuppip4.Px()+bosonp4.Py()*recmetPuppip4.Py())/(bosonp4.Pt()*recmetPuppip4.Pt());
       float sinPhiPuppi  = sqrt(1.-cosPhiPuppi*cosPhiPuppi);
       recmetT1_par_    = cosPhiT1*recmetT1p4.Pt();
       recmetT1_per_    = sinPhiT1*recmetT1p4.Pt();
       recmetRaw_par_   = cosPhiRaw*recmetRawp4.Pt();
       recmetRaw_per_   = sinPhiRaw*recmetRawp4.Pt();
       recmetPuppi_par_ = cosPhiPuppi*recmetPuppip4.Pt();
       recmetPuppi_per_ = sinPhiPuppi*recmetPuppip4.Pt();


       dimupt_   = bosonp4.Pt();
       dimueta_  = bosonp4.Eta();
       dimuphi_  = bosonp4.Phi();
       dimumass_ = (mu1p4 + mu2p4).M();
       dimuq_    = dimu[0].charge()*dimu[1].charge();

       upara_illia_ = ((( -1.*metT1p4.Pt()*TMath::Cos(metT1p4.Phi()) - dimupt_ * TMath::Cos(dimuphi_))* dimupt_*TMath::Cos( dimuphi_ )+(-1.*metT1p4.Pt()* TMath::Sin(metT1p4.Phi() )- dimupt_*TMath::Sin(dimuphi_))* dimupt_*TMath::Sin(dimuphi_ ))/dimupt_ + dimupt_);
       uperp_illia_ = ((( -1.*metT1p4.Pt()*TMath::Cos(metT1p4.Phi() ) - dimupt_ * TMath::Cos( dimuphi_))* dimupt_*TMath::Sin( dimuphi_ )-(-1.*metT1p4.Pt()* TMath::Sin(metT1p4.Phi() )- dimupt_*TMath::Sin(dimuphi_))* dimupt_*TMath::Cos( dimuphi_ ))/dimupt_);

     }
   } // end of di-mu case 

   // store the variables                                                                                                                                                                                   
   npv = vertices->size();

   mett1pt     = met.pt();
   mett1phi    = met.phi();
   metrawpt    = met.uncorPt();
   metrawphi   = met.uncorPhi();
   metpuppipt  = metpuppi.pt();
   metpuppiphi = metpuppi.phi();
   uparat1    = recmetT1_par_;
   uperpt1    = recmetT1_per_;
   upararaw   = recmetRaw_par_;
   uperpraw   = recmetRaw_per_;
   uparapuppi = recmetPuppi_par_;
   uperppuppi = recmetPuppi_per_;
   upara_illia = upara_illia_;
   uperp_illia = uperp_illia_;

   ndimu    = dimu.size();
   dimupt   = dimupt_;
   dimueta  = dimueta_;
   dimuphi  = dimuphi_;
   dimumass = dimumass_;
   dimuq    = dimuq_;

   tree->Fill();

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
CMSDAS_MET_AnalysisExercise3::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
CMSDAS_MET_AnalysisExercise3::endJob()
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CMSDAS_MET_AnalysisExercise3::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(CMSDAS_MET_AnalysisExercise3);
