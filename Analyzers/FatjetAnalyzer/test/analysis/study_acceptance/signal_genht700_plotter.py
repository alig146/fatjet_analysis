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
# :VARIABLES

# FUNCTIONS:
def main():
	tf_out = ROOT.TFile("signal_genht700_plots.root", "recreate")
	tg_accept = ROOT.TGraphErrors()
	h_pre = ROOT.TH1F("h_pre", "", 9, 75, 525)
	h_post = ROOT.TH1F("h_post", "", 9, 75, 525)
	tg_accept.SetName("acceptance_sqto4j_genht700")
	for im, m in enumerate(masses):
		subprocess = "sq{}to4j".format(m)
		maod = dataset.fetch_miniaod(subprocess, "0706")
		print maod.weight
		post = maod.weight*100000
		pre = maod.weight*100000/maod.preweight
		h_pre.SetBinContent(h_pre.GetXaxis().FindBin(m), pre)
		h_pre.SetBinError(h_pre.GetXaxis().FindBin(m), pre**(0.5))
		h_post.SetBinContent(h_post.GetXaxis().FindBin(m), post)
		h_post.SetBinError(h_post.GetXaxis().FindBin(m), post**(0.5))
		accept = post/pre
		accepte = accept*(1-accept)/pre
		tg_accept.SetPoint(im, float(m), accept)
		tg_accept.SetPointError(im, 0.0, accepte)
		print post, pre, accept, accepte
	tf_out.WriteTObject(tg_accept)
	tf_out.WriteTObject(h_pre)
	tf_out.WriteTObject(h_post)
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

