import FWCore.ParameterSet.Config as cms

process = cms.Process("CMSDASMET")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.TFileService = cms.Service("TFileService",
    fileName = cms.string("cmsdas_met_exercise3.root"),
    closeFileFast = cms.untracked.bool(True)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring([
            'root://cmsxrootd.fnal.gov//store/user/cmsdas/2025/short_exercises/MET/DYJetsToLL_M50_amcatnloFXFX.root',
            ]
                                      )
    )

process.cmsdasmetexercise3 = cms.EDAnalyzer('CMSDAS_MET_AnalysisExercise3',
                                   vertices  = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                   mets       = cms.InputTag("slimmedMETs"),
                                   metspuppi  = cms.InputTag("slimmedMETsPuppi"),
                                   muons     = cms.InputTag("slimmedMuons"),
                                   )


process.p = cms.Path(process.cmsdasmetexercise3)
