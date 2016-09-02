#########################################################
# CMSSW CONFIGURATION FILE                              #
#                                                       #
# Name: fatjetsimulation_cfg.py                         #
# Author: Elliot Hughes                                 #
#                                                       #
# Description: [something]                              #
#########################################################

# Imports:
## Normal python things:
import sys
import re		# For regular expressions
from subprocess import Popen, PIPE
## CMSSW things:
import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing		# Module used to pass arguments from the command line.
from Configuration.StandardSequences.Eras import eras
## Custom:
from truculence import cmssw                # For "get_cmssw()"
# /Imports

# SET UP:
## Very basic variables:
out_dir_default = "."      # This is where output goes when it's not put into EOS by CRAB.
cmssw_version = cmssw.get_version()                    # The CMSSW version that this configuration file is using.

## Construct process:
process = cms.Process("SIM", eras.Run2_25ns)		# 160829: Changed "HLT" to "SIM"

## Set up variables and options:
options = VarParsing('analysis')
### General options:
options.register('crab',
	False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Turn this on from inside crab configuration files."
)
options.register('subprocess',
	'sq100to4j',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"What is the dataset's subprocess (sq100to2j, qcdp80, etc.)?"
)
options.register('generation',
	'0706',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"What is the dataset's generation (spring15, 0706, etc.)?"
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
	"file:/uscms/home/tote/lhe",
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Input directory"
)
options.register ('inFile',
	'',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Input LHE file"
)
options.register ('auto',
	0,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Turn on auto file finding."
)
options.maxEvents = 2
options.parseArguments()
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))		# Set up the number of events to run over.

### Output:
if not options.outFile:
	options.outFile = "{0}_{1}_miniaod.root".format(options.subprocess, cmssw_version)
if options.crab:
	out_location = "{0}".format(options.outFile)
else:
	out_location = "{0}/{1}".format(options.outDir, options.outFile)
	
### Input:
if not options.inFile:
	options.inFile = "{0}.lhe".format(options.subprocess)
if options.crab:
	if "/" not in options.inFile:
		in_file = "file:{0}".format(options.inFile)
	else:
		in_file = options.inFile
else:
	in_file = "{0}/{1}".format(options.inDir, options.inFile)

#raw_output = Popen(['xrdcp root://cmseos.fnal.gov//store/user/tote/lhe/{} .'.format(in_file)], shell=True, stdout=PIPE, stderr=PIPE).communicate()
#print in_file
# /SET UP

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')		# 160829: Added line
process.load('Configuration.Geometry.GeometrySimDB_cff')		# 160829: Added line
#process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')		# 160829: Commented line
#process.load('Configuration.Geometry.GeometryExtended2015_cff')		# 160829: Commented line
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')		# 160829: Updated from "8TeVCollision" (there is no "25ns" ...)
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')		# 160829: Updated from "GlobalTag_condDBv2"
process.load('SimGeneral.MixingModule.mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi')		# (Step 1) 160829: Updated from "Startup"
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')		# (Step 1)
process.load('Configuration.StandardSequences.Digi_cff')		# (Step 1)
process.load('Configuration.StandardSequences.SimL1Emulator_cff')		# (Step 1)
process.load('Configuration.StandardSequences.DigiToRaw_cff')		# (Step 1)
process.load('HLTrigger.Configuration.HLT_25ns14e33_v4_cff')		# (Step 1) 160829: Updated from "v1".
process.load('Configuration.StandardSequences.RawToDigi_cff')		# (Step 2)
process.load('Configuration.StandardSequences.L1Reco_cff')		# (Step 2)
process.load('Configuration.StandardSequences.Reconstruction_cff')		# (Step 2)
#process.load('CommonTools.ParticleFlow.EITopPAG_cff')		# (Step 2?) 160829: Added commented line.

# Input source
process.source = cms.Source(
	"LHESource",
	fileNames = cms.untracked.vstring(in_file)
)

process.options = cms.untracked.PSet(
	SkipEvent = cms.untracked.vstring('ProductNotFound')
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(		# 160829: I don't how, if at all, any of this matters.
	version = cms.untracked.string('$0.0.0$'),
	annotation = cms.untracked.string('something'),
	name = cms.untracked.string('Applications?')
)

# Output definition
process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
	compressionAlgorithm = cms.untracked.string('LZMA'),
	compressionLevel = cms.untracked.int32(4),
	dataset = cms.untracked.PSet(
		dataTier = cms.untracked.string('MINIAODSIM'),
		filterName = cms.untracked.string('')
	),
	dropMetaData = cms.untracked.string('ALL'),
	eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
	fastCloning = cms.untracked.bool(False),
	fileName = cms.untracked.string(out_location),
	outputCommands = process.MINIAODSIMEventContent.outputCommands,
	overrideInputFileSplitLevels = cms.untracked.bool(True)
)
#process.Output = cms.OutputModule("PoolOutputModule",
#    compressionLevel = cms.untracked.int32(4),
#    compressionAlgorithm = cms.untracked.string('LZMA'),
#    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
##    outputCommands = process.AODSIMEventContent.outputCommands,
#	outputCommands = cms.untracked.vstring(
#		'drop *',
#		"keep *_genParticles_*_*",
#		"keep *_particleFlow__*",
#	),
#	fileName = cms.untracked.string ("{0}/test.root".format(options.outDir)),
#    dataset = cms.untracked.PSet(
#        filterName = cms.untracked.string(''),
#        dataTier = cms.untracked.string('GEN-SIM-RAW')
#    )
#)

# Additional output definition

# Other statements
## Global tag:
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9', '')		# 160829: commented line
process.GlobalTag = GlobalTag(process.GlobalTag, '76X_mcRun2_asymptotic_v12', '')		# 160829: added line

process.genstepfilter.triggerConditions=cms.vstring("generation_step")

#process.htFilter = cms.EDFilter("PythiaFilterHT",		# 160829: This sort of thing is really useful ... If I recomment this, I need to make sure to add it to the path below. Look at Ale's code for reference.
#	MinHT = cms.untracked.double(500.0)
#)


from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *

process.generator = cms.EDFilter("Pythia8HadronizerFilter",
	maxEventsToPrint = cms.untracked.int32(1),		# Both Ale and Savvas
    pythiaPylistVerbosity = cms.untracked.int32(1),		# Both Ale and Savvas
    filterEfficiency = cms.untracked.double(1.0),		# Both Ale and Savvas
    pythiaHepMCVerbosity = cms.untracked.bool(False),		# Both Ale and Savvas
    comEnergy = cms.double(13000.0),		# Both Ale and Savvas
    PythiaParameters = cms.PSet(
        pythia8CommonSettingsBlock,
        pythia8CUEP8M1SettingsBlock,
        parameterSets = cms.vstring(
        	'pythia8CommonSettings',
            'pythia8CUEP8M1Settings',
        )
    )
)

##process.generator = cms.EDFilter("Pythia8HadronizerFilter",
##	maxEventsToPrint = cms.untracked.int32(1),
##	pythiaPylistVerbosity = cms.untracked.int32(1),
##	filterEfficiency = cms.untracked.double(1.0),
##	pythiaHepMCVerbosity = cms.untracked.bool(False),
##	comEnergy = cms.double(13000.0),
##	PythiaParameters = cms.PSet(
##		processParameters = cms.vstring(
##			'Main:timesAllowErrors    = 10000', 
##			'ParticleDecays:limitTau0 = on', 
##			'ParticleDecays:tauMax = 10', 
##			'Tune:ee 3', 
##			'Tune:pp 5',
##			'SpaceShower:pTmaxMatch = 1',
##			'SpaceShower:pTmaxFudge = 1',
##			'SpaceShower:MEcorrections = off',
##			'TimeShower:pTmaxMatch = 1',
##			'TimeShower:pTmaxFudge = 1',
##			'TimeShower:MEcorrections = off',
##			'TimeShower:globalRecoil = on',
##			#'TimeShower:limitPTmaxGlobal = on',
##			'TimeShower:nMaxGlobalRecoil = 1',
##			#'TimeShower:globalRecoilMode = 2',
##			#'TimeShower:nMaxGlobalBranch = 1',
##			'SLHA:keepSM = on',
##			'SLHA:minMassSM = 1000.',
##			'Check:epTolErr = 0.01'
##		),
##		parameterSets = cms.vstring('processParameters')
##	)
##)

# PU (Step 1)
minBiasFiles = __import__('MinBias_TuneCUETP8M1_13TeV-pythia8_cfi')
process.mix.input.fileNames = cms.untracked.vstring(minBiasFiles.readFiles)

#process.mix.input.fileNames = cms.untracked.vstring([
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/0028ABCB-74B0-E411-B596-0025904C4F50.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/007FD3D8-74B0-E411-AC02-782BCB67826E.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/00AAE11D-74B0-E411-BCF8-DF448471F33D.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/00BC66B7-6CAF-E411-86B6-20CF3056171F.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/00C59C87-75B0-E411-918F-63BFB108B170.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/02052468-72AF-E411-9F90-002590D9D990.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/022D9B0E-6EAF-E411-90AA-0022195E66A7.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/027B2D2C-74B0-E411-8DF1-001E682F8C7C.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/02B9B6A5-74B0-E411-A287-002590491B22.root',
#	'/store/mc/RunIIWinter15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/00000/02C48802-6EAF-E411-96A0-001E67248688.root',
#])

# Path and EndPath definitions
process.generation_step = cms.Path(process.generator * process.pgen)
process.simulation_step = cms.Path(process.generator * process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.digitisation_step = cms.Path(process.pdigi)		# (Step 1)
process.L1simulation_step = cms.Path(process.SimL1Emulator)		# (Step 1)
process.digi2raw_step = cms.Path(process.DigiToRaw)		# (Step 1)
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)		# (Step 1)
process.raw2digi_step = cms.Path(process.RawToDigi)		# (Step 2)
process.L1Reco_step = cms.Path(process.L1Reco)		# (Step 2)
process.reconstruction_step = cms.Path(process.reconstruction)		# (Step 2)
#process.eventinterpretaion_step = cms.Path(process.EIsequence)		# (Step 2) 160829: Added commented line.
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)		# (Step 2)
process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)		# (Step 3)
#process.Output_step = cms.EndPath(process.Output)		# Step 2

process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(
	process.generation_step,
	process.genfiltersummary_step,
	process.simulation_step
#	process.endjob_step,
#	process.Output_step
)
# Step 1 paths:
process.schedule.extend([
	process.digitisation_step,
	process.L1simulation_step,
	process.digi2raw_step
])
process.schedule.extend(process.HLTSchedule)

# Step 2 paths:
process.schedule.extend([
	process.raw2digi_step,
	process.L1Reco_step,
	process.reconstruction_step,
#	process.eventinterpretaion_step,		# 160829: Added commented line.
	process.endjob_step,
#	process.AODSIMoutput_step
	process.MINIAODSIMoutput_step
#	process.Output_step
])

# Customization:
from Configuration.DataProcessing.Utils import addMonitoring 
process = addMonitoring(process)

## Step 1:
# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC		# 160826: I think Ale uses "FullSim" instead of "MC".
process = customizeHLTforMC(process)

## Other:
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process = convertToUnscheduled(process)
process.load('Configuration.StandardSequences.PATMC_cff')
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process = cleanUnscheduled(process)

## Step 3:
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC
process = miniAOD_customizeAllMC(process)


