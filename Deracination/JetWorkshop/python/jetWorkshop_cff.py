
# IMPORTS:
import re

import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.PFJetParameters_cfi import PFJetParameters
from RecoJets.JetProducers.AnomalousCellParameters_cfi import AnomalousCellParameters
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters

#import RecoJets.Configuration.RecoPFJets_cff as PFJets
from RecoJets.Configuration.RecoPFJets_cff import ak4PFJets, ak8PFJetsCHSSoftDrop, ak8PFJetsCHSSoftDropMass, ak8PFJetsCHSPruned, ak8PFJetsCHSPrunedMass, ak8PFJetsCHSTrimmed, ak8PFJetsCHSTrimmedMass, ak8PFJetsCHSFiltered, ak8PFJetsCHSFilteredMass, ak8PFJetsCSConstituents, ak4PFJetsCHS
from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from RecoJets.JetProducers.CATopJetParameters_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection, updateJetCollection
from RecoJets.JetProducers.nJettinessAdder_cfi import Njettiness
from Deracination.JetWorkshop.subjetAdder_cfi import Subjetter, subjet_variables
# /IMPORTS

# CLASSES:
class jet_algorithm:
	known_types = {
		"kt": {"title": "Kt"},
		"ak": {"title": "AntiKt"},
		"ca": {"title": "CambridgeAachen"},
	}
	
	def __init__(self, name):
		self.name = name.lower()
		self.update()
	
	def __nonzero__(self):
		return self.known
	
	def update(self):
		self.assign_type_size()
		self.r = float(self.size)/10
		self.known = True if self.type in jet_algorithm.known_types else False
		for key in jet_algorithm.known_types["kt"].keys():
			if self.known:
				setattr(self, key, jet_algorithm.known_types[self.type][key])
			else:
				setattr(self, key, None)
	
	def assign_type_size(self):
		m = re.match("([a-z]+)(\d+)", self.name)
		if m:
			self.type = m.group(1)
			self.size = int(m.group(2))


class pileup_mitigation:
	known_types = {
		"puppi": {"title": "Puppi"},
		"chs": {"title": "CHS"},
		"sk": {"title": "SK"},
		"cs": {"title": "CS"},
	}
	
	def __init__(self, name):
		self.name = name.lower()
		self.update()
	
	def __nonzero__(self):
		return self.known
	
	def update(self):
		self.known = True if self.name in pileup_mitigation.known_types else False
		for key in pileup_mitigation.known_types["chs"].keys():
			if self.known:
				setattr(self, key, pileup_mitigation.known_types[self.name][key])
			else:
				setattr(self, key, None)


class jet_groomer:
	known_types = {
		"p": {"title": "Pruned", "params": {
			"usePruning": cms.bool(True),
			"zcut": cms.double(0.1),
			"rcut_factor": cms.double(0.5),
			"nFilt": cms.int32(2),
		}},
		"s": {"title": "SoftDrop", "params": {
			"useSoftDrop": cms.bool(True),
			"zcut": cms.double(0.1),
			"beta": cms.double(0.0),
		}},
		"f": {"title": "Filtered", "params": {
			"useFiltering": cms.bool(True),
			"rFilt": cms.double(0.3),
			"nFilt": cms.int32(3),
		}},
		"t": {"title": "Trimmed", "params": {
			"useTrimming": cms.bool(True),
			"rFilt": cms.double(0.2),
			"trimPtFracMin": cms.double(0.03),
		}},
	}
	
	def __init__(self, name):
		self.name = name.lower()
		self.update()
	
	def __nonzero__(self):
		# Boolean behavior:
		return self.known
	
	def update(self):
		# Assign attributes based on "known_types":
		self.known = True if self.name in jet_groomer.known_types else False
		if self.known:
			for key in jet_groomer.known_types[self.name].keys():
				setattr(self, key, jet_groomer.known_types[self.name][key])
# /CLASSES:

# VARIABLES:
input_tags_maod = {		# Labels from miniAOD for the input objects.
	"pf": "packedPFCandidates",
	"gn_pat": "packedGenParticles",			# I use these to actually make gnjets.
	"gn_reco": "prunedGenParticles",		# Forming a patjet collection asks for this (not "pat").
	"vp": "offlineSlimmedPrimaryVertices",
	"vs": "slimmedSecondaryVertices",
	"t": "unpackedTracksAndVertices",
	"le": "slimmedElectrons",
	"lm": "slimmedMuons",
}
input_tags_aod = {		# Labels from AOD for the input objects. (I don't use these.)
	"gn": "genParticles",
	"pf": "particleFlow",
	"vp": "offlinePrimaryVertices",
	"vs": "inclusiveCandidateSecondaryVertices",
	"t": "generalTracks",
	"le": "gedGsfElectrons",
	"lm": "muons",
}
input_tags = input_tags_maod
# /VARIABLES

# FUNCTIONS:
def remove_neutrinos(process, sequence, tags_dict):
	# Remove neutrinos from the GN particle collection.
	tag = tags_dict["gn_pat"] + "NoNu"
	setattr(process, tag,
		cms.EDFilter("CandPtrSelector",
			src=cms.InputTag(tags_dict["gn_pat"]),
			cut=cms.string("abs(pdgId) != 12 && abs(pdgId) != 14 && abs(pdgId) != 16")
		)
	)
	sequence += getattr(process, tag)
	tags_dict["gn_pat"] = tag


def apply_chs(process, sequence, tags_dict):
	# Apply charged-hadron subtraction to the PF parcticle collection.
	tag = "{}CHS".format(tags_dict["pf"])
	setattr(process, tag,
		cms.EDFilter('CandPtrSelector',
			src=cms.InputTag(tags_dict["pf"]),
			cut=cms.string('fromPV')
		)
	)
	sequence += getattr(process, tag)
	tags_dict["pf"] = tag
	return tag


def make_pfjet_collection(process, sequence, pfcon_tag, algo, pum, groom=None):
	# Make a PF jet collection:
	tag_jets = "jetsPF" + algo.name.upper() + pum.title
	if groom:
		tag_jets += groom.title
	
	# Jet producer arguments:
	arguments = {
		"src": cms.InputTag(pfcon_tag),
		"doAreaFastjet": True,
	}
##	## Subjet arguments:
#	arguments.update({
#		"writeCompound": cms.bool(True),
#		"jetCollInstanceName": cms.string("subjets"),
#		"nSubjets": cms.int32(3),
#	})
	
	if groom:
		arguments.update({
			"useExplicitGhosts": cms.bool(True),		# The code (https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/RecoJets/JetProducers/plugins/FastjetJetProducer.cc#L152) makes it look like I don't need this, but warnings say otherwise.
		})
		for key, value in groom.params.items():
			arguments[key] = value
		if groom.name in ["s"]:
			arguments["R0"] = cms.double(algo.r)
	
	jet_producer_base = cms.EDProducer("FastjetJetProducer", 
		PFJetParameters,
		AnomalousCellParameters,
#		SubJetParameters,
		cms.PSet(jetAlgorithm=cms.string(algo.title), rParam=cms.double(algo.r)),
	)
	jet_producer = jet_producer_base.clone(**arguments)
	setattr(process, tag_jets, jet_producer)
	sequence += getattr(process, tag_jets)
	
	
	# Recover constituents:
	tag_cons = tag_jets.replace("jets", "constituents")
	con_producer = cms.EDFilter("MiniAODJetConstituentSelector",
		src=cms.InputTag(tag_jets),
		cut=cms.string("")
	)
	setattr(process, tag_cons, con_producer)
	sequence += getattr(process, tag_cons)
	
	return tag_jets


def groom_pfjet_collection(process, sequence, pfjet_tag, patjet_tag, algo, groom):
	# Make a groomed basicjet collection with the same ordering as the ungroomed pfjet collection.
	# NOTES: You can only run this after you run "make_pfjet_collection"
	
	tag_jets = pfjet_tag.replace("jets", "jetsBasic") + groom.title
	tag_mass = pfjet_tag.replace("jetsPF", "mass") + groom.title
	tag_cons = pfjet_tag.replace("jets", "constituents")
	
	# Jet producer arguments:
	arguments_jet = {
		"src": cms.InputTag(tag_cons + ':constituents'),
		"writeCompound": cms.bool(True),
		"doAreaFastjet": cms.bool(True),
		"jetCollInstanceName": cms.string('constituents'),
		"useExplicitGhosts": cms.bool(True),
	}
	for key, value in groom.params.items(): arguments_jet[key] = value
	if groom.name in ["s"]: arguments_jet["R0"] = cms.double(algo.r)
	
	# Make the groomed jet producer:
	jet_producer_base = cms.EDProducer("FastjetJetProducer", 
		PFJetParameters,
		AnomalousCellParameters,
		cms.PSet(jetAlgorithm=cms.string(algo.title), rParam=cms.double(algo.r)),
	)
	jet_producer = jet_producer_base.clone(**arguments_jet)
	setattr(process, tag_jets, jet_producer)
	sequence += getattr(process, tag_jets)
	
	# Make the groomed mass producer:
	arguments_mass = {
		"src": cms.InputTag(pfjet_tag),
		"matched": cms.InputTag(tag_jets),
		"distMax": cms.double(algo.r),
	}
	#mass_producer = PFJets.ak8PFJetsCHSPrunedMass.clone(**arguments_mass)
	mass_producer = ak8PFJetsCHSPrunedMass.clone(**arguments_mass) 
	setattr(process, tag_mass, mass_producer)
	sequence += getattr(process, tag_mass)
	getattr(process, patjet_tag).userData.userFloats.src += [tag_mass]
	
	return tag_jets


def make_pfsubjet_collection(process, sequence, pfcon_tag, algo, pum, nsubjets=4):
	# Make a PF jet collection:
	tag_subjets = "subjetsPF" + algo.name.upper() + pum.title
	
	# Subjet producer arguments:
	arguments = {
		"src": cms.InputTag(pfcon_tag),
		"nSubjets": cms.int32(nsubjets),
	}
	
	subjet_producer_base = cms.EDProducer("SubJetProducer", 
		PFJetParameters,
		AnomalousCellParameters,
		SubJetParameters,
		cms.PSet(jetAlgorithm=cms.string(algo.title), rParam=cms.double(algo.r)),
	)
	subjet_producer = subjet_producer_base.clone(**arguments)
	setattr(process, tag_subjets, subjet_producer)
	sequence += getattr(process, tag_subjets)
	
	return tag_subjets


def make_gnjet_collection(process, sequence, collection, algo, suffix="", data=False):
	tag = "jetsGN" + algo.name.upper() + suffix
	if data: return tag
	
	jet_producer = ak4GenJets.clone(
		src=cms.InputTag(collection),
		rParam=algo.r,
		jetAlgorithm=algo.title,
	)
	setattr(process, tag, jet_producer)
	sequence += getattr(process, tag)
	return tag

def make_patjet_collection(process, sequence, pfjet_tag, gnjet_tag, tags_dict, algo, tag, matching=True):
	# "addJetCollection" makes two sets of PF and GN jets each: "patJets" + tag and "selectedPatJets" + tag.
#	print tags_dict
	addJetCollection(process,
		labelName=tag,
		jetSource=cms.InputTag(pfjet_tag),
		algo=algo.name,
		rParam=algo.r,
		jetCorrections=None,
		genJetCollection=cms.InputTag(gnjet_tag),
		pfCandidates=cms.InputTag(tags_dict["pf"]),
		genParticles=cms.InputTag(tags_dict["gn_reco"]),
		pvSource=cms.InputTag(tags_dict["vp"]),
		svSource=cms.InputTag(tags_dict["vs"]),
		elSource=cms.InputTag(tags_dict["le"]),
		muSource=cms.InputTag(tags_dict["lm"]),
#		btagDiscriminators=bTagDiscriminators,
#		btagInfos=bTagInfos,
#		getJetMCFlavour=GetJetMCFlavour,
		outputModules = ['outputFile'],
		getJetMCFlavour=matching,
	)
	tag = "patJets" + tag
	getattr(process, tag).addTagInfos = cms.bool(True)
	return tag

def add_tau_variables(process, sequence, pfjet_tag, patjet_tag, algo, taus):
#	tag = "taus" + patjet_tag.replace("patJets", "")
	tag = "taus" + pfjet_tag.replace("jetsPF", "")
	tau_calculator = Njettiness.clone(
		src=cms.InputTag(pfjet_tag),
		Njets=cms.vuint32(taus),
		# variables for measure definition: 
		measureDefinition = cms.uint32(0),    # CMS default is normalized measure
		beta=cms.double(1.0),                 # CMS default is 1
		R0=cms.double(algo.r),                # CMS default is jet cone size
		Rcutoff=cms.double(999.0),            # not used by default
		# variables for axes definition:
		axesDefinition=cms.uint32(6),         # CMS default is 1-pass KT axes
		nPass=cms.int32(999),                 # not used by default
		akAxesR0=cms.double(-999.0),          # not used by default
	)
	setattr(process, tag, tau_calculator)
	
	for tau in taus:
		getattr(process, patjet_tag).userData.userFloats.src += ['{}:tau{}'.format(tag, tau)]
	
	sequence += getattr(process, tag)
	return tag


def add_subjet_variables(process, sequence, pfjet_tag, patjet_tag, algo, nsubjets):
	tag = "subjets" + patjet_tag.replace("patJets", "")
	subjet_calculator = Subjetter.clone(
		src=cms.InputTag(pfjet_tag),
		nSubjets=cms.uint32(nsubjets),
	)
	setattr(process, tag, subjet_calculator)
	
	for var in subjet_variables:
		for isj in range(nsubjets):
			getattr(process, patjet_tag).userData.userFloats.src += ['{}:{}{}'.format(tag, var, isj)]
	
	sequence += getattr(process, tag)
	return tag


def add_jet_collection(
	process,
	algo_name="ca12",               # The name of the jet algorithm to use.
	pum_name="chs",                 # Pileup mitigation name.
	groom_names=None,               # List of groomer names.
	data=False,                     # Is the input dataset data? (False: MC, True: data)
	tags_dict=input_tags.copy(),
	output="out",                   # The name of the PoolOutputModule.
	taus=None,
	keep_all=False,
):
	# Arguments:
	tags_dict_original = tags_dict.copy()
	tags_dict = tags_dict.copy()
	
	algo = jet_algorithm(algo_name)   # Jet algorithm object to use.
	if not algo:
		print "ERROR: I don't know about the jet algorithm you called {}.".format(algo.name)
		sys.exit()
	pum = pileup_mitigation(pum_name)   # Pileup mitigation object to use.
	#p

	#p
	grooms = []
	if groom_names != None:
		grooms = [jet_groomer(groom_name) for groom_name in groom_names]   # Groomer objects to use.
	
	sequence = cms.Sequence()         # Sequence of modules to run.
	#p                                                                                                                                                                                                                                    

        #p
#	## Data versus MC:
#	if data:
#		getJetMCFlavour = False
#		getSubjetMCFlavour = False
	
	# Filter constituents:
	if not data: remove_neutrinos(process, sequence, tags_dict)
	
	# Apply PUM (pileup mitigation):
	if pum.name == "chs":
		apply_chs(process, sequence, tags_dict)
	else:
		print "ERROR: The PUM you called {} is not implemented.".format(pum.name)
		sys.exit()
	#p                                                                                                                                                                                                                                    

        #p
	# Make normal jet collections:
	pfjet_tag = make_pfjet_collection(process, sequence, tags_dict["pf"], algo, pum)
#	pfsubjet_tag = make_pfsubjet_collection(process, sequence, tags_dict["pf"], algo, pum)
	gnjet_tag = make_gnjet_collection(process, sequence, tags_dict["gn_pat"], algo, "NoNu", data=data)
	patjet_tag = make_patjet_collection(process, sequence, pfjet_tag, gnjet_tag, tags_dict, algo, algo.name.upper() + pum.title, matching=not data)
	## Nsubjettiness for ungroomed collections:
	if taus: tau_tag = add_tau_variables(process, sequence, pfjet_tag, patjet_tag, algo, taus)
	
	if algo.name == "ca12": subjet_tag = add_subjet_variables(process, sequence, pfjet_tag, patjet_tag, algo, 4)
	
#	blah = make_groomed_jet_collection(process, sequence, pfjet_tag, algo, jet_groomer("p"))
#	blah2 = make_patjet_collection(process, sequence, blah, gnjet_tag, tags_dict, algo, algo.name.upper() + pum.title + jet_groomer("p").title, matching=not data)
	
	# Make groomed collections:
	basicjet_tags_groomed = {}
	pfjet_tags_groomed = {}
	patjet_tags_groomed = {}
	tau_tags_groomed = {}
	for groom in grooms:
		basicjet_tags_groomed[groom.name] = groom_pfjet_collection(process, sequence, pfjet_tag, patjet_tag, algo, groom)
		pfjet_tags_groomed[groom.name] = make_pfjet_collection(process, sequence, tags_dict["pf"], algo, pum, groom)
		patjet_tags_groomed[groom.name] = make_patjet_collection(process, sequence, pfjet_tags_groomed[groom.name], gnjet_tag, tags_dict, algo, algo.name.upper() + pum.title + groom.title, matching=False)
		## Nsubjettiness for groomed collections:
		if taus: tau_tags_groomed[groom.name] = add_tau_variables(process, sequence, pfjet_tags_groomed[groom.name], patjet_tags_groomed[groom.name], algo, taus)		#patjet_tags_groomed[groom.name]

	# Define output:
	products_keep = [
#		"*",
#		"*_jetsPF*_*_*",		# This is the pfjet collection corresponding to the ungroomed patjet collection.
#		"*_jetsBasicPF*_*_*",		# This is a groomed basicjet (4-vector) collection with the same order as the ungroomed patjet collection.
#		"*_constituentsPF*_*_*",
#		"*_jets*_subjets_*",
#		"*_patJets*_pfCandidates_*",
		"*_selectedPatJets*_*_*",
		"*_mass*_*_*",
		"*_taus*_*_*",
		"*_subjets*_*_*",
#		"*_packedPFCandidatesCHS_*_*",
	]
#	if keep_all: products_keep.extend(["*_jets*_*_*", "*_patJets*_*_*"])
	
	products_drop = [
#		"*_jets*_*_*",
		"*_patJets*_calo*_*",
		"*_patJets*_tagInfos_*",
		"*_selectedPatJets*Filtered_genJets_*",
		"*_selectedPatJets*Pruned_genJets_*",
		"*_selectedPatJets*SoftDrop_genJets_*",
		"*_selectedPatJets*Trimmed_genJets_*",
		"*_selectedPatJets*Filtered_pfCandidates_*",
		"*_selectedPatJets*Pruned_pfCandidates_*",
		"*_selectedPatJets*SoftDrop_pfCandidates_*",
		"*_selectedPatJets*Trimmed_pfCandidates_*",
		"*_selectedPatJets*_calo*_*",
		"*_selectedPatJets*_tagInfos_*",
	]
	#p                                                                                                                                                                                                                                    

        #p
	getattr(process, output).outputCommands.extend(["keep {}".format(tag) for tag in products_keep])
	getattr(process, output).outputCommands.extend(["drop {}".format(tag) for tag in products_drop])
        #p                                                                                                                                                                                                                                    

        #p
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	# You can use this area for testing stuff:
	algo = jet_algorithm("ca12")
	pum = pileup_mitigation("chs")
	print algo.type, algo.size, algo.title
	print pum.name, bool(pum)
	
#	import RecoJets.Configuration.RecoPFJets_cff as pfjets
#	print type(getattr(pfjets, 'ak4PFJets'))
# /MAIN
