####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys
from truculence import root
from ROOT import TFile
from plotter import get_cuts
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
	known_cuts = get_cuts()
	cut_key = "preselAndMasy"
	if len(sys.argv) > 2:
		if sys.argv[2] in known_cuts:
			cut_key = sys.argv[2]
	cut = known_cuts[cut_key].GetTitle()
	f_out = f_in.replace(".root", "_{}.root".format(cut_key))
	
	# Apply the cut:
	rf = root.rfile(f_in)
	tts = rf.get_ttrees()
	tf_out = TFile(f_out, "RECREATE")
	for tt in tts:
		print "Cutting {} ...".format(tt.GetName())
		tt_out = tt.CopyTree(cut)
		tf_out.WriteTObject(tt_out)
	tf_out.Close()
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

