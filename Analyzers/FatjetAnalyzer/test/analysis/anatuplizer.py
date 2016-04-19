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
#precut_pt = 400           # pT cut applied to all jets in the pair selection
#precut_m = 50             # m cut applied to all jets in the pair selection
#precut_eta = 2.5          # eta cut applied to all jets in the pair selection
precut_pt = 0           # pT cut applied to all jets in the pair selection
precut_m = 0             # m cut applied to all jets in the pair selection
precut_eta = 10          # eta cut applied to all jets in the pair selection
L = 10000                 # Luminosity (in inverse pb)
variables = [
	 "sigma", "w", "pt_hat", "ak4_pf_ht",
	"ak4_pf_pt",
	"ak8_pf_px", "ak8_pf_py", "ak8_pf_pz", "ak8_pf_e", "ak8_pf_pt", "ak8_pf_M", "ak8_pf_m_t", "ak8_pf_m_p", "ak8_pf_m_s", "ak8_pf_m_f", "ak8_pf_eta", "ak8_pf_phi","ak8_pf_tau1", "ak8_pf_tau2","ak8_pf_tau3", "ak8_pf_tau4", "ak8_pf_tau5", "ak8_pf_ht",
	"ca12_pf_px", "ca12_pf_py", "ca12_pf_pz", "ca12_pf_e", "ca12_pf_pt", "ca12_pf_M", "ca12_pf_m_t", "ca12_pf_m_p", "ca12_pf_m_s", "ca12_pf_m_f", "ca12_pf_eta", "ca12_pf_phi","ca12_pf_tau1", "ca12_pf_tau2","ca12_pf_tau3", "ca12_pf_tau4", "ca12_pf_tau5", "ca12_pf_ht",
]
variables_out = {
	# Event variables:
	"w": 1,            # The event weight
	"sigma": 1,        # The process cross section
	"n1": 1,
	"n2": 1,
	"njets": 1,        # Njettiness of the event (the pt_cut is a parameter)
	"pt_hat": 1,       #
	"htak4": 1,        # HT of the event calculated from all AK4 PF jets
	"htak8": 1,        # HT of the event calculated from AK8 PF jets with pT>150
	# Fatjet pair variables:
	"M": 2,            # Ungroomed Mass
	"m_t": 2,          # Trimmed Mass
	"m_p": 2,          # Pruned Mass
	"m_s": 2,          # SoftDrop Mass
	"m_f": 2,          # Filtered Mass
	"M4": 4,           # Pruned masses of first four jets
	"m4_p": 4,         # Pruned masses of first four jets
	"m4_t": 4,         # Pruned masses of first four jets
	"dM": 1,           # Delta M of the fatjet pair
	"dm_t": 1,         # Delta m of the fatjet pair
	"dm_p": 1,         # Delta m of the fatjet pair
	"dm_s": 1,         # Delta m of the fatjet pair
	"dm_f": 1,         # Delta m of the fatjet pair
	"Mavg": 1,         # Delta trimmed mass of the fatjet pair
	"mavg_t": 1,       # Delta trimmed mass of the fatjet pair
	"mavg_p": 1,       # Delta trimmed mass of the fatjet pair
	"mavg_s": 1,       # Delta trimmed mass of the fatjet pair
	"mavg_f": 1,       # Delta trimmed mass of the fatjet pair
	"Masy": 1,         # Mass asymmetry: dM/(M0 + M1)
	"masy_t": 1,       # Mass asymmetry: dm/(m0 + m1)
	"masy_p": 1,       # Mass asymmetry: dm/(m0 + m1)
	"masy_s": 1,       # Mass asymmetry: dm/(m0 + m1)
	"masy_f": 1,       # Mass asymmetry: dm/(m0 + m1)
	"pt": 2,           # pT of each fatjet in the pair
	"e": 2,            # E of each fatjet in the pair
	"eta": 2,          # eta of each fatjet in the pair
	"phi": 2,          # phi of each fatjet in the pair
	"deta": 1,         # Delta eta between the fatjets of the pair
	"tau1": 2,         # Nsubjettiness of the fatjets in the pair
	"tau2": 2,         # Nsubjettiness of the fatjets in the pair
	"tau3": 2,         # Nsubjettiness of the fatjets in the pair
	"tau4": 2,         # Nsubjettiness of the fatjets in the pair
#	"tau5": 2,         # Nsubjettiness of the fatjets in the pair
	"tau42": 2,        # Nsubjettiness of the fatjets in the pair
	"tau42s": 1,       # Product of tau42 of each fatjet in the pair
	"tau43": 2,        # Nsubjettiness of the fatjets in the pair
	"tau43s": 1,       # Product of tau43 of each fatjet in the pair
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
		"-d", "--dataset", dest="processes",
		default=None,
		help="The processes of the datasets you want to anatuplize (e.g. \"sq200to4j\")",
		metavar="STR"
	)
	parser.add_argument(
		"-n", "--nevents", dest="n",
		type=int,
		default=-1,
		help="The number of events you want to process for each dataset",
		metavar="INT"
	)
	parser.add_argument(
		"-s", "--suffix", dest="s",
		type=str,
		default='',
		help="The words after 'tuple_' if there are any in the dataset you want.",
		metavar="STR"
	)
	
	args = parser.parse_args()
	
	# Interpretation:
	processes = args.processes
	if processes:
		processes = processes.split(",")
	results["args"] = args
	results["datasets"] = dataset.get_datasets(process=processes, set_info=True, suffix=args.s)
	
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
	pair = fat.get_pair(event, cut_pt=precut_pt, cut_m=precut_m, cut_eta=precut_eta, ca=True, r=12, leading=True)
	if pair:
		# Pair variables:
		Mavg = numpy.mean([p.M for p in pair])
		mavg_t = numpy.mean([p.m_t for p in pair])
		mavg_p = numpy.mean([p.m_p for p in pair])
		mavg_s = numpy.mean([p.m_s for p in pair])
		mavg_f = numpy.mean([p.m_f for p in pair])
		dM = physics.delta_m(pair)
		dm_t = physics.delta_m(pair, groom="t")
		dm_p = physics.delta_m(pair, groom="p")
		dm_s = physics.delta_m(pair, groom="s")
		dm_f = physics.delta_m(pair, groom="f")
#		print dM, Mavg
		Masy = dM/Mavg/2
		masy_t = dm_t/mavg_t/2
		masy_p = dm_p/mavg_p/2
		masy_s = dm_s/mavg_s/2
		masy_f = dm_f/mavg_f/2
		tau = {}
#		for i in xrange(1, 6):
		for i in xrange(1, 5):
			tau[i] = (getattr(pair[0], "tau{}".format(i)), getattr(pair[1], "tau{}".format(i)))
		try:
			tau[42] = (pair[0].tau4/pair[0].tau2, pair[1].tau4/pair[1].tau2)
		except:		# Division by zero!
			tau[42] = (100, 100)
		try:
			tau[43] = (pair[0].tau4/pair[0].tau3, pair[1].tau4/pair[1].tau3)
		except:		# Division by zero!
			tau[43] = (100, 100)
		try:
			tau[24] = (1/tau[42][0], 1/tau[42][1])
		except:
			tau[24] = (0, 0)
		try:
			tau[21] = (pair[0].tau2/pair[0].tau1, pair[1].tau2/pair[1].tau1)
		except:
			tau[21] = (100, 100)
		deta = abs(pair[0].eta - pair[1].eta)
		
		# Fill non-pair branches:
		branches["n1"][0] = n_events_tc
		branches["n2"][0] = n_events
		branches["sigma"][0] = sigma
		branches["w"][0] = w
		branches["njets"][0] = n_jets
		branches["pt_hat"][0] = pt_hat
		branches["htak4"][0] = event.ak4_pf_ht[0]
		branches["htak8"][0] = event.ak8_pf_ht[0]
		# Fill first element of selection branches:
		branches["precut_pt"][1] = precut_pt
		branches["precut_m"][1] = precut_m
		branches["precut_eta"][1] = precut_eta
		# Fill pair branches:
		branches["M"][0] = pair[0].M
		branches["M"][1] = pair[1].M
		branches["m_t"][0] = pair[0].m_t
		branches["m_t"][1] = pair[1].m_t
		branches["m_p"][0] = pair[0].m_p
		branches["m_p"][1] = pair[1].m_p
		branches["m_s"][0] = pair[0].m_s
		branches["m_s"][1] = pair[1].m_s
		branches["m_f"][0] = pair[0].m_f
		branches["m_f"][1] = pair[1].m_f
		if len(event.ca12_pf_M) >= 4:
			for i in range(4):
				branches["M4"][i] = event.ca12_pf_M[i]
		else:
			for i in range(4):
				branches["M4"][i] = -1
		if len(event.ca12_pf_m_t) >= 4:
			for i in range(4):
				branches["m4_t"][i] = event.ca12_pf_m_t[i]
		else:
			for i in range(4):
				branches["m4_t"][i] = -1
		if len(event.ca12_pf_m_p) >= 4:
			for i in range(4):
				branches["m4_p"][i] = event.ca12_pf_m_p[i]
		else:
			for i in range(4):
				branches["m4_p"][i] = -1
		branches["pt"][0] = pair[0].pt
		branches["pt"][1] = pair[1].pt
		branches["e"][0] = pair[0].e
		branches["e"][1] = pair[1].e
		branches["eta"][0] = pair[0].eta
		branches["eta"][1] = pair[1].eta
		branches["deta"][0] = deta
		branches["phi"][0] = pair[0].phi
		branches["phi"][1] = pair[1].phi
		branches["dM"][0] = dM
		branches["dm_t"][0] = dm_t
		branches["dm_p"][0] = dm_p
		branches["dm_s"][0] = dm_s
		branches["dm_f"][0] = dm_f
		branches["Mavg"][0] = Mavg
		branches["mavg_t"][0] = mavg_t
		branches["mavg_p"][0] = mavg_p
		branches["mavg_s"][0] = mavg_s
		branches["mavg_f"][0] = mavg_f
		branches["Masy"][0] = Masy
		branches["masy_t"][0] = masy_t
		branches["masy_p"][0] = masy_p
		branches["masy_s"][0] = masy_s
		branches["masy_f"][0] = masy_f
		for ntau in [1, 2, 3, 4, 42, 43, 21]:
#		for ntau in [1, 2, 3, 4, 5, 42, 43, 21]:
			word = "tau{}".format(ntau)
			branches[word][0] = tau[ntau][0]
			branches[word][1] = tau[ntau][1]
		branches["tau42s"][0] = tau[42][0]*tau[42][1]
		branches["tau43s"][0] = tau[43][0]*tau[43][1]
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
	tuples = {}
	for process, dss in dsd.iteritems():
		tuples[process] = []
		for ds in dss:
			if ds.analyze:
				for path in ds.tuple_path:
					path_full = "root://cmsxrootd.fnal.gov/" + path
					tuples[process].append(path_full)
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
