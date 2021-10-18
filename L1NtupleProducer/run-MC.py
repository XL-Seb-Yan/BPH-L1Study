import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing('analysis')
options.parseArguments()

process = cms.Process("L1NTUPLE")

# conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond_condDBv2 import autoCond
process.GlobalTag.globaltag = cms.string("106X_upgrade2018_realistic_v11_L1v1")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.MessageLogger.cerr.threshold = "ERROR"

# Input source
# process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5))
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
process.l1UpgradeTree = cms.EDAnalyzer(
    "L1UpgradeTreeProducer",
    egToken = cms.untracked.InputTag("caloStage2Digis","EGamma"),
    tauTokens = cms.untracked.VInputTag(cms.InputTag("caloStage2Digis","Tau")),
    jetToken = cms.untracked.InputTag("caloStage2Digis","Jet"),
    muonToken = cms.untracked.InputTag("gmtStage2Digis","Muon"),
    muonLegacyToken = cms.untracked.InputTag("muonLegacyInStage2FormatDigis","legacyMuon"),
    sumToken = cms.untracked.InputTag("caloStage2Digis","EtSum"),
    maxL1Upgrade = cms.uint32(60)
)

process.genTree = cms.EDAnalyzer("L1GenTreeProducerMINIAOD",
    genJetToken     = cms.untracked.InputTag("ak4GenJets"),
    # genParticleToken = cms.untracked.InputTag("prunedGenParticles"),
    genParticleToken = cms.untracked.InputTag("prunedGenParticles"),
    genInfoToken = cms.InputTag("generator")
)
process.HLTTree = cms.EDAnalyzer("MiniAODTriggerProducer",
    bits = cms.InputTag("TriggerResults","","HLT"),
    objects = cms.InputTag("slimmedPatTrigger"),
    prescales = cms.InputTag("patTrigger")
)

process.HLTree = cms.EDAnalyzer("MiniAODHLProducer",
    electrons = cms.InputTag("slimmedElectrons"),
)

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1Ntuple.root')
)

process.p = cms.Path(
  process.l1UpgradeTree * process.genTree * process.HLTTree * process.HLTree
)