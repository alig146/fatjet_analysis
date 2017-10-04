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
#weight_factor = 38.180/2.183
# /VARIABLES

# FUNCTIONS:
def construct_histograms(ds):
	histograms = {}
	
	for cut in cuts:
		histograms["mavg", cut] = TH1F("_".join(["mavg", ds, cut]), "", 1200, 0, 1200)
		histograms["masy", cut] = TH1F("_".join(["masy", ds, cut]), "", 1200, 0, 1)
	
	return histograms

def event_passes(event, cut):
	if cut == "sig":
		return True if event.masy_p[0] < 0.1 and event.deta[0] < 1.0 and max(event.tau21) < 0.75 and max(event.tau42) < 0.45 and max(event.tau43) < 0.80 else False
	if cut == "sigl":
		return True if event.masy_p[0] < 0.1 and event.deta[0] < 1.0 and max(event.tau21) < 0.75 and max(event.tau42) < 0.50 and max(event.tau43) < 0.90 else False
	elif cut == "sb":
		eta<1.0&&Max$(tau43)<0.90&&Max$(tau42)<0.55&&((tau42[0]>0.45||tau43[0]>0.80)&&(tau42[1]>0.45||tau43[1]>0.80))&&Max$(tau21)<0.75&&masy_p<0.1&&Min$(bd)>0.46
		return True if event.masy_p[0] < 0.1 and event.deta[0] < 1.0 and max(event.tau21) < 0.75 and max(event.tau42) < 0.55 and max(event.tau43) < 0.90 and ((event.tau42[0] > 0.45 or event.tau43[0] > 0.80) and (event.tau42[1] > 0.45 or event.tau43[1] > 0.80)) else False
	elif cut == "sbb":
		eta<1.0&&Max$(tau43)<0.90&&Max$(tau42)<0.55&&((tau42[0]>0.45||tau43[0]>0.80)&&(tau42[1]>0.45||tau43[1]>0.80))&&Max$(tau21)<0.75&&masy_p<0.1&&Min$(bd)>0.46
		return True if event.masy_p[0] < 0.1 and event.deta[0] < 1.0 and max(event.tau21) < 0.75 and max(event.tau42) < 0.55 and max(event.tau43) < 0.90 and ((event.tau42[0] > 0.45 or event.tau43[0] > 0.80) and (event.tau42[1] > 0.45 or event.tau43[1] > 0.80)) and min(event.bd > 0.46) else False
	else: return False

def main():
	cuts = ["sig", "sb", "sbb", "sbide", "sbideb", "sbl", "sblb", "sbt", "sbtb", "sigl", "sbl42", "sbl42b", "sbl43", "sbl43b"];
	
	gROOT.SetBatch()
	rf = root.rfile(anatuple_in)
	ttrees = rf.get_ttrees()
	info = get_cutnames(style="root")
	
	tf = TFile("basic_plots_test.root", "recreate")
	
	for tt_name, ttree in ttrees.items():
		print "[..] Basic plots for {}.".format(tt_name)
		
		hs = construct_histograms(tt_name)
		for event in ttree:
			w = abs(event.W[0])
			for cut in cuts:
				if event_passes(event, cut):
					hs["mavg", cut].Fill(event.mavg_p[0], w)
					hs["masy", cut].Fill(event.masy_p[0], w)
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

