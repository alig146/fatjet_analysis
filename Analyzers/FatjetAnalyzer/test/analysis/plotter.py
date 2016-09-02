####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys, yaml
from ROOT import gROOT, gStyle, gDirectory, SetOwnership, TFile, TCut
from truculence import analysis
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def get_cuts(path="plotter_cuts.txt"):
	with open(path) as f:
		cuts_raw = f.readlines()
		cuts = [cut.strip() for cut in cuts_raw if cut[0] != "#"]
		return [TCut("cut{}".format(i), "w[0]*({})".format(cut)) for i, cut in enumerate(cuts)]


def get_ttrees(f):
	tf_in = TFile.Open(f)
	SetOwnership(tf_in, 0)
	tts = analysis.get_tobjects(tf_in, kind="ttree")
	return tts


#def escape_cut(cut):
#	return cut.replace(
#		"_", ""
#	).replace(
#		"$", "-"
#	).replace(
#		"(", ""
#	).replace(
#		")", ""
#	).lower()


def main():
	with open("plotter_info.yaml", 'r') as f:
		plot_info = yaml.load(f)
	
	gROOT.SetBatch()
	tcuts = get_cuts()
#	tts = get_ttrees("/uscms_data/d3/tote/data/fat/anatuples/anatuple_ca12_spring15_cutpt400.root")
	tts = get_ttrees("/uscms_data/d3/tote/data/fat/anatuples/anatuple_ca12_fall15_cutpt400.root")
#	tts = get_ttrees("/uscms_data/d3/tote/data/fat/anatuples/anatuple_ak8_spring15_cutpt400.root")
	
	f_out = "plots.root"
	if len(sys.argv) > 1:
		f_out = sys.argv[1]
	tf_out = TFile(f_out, "RECREATE")
	
	# Write cuts:
	## (I do this seperately so they appear first in the file list.)
	for tcut in tcuts:
		tcut.Write()
	
	# Write histograms:
	for icut, tcut in enumerate(tcuts):
		for var_name, info in plot_info["th1"].items():
			print "Plotting {} with {} ...".format(info["var"], tcut.GetTitle())
			for tt in tts:
				tt_name = tt.GetName()
				th_name = "{}_{}_cut{}".format(tt_name, var_name, icut)
#				print tt_name, var
				tt.Draw("{}>>{}({}, {}, {})".format(info["var"], th_name, info["binning"][0], info["binning"][1], info["binning"][2]), tcut)
				th = gDirectory.Get(th_name)
				if tt.GetName() == "sq150to4j_spring15cutht700_pt400":
					th.Scale(1/24.2)
				th.Write()
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

