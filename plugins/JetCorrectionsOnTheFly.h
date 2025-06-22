// Template class: JetCorrectionsOnTheFly
// Description:  Example of simple EDAnalyzer correcting jets "on the fly".
// Author: S. Rappoccio
// Date: 07 Dec 2011
#ifndef JetCorrectionsOnTheFly_h
#define JetCorrectionsOnTheFly_h
#include <vector>
#include <string>
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include <memory>

class JetCorrectionsOnTheFly : public edm::one::EDAnalyzer<edm::one::SharedResources>
   {
     public:
       explicit JetCorrectionsOnTheFly(edm::ParameterSet const& cfg);
       
       using ModuleType = JetCorrectionsOnTheFly;
       static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
       static void prevalidate(edm::ConfigurationDescriptions& descriptions) {}
       static const std::string& baseType();
       
     private:
       void beginJob() override;
       void analyze(edm::Event const& e, edm::EventSetup const& iSetup) override;
       void endJob() override;

       edm::EDGetTokenT<edm::View<reco::Jet>> jetToken_;
       edm::EDGetTokenT<double> rhoToken_;
       edm::EDGetTokenT<std::vector<reco::Vertex>> pvToken_;

       std::vector<std::string> jecPayloadNames_;
       std::string              jecUncName_;
       
	TH1F *corrPt;
	TH1F *corrPtUp;
	TH1F *corrPtDown;
       

       std::shared_ptr<JetCorrectionUncertainty> jecUnc_;
       std::shared_ptr<FactorizedJetCorrector> jec_;
   };
#endif
