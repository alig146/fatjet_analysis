####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import ROOT
from decortication import dataset
from truculence import root
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
masses = [100, 150, 200, 250, 300, 400, 500]
#weight_factor = 38.180/2.183
# :VARIABLES

# FUNCTIONS:
def main():
	tf_out = ROOT.TFile("signal_acceptance_plots.root", "recreate")
	tg_accept = ROOT.TGraphErrors()
	tg_accept.SetName("acceptance_sqto4j_sig")
	plots = root.rfile("../study_contamination/contamination_plots.root");
	for im, m in enumerate(masses):
		subprocess = "sq{}to4j".format(m)
		th1 = plots.get_tobject("{}_sig".format(subprocess))
		post = th1.Integral(0, th1.GetNbinsX() + 1)
		maod = dataset.fetch_miniaod(subprocess, "0706")
		pre = maod.weight*maod.n/maod.preweight
		accept = post/pre
		accepte = accept*(1-accept)/pre
		tg_accept.SetPoint(im, float(m), accept)
		tg_accept.SetPointError(im, 0.0, accepte)
		print post, pre, accept, accepte
	tf_out.WriteTObject(tg_accept)
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

