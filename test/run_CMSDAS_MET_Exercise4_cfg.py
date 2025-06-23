import FWCore.ParameterSet.Config as cms

process = cms.Process("CMSDASMET")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1


#process.TFileService = cms.Service("TFileService", fileName = cms.string("./outputs/cmsdas_met_exercise5.root") )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring([
            'root://eosuser.cern.ch//eos/user/c/cmsdas/2025/MET/cmsdas_met_METFilters2.root',
            ]
                                      )
    )

process.cmsdasmetexercise5 = cms.EDAnalyzer('CMSDAS_MET_AnalysisExercise4',
                                   doprints = cms.bool(False),
                                   metFilters = cms.InputTag("TriggerResults","","RECO"),
                                   beamHaloFilterSel = cms.string("Flag_globalSuperTightHalo2016Filter"),
                                   hbheFilterSel = cms.string("Flag_HBHENoiseFilter"),
                                   hbheIsoFilterSel = cms.string("Flag_HBHENoiseIsoFilter"),
                                   ecalTPFilterSel = cms.string("Flag_EcalDeadCellTriggerPrimitiveFilter"),
                                   badPFMuonFilterSel = cms.string("Flag_BadPFMuonFilter"),
                                   badChargedCandFilterSel = cms.string("Flag_BadChargedCandidateFilter"),
                                   eeBadScFilterSel = cms.string("Flag_eeBadScFilter"),
                                   )

process.p = cms.Path(process.cmsdasmetexercise5)

