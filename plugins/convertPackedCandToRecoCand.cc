////////////////////////////////////////////////////////////////////////////////
//
// convertPackedCandToRecoCand
// ---------------------------
//
//                          01/08/2015 Alexx Perloff <aperloff@physics.tamu.edu>
////////////////////////////////////////////////////////////////////////////////


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
 
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include <iostream>
#include <memory>

using namespace std;
using namespace edm;
using namespace reco;


////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
class convertPackedCandToRecoCand : public edm::one::EDProducer<>
{
public:
  // construction/destruction
  explicit convertPackedCandToRecoCand(const edm::ParameterSet& iConfig);
  ~convertPackedCandToRecoCand() {;}
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  // member functions
  void produce(edm::Event& iEvent,const edm::EventSetup& iSetup) override;
  void endJob() override;

private:
  // member data
  edm::EDGetTokenT<edm::View<pat::PackedCandidate>> srcToken_;

  std::string  moduleName_;

};




////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
convertPackedCandToRecoCand::convertPackedCandToRecoCand(const edm::ParameterSet& iConfig)
  : srcToken_(consumes<edm::View<pat::PackedCandidate>>(iConfig.getParameter<InputTag>("src")))
  , moduleName_(iConfig.getParameter<string>("@module_label"))
{
  //produces<reco::PFCandidateCollection>("convertedPackedPFCandidates");
  produces<reco::PFCandidateCollection>();
}


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void convertPackedCandToRecoCand::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{

  edm::Handle<edm::View<pat::PackedCandidate>> packedCands_;
  //edm::Handle<View<Candidate> > packedCands_;
  
  iEvent.getByToken(srcToken_, packedCands_);
  
  size_t nCands = (size_t)packedCands_->size();
  
  auto recoCands = std::make_unique<vector<reco::PFCandidate>>();
  recoCands->reserve(packedCands_->size());

  reco::PFCandidate dummy;
    
  for(unsigned int iCand = 0; iCand<nCands; iCand++) {
    //reco::CandidateBaseRef intCandBaseRef = packedCands_->at(iCand).masterClone();
    //reco::PFCandidateRef intPFCandRef = intCandBaseRef.castTo<reco::PFCandidateRef>();
    //recoCands->push_back(*intPFCandRef);
    
    //reco::PFCandidate intPFCand = static_cast<reco::PFCandidate>(packedCands_->at(iCand));

    reco::PFCandidate intPFCand(packedCands_->at(iCand).charge(),packedCands_->at(iCand).p4(),dummy.translatePdgIdToType(packedCands_->at(iCand).pdgId()));
    recoCands->push_back(intPFCand);
  }
  
  //iEvent.put(recoCands,"convertedPackedPFCandidates");
  iEvent.put(std::move(recoCands));
}


//______________________________________________________________________________
void convertPackedCandToRecoCand::endJob()
{
}

//______________________________________________________________________________
void convertPackedCandToRecoCand::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("src");
  descriptions.addDefault(desc);
}


////////////////////////////////////////////////////////////////////////////////
// plugin definition
////////////////////////////////////////////////////////////////////////////////

DEFINE_FWK_MODULE(convertPackedCandToRecoCand);
