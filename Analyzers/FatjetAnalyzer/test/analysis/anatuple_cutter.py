####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
from truculence import root
from ROOT import TFile
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def main():
	rf = root.rfile("/uscms_data/d3/tote/data/fat/anatuples/anatuple_ca12_fall15_cutpt400eta25jetidloose_jetht-ttbar-qcdmg.root")
	tts = rf.get_ttrees()
	tf_out = TFile("anatuple_ca12_fall15_cutpt400eta25jetidloose_cutpsdeta10_jetht-ttbar-qcdmg.root", "RECREATE")
	cut = "Max$(m_t)>50&&htak8>900&&Min$(tau21)>0.5&&deta<1.0"
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

