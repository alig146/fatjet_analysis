####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys
import yaml
from decortication.dataset import fetch_sample, fetch_miniaod, fetch_entries
from truculence import root
#from decortication import dataset
from ROOT import TFile, TCut, TH1F, gROOT
#from plotter import get_cuts, get_ttrees
# /IMPORTS

# CLASSES:
#class cut:
#	def __init__(self):
# /CLASSES

# VARIABLES:
#anatuple_in = "~/anatuples/anatuple_cutpt400eta25.root"
anatuple_in = "~/anatuples/anatuple_cutpt400eta25_pre.root"
anatuple_in_sq100 = "~/anatuples/anatuple_sq100to4j_cutpt300eta20_pre.root"
anatuple_in_bosons = "~/anatuples/anatuple_vbosons_moriond17_cutpt400eta25_pre.root"
#weight_factor = 38.180/2.183
# /VARIABLES

# FUNCTIONS:
def get_cutnames(style="latex"):
	if style == "latex":
		return {
			"sig": [
				"Gen-level $\HT > 700$ GeV",
				"Pre-selection",
				"$\\abs{\\Delta\\eta} < 1.0$",
				"$A_m < 0.1$",
				"$\\tau_{ij}$ cuts",
			]
		}
	elif style == "root":
		return {
			"sig": [
				"Gen-level #it{H}_{T} > 700 GeV",
				"Pre-selection",
				"#left|#Delta#eta#right| < 1.0",
				"#it{A}_{m} < 0.1",
				"#tau_{#it{ij}} cuts",
			]
		}



#def parse_cuts(cuts_file="cuts_tau21s.txt"):
#	with open(cuts_file) as f:
#		return [line.strip() for line in f.read().strip().split("\n")]

#def apply_cut(tcut, tt, v=False):
#	if v: print "{}, cut = {}:".format(tt.GetName(), tcut.GetName())
#	tt.Draw("mavg_p>>h(11, 25, 575)", tcut)
#	h = gDirectory.Get("h")
#	return h
def apply_cut(cut, tt, v=False):
	if v: print "{}, cut = {}:".format(tt.GetName(), tcut.GetName())
	tt.Draw("mavg_p>>h(11, 25, 575)", tcut)
	h = gDirectory.Get("h")
	return h


def main():
	gROOT.SetBatch()
	rf = root.rfile(anatuple_in)
	rf_sq100 = root.rfile(anatuple_in_sq100)
	rf_bosons = root.rfile(anatuple_in_bosons)
	ttrees = rf.get_ttrees()
	ttrees_sq100 = rf_sq100.get_ttrees()
	ttrees_bosons = rf_bosons.get_ttrees()
	ttrees["sq100to4j"] = ttrees_sq100["sq100to4j"]
	ttrees["wjets"] = ttrees_bosons["wjets"]
	ttrees["zjets"] = ttrees_bosons["zjets"]
	
	print ttrees.keys()
	
	info = get_cutnames(style="root")
#	with open("cutflow.yaml") as f:
#		info = yaml.load(f)
	
	tf = TFile("cutflow_plots.root", "recreate")		# This prevents new TTrees from giving an error about being memory residents.
	ths = {(tt_name, cut_key, metric): TH1F("{}_{}_{}".format(tt_name, cut_key, metric), "", len(cuts), 0, len(cuts)) for tt_name in ttrees.keys() for cut_key, cuts in info.items() for metric in ["n", "w", "p"]}
	for th in ths.values():
		for key, cuts in info.items():
			for icut, cut in enumerate(cuts):
				th.GetXaxis().SetBinLabel(icut + 1, cut)
	
	for tt_name, ttree in ttrees.items():
		if tt_name == "jetht": continue
#		sample = fetch_sample(tt_name)
		print "[..] Making cutflow for {}.".format(tt_name)
		wf = 1 # if "jetht" in tt_name else weight_factor
		print "\tWill apply a weight factor of {:.2f}".format(wf)
		generation = "moriond17"
		if "sq" in tt_name: generation = "moriond17cutht700"
		miniaods = fetch_entries("miniaod", {"process": tt_name, "generation": generation})
		original_w = sum([miniaod.n*miniaod.weight for miniaod in miniaods])
		
		gen_n = 0
		gen_w = 0
		gen_p = 0
		if "sq" in tt_name:
			gen_n = miniaod.n
			gen_w = miniaod.n*miniaod.weight
			gen_p = miniaod.preweight*100
			original_w /= miniaod.preweight
		ths[tt_name, "sig", "n"].SetBinContent(1, gen_n)
		ths[tt_name, "sig", "w"].SetBinContent(1, gen_w)
		ths[tt_name, "sig", "p"].SetBinContent(1, gen_p)
		ths[tt_name, "sig", "w"].SetBinContent(0, original_w)
		for ievent, event in enumerate(ttree):
			if ievent%100000 == 0: print "\t[..] Processing event {}.".format(ievent)
			if event.htak8 > 900 and max(event.m_t) > 50 and min(event.pt) > 400 and max([abs(v) for v in event.eta]) < 2.0:
				ths[tt_name, "sig", "n"].Fill(1)
				ths[tt_name, "sig", "w"].Fill(1, event.W*wf)
				if event.deta < 1.0:
					ths[tt_name, "sig", "n"].Fill(2)
					ths[tt_name, "sig", "w"].Fill(2, event.W*wf)
					if event.masy_p < 0.1:
						ths[tt_name, "sig", "n"].Fill(3)
						ths[tt_name, "sig", "w"].Fill(3, event.W*wf)
						if max(event.tau43) < 0.80 and max(event.tau42) < 0.50 and max(event.tau21) < 0.75:
							ths[tt_name, "sig", "n"].Fill(4)
							ths[tt_name, "sig", "w"].Fill(4, event.W*wf)
		for icut, cutname in enumerate(info["sig"]):
			if icut < 1: continue
#			print icut, ths[tt_name, "sig", "w"].GetBinContent(icut), ths[tt_name, "sig", "w"].GetBinContent(2)
			ths[tt_name, "sig", "p"].SetBinContent(icut + 1, ths[tt_name, "sig", "w"].GetBinContent(icut + 1)/ths[tt_name, "sig", "w"].GetBinContent(0)*100)
	for th in ths.values(): th.Write()
	sys.exit()
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

