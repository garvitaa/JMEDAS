import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10)  )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                        '/store/mc/RunIISummer20UL18MiniAODv2/QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/2430000/06232CB0-5397-074F-ABE9-2C2414756085.root'

                            )
                            )

process.demo = cms.EDAnalyzer("MiniAnalyzer",
                                  vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                  jets = cms.InputTag("slimmedJets"),
                                  fatjets = cms.InputTag("slimmedJetsAK8")
                              )

process.p = cms.Path(process.demo)
