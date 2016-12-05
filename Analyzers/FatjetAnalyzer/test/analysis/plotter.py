####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: "python plotter.py anatuple.root [out.root]"        #
####################################################################

# IMPORTS:
import sys, yaml
from ROOT import gROOT, gStyle, gDirectory, SetOwnership, TFile, TCut
from truculence import analysis, root
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def get_cuts(path="plotter_cuts.yaml"):
	with open(path, 'r') as f:
		cuts_info = yaml.load(f)
		return {key: TCut(key, "w*({})".format(cut_raw)) for key, cut_raw in cuts_info.items()}


#def get_ttrees(f):
#	tf_in = TFile.Open(f)
#	SetOwnership(tf_in, 0)
#	tts = analysis.get_tobjects(tf_in, kind="ttree")
#	return tts

def main():
	# Input:
	## Anatuple:
	assert(len(sys.argv) >= 2)
	atuple = root.rfile(sys.argv[1])
	tts = atuple.get_ttrees()
	
	## Plots to make:
	with open("plotter_info.yaml", 'r') as f:
		plot_info = yaml.load(f)
	
	## Cuts to make:
	cuts_dict = get_cuts()
	
	# Output:
	if len(sys.argv) >= 3:
		f_out = sys.argv[2]
	else:
		f_out = atuple.name.replace("anatuple", "plots")
	tf_out = TFile(f_out, "RECREATE")
	
	# Start:
	gROOT.SetBatch()
	
	# Write cuts:
	## (I do this seperately so they appear first in the file list.)
	for key, tcut in cuts_dict.items():
		tcut.Write()
	
	# Write histograms:
	for cut_key, tcut in cuts_dict.items():
		print "[..] Making plots for {}, which is\n\t{}".format(cut_key, tcut.GetTitle())
		# TH1s:
		for var_name, info in plot_info["th1"].items():
			for tt in tts:
				tt_name = tt.GetName()
				print "\t[..] Plotting {} for {}.".format(info["var"], tt_name)
				th_name = "{}_{}_{}".format(tt_name, var_name, cut_key)
#				print tt_name, var
				tt.Draw("{}>>{}({}, {}, {})".format(info["var"], th_name, info["binning"][0], info["binning"][1], info["binning"][2]), tcut)
				th = gDirectory.Get(th_name)
				th.Write()
		# TH2s:
		for var_name, info in plot_info["th2"].items():
			for tt in tts:
				tt_name = tt.GetName()
				print "\t[..] Plotting {} for {}.".format(info["var"], tt_name)
				th_name = "{}_{}_cut{}".format(tt_name, var_name, cut_key)
#				print tt_name, var
				tt.Draw("{}:{}>>{}({}, {}, {}, {}, {}, {})".format(info["var"][0], info["var"][1], th_name, info["binning"][0], info["binning"][1], info["binning"][2], info["binning"][3], info["binning"][4], info["binning"][5]), tcut)
				th = gDirectory.Get(th_name)
				th.Write()
	print "[OK] Plots sucessfully written to\n\t{}".format(f_out)
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

