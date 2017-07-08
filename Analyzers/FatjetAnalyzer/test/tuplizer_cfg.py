#########################################################
# CMSSW CONFIGURATION FILE                              #
#                                                       #
# Name: fatjetproducer_cfg.py                           #
# Author: Elliot Hughes                                 #
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
def check_jec(jec_path, data=False, algorithm="ak8"):
	flavors = ["L1FastJet", "L2Relative", "L2L3Residual", "L3Absolute", "Uncertainty"]
	d = "/".join(jec_path.split("/")[:-1])
	prefix = jec_path.split("/")[-1]
	if not os.path.exists(d):
		return False
	txts = [f for f in os.listdir(d) if ".txt" in f]
	for flavor in flavors:
		if not "{}_{}_{}_{}PFchs.txt".format(prefix, ("MC", "DATA")[data == True], flavor, algorithm.upper()) in txts:
			return False
	return True
# /FUNCTIONS:


# SET UP:
## Very basic variables:
#out_dir_default = "~/temp"             # This is where output goes when it's not put into EOS by CRAB.
out_dir_default = "."             # This is where output goes when it's not put into EOS by CRAB.
cmssw_version = cmssw.get_version()                   # The CMSSW version that this configuration file is using.

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
	175,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"Cut on the pT of the leading jets. The default is 175 GeV."
)
options.register ('cutEtaFilter',
	2.5,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"Cut on the abs(eta) of the leading jets. The default is 2.5 GeV."
)
### Tuplizer options:
options.register ('cutPtTuplizer',
	10,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The pT cut applied on jets to be included in the tuple. The default is 10 GeV."
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
print "User defined input files: {}".format(in_files)
if (not in_files) and (not options.crab):
	assert miniaod
	if miniaod.files:
		in_files = miniaod.files
#		if not miniaod.dir.eos: in_files = ["file:" + f for f in in_files]		# This is broken ATM, I need a better way of knowing where data is
		in_files = ["file:" + f if f[:7] != "/store/" else f for f in in_files]
	else:
		print "ERROR (tuplizer): What files should be run over?"
		sys.exit()
#		in_files = dataset.get_paths(ds.miniaod_name)
print "You are running over {} files. An example file is the following:".format(len(in_files))
print "\t" + in_files[0] 

if (not in_files) and (not options.crab):
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
	process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet(
	wantSummary=cms.untracked.bool(False),		# Turn off long summary after job.
	allowUnscheduled=cms.untracked.bool(True),
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
print out_location

process.out = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string(out_location),
	outputCommands = cms.untracked.vstring(
		'drop *',
#		# Keep important stuff from the original MiniAOD:
		'keep *GenEventInfoProduct*_*_*_*',		# pT-hat information
		"keep *_packedGenParticles_*_*",		# GN particle collection
		"keep *_packedPFCandidates_*_*",		# PF particle collection
	),
	SelectEvents = cms.untracked.PSet(
		SelectEvents = cms.vstring("p")
	)
)

### Add jet collections using the JetWorkshop:
from Deracination.JetWorkshop.jetWorkshop_cff import add_jet_collection
add_jet_collection(process, algo_name="ak4", pum_name="chs", groom_names=["p", "f", "s", "t"], data=options.data, taus=range(1,6))
add_jet_collection(process, algo_name="ak8", pum_name="chs", groom_names=["p", "f", "s", "t"], data=options.data, taus=range(1,6))
add_jet_collection(process, algo_name="ca12", pum_name="chs", groom_names=["p", "f", "s", "t"], data=options.data, taus=range(1,6))

### Add jet collections using the JetToolbox:
#from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
#jetToolbox(process, 'ak4', 'jetSequence', 'out',		# Required
#	# Optional:
#	PUMethod='CHS',
#	miniAOD=True,
#	addTrimming=True,
#	addPruning=True,
#	addSoftDrop=True,
#	addFiltering=True,
#	addMassDrop=True,
#)
#jetToolbox(process, 'ak8', 'jetSequence', 'out',		# Required
#	# Optional:
#	PUMethod='CHS',
#	miniAOD=True,
#	addTrimming=True,
#	addPruning=True,
#	addSoftDrop=True,
#	addFiltering=True,
#	addMassDrop=True,
#	addNsub=True,
#		maxTau=5,
#)
#jetToolbox(process, 'ca12', 'jetSequence', 'out',		# Required
#	# Optional:
#	PUMethod='CHS',
#	miniAOD=True,
#	addTrimming=True,
#	addPruning=True,
#	addSoftDrop=True,
#	addFiltering=True,
#	addMassDrop=True,
#	JETCorrPayload='None',
#	addNsub=True,
#		maxTau=5,
#)

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
	jetCollection=cms.InputTag("selectedPatJetsCA12CHS"),
)

#out_location = options.outDir + "/test.root"
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(out_location)
)
# Tuplizer:
## JEC data path:
jec_path = "jec_data/Spring16_25nsV2"
if not check_jec(jec_path, data=options.data):
	print "ERROR: Can't find the JEC data that's supposed to be located in {}.".format(jec_path)
	sys.exit()

## Initialize the EDAnalyzer:
process.tuplizer = cms.EDAnalyzer("JetTuplizer",
	v=cms.bool(True),
	is_data=cms.bool(options.data),
	in_type=cms.int32(1),                    # Input type (0: B2G, 1: fatjets)
	sigma=cms.double(sigma),                 # The dataset's cross section
	weight=cms.double(options.weight),       # The event weight
	cut_pt=cms.double(options.cutPtTuplizer),
	pileup_path=cms.string("pileup_data/"),
	jec_version=cms.string(jec_path),
	genInfo=cms.InputTag("generator"),
	rhoInfo=cms.InputTag("fixedGridRhoFastjetAll"),
	vertexCollection=cms.InputTag("offlineSlimmedPrimaryVertices"),
	pileupInfo=cms.InputTag("slimmedAddPileupInfo"),
	triggerResults=cms.InputTag("TriggerResults", "", "HLT"),
	triggerPrescales=cms.InputTag("patTrigger", ""),
	## AK4 collections:
	ak4MAODCollection=cms.InputTag("slimmedJets"),
	ak4GNCollection=cms.InputTag("selectedPatJetsAK4CHS", "genJets"),
	ak4PFCollection=cms.InputTag("selectedPatJetsAK4CHS"),
	ak4PFPrunedCollection=cms.InputTag("selectedPatJetsAK4CHSPruned"),
	ak4PFTrimmedCollection=cms.InputTag("selectedPatJetsAK4CHSTrimmed"),
	ak4PFSoftDropCollection=cms.InputTag("selectedPatJetsAK4CHSSoftDrop"),
	ak4PFFilteredCollection=cms.InputTag("selectedPatJetsAK4CHSFiltered"),
	## AK8 collections:
	ak8MAODCollection=cms.InputTag("slimmedJetsAK8"),
	ak8GNCollection=cms.InputTag("selectedPatJetsAK8CHS", "genJets"),
	ak8PFCollection=cms.InputTag("selectedPatJetsAK8CHS"),
	ak8PFPrunedCollection=cms.InputTag("selectedPatJetsAK8CHSPruned"),
	ak8PFTrimmedCollection=cms.InputTag("selectedPatJetsAK8CHSTrimmed"),
	ak8PFSoftDropCollection=cms.InputTag("selectedPatJetsAK8CHSSoftDrop"),
	ak8PFFilteredCollection=cms.InputTag("selectedPatJetsAK8CHSFiltered"),
	## CA12 collections:
	ca12GNCollection=cms.InputTag("selectedPatJetsCA12CHS", "genJets"),
	ca12PFCollection=cms.InputTag("selectedPatJetsCA12CHS"),
	ca12PFPrunedCollection=cms.InputTag("selectedPatJetsCA12CHSPruned"),
	ca12PFTrimmedCollection=cms.InputTag("selectedPatJetsCA12CHSTrimmed"),
	ca12PFSoftDropCollection=cms.InputTag("selectedPatJetsCA12CHSSoftDrop"),
	ca12PFFilteredCollection=cms.InputTag("selectedPatJetsCA12CHSFiltered"),
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

# PATH:
process.p = cms.Path(
	process.filter *
	process.tuplizer
)
#process.outpath = cms.EndPath(process.out)
