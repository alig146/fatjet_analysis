import FWCore.ParameterSet.Config as cms
from Configuration.AlCa.autoCond import autoCond          # For automatically determining global tags

jec_tag_mc = "Summer16_23Sep2016V4_MC"
jec_tag_data = "Summer16_23Sep2016AllV4_DATA"

#global_tag = "Spring16_25nsV2"


process = cms.Process("jectxt")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
# define your favorite global tag:
process.GlobalTag.globaltag = cms.string('80X_mcRun2_asymptotic_2016_TrancheIV_v8')
#process.GlobalTag.globaltag = cms.string('74X_mcRun2_asymptotic_v4')
#process.GlobalTag.globaltag = cms.string(autoCond['run2_mc'])		# Set global tags automatically.
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")
process.readAK4PFchsMC    = cms.EDAnalyzer('JetCorrectorDBReader',  
        # below is the communication to the database 
        payloadName    = cms.untracked.string('AK4PFchs'),
        # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
        # but it is recommended to use the GT name that you retrieved the files from.
        globalTag      = cms.untracked.string(jec_tag_mc),		# Kludge: should include the GlobalTag name here.
        printScreen    = cms.untracked.bool(False),
        createTextFile = cms.untracked.bool(True)
)
process.readAK8PFchsMC = process.readAK4PFchsMC.clone(
	payloadName="AK8PFchs",
	globalTag=jec_tag_mc,
)
process.readAK4PFchsDATA = process.readAK4PFchsMC.clone(
	payloadName="AK4PFchs",
	globalTag=jec_tag_data,
)
process.readAK8PFchsDATA = process.readAK4PFchsMC.clone(
	payloadName="AK8PFchs",
	globalTag=jec_tag_data,
)
process.p = cms.Path(
	process.readAK4PFchsMC *
	process.readAK8PFchsMC *
	process.readAK4PFchsDATA *
	process.readAK8PFchsDATA
)
