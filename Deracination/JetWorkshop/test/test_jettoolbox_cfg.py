import sys
import FWCore.ParameterSet.Config as cms


process = cms.Process('jetToolbox')

process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v4'


process.load("FWCore.MessageService.MessageLogger_cfi")
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.load('FWCore.MessageLogger.MessageLogger_cfi')
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.suppressWarning = cms.untracked.vstring('ecalLaserCorrFilter','manystripclus53X','toomanystripclus53X')
#process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
#process.options.allowUnscheduled = cms.untracked.bool(True)

#process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")
#process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")

########################################################################
#### THESE EXAMPLES ARE JUST TESTS, MOST OF THEM ARE TOTALLY WRONG. 
#### THERE ARE JUST TO TEST SEVERAL FEATURES OF THE JETTOOLBOX.
#######################################################################

from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

listBtagDiscriminatorsAK4 = [ 
                'pfJetProbabilityBJetTags',
                'pfCombinedInclusiveSecondaryVertexV2BJetTags',
                'pfCombinedMVAV2BJetTags',
                'pfCombinedCvsLJetTags',
                'pfCombinedCvsBJetTags',
                ]
#jetToolbox( process, 'ak8', 'ak8JetSubs', 'out', 
#	PUMethod='CHS',
#	miniAOD=True,
#	updateCollection='slimmedJetsAK8', 
#	JETCorrPayload='AK8PFchs', 
#	runOnMC=True,
##		addEnergyCorrFunc=True, 
##		updateCollectionSubjets='slimmedJetsAK8PFCHSSoftDropPacked:SubJets', 
##		subJETCorrPayload='AK4PFchs', 
#	bTagDiscriminators=listBtagDiscriminatorsAK4, 
##	addNsubSubjets=True, 
#	#addSoftDropSubjets=True,
#	addTrimming=True, 
#	addFiltering=True, 
#	addPruning=True, 
#	addSoftDrop=True
#)

jetToolbox(process, 'ca12', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
#	runOnMC=True,
	addTrimming=True,
	addPruning=True,
	addPrunedSubjets=True,
	addSoftDrop=True,
	addFiltering=True,
#	addMassDrop=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=5,
)

#print type(process.patJetsCA12PFCHS)
#print type(process.ca12PFJetsCHS)
#sys.exit()


#process.endpath = cms.EndPath(process.out)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.source = cms.Source("PoolSource",
#		fileNames = cms.untracked.vstring(#'file:example.root'
		fileNames = cms.untracked.vstring(
#			"file:/uscms_data/d3/tote/data/examples/maod_data_0800_1000.root",
			"file:/cms/tote/store/examples/maod_mc_qcdmg2000_fall15_10000.root",
#			'/store/data/Run2016G/JetHT/MINIAOD/PromptReco-v1/000/280/002/00000/E82E26C7-4375-E611-AE7F-FA163E48F736.root'
	#		'/store/mc/RunIISpring15DR74/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/60000/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root'
#		#	'/store/relval/CMSSW_7_4_0_pre9_ROOT6/RelValQCD_Pt_600_800_13/MINIAODSIM/MCRUN2_74_V7-v1/00000/281DDBC1-D3D1-E411-8534-0025905AA9F0.root',
#		#	'/store/relval/CMSSW_7_4_0_pre9_ROOT6/RelValQCD_Pt_600_800_13/MINIAODSIM/MCRUN2_74_V7-v1/00000/C01855C8-D3D1-E411-A702-0026189438D7.root'
			)
		)
#from PhysicsTools.PatAlgos.patInputFiles_cff import filesRelValTTbarPileUpMINIAODSIM
#process.source.fileNames = filesRelValTTbarPileUpMINIAODSIM

#from PhysicsTools.PatAlgos.patInputFiles_cff import filesRelValProdTTbarAODSIM
#process.source.fileNames = filesRelValProdTTbarAODSIM

process.endpath = cms.EndPath(process.out)  
