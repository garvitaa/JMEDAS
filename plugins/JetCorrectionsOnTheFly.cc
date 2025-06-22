// Implementation of template class: JetCorrectionsOnTheFly
// Description:  Example of simple EDAnalyzer correcting jets "on the fly".
// Author: S. Rappoccio
// Date: 07 Dec 2011
#include "Analysis/JMEDAS/plugins/JetCorrectionsOnTheFly.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include <TFile.h>
#include <cmath>


////////////////////////////////////////////////////////////////////////////////////////
JetCorrectionsOnTheFly::JetCorrectionsOnTheFly(edm::ParameterSet const& cfg) :
  jetToken_   (consumes<edm::View<reco::Jet>>(cfg.getParameter<edm::InputTag>("jetSrc"))),
  rhoToken_   (consumes<double>(cfg.getParameter<edm::InputTag>("rhoSrc"))),
  pvToken_    (consumes<std::vector<reco::Vertex>>(cfg.getParameter<edm::InputTag>("pvSrc"))),
  jecPayloadNames_( cfg.getParameter<std::vector<std::string> >("jecPayloadNames") ), // JEC level payloads
  jecUncName_( cfg.getParameter<std::string>("jecUncName") )                          // JEC uncertainties
{
  usesResource("TFileService");

  // Get the TFileService to handle plots
  edm::Service<TFileService> fileService;

  // Make histograms in that directory
  corrPt = fileService->make<TH1F>("corrPt", "Corrected pt", 50, 0., 500.);
  corrPtUp = fileService->make<TH1F>("corrPtUp", "Corrected pt Plus Uncertainty ", 50, 0., 500.);
  corrPtDown = fileService->make<TH1F>("corrPtDown", "Corrected pt Minus Uncertainty ", 50, 0., 500.);
  
  //Get the factorized jet corrector parameters. 
  std::vector<JetCorrectorParameters> vPar;
  for ( std::vector<std::string>::const_iterator payloadBegin = jecPayloadNames_.begin(),
	  payloadEnd = jecPayloadNames_.end(), ipayload = payloadBegin; ipayload != payloadEnd; ++ipayload ) {
    JetCorrectorParameters pars(*ipayload);
    vPar.push_back(pars);
  }
  
  // Make the FactorizedJetCorrector and Uncertainty
  jec_ = std::make_shared<FactorizedJetCorrector>(vPar);
  jecUnc_ = std::make_shared<JetCorrectionUncertainty>(jecUncName_);

}

JetCorrectionsOnTheFly::~JetCorrectionsOnTheFly()
{
}
////////////////////////////////////////////////////////////////////////////////////////
void JetCorrectionsOnTheFly::beginJob() 
{
}
////////////////////////////////////////////////////////////////////////////////////////
void JetCorrectionsOnTheFly::analyze(edm::Event const& evt, edm::EventSetup const& iSetup) 
{
  // Get the jets
  edm::Handle<edm::View<reco::Jet>> h_jets;
  evt.getByToken( jetToken_, h_jets );

  // Get the mean pt per unit area ("rho")
  edm::Handle<double> h_rho;
  evt.getByToken( rhoToken_, h_rho );

  // Get the primary vertex collection
  edm::Handle<std::vector<reco::Vertex>> h_pv;
  evt.getByToken( pvToken_, h_pv );  

  // Loop over jets, get the correction, and plot
  // the corrected jet pt
  for ( edm::View<reco::Jet>::const_iterator ibegin = h_jets->begin(),
	  iend = h_jets->end(), ijet = ibegin; ijet != iend; ++ijet ) {
    // get a copy of the uncorrected p4
    reco::Candidate::LorentzVector uncorrJet;
    // The pat::Jet "knows" if it has been corrected, so here
    // we can "uncorrect" the entire jet to apply the corrections
    // we want here. 
    pat::Jet const * pJet = dynamic_cast<pat::Jet const *>( &*ijet );
    if ( pJet != 0 ) {
      uncorrJet = pJet->correctedP4(0);
    } 
    // Otherwise, if we do not have pat::Jets on input, we just assume
    // the user has not corrected them upstream and use it as raw. 
    else {
      uncorrJet = ijet->p4();
    }
    // Get the correction itself. This needs the jet area,
    // the rho value, and the number of primary vertices to
    // run the correction. 
    jec_->setJetEta( uncorrJet.eta() );
    jec_->setJetPt ( uncorrJet.pt() );
    jec_->setJetE  ( uncorrJet.energy() );
    jec_->setJetA  ( ijet->jetArea() );
    jec_->setRho   ( *(h_rho.product()) );
    jec_->setNPV   ( h_pv->size() );
    double corr = jec_->getCorrection();

    // Now access the uncertainty on the jet energy correction.
    // Pass the corrected jet pt to the "setJetPt" method. 

    // Access the "scale up" uncertainty (+1)
    jecUnc_->setJetEta( uncorrJet.eta() );
    jecUnc_->setJetPt( corr * uncorrJet.pt() );
    double corrUp = corr * (1 + fabs(jecUnc_->getUncertainty(1)));
    // Access the "scale down" uncertainty (-1)
    jecUnc_->setJetEta( uncorrJet.eta() );
    jecUnc_->setJetPt( corr * uncorrJet.pt() );
    double corrDown = corr * ( 1 - fabs(jecUnc_->getUncertainty(-1)) );

    // Now plot the value. 
    corrPt->Fill( corr * uncorrJet.pt() );
    corrPtUp->Fill( corrUp * uncorrJet.pt() );
    corrPtDown->Fill( corrDown * uncorrJet.pt() );
  }
  
}
////////////////////////////////////////////////////////////////////////////////////////
void JetCorrectionsOnTheFly::endJob() 
{
}

////////////////////////////////////////////////////////////////////////////////////////
void JetCorrectionsOnTheFly::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("jetSrc");
  desc.add<edm::InputTag>("rhoSrc");
  desc.add<edm::InputTag>("pvSrc");
  desc.add<std::vector<std::string>>("jecPayloadNames");
  desc.add<std::string>("jecUncName");
  descriptions.addDefault(desc);
}

////////////////////////////////////////////////////////////////////////////////////////
/////////// Register Modules ////////
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(JetCorrectionsOnTheFly);
