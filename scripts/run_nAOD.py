#!/usr/bin/env python
import os, sys
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from importlib import import_module
from PhysicsTools.NanoAODTools.postprocessing.framework.postprocessor import PostProcessor
from PhysicsTools.NanoAODTools.postprocessing.framework.datamodel import Collection, Object
from PhysicsTools.NanoAODTools.postprocessing.framework.eventloop import Module

class ExampleAnalysis(Module):
    def __init__(self):
        self.writeHistFile=True
        self.puppimetBranchName= "PuppiMET"
        self.rawmetBranchName= "RawMET"
        self.pfmetBranchName= "MET"
        self.flagBranchName= "Flag"

    def beginJob(self,histFile=None,histDirName=None):
        Module.beginJob(self,histFile,histDirName)

        self.h_dimuonmass=ROOT.TH1F("dimuon_mass", "dimuon_mass", 60, 60, 120)
        self.h_zpt=ROOT.TH1F("z_pt", "z pt", 60, 0, 60)
        self.h_rawmet=ROOT.TH1F('rawmet',   'rawmet',   100, 0, 50)
        self.h_pfmet=ROOT.TH1F('pfmet',   'pfmet',   100, 0, 50)
        self.h_puppimet=ROOT.TH1F('puppimet',   'puppimet',   100, 0, 50)
        self.addObject(self.h_dimuonmass)
        self.addObject(self.h_zpt)
        self.addObject(self.h_rawmet )
        self.addObject(self.h_pfmet )
        self.addObject(self.h_puppimet )

    def analyze(self, event):
        jets = Collection(event, "Jet")
        muons = Collection(event, "Muon")
        pfmet = Object(event, self.pfmetBranchName)
        rawmet = Object(event, self.rawmetBranchName)
        puppimet = Object(event, self.puppimetBranchName)
        flag= Object(event,self.flagBranchName)

        #print flag.goodVertices, flag.HBHENoiseFilter, flag.HBHENoiseIsoFilter, flag.EcalDeadCellTriggerPrimitiveFilter, flag.BadPFMuonFilter, flag.ecalBadCalibFilter
        if (flag.goodVertices  == False or flag.HBHENoiseFilter == False or flag.HBHENoiseIsoFilter  == False or flag.EcalDeadCellTriggerPrimitiveFilter  == False or  flag.BadPFMuonFilter  == False or flag.ecalBadCalibFilter == False) : return False

        mu0 = ROOT.TLorentzVector()
        mu0.SetPtEtaPhiM(muons[0].pt, muons[0].eta, muons[0].phi, muons[0].mass)
        mu1 = ROOT.TLorentzVector()
        mu1.SetPtEtaPhiM(muons[1].pt, muons[1].eta, muons[1].phi, muons[1].mass)

        mass_dimuon = (mu0 + mu1).M()
        zpt = (mu0 + mu1).Pt()
        self.h_dimuonmass.Fill(mass_dimuon)

        #select events with at least 2 muons
        if abs(mass_dimuon-91.0) < 10.0:
            self.h_zpt.Fill(zpt)
            self.h_rawmet.Fill(rawmet.pt) #fill histogram
            self.h_pfmet.Fill(pfmet.pt) #fill histogram
            self.h_puppimet.Fill(puppimet.pt) #fill histogram
        return True


files=[
    "root://cmsxrootd.fnal.gov//store/user/cmsdas/2021/short_exercises/METandPU/DYJetsToLL_M50_amcatnloFXFX_Nano.root",
]

preselection="nMuon >= 2 && Muon_pt[0] > 25.0 && Muon_pt[1] > 25.0 && fabs(Muon_eta[0]) < 2.4 && fabs(Muon_eta[1]) < 2.4 && Muon_pfRelIso04_all[0] < 0.15 && Muon_pfRelIso04_all[1] < 0.15"
p=PostProcessor(".",files,cut=preselection,branchsel=None,modules=[ExampleAnalysis()],noOut=True,histFileName="histOut.root",histDirName="plots")
p.run()


