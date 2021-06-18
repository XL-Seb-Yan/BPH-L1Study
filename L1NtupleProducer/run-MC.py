import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing('analysis')
options.parseArguments()

process = cms.Process("L1NTUPLE")

# conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond_condDBv2 import autoCond
process.GlobalTag.globaltag = cms.string(autoCond['run2_mc'])

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck")
)

# import FWCore.PythonUtilities.LumiList as LumiList
# import FWCore.ParameterSet.Types as CfgTypes
# process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
# myLumis = LumiList.LumiList(filename = "JSON_dataNtuples.txt").getCMSSWString().split(',')
# process.source.lumisToProcess.extend(myLumis) 

# producer under test
# process.load("L1Trigger.L1TNtuples.l1UpgradeTree_cfi")
process.load("BPHL1Study.L1NtupleProducer.l1UpgradeTree_cfi")
process.genTree = cms.EDAnalyzer("L1GenTreeProducerMINIAOD",
    genJetToken     = cms.untracked.InputTag("ak4GenJets"),
    # genParticleToken = cms.untracked.InputTag("prunedGenParticles"),
    genParticleToken = cms.untracked.InputTag("genParticles"),
    genInfoToken = cms.InputTag("generator")
)

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1Ntuple.root')
)

process.p = cms.Path(
  process.l1UpgradeTree * process.genTree
)