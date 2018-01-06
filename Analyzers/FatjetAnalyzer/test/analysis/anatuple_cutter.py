####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys, yaml
from truculence import root
from ROOT import TFile, TCut
#from plotter import get_cuts
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def main():
	# Arguments:
	assert len(sys.argv) > 1
	f_in = sys.argv[1]
	with open("cuts.yaml") as f:
		known_cuts = yaml.load(f)
	cut_key = "pre"
	if len(sys.argv) > 2:
		if sys.argv[2] in known_cuts:
			cut_key = sys.argv[2]
	tcut = TCut(cut_key, known_cuts[cut_key])
	cut = tcut.GetTitle()
	print "[..] Applying the following cut to {}:\n{}\n".format(f_in, cut)
	f_out = f_in.replace(".root", "_{}.root".format(cut_key))
	
	# Apply the cut:
	rf = root.rfile(f_in)
	tts = rf.get_ttrees()
	tf_out = TFile(f_out, "RECREATE")
	for tt_name, tt in tts.items():
		print "[..] Cutting {}.".format(tt.GetName())
		tt_out = tt.CopyTree(cut)
		tf_out.WriteTObject(tt_out)
	tf_out.Close()
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

