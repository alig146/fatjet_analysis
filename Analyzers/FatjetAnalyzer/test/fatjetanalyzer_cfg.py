#########################################################
# CMSSW CONFIGURATION FILE                              #
#                                                       #
# Name: fatjetanalyzer_cfg.py                           #
# Author: Elliot Hughes                                 #
#                                                       #
# Description: This runs an EDAnalyzer that creates     #
# analysis ntuples.                                     #
#########################################################

# IMPORTS
import re
import FWCore.ParameterSet.Config as cms			# Standard config file information.
from FWCore.ParameterSet.VarParsing import VarParsing		# Module used to pass arguments from the command line.
#from PhysicsTools.PatAlgos.tools.pfTools import *		# Deep in here (PhysicsTools.PatAlgos.tools.jetTools) is addJetCollection.
from Configuration.AlCa.autoCond import autoCond		# For automatically determining global tags
from truculence import *
from decortication import *

# SET UP:
## Very basic variables:
out_dir_default = samples.ntuple_location		# "/eos/uscms/store/user/tote/data/fat"

## Construct process:
process = cms.Process("analyzer")

## Set up variables and options:
options = VarParsing('analysis')
### General options:
options.register('crab',
	False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Turn this on from inside crab configuration files."
)
options.register('inType',
	1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Input type. 0: B2G ntuple, 1: fatjets"
)
options.register('inDir',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Input directory"
)
options.register('inFile',
	'',
	VarParsing.multiplicity.list,
	VarParsing.varType.string,
	"Input file"
)
options.register('outDir',
	out_dir_default,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Output directory"
)
options.register('outFile',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Output file"
)
options.register ('dataset',
	"",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"The input dataset. (Used instead of \"auto\" at the moment.)"
)
options.register("generation",
	"spring15",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Dataset generation."
)
#options.register ('auto',		# Use a "dataset" instead.
#	False,
#	VarParsing.multiplicity.singleton,
#	VarParsing.varType.bool,
#	"Turn on auto file finding. This will make an ntuple out of every QCD event."
#)
options.register ('verbose',
	False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Turn on verbose mode."
)
options.register ('cutPt',
	175,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The pT requirement to be a candidate for dijet selection."
)
options.register ('cutM',
	50,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"For a dijet pair to be selected, at least one of the elements has to have a mass above this value."
)
options.register ('sigma',
	-1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The cross section of the input dataset. (Not needed when using \"dataset\".)"
)
options.register ('nevents',
	-1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"The number of events in the input dataset. (Not needed when using \"dataset\".)"
)

options.parseArguments()
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))		# Set up the number of events to run over.

### Output:
if not options.outFile:
	options.outFile = "{0}_ntuple.root".format(("b2g", "fatjets")[options.inType == 1])

### Input:
if options.inDir:
	in_files = ["file:{0}/{1}".format(options.inDir, f) for f in options.inFile]
else:
	in_files = ["file:{0}".format(f) for f in options.inFile]
#### Auto:
if options.dataset:
	in_files = []
	ds = samples.get_dataset(name=options.dataset, flavor="fatjets", generation=options.generation)
	if ds:
		if ds.set_info():
			print "Getting info about the {0} dataset ...".format(ds.name)
			if options.sigma < 0:
				options.sigma = ds.sigma
			if options.nevents < 0:
				print "\tThere are {0} events in the {1} dataset.".format(options.nevents, options.dataset)
				options.nevents = ds.nevents_fatjet
			print "Sigma = {0}".format(options.sigma)
			print "NEvents = {0}".format(options.nevents)
			in_files.extend(["file:{0}".format(f) for f in ds.files_fatjet])
			options.outFile = "{0}_ntuple.root".format(options.dataset)
		else:
			print "ERROR: I can't find the info about the dataset named {0}".format(options.dataset)
			options.nevents = 0
	else:
		print "I can't understand the dataset named {0}".format(options.dataset)
#print in_files
# /SET UP

# CONFIGURATION:
## Process options:
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(
	wantSummary=cms.untracked.bool(False),		# Turn off long summary after job.
	allowUnscheduled=cms.untracked.bool(True),
#	SkipEvent=cms.untracked.vstring('ProductNotFound')		# Dangerous. Use only for testing specific things.
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
if options.crab or not options.outDir:
	out_location = "{0}".format(options.outFile)
else:
	out_location = "{0}/{1}".format(options.outDir, options.outFile)

#process.out = cms.OutputModule("PoolOutputModule", fileName=cms.untracked.string(out_location))
#process.outpath = cms.EndPath(process.out)

#print out_location
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(out_location)
#	fileName = cms.string("file:/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Analyzers/FatjetAnalyzer/test/fatjets_ntuple.root")
)
# ANALYZER:
## Construct analyzer:
process.analyzer = cms.EDAnalyzer("FatjetAnalyzer",
	in_type=cms.int32(options.inType),		# Input type (0: B2G, 1: fatjets)
	v=cms.bool(options.verbose),		# Verbose mode
	cut_pt=cms.double(options.cutPt),		# pT cut
	cut_m=cms.double(options.cutM),		# m cut
	sigma=cms.double(options.sigma),		# The dataset's cross section
	nevents=cms.double(options.nevents)		# The number of events in the dataset
)
# PATH:
process.p = cms.Path(
	process.analyzer
)
