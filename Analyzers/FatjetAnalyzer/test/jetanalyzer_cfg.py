#########################################################
# CMSSW CONFIGURATION FILE                              #
#                                                       #
# Name: jetanalyzer_cfg.py                              #
# Author: Elliot Hughes                                 #
#                                                       #
# Description: This runs an EDAnalyzer that creates     #
# jet analysis tuples.                                  #
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
	"Input type. (0: B2G ntuple, 1: JetToolbox collections)"
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
	"The output file name. This will overwrite the default one assigned by the dataset."
)
options.register ('dataset',
	"",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"The input dataset."
)
options.register("generation",
	"spring15",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Dataset generation."
)
options.register ('verbose',
	False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Turn on verbose mode."
)
options.register ('sigma',
	-1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The cross section of the input dataset. (Not needed when using \"dataset\".)"
)
options.register ('weight',
	-1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The event weight of the input dataset. (Not needed when using \"dataset\".)"
)
options.register ('cutPt',
	10,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.float,
	"The pT cut applied on jets to be included in the tuple. The default is 10 GeV."
)

options.parseArguments()
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))		# Set up the number of events to run over.

### Input:
if options.inDir:
	in_files = ["file:{0}/{1}".format(options.inDir, f) for f in options.inFile]
else:
	in_files = ["file:{0}".format(f) for f in options.inFile]
#### Auto:
out_file = ""
nevents = options.maxEvents
if options.dataset:
	in_files = []
	result = dataset.get_datasets(name=options.dataset, generation=options.generation, set_info=True)
	if result:
		if len(result) == 1:		# Only one process
			dss = result.values()[0]
			if len(dss) == 1:		# Only one subprocess (there should be only one corresponding to each dataset name).
				ds = dss[0]
				if options.sigma < 0:
					options.sigma = ds.sigma
				if nevents > 0:		# If maxEvents is -1, run over everything
					factor = ds.jets_n/nevents
				else:
					factor = 1
				options.weight = ds.weight*factor
				print "Sigma = {0}".format(options.sigma)
#				in_files.extend(["file:{0}".format(f) for f in ds.jets_path])
#				in_files.extend(["root://cmsxrootd.fnal.gov/{0}".format(f) for f in ds.jets_path])
				in_files.extend(["root://cmseos.fnal.gov/{0}".format(f) for f in ds.jets_path])
#				print in_files
				out_file = "{0}_tuple.root".format(options.dataset)
			else:
				print "ERROR: There were multiple datasets found for the name {}. They are listed below:\n{}".format(options.dataset, dss)
		else:
			print "ERROR: There were multiple processes returned for the dataset name of {}:\n{}".format(options.dataset, result)
	else:
		print "ERROR: I can't understand the dataset named {}".format(options.dataset)

### Output:
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
if options.crab:
	process.source = cms.Source("PoolSource")
else:
	process.source = cms.Source(
		"PoolSource",
		fileNames= cms.untracked.vstring(in_files)		# "in_files" is a list of input files defined above in the SET UP section.
	)

## Output:
if options.crab or not options.outDir:
	out_location = "{0}".format(out_file)
else:
	out_location = "{0}/{1}".format(options.outDir, out_file)

#process.out = cms.OutputModule("PoolOutputModule", fileName=cms.untracked.string(out_location))
#process.outpath = cms.EndPath(process.out)

#print out_location
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(out_location)
#	fileName = cms.string("file:/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Analyzers/FatjetAnalyzer/test/fatjets_ntuple.root")
)
# ANALYZER:
## Construct analyzer:
process.analyzer = cms.EDAnalyzer("JetAnalyzer",
	in_type=cms.int32(options.inType),      # Input type (0: B2G, 1: fatjets)
	v=cms.bool(options.verbose),            # Verbose mode
	sigma=cms.double(options.sigma),        # The dataset's cross section
	weight=cms.double(options.weight),               # The number of events to run over
	cut_pt=cms.double(options.cutPt)
)
# PATH:
process.p = cms.Path(
	process.analyzer
)
