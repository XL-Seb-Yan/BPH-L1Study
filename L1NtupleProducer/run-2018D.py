
import FWCore.ParameterSet.Config as cms

process = cms.Process("L1NTUPLE")

# conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond_condDBv2 import autoCond
process.GlobalTag.globaltag = cms.string( autoCond['run2_mc'] )

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
# get and parse the command line arguments

options.register('skipEvents',
                 0,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")
options.register('outFile',
                 'L1Ntuple.root',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 'Output file')
options.register('inputFile',
                 '',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 'Input file')
                                  

options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)


# Input source
process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(options.skipEvents), #added
    fileNames = cms.untracked.vstring(options.inputFile),
    secondaryFileNames = cms.untracked.vstring()
)

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1Ntuple.root')
)

# producer under test
process.load("L1Trigger.L1TNtuples.l1UpgradeTree_cfi")

process.p = cms.Path(
  process.l1UpgradeTree
)

