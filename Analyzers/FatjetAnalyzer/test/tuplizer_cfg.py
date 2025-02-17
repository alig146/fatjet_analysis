#########################################################
# CMSSW CONFIGURATION FILE                              #
#                                                       #
# Name: fatjetproducer_cfg.py                           #
# Author: Ali Garabaglu                                 #
#                                                       #
# Description: [something]                              #
#########################################################

# IMPORTS:
## Generic:
import re         # For regular expressions
import os, sys
## CMSSW:
import FWCore.ParameterSet.Config as cms                  # Standard config file information.
from FWCore.ParameterSet.VarParsing import VarParsing     # Module used to pass arguments from the command line.
from Configuration.AlCa.autoCond import autoCond          # For automatically determining global tags
## Custom:
from decortication import dataset
from truculence import cmssw
# /IMPORTS

# FUNCTIONS:
def check_jec(jec_paths, data=False, algorithm="ak8"):
	flavors = ["L1FastJet", "L2Relative", "L2L3Residual", "L3Absolute", "Uncertainty"]
	for jec_path in jec_paths:
		d = "/".join(jec_path.split("/")[:-1])
		prefix = jec_path.split("/")[-1]
		if not os.path.exists(d):
			return False
		txts = [f for f in os.listdir(d) if ".txt" in f]
		for flavor in flavors:
			if not "{}_{}_{}PFchs.txt".format(prefix, flavor, algorithm.upper()) in txts:
				return False
	return True
# /FUNCTIONS:


# SET UP:
## Very basic variables:
#out_dir_default = "~/temp"             # This is where output goes when it's not put into EOS by CRAB.
out_dir_default = "."             # This is where output goes when it's not put into EOS by CRAB.
cmssw_version = cmssw.get_version()                   # The CMSSW version that this configuration file is using.
print

## Construct process:
process = cms.Process("fatjets")

## Set up variables and options:
options = VarParsing('analysis')
### General options:
options.register ('crab',
	False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Turn this on from inside crab configuration files."
)
options.register ('data',
	False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Turn this on when running on data (instead of MC)."
)
options.register ('subprocess',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"What kind of data is this (sq150to4j, qcdmg100, etc.)?"
)
options.register("generation",
	"spring15",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Dataset generation."
)
options.register("suffix",
	"1",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Dataset generation."
)
options.register ('outDir',
	out_dir_default,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Output directory"
)
options.register ('outFile',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Output file"
)
#options.register ('inDir',
#	'',
#	VarParsing.multiplicity.singleton,
#	VarParsing.varType.string,
#	"Input directory"
#)
options.register ('inFile',
	'',
	VarParsing.multiplicity.list,
	VarParsing.varType.string,
	"Input file(s)"
)
options.maxEvents = -1
### Filter options:
options.register ('cutPtFilter',
	300,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"Cut on the pT of the leading jets. The default is 300 GeV."
)
options.register ('cutEtaFilter',
	2.0,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"Cut on the abs(eta) of the leading jets. The default is 2.0 GeV."
)
options.register ('cutSmuFilter',
	False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Cut based on the single-muon pT > 50 GeV trigger."
)
### Tuplizer options:
options.register ('cutPtTuplizer',
	20,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The pT cut applied on jets to be included in the tuple. The default is 20 GeV."
)
options.register ('weight',
	-1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The event weight."
)
options.register ('mask',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Luminosity mask"
)

options.parseArguments()
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))		# Set up the number of events to run over.

if options.suffix == "1":
	options.suffix = "pt{0}".format(options.cutPtFilter)

### Input:
if options.subprocess:
	miniaod = dataset.fetch_miniaod(options.subprocess, options.generation)
	sample = miniaod.get_sample()
	sigma = sample.sigma
	if sigma == None:		# 161013: Added after removing sigma from jetht datasets.
		sigma = -1
	if options.weight < 0:
		weight = miniaod.weight
		factor = 1
		if not options.crab:
			if options.maxEvents > 0:		# If maxEvents is -1, run over everything
				factor = miniaod.n/options.maxEvents
		options.weight = weight*factor
else:
	miniaod = False
	sigma = -1
	if options.weight < 0:
		options.weight = 1

if options.data:
	options.weight = 1
#print "Weight:", options.weight
#in_files = ["{0}/{1}".format(options.inDir, f) for f in options.inFile]
in_files = options.inFile
if not options.crab:
	if in_files: print "[OK] User has defined these input files: {}".format(in_files)
	else:
		print "[..] Finding files to run over."
		assert miniaod
		if miniaod.files:
			in_files = miniaod.files
	#		if not miniaod.dir.eos: in_files = ["file:" + f for f in in_files]		# This is broken ATM, I need a better way of knowing where data is
			in_files = ["file:" + f if f[:7] != "/store/" else f for f in in_files]
		else:
			print "ERROR (tuplizer): What files should be run over?"
			sys.exit()
	#		in_files = dataset.get_paths(ds.miniaod_name)
	if in_files:
		print "[OK] You are running over {} files. An example file is the following:".format(len(in_files))
		print "\t" + in_files[0]
	else:
		print "ERROR: You're not running over any files! Check your dataset."
		sys.exit()

### Output:
if not options.outFile:
	options.outFile = "tuple_{0}_{1}_{2}_{3}.root".format(options.subprocess, options.generation, options.suffix, cmssw_version)
# /SET UP

# CONFIGURATION:
## Process options:
process.load("FWCore.MessageLogger.MessageLogger_cfi")
if options.maxEvents > 1000 or options.maxEvents < 0:
	process.MessageLogger.cerr.FwkReport.reportEvery = 1000





process.options = cms.untracked.PSet(
	wantSummary=cms.untracked.bool(False),		# Turn off long summary after job.
#	allowUnscheduled=cms.untracked.bool(True),
	IgnoreCompletely=cms.untracked.vstring('InvalidReference')		# Dangerous.
#	SkipEvent=cms.untracked.vstring('ProductNotFound')		# Dangerous.
)




## Input:
#in_files = ["file:fall15.root"]
process.source = cms.Source("PoolSource")
if not options.crab:
	setattr(process.source, "fileNames", cms.untracked.vstring([f.encode('utf8') for f in in_files]))		# "in_files" is a list of input files defined above in the SET UP section.
#print process.source.fileNames
if options.mask:
	setattr(process.source, "lumisToProcess", cms.untracked.VLuminosityBlockRange(cmssw.convert_lumi_json(options.mask)))
process.source.duplicateCheckMode = cms.untracked.string("noDuplicateCheck")		# 170608: I added this for sgto3j miniaods ...

## Output:
if options.crab:
	out_location = "{0}".format(options.outFile)
else:
	out_location = "{0}/{1}".format(options.outDir, options.outFile)
print "Output path: {}".format(out_location)




# process.out = cms.OutputModule("PoolOutputModule",
# 	fileName = cms.untracked.string(out_location),
# 	outputCommands = cms.untracked.vstring(
# 		'drop *',
# #		# Keep important stuff from the original MiniAOD:
# 		'keep *GenEventInfoProduct*_*_*_*',		# pT-hat information
# 		"keep *_packedGenParticles_*_*",		# GN particle collection
# 		"keep *_packedPFCandidates_*_*",		# PF particle collection
# 	),
# 	SelectEvents = cms.untracked.PSet(
# 		SelectEvents = cms.vstring("p")
# 	)
# )




### Add jet collections using the JetWorkshop:
# from Deracination.JetWorkshop.jetWorkshop_cff import add_jet_collection
# add_jet_collection(process, algo_name="ak4", pum_name="chs", groom_names=["p", "f", "s", "t"], data=options.data, taus=range(1,6))
# add_jet_collection(process, algo_name="ak8", pum_name="chs", groom_names=["p", "f", "s", "t"], data=options.data, taus=range(1,6))
# add_jet_collection(process, algo_name="ca12", pum_name="chs", groom_names=["p", "f", "s", "t"], data=options.data, taus=range(1,6))

### Add jet collections using the JetToolbox:
from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
jetToolbox(process, 'ak4', 'jetSequence', 'out',		# Required
        # Optional:
        PUMethod='CHS',
        miniAOD=True,
        addTrimming=True,
        addPruning=True,
        addSoftDrop=True,
        addFiltering=True,
#        addMassDrop=True,
        addNsub=True,
        maxTau=5,
)
jetToolbox(process, 'ak8', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
#	addMassDrop=True,
	addNsub=True,
	maxTau=5,
)
jetToolbox(process, 'ca12', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
#	addMassDrop=True,
	JETCorrPayload='None',
	addNsub=True,
	maxTau=5,
)

## Geometry and detector configuration:
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string(autoCond['run2_mc'])		# Set global tags automatically.
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.GlobalTag.globaltag = 'MCRUN2_74_V7'
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#print options.cutPtFilter, options.cutEtaFilter
# Filter:
process.filter = cms.EDFilter("JetFilter",
	cut_pt=cms.double(options.cutPtFilter),
	cut_eta=cms.double(options.cutEtaFilter),
	cut_smu=cms.bool(options.cutSmuFilter),
	jetCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),
	triggerResults=cms.InputTag("TriggerResults", "", "HLT"),
	triggerPrescales=cms.InputTag("patTrigger", ""),
)

#out_location = options.outDir + "/test.root"
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(out_location)
)
# Tuplizer:
## JEC data path:
jec_path_mc = "jec_data/Summer16_23Sep2016V4_MC"
jec_path_data = "jec_data/Summer16_23Sep2016AllV4_DATA"
if not check_jec([jec_path_mc, jec_path_data], data=options.data):
	print "ERROR: Can't find the JEC data that's supposed to be located in {}.".format(jec_path)
	sys.exit()

## Initialize the EDAnalyzer:
process.tuplizer = cms.EDAnalyzer("JetTuplizer",
	v=cms.bool(False),
	is_data=cms.bool(options.data),
	in_type=cms.int32(1),                    # Input type (0: B2G, 1: fatjets)
	sigma=cms.double(sigma),                 # The dataset's cross section
	weight=cms.double(options.weight),       # The event weight
	cut_pt=cms.double(options.cutPtTuplizer),
	pileup_path=cms.string("pileup_data/"),
	jec_version_mc=cms.string(jec_path_mc),
	jec_version_data=cms.string(jec_path_data),
	genInfo=cms.InputTag("generator"),
	rhoInfo=cms.InputTag("fixedGridRhoFastjetAll"),
	vertexCollection=cms.InputTag("offlineSlimmedPrimaryVertices"),
	pileupInfo=cms.InputTag("slimmedAddPileupInfo"),
	triggerResults=cms.InputTag("TriggerResults", "", "HLT"),
	triggerPrescales=cms.InputTag("patTrigger", ""),
	# ## AK4 collections:
	# ak4MAODCollection=cms.InputTag("slimmedJets"),
	# ak4GNCollection=cms.InputTag("selectedPatJetsAK4PFCHS", "genJets"),
	# ak4PFCollection=cms.InputTag("selectedPatJetsAK4PFCHS"),
	# ak4PFPrunedCollection=cms.InputTag("selectedPatJetsAK4PFCHSPruned"),
	# ak4PFTrimmedCollection=cms.InputTag("selectedPatJetsAK4PFCHSTrimmed"),
	# ak4PFSoftDropCollection=cms.InputTag("selectedPatJetsAK4PFCHSSoftDrop"),
	# ak4PFFilteredCollection=cms.InputTag("selectedPatJetsAK4PFCHSFiltered"),
	# ## AK8 collections:
	# ak8MAODCollection=cms.InputTag("slimmedJetsAK8"),
	# ak8GNCollection=cms.InputTag("selectedPatJetsAK8PFCHS", "genJets"),
	# ak8PFCollection=cms.InputTag("selectedPatJetsAK8PFCHS"),
	# ak8PFPrunedCollection=cms.InputTag("selectedPatJetsAK8PFCHSPruned"),
	# ak8PFTrimmedCollection=cms.InputTag("selectedPatJetsAK8PFCHSTrimmed"),
	# ak8PFSoftDropCollection=cms.InputTag("selectedPatJetsAK8PFCHSSoftDrop"),
	# ak8PFFilteredCollection=cms.InputTag("selectedPatJetsAK8PFCHSFiltered"),
	# ## CA12 collections:
	# ca12GNCollection=cms.InputTag("selectedPatJetsCA12PFCHS", "genJets"),
	# ca12PFCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),
	# ca12PFPrunedCollection=cms.InputTag("selectedPatJetsCA12PFCHSPruned"),
	# ca12PFTrimmedCollection=cms.InputTag("selectedPatJetsCA12PFCHSTrimmed"),
	# ca12PFSoftDropCollection=cms.InputTag("selectedPatJetsCA12PFCHSSoftDrop"),
	# ca12PFFilteredCollection=cms.InputTag("selectedPatJetsCA12PFCHSFiltered"),



        # ## AK8
        # ak8MAODCollection=cms.InputTag("slimmedJetsAK8"),
        # ak8GNCollection=cms.InputTag("selectedPatJetsAK8PFCHS", "genJets"),
        # ak8PFCollection=cms.InputTag("selectedPatJetsAK8PFCHS"),
        # ak8PFPrunedCollection=cms.InputTag("ak8PFJetsCHSPrunedMass"),
        # ak8PFTrimmedCollection=cms.InputTag("ak8PFJetsCHSTrimmedMass"),
        # ak8PFSoftDropCollection=cms.InputTag("ak8PFJetsCHSSoftDropMass"),
        # ak8PFFilteredCollection=cms.InputTag("ak8PFJetsCHSFiltereddMass"),
        # ## AK4 collections:
	# ak4MAODCollection=cms.InputTag("slimmedJets"),
	# ak4GNCollection=cms.InputTag("selectedPatJetsAK4PFCHS", "genJets"),
	# ak4PFCollection=cms.InputTag("selectedPatJetsAK4PFCHS"),
	# ak4PFPrunedCollection=cms.InputTag("ak4PFJetsCHSPrunedMass"),
	# ak4PFTrimmedCollection=cms.InputTag("ak4PFJetsCHSTrimmedMass"),
	# ak4PFSoftDropCollection=cms.InputTag("ak4PFJetsCHSSoftDropMass"),
	# ak4PFFilteredCollection=cms.InputTag("ak8PFJetsCHSFiltereddMass"),
	# ## CA12 collections:
	# ca12GNCollection=cms.InputTag("selectedPatJetsCA12PFCHS", "genJets"),
	# ca12PFCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),
	# ca12PFPrunedCollection=cms.InputTag("ca12PFJetsCHSPrunedMass"),
	# ca12PFTrimmedCollection=cms.InputTag("ca12PFJetsCHSTrimmedMass"),
	# ca12PFSoftDropCollection=cms.InputTag("ca12PFJetsCHSSoftDropMass"),
	# ca12PFFilteredCollection=cms.InputTag("ca12PFJetsCHSFiltereddMass"),



        ## AK4 collections:
        ak4MAODCollection=cms.InputTag("slimmedJets"),
        ak4GNCollection=cms.InputTag("selectedPatJetsAK4PFCHS", "genJets"),
        ak4PFCollection=cms.InputTag("selectedPatJetsAK4PFCHS"),
        ak4PFPrunedCollection=cms.InputTag("selectedPatJetsAK4PFCHS"),
        ak4PFTrimmedCollection=cms.InputTag("selectedPatJetsAK4PFCHS"),
	ak4PFSoftDropCollection=cms.InputTag("selectedPatJetsAK4PFCHS"),
	ak4PFFilteredCollection=cms.InputTag("selectedPatJetsAK4PFCHS"),

        ## AK8 collections:   ak4PFJetsCHSFilteredMass
        ak8MAODCollection=cms.InputTag("slimmedJetsAK8"),
        ak8GNCollection=cms.InputTag("selectedPatJetsAK8PFCHS", "genJets"),
        ak8PFCollection=cms.InputTag("selectedPatJetsAK8PFCHS"),
        ak8PFPrunedCollection=cms.InputTag("selectedPatJetsAK8PFCHS"),
        ak8PFTrimmedCollection=cms.InputTag("selectedPatJetsAK8PFCHS"),
        ak8PFSoftDropCollection=cms.InputTag("selectedPatJetsAK8PFCHS"),
        ak8PFFilteredCollection=cms.InputTag("selectedPatJetsAK8PFCHS"),

        ## CA12 collections:  selectedPatJetsAK4PFCHSFiltered
        ca12GNCollection=cms.InputTag("selectedPatJetsCA12PFCHS", "genJets"),
        ca12PFCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),
        ca12PFPrunedCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),
        ca12PFTrimmedCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),
        ca12PFSoftDropCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),
        ca12PFFilteredCollection=cms.InputTag("selectedPatJetsCA12PFCHS"),

	## Lepton collections:
	electronCollection=cms.InputTag("slimmedElectrons"),
	muonCollection=cms.InputTag("slimmedMuons"),
	tauCollection=cms.InputTag("slimmedTaus"),
	photonCollection=cms.InputTag("slimmedPhotons"),
	## Particles:
#	genCollection=cms.InputTag("packedGenParticles"),
	genCollection=cms.InputTag("prunedGenParticles"),
)
#process.tuplizer.testtt = "test"		# This works.

#process.t = cms.Task(process.tuplizer)
#process.dump=cms.EDAnalyzer('EventContentAnalyzer')

# PATH:
process.p = cms.Path(
	process.filter *
	process.tuplizer
#	process.dump
)

#process.p.associate(process.t)

#process.EndPath = cms.EndPath(process.out)
