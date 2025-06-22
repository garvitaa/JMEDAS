// Example of jet substructure example: JetPlotsExample
// Description:  Example of simple EDAnalyzer to check into jet substructure
// Author: S. Rappoccio
// Date: 07 Dec 2011
#ifndef JetPlotsExample_h
#define JetPlotsExample_h
#include <TH1.h>
#include <TFile.h>
#include "TNamed.h"
#include <vector>
#include <map>
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

class JetPlotsExample : public edm::one::EDAnalyzer<edm::one::SharedResources>
   {
     public:
       explicit JetPlotsExample(edm::ParameterSet const& cfg);
       
       using ModuleType = JetPlotsExample;
       static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
       static void prevalidate(edm::ConfigurationDescriptions& descriptions) {}
       static const std::string& baseType();
       
     private:
       void beginJob() override;
       void analyze(edm::Event const& e, edm::EventSetup const& iSetup) override;
       void endJob() override;

       edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;      /// Jet source to plot
       double                   leadJetPtMin_;/// Minimum leading jet pt, in place to be above trigger threshold
       double                   jetPtMin_;    /// Minimum jet pt to plot
       bool                     plotSubstructure_; /// Plot substructure quantities? 

       TH1F * hPt;
       TH1F * hRapidity;
       TH1F * hPhi;
       TH1F * hMass;
       TH1F * hArea;
       TH1F * hMassPruned;
       TH1F * hMassTrimmed;
       TH1F * hMassFiltered;
       TH1F * hTau21;
       TH1F * hTau32;
       TH1F * hCATopMinMass;
       TH1F * hCATopNsubjets;
       
	 
   };
#endif
