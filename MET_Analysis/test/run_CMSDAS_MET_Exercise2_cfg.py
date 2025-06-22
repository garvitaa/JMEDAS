import FWCore.ParameterSet.Config as cms

process = cms.Process("CMSDASMET")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1


process.TFileService = cms.Service("TFileService", fileName = cms.string("./outputs/cmsdas_met_exercise2.root") )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring([
            'root://eosuser.cern.ch//eos/user/c/cmsdas/2025/MET/DYJetsToLL_M50_amcatnloFXFX.root',
            ]
                                      )
    )

process.cmsdasmetexercise2 = cms.EDAnalyzer('CMSDAS_MET_AnalysisExercise2',
                                    doprints = cms.bool(True),
                                    mets     = cms.InputTag("slimmedMETs")
                                    )

process.p = cms.Path(process.cmsdasmetexercise2)
