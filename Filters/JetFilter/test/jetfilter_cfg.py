#########################################################
# CMSSW CONFIGURATION FILE                              #
#                                                       #
# Name: jetfilter_cfg.py                                #
# Author: Elliot Hughes                                 #
#                                                       #
# Description: This runs an EDFilter that filters       #
# events with jets made by the JetToolbox.              #
#########################################################

# IMPORTS
import re, sys
import FWCore.ParameterSet.Config as cms			# Standard config file information.
from FWCore.ParameterSet.VarParsing import VarParsing		# Module used to pass arguments from the command line.
#from PhysicsTools.PatAlgos.tools.pfTools import *		# Deep in here (PhysicsTools.PatAlgos.tools.jetTools) is addJetCollection.
from Configuration.AlCa.autoCond import autoCond		# For automatically determining global tags
from truculence import *
from decortication import dataset

# SET UP:
## Very basic variables:
out_dir_default = "/uscms/home/tote/temp"

## Construct process:
process = cms.Process("filter")

## Set up variables and options:
options = VarParsing('analysis')
### General options:
#options.register('inDir',
#	'',
#	VarParsing.multiplicity.singleton,
#	VarParsing.varType.string,
#	"Input directory"
#)
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
	'test_filter.root',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"The output file name. This will overwrite the default one assigned by the dataset."
)
options.register ('cutPt',
	175,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The pT filtering cut. The default is 150 GeV."
)

options.parseArguments()
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))		# Set up the number of events to run over.

### Input:
in_files = ["{0}".format(f) for f in options.inFile]
#### Stuff:
nevents = options.maxEvents
### Output:
out_file = ""
if options.outFile:
	out_file = options.outFile
if not out_file:
	print "ERROR: There's no out_file specified!"
	sys.exit()
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

process.source = cms.Source(
	"PoolSource",
	fileNames= cms.untracked.vstring(in_files)		# "in_files" is a list of input files defined above in the SET UP section.
)

## Output:
out_location = "{0}/{1}".format(options.outDir, out_file)

#process.out = cms.OutputModule("PoolOutputModule", fileName=cms.untracked.string(out_location))
#process.outpath = cms.EndPath(process.out)

#print out_location
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(out_location)
#	fileName = cms.string("file:/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Analyzers/FatjetAnalyzer/test/fatjets_ntuple.root")
)
# FILTER:
## Construct filter:
process.filter = cms.EDFilter("JetFilter",
	cut_pt=cms.double(options.cutPt)
)
# PATH:
process.p = cms.Path(
	process.filter
)
