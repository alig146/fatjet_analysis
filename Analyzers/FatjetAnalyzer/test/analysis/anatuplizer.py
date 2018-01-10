####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
print "Importing packages ..."
import sys            # Allows "sys.exit()"
#import argparse       # For commandline options
import random
import numpy
import yaml
import math           # For math.pi
from time import time
from truculence import *
from decortication import dataset, analyzer, fat, variables
#from array import array
print "\tImporting ROOT ..."
from ROOT import *
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
# /VARIABLES

# FUNCTIONS:
#def parse_event(event):

def get_variables(f="anatuple.yaml"):
	with open(f, 'r') as f_in:
		info = utilities.ordered_load(f_in)
	return info["variables"]

def match_leptons(loop):
	for ijet, jet_pt in enumerate(loop.branches["pt"]):
		print ijet, jet_pt

def treat_event(loop, event, args):		# Where "loop" refers to an event_loop object
	# Parse arguments:
	alg = "ca12"
	if args:
		if "alg" in args: alg = args["alg"]
	
	branches = loop.branches
	
	# Empty branches:
	for key in branches.keys():
#		print branches[key]
		for i in xrange(len(branches[key])):
			branches[key][i] = -1
	
	# Variables:
	variables = loop.ana.variables
	n_events_tc = loop.n                      # The total number of events in the TChain
	n_events = loop.n_run                     # The number of events to anatuplize
	
	groomers = ["", "f", "p", "s", "t"]
##	groomers = ["", "p"]
#	if all([len(getattr(event, "{}_pf{}_pt".format(alg, groomer))) > 1 for groomer in groomers]):   # Sometimes there might not be two jets?
#		vars_calc = {};
#		for groomer in groomers:
#			
#			print suffix
			
		# Fetch basic kinematics:
		vars_calc["px"] = [getattr(event, "{}_pf_px".format(alg))[i] for i in range(2)]
		vars_calc["py"] = [getattr(event, "{}_pf_py".format(alg))[i] for i in range(2)]
		vars_calc["pz"] = [getattr(event, "{}_pf_pz".format(alg))[i] for i in range(2)]
		vars_calc["e"] = [getattr(event, "{}_pf_e".format(alg))[i] for i in range(2)]
		vars_calc["pt"] = [getattr(event, "{}_pf_pt".format(alg))[i] for i in range(2)]
		vars_calc["eta"] = [getattr(event, "{}_pf_eta".format(alg))[i] for i in range(2)]
		vars_calc["phi"] = [getattr(event, "{}_pf_phi".format(alg))[i] for i in range(2)]
		# Fetch corrections for crosschecking (they're already applied at the tuple level):
		vars_calc["jec"] = [getattr(event, "{}_pf_jec".format(alg))[i] for i in range(2)]
		vars_calc["jmc"] = [getattr(event, "{}_pf_jmc".format(alg))[i] for i in range(2)]
		# Fetch more jet variables:
		for groomer in grommers:
			suffix = groomer if not groomer else "_" + groomer
#			ngroomed = min(2, len(getattr))
			vars_calc["m" + suffix] = [getattr(event, "{}_pf_m{}".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau1" + suffix] = [getattr(event, "{}_pf_tau1{}".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau2" + suffix] = [getattr(event, "{}_pf_tau2{}".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau3" + suffix] = [getattr(event, "{}_pf_tau3{}".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau4" + suffix] = [getattr(event, "{}_pf_tau4{}".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau5" + suffix] = [getattr(event, "{}_pf_tau5{}".format(alg, groomer))[i] for i in range(2)]
			if not groomer:
				vars_calc["tau21" + suffix] = [getattr(event, "{}_pf_tau21{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau31" + suffix] = [getattr(event, "{}_pf_tau31{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau32" + suffix] = [getattr(event, "{}_pf_tau32{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau41" + suffix] = [getattr(event, "{}_pf_tau41{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau42" + suffix] = [getattr(event, "{}_pf_tau42{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau43" + suffix] = [getattr(event, "{}_pf_tau43{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau51" + suffix] = [getattr(event, "{}_pf_tau51{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau52" + suffix] = [getattr(event, "{}_pf_tau52{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau53" + suffix] = [getattr(event, "{}_pf_tau53{}".format(alg, groomer))[i] for i in range(2)]
				vars_calc["tau54" + suffix] = [getattr(event, "{}_pf_tau54{}".format(alg, groomer))[i] for i in range(2)]
	
		# Calculated variables (not in the tuple):
		vars_calc["ptavg"] = [(vars_calc["pt"][0] + vars_calc["pt"][1])/2]
		vars_calc["dpt"] = [abs(vars_calc["pt"][0] - vars_calc["pt"][1])]
		vars_calc["ptasy"] = [vars_calc["dpt"][0]/vars_calc["ptavg"][0]/2]
		vars_calc["deta"] = [abs(vars_calc["eta"][0] - vars_calc["eta"][1])]
		vars_calc["dphi"] = [math.pi - abs(math.pi - abs(vars_calc["phi"][0] - vars_calc["phi"][1]))]
		vars_calc["dr"] = [(vars_calc["deta"][0]**2 + vars_calc["dphi"][0]**2)**0.5]
		for groomer in groomers:
			vars_calc["mavg" + suffix] = [(vars_calc["m" + suffix][0] + vars_calc["m" + suffix][1])/2]
			vars_calc["dm" + suffix] = [abs(vars_calc["m" + suffix][0] - vars_calc["m" + suffix][1])]
			vars_calc["masy" + suffix] = [vars_calc["dm" + suffix][0]/vars_calc["mavg" + suffix][0]/2]


	for variable in variables:
		var_name = variable["name"]
		var_dim = variable["dimension"]
		var_type = variable["type"]
#		print var_name, var_dim, var_type
		
		# Skip lepton variables for now: (I match them later.)
		if var_type == "lepton": continue
	
		if var_name == "w": branches[var_name][0] = getattr(event, var_name)[0]*n_events_tc/n_events
		elif var_name == "njets": branches[var_name][0] = getattr(event, "ak4_pf_njets")[0]
#		elif var_name == "pthat": branches[var_name][0] = getattr(event, "pt_hat")[0]
		elif var_name == "htak4": branches[var_name][0] = getattr(event, "ak4_pf_ht")[0]
		elif var_name == "htak8": branches[var_name][0] = getattr(event, "ak8_pf_ht")[0]
		elif var_name == "wtt": branches[var_name][0] = (getattr(event, "q_gn_sf")[0]*getattr(event, "q_gn_sf")[1])**0.5 if len(getattr(event, "q_gn_sf")) == 2 else 1
		elif var_name == "bd":
			for i in range(var_dim): branches[var_name][i] = getattr(event, alg + "_pf_bd_csv")[i]
		elif var_name == "jetid":
			for i in range(var_dim): branches[var_name][i] = getattr(event, alg + "_pf_jetid_l")[i]
		elif var_name in vars_calc:
			for i in range(var_dim): branches[var_name][i] = vars_calc[var_name][i]
		else:
			pieces = var_name.split("_")
			var_stem = pieces[0]
			if var_type == "event":
				for i in range(var_dim): 
					try: branches[var_name][i] = getattr(event, var_name)[i]		# Sometimes a trigger (for example) might have an empty branch.
					except: pass
#			if var_type == "jet":
#				groomer = pieces[1] if len(pieces) > 1 else ""
#				for i in range(var_dim): branches[var_name][i] = getattr(event, "{}_pf{}_{}".format(alg, groomer, var_stem))[i]
	
	# Calculate total weight:
	branches["W"][0] = branches["w"][0]*branches["wtt"][0]
	if branches["wpu"][0] > 0: branches["W"][0] *= branches["wpu"][0]
	
#	match_leptons(loop)
	
	loop.tt_out.Fill()


def main():
	# Setup:
	## ROOT:
	gROOT.SetBatch()
	
	## Arguments:
	a = variables.arguments()
	args = a.args
	
	tuples = a.input
	if not tuples:
		tuples = dataset.fetch_entries("tuple", a.query)
		tuples = dataset.sort_datasets(tuples, collapse=True)		# This combines "extension" datasets in addition to other things.
	else:
		tuples = {args.process: tuples}
	
	## Combine jetht15 and jetht16:
	for name in ["jetht15", "jetht16"]:
		if name in tuples:
			if "jetht" not in tuples: tuples["jetht"] = []
			tuples["jetht"].extend(tuples[name])
			tuples.pop(name, None)
	
	
	## Print an introduction:
	print "The analyzer will run over the following tuples:"
	if isinstance(tuples, dict):
		for key, tups in tuples.items():
			print "\t* {}:".format(key)
			for tup in tups:
				print "\t\t* {}".format(tup)
	else:
		print "\t{}".format(tuples)
	out_dir = None
	out_file = None
	if args.output:
		out_file = args.output.split("/")[-1]
		out_dir = "/".join(args.output.split("/")[:-1])
	
	ana = analyzer.analyzer(tuples, save=True, v=args.verbose, out_file=out_file, use_condor=args.condor)
	vs_out = get_variables()
	ana.define_branches(vs_out)
#	branches = create_tuples(ana)
#	colors = color.pick(len(tuples.keys()))
	
	# Event loop:
	for key, loop in ana.loops.iteritems():
		loop.treatment = treat_event
#		print "here"
		loop.progress = True if not ana.condor else False
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
