####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
print "Importing packages ..."
import sys            # Allows "sys.exit()"
import argparse       # For commandline options
import random
import numpy
from time import time
from truculence import *
from decortication import dataset, analyzer, fat
from array import array
print "\tImporting ROOT ..."
from ROOT import *
# /IMPORTS

# CLASSES:
# /CLASSES

## VARIABLES:
precut_pt = 150           # pT cut applied to all jets in the pair selection
precut_m = 50             # m cut applied to all jets in the pair selection
precut_eta = 2.5          # eta cut applied to all jets in the pair selection
L = 10000                 # Luminosity (in inverse pb)
variables = [
	 "sigma", "nevents", "w", "pt_hat", "ak4_pf_ht",
	"ak4_pf_pt",
	"ak8_pf_px", "ak8_pf_py", "ak8_pf_pz", "ak8_pf_e", "ak8_pf_pt", "ak8_pf_M", "ak8_pf_m", "ak8_pf_eta", "ak8_pf_phi","ak8_pf_tau1", "ak8_pf_tau2","ak8_pf_tau3", "ak8_pf_tau4", "ak8_pf_ht",
	"ca12_pf_px", "ca12_pf_py", "ca12_pf_pz", "ca12_pf_e", "ca12_pf_pt", "ca12_pf_M", "ca12_pf_m", "ca12_pf_eta", "ca12_pf_phi","ca12_pf_tau1", "ca12_pf_tau2","ca12_pf_tau3", "ca12_pf_tau4", "ca12_pf_ht",
]
variables_out = {
	# Event variables:
	"w": 1,            # The event weight
	"sigma": 1,        # The process cross section
	"n0": 1,
	"n1": 1,
	"n2": 1,
	"njets": 1,        # Njettiness of the event (the pt_cut is a parameter)
	"pt_hat": 1,       #
	"ht": 1,           # HT of the event (calculated from AK4 PF jets)
	# Fatjet pair variables:
	"m": 2,            # Trimmed Mass
	"dm": 1,           # Delta m of the fatjet pair
	"mavg": 1,         # Delta trimmed mass of the fatjet pair
	"pt": 2,           # pT of each fatjet in the pair
	"e": 2,            # E of each fatjet in the pair
	"eta": 2,          # eta of each fatjet in the pair
	"phi": 2,          # phi of each fatjet in the pair
	"deta": 1,         # Delta eta between the fatjets of the pair
	"tau1": 2,         # Nsubjettiness of the fatjets in the pair
	"tau2": 2,         # Nsubjettiness of the fatjets in the pair
	"tau3": 2,         # Nsubjettiness of the fatjets in the pair
	"tau4": 2,         # Nsubjettiness of the fatjets in the pair
	"tau42": 2,        # Nsubjettiness of the fatjets in the pair
	"tau42s": 1,       # Product of tau42 of each fatjet in the pair
	"tau21": 2,        # Nsubjettiness of the fatjets in the pair
	"tau21s": 1,       # Product of tau21 of each fatjet in the pair
	# Preselection:
	"precut_pt": 2,    # Cut on the pT of pair-candidate fatjets
	"precut_m": 2,     # Cut on the mass of pair-candidate fatjets
	"precut_eta": 2,   # Cut on the eta of pair-candidate fatjets
}
## /VARIABLES

# FUNCTIONS:

def arguments():
	results = {}
	
	# Argument parser:
	parser = argparse.ArgumentParser()
	
	parser.add_argument(
		"-d", "--dataset", dest="keys",
		default=None,
		help="The keys of the datasets you want to anatuplize (e.g. \"sq200tojjjj\")",
		metavar="STR"
	)
	parser.add_argument(
		"-n", "--nevents", dest="n",
		type=int,
		default=1000,
		help="The number of events you want to provess for each dataset",
		metavar="INT"
	)
	
	args = parser.parse_args()
	
	# Interpretation:
	keys = args.keys
	if keys:
		keys = keys.split(",")
	results["args"] = args
	results["datasets"] = dataset.get_datasets(key=keys)
	
	return results

def treat_event(loop, event):		# Where "loop" refers to an event_loop object
#	print "in treat"
	branches = loop.branches
	
	# Empty branches:
	for key in branches.keys():
#		print branches[key]
		for i in xrange(len(branches[key])):
			branches[key][i] = -1

	# Variables:
	n_events_tc = loop.n_total            # The total number of events in the TChain
	n_events = loop.n                     # The number of events to anatuplize
	nevents = event.nevents[0]            # The number of events in the tuple (should not equal n_events_tc for multi-tuple datasets)
#	if n_events_tc != nevents:
#		print "WARNING: Event numbers don't match:\n\tn_events_tc = {}\n\tnevents = {}".format(n_events_tc, nevents)
#		sys.exit()
	sigma = event.sigma[0]
	w = event.w[0]*n_events_tc/n_events       # The event weight (how many events each event represents)
#	if event.nevent[0] == 1:
#		print w, n_events_tc/n_events
#		sys.exit()
	try:
		pt_hat = event.pt_hat[0]          # This is pt_hat for qcdp (and ht for qcdmg? no, I think you need ak4_ht)
	except:
		pt_hat = -1                       # This branch doesn't exist for signals
	n_jets = len(event.ak4_pf_pt)         # This is the number of AK4 jets in the event with pT > 10 GeV.
	
	# Selection:
	pair = fat.get_pair(event, cut_pt=precut_pt, cut_m=precut_m, cut_eta=precut_eta, ca=False, r=8)
	if pair:
		# Pair variables:
		mavg = numpy.mean([p.m for p in pair])
		dm = physics.delta_m(pair)
		tau = {}
		for i in xrange(1, 5):
			tau[i] = (getattr(pair[0], "tau{}".format(i)), getattr(pair[1], "tau{}".format(i)))
		try:
			tau[42] = (pair[0].tau4/pair[0].tau2, pair[1].tau4/pair[1].tau2)
		except:		# Division by zero!
			tau[42] = 100
		try:
			tau[24] = (1/tau[42][0], 1/tau[42][1])
		except:
			tau[24] = 0
		try:
			tau[21] = (pair[0].tau2/pair[0].tau1, pair[1].tau2/pair[1].tau1)
		except:
			tau[21] = 100
		deta = abs(pair[0].eta - pair[1].eta)
		
		# Fill non-pair branches:
		branches["n0"][0] = nevents
		branches["n1"][0] = n_events_tc
		branches["n2"][0] = n_events
		branches["w"][0] = w
		branches["njets"][0] = n_jets
		branches["pt_hat"][0] = pt_hat
		branches["ht"][0] = event.ak4_pf_ht[0]
		# Fill first element of selection branches:
		branches["precut_pt"][1] = precut_pt
		branches["precut_m"][1] = precut_m
		branches["precut_eta"][1] = precut_eta
		# Fill pair branches:
		branches["m"][0] = pair[0].m
		branches["m"][1] = pair[1].m
		branches["pt"][0] = pair[0].pt
		branches["pt"][1] = pair[1].pt
		branches["e"][0] = pair[0].e
		branches["e"][1] = pair[1].e
		branches["eta"][0] = pair[0].eta
		branches["eta"][1] = pair[1].eta
		branches["deta"][0] = deta
		branches["phi"][0] = pair[0].phi
		branches["phi"][1] = pair[1].phi
		branches["dm"][0] = dm
		branches["mavg"][0] = mavg
		for ntau in [1, 2, 3, 4, 42, 21]:
			word = "tau{}".format(ntau)
			branches[word][0] = tau[ntau][0]
			branches[word][1] = tau[ntau][1]
		branches["tau42s"][0] = tau[42][0]*tau[42][1]
		branches["tau21s"][0] = tau[21][0]*tau[21][1]
		# Preselection:
		branches["precut_pt"][0] = 1
		branches["precut_m"][0] = 1
		branches["precut_eta"][0] = 1
		
#		print "filling tree"
		loop.tt_out.Fill()

def main():
	# Setup:
	## ROOT:
	gROOT.SetBatch()
	
	## Arguments:
	arg_result = arguments()
	args = arg_result["args"]
	dsd = arg_result["datasets"]
#	print dsd
	
	## Analyzer object:
	tuples = {key: ["root://cmsxrootd.fnal.gov/" + ds.tuple_path for ds in dss] for key, dss in dsd.iteritems()}
	ana = analyzer.analyzer(tuples, save=True, v=True)		# Add "out_dir=" and "out_file=".
	ana.define_branches(variables_out)
#	branches = create_tuples(ana)
#	colors = color.pick(len(tuples.keys()))
	
	# Event loop:
	for key, loop in ana.loops.iteritems():
#		print key
		loop.treatment = treat_event
#		print "here"
		loop.run(n=args.n)
#	event_loop(ana.tt[key]["analyzer/events"], branches[key], ana.tuples[key], n_events=n_events_sq)

	# Output:
	ana.write()
	print ana.out_path
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
