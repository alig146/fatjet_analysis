####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import argparse       # For commandline options
#from ROOT import *
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True    # To stop ROOT interpreting the commandline arguments.
from truculence import root, color
from decortication import analyzer, fat
from anatuplizer import variables_out
import sys
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
#variables = ["w", "sigma", "n0", "n1", "n2", "njets", "ht", "m", "dm", "mavg", "pt", "e", "eta", "phi", "deta", "tau42", "tau42s", "tau21", "tau21s", "precut_pt", "precut_m", "precut_eta"]
variables = variables_out.keys()
# /VARIABLES

# FUNCTIONS:
def main():
	# Setup:
	## ROOT:
	ROOT.gROOT.SetBatch()
	
	## Arguments:
	arg_result = arguments()
	args = arg_result["args"]
#	dsd = arg_result["datasets"]
	keys = arg_result["keys"]
	cut = args.cut
	n = args.n
	
	in_file = "anatuples.root"
	if keys is None:
		tf = ROOT.TFile.Open(in_file)
		keys = root.listdir(tf)
	name = keys[0] if len(keys) == 1 else None		# Kludge to prevent "plotter" becoming the key for tt_in etc when there's only one input dataset
	ana = analyzer.analyzer(in_file, name=name, tt_names=keys, save=True, v=True)		# Add "out_dir=" and "out_file=".
#	print ana.tt_in
	ana.cut = cut
	ana.th1 = {}
	ana.th2 = {}
	colors = color.pick(len(ana.tt_in.values()))
	for i, tt in enumerate(ana.tt_in.values()):		# ana.tt = {"plotter": [ttrees for each sample]}
		key = tt.GetName()
		ana.th1[key], ana.th2[key] = create_plots_individual(name=key, color=colors[i].id)
		ana.plots.extend(ana.th1[key].values())
		ana.plots.extend(ana.th2[key].values())
		
#	print ana.tt
#	print ana.loops
	for key, loop in ana.loops.iteritems():
		loop.treatment = treat_event
		loop.run(n=n, variables=variables)
	
	# Output:
	ana.write(Print=args.Print)
	print ana.out_path

def treat_event(loop, event):
	# Variables:
	th1 = loop.ana.th1[loop.key]
	th2 = loop.ana.th2[loop.key]
#	print th1
	n_events_tt = event.GetEntries()
	w = event.w[0]
#	sigma = event.sigma
#	nevents = event.ak4_pf_nevents[0]
#	try:
#		gn_var = event.ak4_gn_pt_hat[0]		# This is pt_hat for qcdp and ht for qcdmg
#	except:
#		gn_var = -1		# This branch doesn't exist for signals
#	w = (sigma*L)/nevents*(float(n_events_tt)/n_events) if n_events != -1 else (sigma*L)/nevents
#	n_jets = len([pt for pt in event.ak4_pf_pt if pt > precut_npt])
#	th1["n"].Fill(n_jets, w)
#	
#	# Plots before selection:
##	th1["gn_var"].Fill(gn_var, w)
#	
#	# Selection:
#	if n_jets >= cut_njets:
#		pair = fat.get_pair(event, cut_pt=pair_pt)
##		print "here 52"
#		if pair:
#			# Pair variables:
#			mavg = numpy.mean([p.m for p in pair])
#			dm = physics.delta_m(pair)
#	tau = {}
#	tau[42] = (event.tau4[0]/event.tau2[0], event.tau4[1]/event.tau2[1])
#	tau[24] = (1/tau[42][0], 1/tau[42][1])
#	tau[21] = (event.tau2[0]/event.tau1[0], event.tau2[1]/event.tau1[1])
#	deta = abs(event.eta[0] - event.eta[1])
	
	# Fill histograms:
	th1["mavg"].Fill(event.mavg, w)
	th1["tau42"].Fill(event.tau42[0], w)
	th1["tau42"].Fill(event.tau42[1], w)
#	th1["tau42s"].Fill(tau[42][0]*tau[42][1], w)
	th1["tau21"].Fill(event.tau21[0], w)
	th1["tau21"].Fill(event.tau21[1], w)
#	th1["tau21s"].Fill(tau[21][0]*tau[21][1], w)
#	th2["tau"].Fill(tau[42][0]*tau[42][1], tau[21][0]*tau[21][1], w)
	th1["deta"].Fill(event.deta, w)
	
#	if all(t42 <= cut_tau42 for t42 in tau[42]):
#		# Fill histograms:
#		th1["ht"].Fill(event.ak8_pf_ht[0], w)
#		dr = physics.delta_r(pair[0], pair[1])
##						th1["dr"].Fill(dr, w)
##						if dr <= cut_dr:
#		th1["m"].Fill(mavg, w)

def create_plots_individual(name="", color=ROOT.kBlue):
	th1_info = {
		"n": ("Njettiness", "Number of jets", "Events", 50, 0, 20),
		"mavg": ("Average Mass", "mass (GeV)", "Events", 50, 0, 800),
		"m": ("Trimmed Mass", "mass (GeV)", "Events", 50, 0, 500),
		"ht": ("H_{T}", "H_{T} (GeV)", "Events", 50, 500, 3000),
		"deta": ("#Delta #eta", "#Delta #eta (#eta)", "Events", 50, 0, 10),
		"tau42": ("Nsubjettiness #tau_{42}", "#tau_{42}", "Events", 50, 0, 1),
		"tau42s": ("Nsubjettiness #tau_{42}^{2}", "#tau_{42}^{2}", "Events", 50, 0, 1),
		"tau21": ("Nsubjettiness #tau_{21}", "#tau_{21}", "Events", 50, 0, 1),
		"tau21s": ("Nsubjettiness #tau_{21}^{2}", "#tau_{21}^{2}", "Events", 50, 0, 1),
	}
	th2_info = {
		"tau": ("Nsubjettiness", "#tau_{42}^{2}", "#tau_{21}^{2}", "Events", 50, 0, 1, 50, 0, 1),
	}
	th1 = {}
	for key, values in th1_info.iteritems():
		th1[key] = ROOT.TH1F("th1{}_{}".format("_{}".format(name) if name else "", key), values[0], values[3], values[4], values[5])
		th1[key].GetXaxis().SetTitle(values[1])
		th1[key].GetXaxis().CenterTitle()
		th1[key].GetYaxis().SetTitle(values[2])
		th1[key].GetYaxis().CenterTitle()
		th1[key].SetLineColor(color)
		th1[key].SetMinimum(1)
	
	th2 = {}
	for key, values in th2_info.iteritems():
		th2[key] = ROOT.TH2F("th2{}_{}".format("_{}".format(name) if name else "", key), values[0], values[4], values[5], values[6], values[7], values[8], values[9])
		th2[key].GetXaxis().SetTitle(values[1])
		th2[key].GetXaxis().CenterTitle()
		th2[key].GetYaxis().SetTitle(values[2])
		th2[key].GetYaxis().CenterTitle()
		th2[key].GetZaxis().SetTitle(values[3])
		th2[key].GetZaxis().CenterTitle()
	
	return (th1, th2)

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
		default=-1,
		help="The number of events you want to process for each dataset",
		metavar="INT"
	)
	parser.add_argument(
		"-c", "--cut", dest="cut",
		type=str,
		default="",
		help="The ROOT TCut string to apply to each input TTree (e.g. \"pt[0]>400\")",
		metavar="STR"
	)
	parser.add_argument(
		"-p", "--print", dest="Print",
		action="store_true",
		default=False,
		help="Use to save all plots in PDF format."
	)
	
	args = parser.parse_args()
	
	# Interpretation:
	keys = args.keys
	if keys:
		keys = keys.split(",")
	results["args"] = args
	results["keys"] = keys
#	results["datasets"] = dataset.get_datasets(key=keys)
	return results
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
