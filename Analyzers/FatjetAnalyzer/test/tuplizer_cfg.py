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
import sys        # For "sys.exit()"
## CMSSW:
import FWCore.ParameterSet.Config as cms                  # Standard config file information.
from FWCore.ParameterSet.VarParsing import VarParsing     # Module used to pass arguments from the command line.
from Configuration.AlCa.autoCond import autoCond          # For automatically determining global tags
## Custom:
from decortication import dataset
from truculence import analysis           # For "get_cmssw()"
# /IMPORTS

# SET UP:
## Very basic variables:
out_dir_default = "/uscms/home/tote/temp"      # This is where output goes when it's not put into EOS by CRAB.
cmssw = analysis.get_cmssw()                   # The CMSSW version that this configuration file is using.

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
options.register("generation",
	"spring15",
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
options.maxEvents = 2
### Filter options:
options.register ('cutPtFilter',
	175,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"Cut on the second leading jet pT. The default is 175 GeV."
)
### Analyzer options:
options.register ('cutPtAnalyzer',
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

options.parseArguments()
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))		# Set up the number of events to run over.

### Input:
if options.subprocess:
	result = dataset.get_datasets(subprocess=options.subprocess, generation=options.generation, set_info=False)
#	print result
	ds = result.values()[0][0]
#	print ds.miniaod_path
	sigma = ds.sigma
	if options.weight < 0:
		factor = 1
		if not options.crab:
			if options.maxEvents > 0:		# If maxEvents is -1, run over everything
				factor = ds.miniaod_n/options.maxEvents
		options.weight = ds.weight*factor
else:
	ds = False
	sigma = -1
	if options.weight < 0:
		options.weight = 1

in_files = ["{0}/{1}".format(options.inDir, f) for f in options.inFile]
if (not in_files) and (not options.crab):
	assert ds
	if ds.miniaod_path:
		in_files = ds.miniaod_path
	else:
		in_files = dataset.get_paths(ds.miniaod_name)

if (not in_files) and (not options.crab):
	print "ERROR: You're not running over any files! Check your dataset."
	sys.exit()

### Output:
if not options.outFile:
	options.outFile = "{0}_{1}_tuple.root".format(options.subprocess, cmssw)
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
		fileNames= cms.untracked.vstring([f.encode('utf8') for f in in_files])		# "in_files" is a list of input files defined above in the SET UP section.
	)

## Output:
if options.crab:
	out_location = "{0}".format(options.outFile)
else:
	out_location = "{0}/{1}".format(options.outDir, options.outFile)
	print out_location

#process.out = cms.OutputModule("PoolOutputModule",
#	fileName = cms.untracked.string(out_location),
#	outputCommands = cms.untracked.vstring(
#		'drop *',
#		# Keep important stuff from the original MiniAOD:
#		'keep *GenEventInfoProduct*_*_*_*',		# pT-hat information
#		"keep *_packedGenParticles_*_*",		# GN particle collection
#		"keep *_packedPFCandidates_*_*",		# PF particle collection
#	),
#	SelectEvents = cms.untracked.PSet(
#		SelectEvents = cms.vstring("p")
#	)
#)

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
	addSoftDrop=True,
	addFiltering=True,
	addMassDrop=True,
	addNsub=True,
		maxTau=5,
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
		maxTau=5,
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
		maxTau=5,
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
		maxTau=5,
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
	addMassDrop=True,
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

# Filter:
process.filter = cms.EDFilter("JetFilter",
	cut_pt=cms.double(options.cutPtFilter)
)

process.TFileService = cms.Service("TFileService",
	fileName = cms.string(out_location)
)
# Analyzer:
process.analyzer = cms.EDAnalyzer("JetAnalyzer",
	in_type=cms.int32(1),                    # Input type (0: B2G, 1: fatjets)
	sigma=cms.double(sigma),                 # The dataset's cross section
	weight=cms.double(options.weight),       # The event weight
	cut_pt=cms.double(options.cutPtAnalyzer)
)

# PATH:
process.p = cms.Path(
	process.filter *
	process.analyzer
)
#process.outpath = cms.EndPath(process.out)
