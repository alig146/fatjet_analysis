import sys
import FWCore.ParameterSet.Config as cms

process = cms.Process('fat')

process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v4'

process.load('FWCore.MessageLogger.MessageLogger_cfi')
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.suppressWarning = cms.untracked.vstring(
	'ecalLaserCorrFilter',
	'manystripclus53X',
	'toomanystripclus53X',
#	'JetFlavourClustering',		# Does nothing?
)
#process.MessageLogger.suppressInfo = cms.untracked.vstring(
#	'JetFlavourClustering',		# Does nothing?
#)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)

#process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")
#process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")

process.out = cms.OutputModule('PoolOutputModule',
	fileName=cms.untracked.string('jetworkshop.root'),
	outputCommands=cms.untracked.vstring(),
)


# Make jet collections:
from Deracination.JetWorkshop.jetWorkshop_cff import add_jet_collection

#add_jet_collection(process, algo_name="ak8", pum_name="chs", groom_names=["p", "f", "s", "t"], data=False, taus=range(1,6))
#add_jet_collection(process, algo_name="ca12", pum_name="chs", groom_names=["p", "f", "s", "t"], data=False, taus=range(1,6))
add_jet_collection(process, algo_name="ca12", pum_name="chs", groom_names=[], data=False, taus=range(1,6))


process.endpath = cms.EndPath(process.out)
print("out ")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.source = cms.Source("PoolSource",
		fileNames = cms.untracked.vstring(
			"file:/cms/tote/store/examples/maod_mc_qcdmg2000_fall15_10000.root",
			)
		)
#from PhysicsTools.PatAlgos.patInputFiles_cff import filesRelValTTbarPileUpMINIAODSIM
#process.source.fileNames = filesRelValTTbarPileUpMINIAODSIM

#from PhysicsTools.PatAlgos.patInputFiles_cff import filesRelValProdTTbarAODSIM
#process.source.fileNames = filesRelValProdTTbarAODSIM

