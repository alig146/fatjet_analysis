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
from decortication import dataset, analyzer, fat, variables
from array import array
print "\tImporting ROOT ..."
from ROOT import *
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
vs_out = {
	# Event variables:
	"run": 1,
	"lumi": 1,
	"event": 1,
	"w": 1,            # The event weight
	"sigma": 1,        # The process cross section
	"njets": 1,        # Njettiness of the event (the pt_cut is a parameter)
	"pt_hat": 1,       #
	"htak4": 1,        # HT of the event calculated from all AK4 PF jets
	"htak8": 1,        # HT of the event calculated from AK8 PF jets with pT>150
	# Fatjet pair variables:
	"m": 2,            # Ungroomed Mass
	"m_f": 2,          # Filtered Mass
	"m_p": 2,          # Pruned Mass
	"m_s": 2,          # SoftDrop Mass
	"m_t": 2,          # Trimmed Mass
	"m4": 4,           # Ungroomed masses of first four jets
	"m4_p": 4,         # Pruned masses of first four jets
	"dm": 1,           # Delta m of the fatjet pair
	"dm_f": 1,         # Delta m of the fatjet pair
	"dm_p": 1,         # Delta m of the fatjet pair
	"dm_s": 1,         # Delta m of the fatjet pair
	"dm_t": 1,         # Delta m of the fatjet pair
	"mavg": 1,         # Delta ungroomed mass of the fatjet pair
	"mavg_f": 1,       # Delta filtered mass of the fatjet pair
	"mavg_p": 1,       # Delta pruned mass of the fatjet pair
	"mavg_s": 1,       # Delta softdropped mass of the fatjet pair
	"mavg_t": 1,       # Delta trimmed mass of the fatjet pair
	"masy": 1,         # Mass asymmetry: dM/(m0 + m1) of the ungroomed pair
	"masy_f": 1,       # Mass asymmetry of the filtered pair
	"masy_p": 1,       # Mass asymmetry of the pruned pair
	"masy_s": 1,       # Mass asymmetry of the soft-dropped pair
	"masy_t": 1,       # Mass asymmetry of the trimmed pair
	"mdi_p": 1,        # ?
	"px": 2,           # px of each fatjet in the ungroomed pair
	"px_p": 2,         # px of each fatjet in the pruned pair
	"py": 2,           # py of each fatjet in the ungroomed pair
	"py_p": 2,         # py of each fatjet in the pruned pair
	"pz": 2,           # pz of each fatjet in the ungroomed pair
	"pz_p": 2,         # pz of each fatjet in the pruned pair
	"pt": 2,           # pT of each fatjet in the ungroomed pair
	"pt_p": 2,         # pT of each fatjet in the pruned pair
	"ptasy": 1,        # pT asymmetry of the ungroomed pair
	"ptasy_p": 1,      # pT asymmetry of the pruned pair
	"e": 2,            # E of each fatjet in the ungroomed pair
	"e_p": 2,          # E of each fatjet in the pruned pair
	"eta": 2,          # eta of each fatjet in the ungroomed pair
	"eta_p": 2,        # eta of each fatjet in the pruned pair
	"deta": 1,         # Delta eta between the fatjets of the ungroomed pair
	"deta_p": 1,       # Delta eta between the fatjets of the pruned pair
	"phi": 2,          # phi of each fatjet in the ungroomed pair
	"phi_p": 2,        # phi of each fatjet in the pruned pair
	"dr": 1,           # Delta R between the fatjets of the ungroomed pair
	"dr_p": 1,         # Delta R between the fatjets of the pruned pair
	"tau1": 2,         # Nsubjettiness of the fatjets in the ungroomed pair
	"tau1_f": 2,       # Nsubjettiness of the fatjets in the filtered pair
	"tau1_p": 2,       # Nsubjettiness of the fatjets in the pruned pair
	"tau1_s": 2,       # Nsubjettiness of the fatjets in the soft-dropped pair
	"tau1_t": 2,       # Nsubjettiness of the fatjets in the timmed pair
	"tau2": 2,         # Nsubjettiness of the fatjets in the ungroomed pair
	"tau2_f": 2,       # Nsubjettiness of the fatjets in the filtered pair
	"tau2_p": 2,       # Nsubjettiness of the fatjets in the pruned pair
	"tau2_s": 2,       # Nsubjettiness of the fatjets in the soft-dropped pair
	"tau2_t": 2,       # Nsubjettiness of the fatjets in the trimmed pair
	"tau3": 2,         # Nsubjettiness of the fatjets in the ungroomed pair
	"tau3_f": 2,       # Nsubjettiness of the fatjets in the filtered pair
	"tau3_p": 2,       # Nsubjettiness of the fatjets in the pruned pair
	"tau3_s": 2,       # Nsubjettiness of the fatjets in the soft-dropped pair
	"tau3_t": 2,       # Nsubjettiness of the fatjets in the trimmed pair
	"tau4": 2,         # Nsubjettiness of the fatjets in the ungroomed pair
	"tau4_f": 2,       # Nsubjettiness of the fatjets in the filtered pair
	"tau4_p": 2,       # Nsubjettiness of the fatjets in the pruned pair
	"tau4_s": 2,       # Nsubjettiness of the fatjets in the soft-dropped pair
	"tau4_t": 2,       # Nsubjettiness of the fatjets in the trimmed pair
#	"tau5": 2,         # Nsubjettiness of the fatjets in the pair
	"tau43": 2,        # tau4/tau3 of each fatjet in the ungroomed pair
	"tau43_f": 2,      # tau4/tau3 of each fatjet in the filtered pair
	"tau43_p": 2,      # tau4/tau3 of each fatjet in the pruned pair
	"tau43_s": 2,      # tau4/tau3 of each fatjet in the soft-dropped pair
	"tau43_t": 2,      # tau4/tau3 of each fatjet in the trimmed pair
	"tau43s": 1,       # Product of tau43 of each fatjet in the ungroomed pair
	"tau42": 2,        # tau4/tau2 of each fatjet in the ungroomed pair
	"tau42_f": 2,      # tau4/tau2 of each fatjet in the filtered pair
	"tau42_p": 2,      # tau4/tau2 of each fatjet in the pruned pair
	"tau42_s": 2,      # tau4/tau2 of each fatjet in the soft-dropped pair
	"tau42_t": 2,      # tau4/tau2 of each fatjet in the trimmed pair
	"tau42s": 1,       # Product of tau42 of each fatjet in the ungroomed pair
	"tau41": 2,        # tau4/tau1 of each fatjet in the ungroomed pair
	"tau41_f": 2,      # tau4/tau1 of each fatjet in the filtered pair
	"tau41_p": 2,      # tau4/tau1 of each fatjet in the pruned pair
	"tau41_s": 2,      # tau4/tau1 of each fatjet in the soft-dropped pair
	"tau41_t": 2,      # tau4/tau1 of each fatjet in the trimmed pair
	"tau41s": 1,       # Product of tau41 of each fatjet in the ungroomed pair
	"tau32": 2,        # tau3/tau2 of each fatjet in the ungroomed pair
	"tau32_f": 2,      # tau3/tau2 of each fatjet in the filtered pair
	"tau32_p": 2,      # tau3/tau2 of each fatjet in the pruned pair
	"tau32_s": 2,      # tau3/tau2 of each fatjet in the soft-dropped pair
	"tau32_t": 2,      # tau3/tau2 of each fatjet in the trimmed pair
	"tau32s": 1,       # Product of tau32 of each fatjet in the ungroomed pair
	"tau31": 2,        # tau3/tau1 of each fatjet in the ungroomed pair
	"tau31_f": 2,      # tau3/tau1 of each fatjet in the filtered pair
	"tau31_p": 2,      # tau3/tau1 of each fatjet in the pruned pair
	"tau31_s": 2,      # tau3/tau1 of each fatjet in the soft-dropped pair
	"tau31_t": 2,      # tau3/tau1 of each fatjet in the trimmed pair
	"tau31s": 1,       # Product of tau31 of each fatjet in the ungroomed pair
	"tau21": 2,        # tau2/tau1 of each fatjet in the ungroomed pair
	"tau21_f": 2,      # tau2/tau1 of each fatjet in the filtered pair
	"tau21_p": 2,      # tau2/tau1 of each fatjet in the pruned pair
	"tau21_s": 2,      # tau2/tau1 of each fatjet in the soft-dropped pair
	"tau21_t": 2,      # tau2/tau1 of each fatjet in the trimmed pair
	"tau21s": 1,       # Product of tau21 of each fatjet in the ungroomed pair
	# More jet variables:
	"neef": 2,            # Neutral EM energy fraction
	"ceef": 2,            # Charged EM energy fraction
	"nhef": 2,            # Neutral hadron energy fraction
	"chef": 2,            # Charged hadron energy fraction
	"mef": 2,             # Muon energy fraction
	"nm": 2,              # Neutral multiplicity
	"cm": 2,              # Charged multiplicity
	"n": 2,               # Number of constituents
	# B-info:
	"bd": 2,           # B-tag discriminator
	# Corrections:
	"jec": 2,          # Jet energy correction
	"jmc": 2,          # Jet mass correction
}
## /VARIABLES

# FUNCTIONS:
def treat_event(loop, event, args):		# Where "loop" refers to an event_loop object
	# Parse arguments:
	alg = "ca12"
	if args:
		if "alg" in args:
			alg = args["alg"]
	
#	print "in treat"
	branches = loop.branches
	
	# Empty branches:
	for key in branches.keys():
#		print branches[key]
		for i in xrange(len(branches[key])):
			branches[key][i] = -1

	# Variables:
	n_events_tc = loop.n                      # The total number of events in the TChain
	n_events = loop.n_run                     # The number of events to anatuplize
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
	pair = fat.get_pair(event, cut_pt=400, cut_eta=2.5, alg=alg, leading=True)
	if pair:
		# Jet variables:
		e_p = [(p.m_p**2 + p.px**2 + p.py**2 + p.pz**2)**0.5 for p in pair]
		# Pair variables:
		mavg = numpy.mean([p.m for p in pair])
		mavg_f = numpy.mean([p.m_f for p in pair])
		mavg_p = numpy.mean([p.m_p for p in pair])
		mavg_s = numpy.mean([p.m_s for p in pair])
		mavg_t = numpy.mean([p.m_t for p in pair])
		dm = physics.delta_m(pair)
		dm_t = physics.delta_m(pair, groom="t")
		dm_p = physics.delta_m(pair, groom="p")
		dm_s = physics.delta_m(pair, groom="s")
		dm_f = physics.delta_m(pair, groom="f")
		masy = dm/mavg/2
		masy_f = dm_f/mavg_f/2
		masy_p = dm_p/mavg_p/2
		masy_s = dm_s/mavg_s/2
		masy_t = dm_t/mavg_t/2
		pdi_p = (e_p[0] + e_p[1], pair[0].px + pair[1].px, pair[0].py + pair[1].py, pair[0].pz + pair[1].pz)
		mdi_p = (pdi_p[0]**2 - pdi_p[1]**2 - pdi_p[2]**2 - pdi_p[3]**2)**0.5
		deta = abs(pair[0].eta - pair[1].eta)
		deta_p = abs(pair[0].eta_p - pair[1].eta_p)
		dr = physics.delta_r(*pair)
		# Nsubjettiness:
		tau_max = 4
		groomers = ["u", "f", "p", "s", "t"]
		tau = {}
		for groomer in groomers:
			tau[groomer] = {}
			suffix = "" if groomer == "u" else "_" + groomer
			for itau in range(1, tau_max + 1):
				tau[groomer][itau] = [getattr(pair[i], "tau{}{}".format(itau, suffix)) for i in range(2)]
			for itau in range(2, tau_max + 1):
				for jtau in range(1, itau):
					try: tau[groomer][itau*10 + jtau] = [getattr(pair[i], "tau{}{}".format(itau, suffix))/getattr(pair[i], "tau{}{}".format(jtau, suffix)) for i in range(2)]
					except: tau[groomer][itau*10 + jtau] = [100, 100]		# Division by zero!
		
		# B-jet matching:
		bd = [-13, -13]
		b_info = [
			{
				"fj": pair[0],
				"matches": [],
			},
			{
				"fj": pair[1],
				"matches": [],
			},
		]
#		if min([fj.pt for fj in pair]) > 200:
#			print fjs[0].pt, fjs[1].pt
		for i, pt in enumerate(event.ak4_maod_pt):
			if pt > 50:
				j = physics.jet(event.ak4_maod_px[i], event.ak4_maod_py[i], event.ak4_maod_pz[i], event.ak4_maod_e[i], bd=event.ak4_maod_bd_csv[i])
				for k, fj in enumerate(pair):
					if physics.delta_r(fj, j) <= 0.6:
						b_info[k]["matches"].append(j)
#			print b_info
		for i, fjd in enumerate(b_info):
			if fjd["matches"]:
				bd[i] = max([j.bd for j in fjd["matches"]])
		
		
		# Fill non-pair branches:
		branches["run"][0] = event.run[0]
		branches["lumi"][0] = event.lumi[0]
		branches["event"][0] = event.event[0]
		branches["sigma"][0] = sigma
		branches["w"][0] = w
		branches["njets"][0] = n_jets
		branches["pt_hat"][0] = pt_hat
		branches["htak4"][0] = event.ak4_pf_ht[0]
		branches["htak8"][0] = event.ak8_pf_ht[0]
		# Fill pair branches:
		## Basic:
		for i in range(2): branches["px"][i] = pair[i].px
		for i in range(2): branches["px_p"][i] = pair[i].px_p
		for i in range(2): branches["py"][i] = pair[i].py
		for i in range(2): branches["py_p"][i] = pair[i].py_p
		for i in range(2): branches["pz"][i] = pair[i].pz
		for i in range(2): branches["pz_p"][i] = pair[i].pz_p
		for i in range(2): branches["pt"][i] = pair[i].pt
		for i in range(2): branches["pt_p"][i] = pair[i].pt_p
		for i in range(2): branches["e"][i] = pair[i].e
		for i in range(2): branches["e_p"][i] = pair[i].e_p
		for i in range(2): branches["eta"][i] = pair[i].eta
		for i in range(2): branches["eta_p"][i] = pair[i].eta_p
		branches["deta"][0] = deta
		branches["deta_p"][0] = deta_p
		for i in range(2): branches["phi"][i] = pair[i].phi
		for i in range(2): branches["phi_p"][i] = pair[i].phi_p
		branches["dr"][0] = dr
		for i in range(2): branches["m"][i] = pair[i].m
		for i in range(2): branches["m_f"][i] = pair[i].m_f
		for i in range(2): branches["m_p"][i] = pair[i].m_p
		for i in range(2): branches["m_s"][i] = pair[i].m_s
		for i in range(2): branches["m_t"][i] = pair[i].m_t
		branches["dm"][0] = dm
		branches["dm_f"][0] = dm_f
		branches["dm_p"][0] = dm_p
		branches["dm_s"][0] = dm_s
		branches["dm_t"][0] = dm_t
		branches["mavg"][0] = mavg
		branches["mavg_f"][0] = mavg_f
		branches["mavg_p"][0] = mavg_p
		branches["mavg_s"][0] = mavg_s
		branches["mavg_t"][0] = mavg_t
		branches["masy"][0] = masy
		branches["masy_f"][0] = masy_f
		branches["masy_p"][0] = masy_p
		branches["masy_s"][0] = masy_s
		branches["masy_t"][0] = masy_t
		## m4:
		if len(event.ca12_pf_m) >= 4:
			for i in range(4): branches["m4"][i] = event.ca12_pf_m[i]
		else:
			for i in range(4): branches["m4"][i] = -1
		if len(event.ca12_pfp_m) >= 4:
			for i in range(4): branches["m4_p"][i] = event.ca12_pfp_m[i]
		else:
			for i in range(4): branches["m4_p"][i] = -1
		## Others:
		branches["mdi_p"][0] = mdi_p
		## nsubjettiness:
		for groomer in groomers:
			suffix = "" if groomer == "u" else "_" + groomer
			for ntau in [1, 2, 3, 4, 43, 42, 41, 32, 31, 21]:
				word = "tau{}{}".format(ntau, suffix)
				for i in range(2): branches[word][i] = tau[groomer][ntau][i]
		branches["tau43s"][0] = tau["u"][43][0]*tau["u"][43][1]
		branches["tau42s"][0] = tau["u"][42][0]*tau["u"][42][1]
		branches["tau21s"][0] = tau["u"][21][0]*tau["u"][21][1]
#		# Other jet variables:
		for i in range(2): branches["neef"][i] = event.ca12_pf_neef[pair[i].index]
		for i in range(2): branches["ceef"][i] = event.ca12_pf_ceef[pair[i].index]
		for i in range(2): branches["nhef"][i] = event.ca12_pf_nhef[pair[i].index]
		for i in range(2): branches["chef"][i] = event.ca12_pf_chef[pair[i].index]
		for i in range(2): branches["mef"][i] = event.ca12_pf_mef[pair[i].index]
		for i in range(2): branches["nm"][i] = event.ca12_pf_nm[pair[i].index]
		for i in range(2): branches["cm"][i] = event.ca12_pf_cm[pair[i].index]
		for i in range(2): branches["n"][i] = event.ca12_pf_n[pair[i].index]
		# B-tag:
		for i in range(2): branches["bd"][i] = bd[i]
		# Corrections:
		for i in range(2): branches["jec"][i] = pair[i].jec
		for i in range(2): branches["jmc"][i] = pair[i].jmc
		
#		print "filling tree"
		loop.tt_out.Fill()


def main():
	# Setup:
	## ROOT:
	gROOT.SetBatch()
	
	## Arguments:
	a = variables.arguments()
	args = a.args
	tuples = dataset.fetch_entries("tuple", a.query)
#	print tuples
#	print
	tuples = dataset.sort_datasets(tuples)
	
	## Print an introduction:
	print "The analyzer will run over the following tuples:"
	for key, tups in tuples.items():
		print "\t* {}:".format(key)
		for tup in tups:
			print "\t\t* {}".format(tup)
	
#	print tuples
#	sys.exit()
	
	## Analyzer object:
#	tuples = {}
	
#	### food = 1:
#	for tup in tups:
#		tup.set_connections(down=False, up=True)
#		process = tup.sample.process
#		if process not in tuples:
#			tuples[process] = []
#		tuples[process].extend(["root://cmsxrootd.fnal.gov/" + f for f in tup.files])
##		for ds in dss:
##			if ds.analyze:
##				for path in ds.tuple_path:
##					path_full = "root://cmsxrootd.fnal.gov/" + path
##					tuples[process].append(path_full)
#	print tuples
	
	### food = 2:
#	tuples = {(tup.Name_safe if [t.process for t in tups].count(tup.process) > 1 else tup.process): tup for tup in tups}
	ana = analyzer.analyzer(tuples, save=True, v=args.verbose, count=False)		# Add "out_dir=" and "out_file=".
	ana.define_branches(vs_out)
#	branches = create_tuples(ana)
#	colors = color.pick(len(tuples.keys()))
	
	# Event loop:
	for key, loop in ana.loops.iteritems():
		loop.treatment = treat_event
#		print "here"
		loop.progress = False
		loop.run(n=args.n, rand=False, arguments={"alg": args.algorithm})
#	event_loop(ana.tt[key]["analyzer/events"], branches[key], ana.tuples[key], n_events=n_events_sq)
	# Output:
	ana.write()
	print ana.out_path
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
