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
## Custom:
from decortication import dataset
# /IMPORTS

# SET UP:
## Very basic variables:
out_dir_default = "/uscms/home/tote/temp"

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
options.register ('subprocess',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"What kind of data is this (sq150to4j, qcdmg100, etc.)?"
)
options.register ('dataset',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"What is the input dataset name (\"QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8\", etc.)?"
)
options.register("generation",
	"spring15",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Dataset generation."
)
options.register ('cutPt',
	175,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Cut on the second leading jet pT. The default is 175."
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
	'',
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
	'0',
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

### Input:
in_files = ["{0}/{1}".format(options.inDir, f) for f in options.inFile]
if (not in_files) and (not options.crab):
	assert options.dataset
	result = dataset.get_datasets(name=options.dataset, generation=options.generation, set_info=True)
	print result
	ds = result.values()[0][0]
	print ds.miniaod_path
	in_files = ds.miniaod_path
	assert hasattr(ds, "subprocess")
	options.subprocess = ds.subprocess

if (not in_files) and (not options.crab):
	print "ERROR: You're not running over any files!"
	sys.exit()

### Output:
if not options.outFile:
	options.outFile = "{0}_{1}_jets.root".format(options.subprocess, options.cmssw)
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
	print out_location

process.out = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string(out_location),
	outputCommands = cms.untracked.vstring(
		'drop *',
		# Keep important stuff from the original MiniAOD:
		'keep *GenEventInfoProduct*_*_*_*',		# pT-hat information
		"keep *_packedGenParticles_*_*",		# GN particle collection
		"keep *_packedPFCandidates_*_*",		# PF particle collection
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
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
	addMassDrop=True,
)
jetToolbox(process, 'ak8', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
#	addSoftDrop=True,
#	addFiltering=True,
#	addMassDrop=True,
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ca8', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
	addMassDrop=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ak10', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
	addMassDrop=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ca10', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
	addMassDrop=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ak12', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
	addMassDrop=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
jetToolbox(process, 'ca12', 'jetSequence', 'out',		# Required
	# Optional:
	PUMethod='CHS',
	miniAOD=True,
	addTrimming=True,
	addPruning=True,
	addSoftDrop=True,
	addFiltering=True,
	addMassDrop=True,
	JETCorrPayload='None',
	addNsub=True,
		maxTau=4,
)
#jetToolbox(process, 'ak14', 'jetSequence', 'out',		# Required
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
#		maxTau=4,
#)
#jetToolbox(process, 'ca14', 'jetSequence', 'out',		# Required
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
#		maxTau=4,
#)

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
