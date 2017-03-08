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
	if all([len(getattr(event, "{}_pf{}_pt".format(alg, groomer))) > 1 for groomer in groomers]):   # Sometimes there might not be two jets?
		vars_calc = {};
		for groomer in groomers:
			suffix = groomer if not groomer else "_" + groomer
			
			vars_calc["jec" + suffix] = [getattr(event, "{}_pf{}_jec".format(alg, groomer))[i] for i in range(2)]
			vars_calc["jmc" + suffix] = [getattr(event, "{}_pf{}_jmc".format(alg, groomer))[i] for i in range(2)]
			# Apply JEC and JMC: (I apply the ungroomed quantity to groomed and ungroomed jets.)
			vars_calc["m" + suffix] = [getattr(event, "{}_pf{}_m".format(alg, groomer))[i]*vars_calc["jmc"][i] for i in range(2)]
			vars_calc["px" + suffix] = [getattr(event, "{}_pf{}_px".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
			vars_calc["py" + suffix] = [getattr(event, "{}_pf{}_py".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
			vars_calc["pz" + suffix] = [getattr(event, "{}_pf{}_pz".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
			vars_calc["e" + suffix] = [getattr(event, "{}_pf{}_e".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
			vars_calc["pt" + suffix] = [getattr(event, "{}_pf{}_pt".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
			vars_calc["eta" + suffix] = [getattr(event, "{}_pf{}_eta".format(alg, groomer))[i] for i in range(2)]
			vars_calc["phi" + suffix] = [getattr(event, "{}_pf{}_phi".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau1" + suffix] = [getattr(event, "{}_pf{}_tau1".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau2" + suffix] = [getattr(event, "{}_pf{}_tau2".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau3" + suffix] = [getattr(event, "{}_pf{}_tau3".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau4" + suffix] = [getattr(event, "{}_pf{}_tau4".format(alg, groomer))[i] for i in range(2)]
			vars_calc["tau5" + suffix] = [getattr(event, "{}_pf{}_tau5".format(alg, groomer))[i] for i in range(2)]
		
			# Calculated variables (not in the tuple):
			vars_calc["mavg" + suffix] = [(vars_calc["m" + suffix][0] + vars_calc["m" + suffix][1])/2]
			vars_calc["dm" + suffix] = [abs(vars_calc["m" + suffix][0] - vars_calc["m" + suffix][1])]
			vars_calc["masy" + suffix] = [vars_calc["dm" + suffix][0]/vars_calc["mavg" + suffix][0]/2]
			vars_calc["ptavg" + suffix] = [(vars_calc["pt" + suffix][0] + vars_calc["pt" + suffix][1])/2]
			vars_calc["dpt" + suffix] = [abs(vars_calc["pt" + suffix][0] - vars_calc["pt" + suffix][1])]
			vars_calc["ptasy" + suffix] = [vars_calc["dpt" + suffix][0]/vars_calc["ptavg" + suffix][0]/2]
			vars_calc["deta" + suffix] = [abs(vars_calc["eta" + suffix][0] - vars_calc["eta" + suffix][1])]
			vars_calc["dphi" + suffix] = [math.pi - abs(math.pi - abs(vars_calc["phi" + suffix][0] - vars_calc["phi" + suffix][1]))]
			vars_calc["dr" + suffix] = [(vars_calc["deta" + suffix][0]**2 + vars_calc["dphi" + suffix][0]**2)**0.5]
			try: vars_calc["tau43" + suffix] = [vars_calc["tau4" + suffix][i]/vars_calc["tau3" + suffix][i] for i in range(2)]
			except: vars_calc["tau43" + suffix] = [100 for i in range(2)]		# Division by 0.
			try: vars_calc["tau42" + suffix] = [vars_calc["tau4" + suffix][i]/vars_calc["tau2" + suffix][i] for i in range(2)]
			except: vars_calc["tau42" + suffix] = [100 for i in range(2)]		# Division by 0.
			try: vars_calc["tau41" + suffix] = [vars_calc["tau4" + suffix][i]/vars_calc["tau1" + suffix][i] for i in range(2)]
			except: vars_calc["tau41" + suffix] = [100 for i in range(2)]		# Division by 0.
			try: vars_calc["tau32" + suffix] = [vars_calc["tau3" + suffix][i]/vars_calc["tau2" + suffix][i] for i in range(2)]
			except: vars_calc["tau32" + suffix] = [100 for i in range(2)]		# Division by 0.
			try: vars_calc["tau31" + suffix] = [vars_calc["tau3" + suffix][i]/vars_calc["tau1" + suffix][i] for i in range(2)]
			except: vars_calc["tau31" + suffix] = [100 for i in range(2)]		# Division by 0.
			try: vars_calc["tau21" + suffix] = [vars_calc["tau2" + suffix][i]/vars_calc["tau1" + suffix][i] for i in range(2)]
			except: vars_calc["tau21" + suffix] = [100 for i in range(2)]		# Division by 0.
	
	
		for variable in variables:
			var_name = variable["name"]
			var_dim = variable["dimension"]
			var_type = variable["type"]
	#		print var_name, var_dim, var_type
		
			if var_name == "w": branches[var_name][0] = getattr(event, var_name)[0]*n_events_tc/n_events
			elif var_name == "njets": branches[var_name][0] = getattr(event, "ak4_pf_njets")[0]
	#		elif var_name == "pthat": branches[var_name][0] = getattr(event, "pt_hat")[0]
			elif var_name == "htak4": branches[var_name][0] = getattr(event, "ak4_pf_ht")[0]
			elif var_name == "htak8": branches[var_name][0] = getattr(event, "ak8_pf_ht")[0]
			elif var_name == "wtt": branches[var_name][0] = (getattr(event, "q_gn_sf")[0]*getattr(event, "q_gn_sf")[1])**0.5 if len(getattr(event, "q_gn_sf")) == 2 else 1
			elif var_name == "bd":
				for i in range(var_dim): branches[var_name][i] = getattr(event, "ca12_pf_bd_csv")[i]
			elif var_name == "jetid":
				for i in range(var_dim): branches[var_name][i] = getattr(event, "ca12_pf_jetid_l")[i]
			elif var_name in vars_calc:
				for i in range(var_dim): branches[var_name][i] = vars_calc[var_name][i]
			else:
				pieces = var_name.split("_")
				var_stem = pieces[0]
				if var_type == "event":
					for i in range(var_dim): 
						try: branches[var_name][i] = getattr(event, var_name)[i]		# Sometimes a trigger (for example) might have an empty branch.
						except: pass
				if var_type == "jet":
					groomer = pieces[1] if len(pieces) > 1 else ""
					for i in range(var_dim): branches[var_name][i] = getattr(event, "{}_pf{}_{}".format(alg, groomer, var_stem))[i]
	
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
		tuples = dataset.sort_datasets(tuples)
	else:
		tuples = {args.process: tuples}
	
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
