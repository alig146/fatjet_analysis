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
import re		# For regular expressions
import sys
## CMSSW:
import FWCore.ParameterSet.Config as cms		# Standard config file information.
from FWCore.ParameterSet.VarParsing import VarParsing		# Module used to pass arguments from the command line.
from Configuration.AlCa.autoCond import autoCond		# For automatically determining global tags
# /IMPORTS

# SET UP:
## Very basic variables:
out_dir_default = "/uscms_data/d3/tote/data/fat"

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
options.register ('dataset',
	'sqtojjjj',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"What kind of data is this (sqtojj, qcd, etc.)?"
)
options.register ('method',
	'pythia50to80',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"What kind of background is this?"
)
options.register ('m',
	150,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Squark mass"
)
options.register ('cutPt',
	175,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Cut on squark pT"
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
options.register ('inDir',
	"/uscms_data/d3/tote/CMSSW_6_2_5/src/Simulation/jetSubsSimulation/test/tote/data",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Input directory"
)
options.register ('inFile',
	'',
	VarParsing.multiplicity.list,
	VarParsing.varType.string,
	"Input file(s)"
)
options.register ('cmssw',
	'7413',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"The CMSSW release that was used."
)
options.maxEvents = 2
### Producer options:
options.register ('jetAlgo',
	'ca',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Jet clustering algorithm"
)
options.register ('R',
	1.2,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"Fatjet cone size"
)
options.register ('doTrim',
	True,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Set to false to not perform trimming"
)

options.parseArguments()
options.cmssw = ''.join([d for d in list((re.search("/?([^/]*)$", options.cmssw)).group(1)) if d.isdigit()])		# Format the "cmssw" option, so "CMSSW_7_3_2_patch2" turns into "7322" if necessary.
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))		# Set up the number of events to run over.

### Output:
if not options.outFile:
	if (options.dataset in ["sqtojj", "sqtojjjj", "lqtottau", "lqtoue"]):
		options.outFile = "{0}_{1}_{2}_fatjets.root".format(options.dataset, options.m, options.cmssw)
	elif (options.dataset == "qcd"):
		options.outFile = "{0}_{1}_{2}_fatjets.root".format(options.dataset, options.method, options.cmssw)
	elif (options.dataset == "ttbar"):
		options.outFile = "{0}_{1}_fatjets.root".format(options.dataset, options.cmssw)
	else:
		options.outFile = "{0}_{1}_fatjets.root".format(options.dataset, options.cmssw)

### Input:
in_files = ["{0}/{1}".format(options.inDir, f) for f in options.inFile]
#print in_files
# /SET UP

# CONFIGURATION:
## Process options:
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(
	wantSummary=cms.untracked.bool(False),		# Turn off long summary after job.
	allowUnscheduled=cms.untracked.bool(True),
#	SkipEvent=cms.untracked.vstring('ProductNotFound')		# Dangerous.
)

## Input:
if options.crab:
	process.source = cms.Source("PoolSource")
else:
	process.source = cms.Source(
		"PoolSource",
		fileNames= cms.untracked.vstring(in_files)		# "in_files" is a list of input files defined above in the SET UP section.
	)

## Output:
if options.crab:
	out_location = "{0}".format(options.outFile)
else:
	out_location = "{0}/{1}".format(options.outDir, options.outFile)

process.out = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string(out_location),
	outputCommands = cms.untracked.vstring(
		'drop *',
		'keep *GenEventInfoProduct*_*_*_*',		# pT-hat information
		"keep *_packedGenParticles_*_PAT",		# GN particle collection
		"keep *_packedPFCandidates_*_PAT",		# PF particle collection
		"keep *_*_*_jetToolbox",		# Jets
#		'keep *GenParticle*_genParticles_*_*',
#		'keep recoPFCandidates_particleFlow_*_*',
#		'keep *_selectedPatJets*_*_*',
#		'drop *_selectedPatJets*_caloTowers_*',
#	#	'keep *_selectedPatElectrons*_*_*',		# Causes some errors when there are no selected electrons...
#		'keep *_selectedPatMuons*_*_*',
#		'keep *_selectedPatTaus*_*_*',
#		'keep *_patMETs*_*_*',
#	#	'keep *_selectedPatPhotons*_*_*',		# Causes some errors when there are no selected photons...
#		'keep *_selectedPatTaus*_*_*',
#		"keep *_offlinePrimaryVertices_*_*",
#		"keep *_offlinePrimaryVerticesWithBS_*_*",
#		"keep *_generalTracks_*_*",
#		"keep *_electronGsfTracks_*_*",
		"keep *_FatjetProducer_*_*"		# Anything my producer makes, which is not hooked up at the moment.
	),
	SelectEvents = cms.untracked.PSet(
		SelectEvents = cms.vstring("p")
	)
)

## Add jet collections using the JetToolbox:
from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
jetToolbox(process, 'ak4', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
)
jetToolbox(process, 'ak8', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ca8', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ak10', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ca10', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ak12', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ca12', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
#jetToolbox(process, 'ca12', 'jetSequence', 'out',		# Required
#	# Optional:
#	PUMethod='CHS',
#	miniAOD=True,
#	addPruning=True,
#	JETCorrPayload='None',
#	addNsub=True,
#		maxTau=4,
#)
#jetToolbox(process, 'ca12', 'jetSequence', 'out',		# Required
#	# Optional:
#	PUMethod='CHS',
#	miniAOD=True,
#	addFiltering=True,
#	JETCorrPayload='None',
#	addNsub=True,
#		maxTau=4,
#)
jetToolbox(process, 'ak14', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ca14', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)

## Geometry and detector configuration:
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string(autoCond['run2_mc'])		# Set global tags automatically.
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.GlobalTag.globaltag = 'MCRUN2_74_V7'
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

# Filter:
process.filter = cms.EDFilter("JetFilter",
	cut_pt=cms.double(options.cutPt)
)

# PATH:
process.p = cms.Path(
	process.filter
)
process.outpath = cms.EndPath(process.out)
