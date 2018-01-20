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
masses_sq = [100, 150, 175, 200, 250, 300, 400, 500, 600, 700]
masses_sg = [100, 150, 175, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650]
#masses_sg = [100, 150, 200, 250, 300, 400, 450, 500, 550, 600, 650]
#weight_factor = 38.180/2.183
# :VARIABLES


# FUNCTIONS:
def main():
	tf_out = ROOT.TFile("signal_efficiency_plots.root", "recreate")
#	tg_accept = ROOT.TGraphErrors()
#	tg_accept.SetName("acceptance_sqto4j_sig")
	h_total_sq = ROOT.TH1F("total_sq", "", 25, 87.5, 712.5)
	h_pass_sq = ROOT.TH1F("pass_sq", "", 25, 87.5, 712.5)
	h_pass_pre_sq = ROOT.TH1F("pass_pre_sq", "", 25, 87.5, 712.5)
	h_total_sg = ROOT.TH1F("total_sg", "", 25, 87.5, 712.5)
	h_pass_sg = ROOT.TH1F("pass_sg", "", 25, 87.5, 712.5)
	h_pass_pre_sg = ROOT.TH1F("pass_pre_sg", "", 25, 87.5, 712.5)
	plots = root.rfile("../study_contamination/contamination_plots.root")
	for im, m in enumerate(masses_sq):
		subprocess = "sq{}to4j".format(m)
		
		# Total:
		maod = dataset.fetch_miniaod(subprocess, "moriond17cutht700")
		total = maod.weight*maod.n/maod.preweight
		totale = maod.weight*(maod.n/maod.preweight)**(0.5)
		h_total_sq.SetBinContent(h_total_sq.GetXaxis().FindBin(m), total)
		h_total_sq.SetBinError(h_total_sq.GetXaxis().FindBin(m), totale)
		
		# post pre-selection:
		th1 = plots.get_tobject("{}_pre".format(subprocess))
		poste_pre = ROOT.Double(0.0)
		post_pre = th1.IntegralAndError(0, th1.GetNbinsX() + 1, poste_pre)
		h_pass_pre_sq.SetBinContent(h_pass_pre_sq.GetXaxis().FindBin(m), post_pre)
		h_pass_pre_sq.SetBinError(h_pass_pre_sq.GetXaxis().FindBin(m), poste_pre)
		
		# post signal selection:
		th1 = plots.get_tobject("{}_sig".format(subprocess))
		poste = ROOT.Double(0.0)
		post = th1.IntegralAndError(0, th1.GetNbinsX() + 1, poste)
		h_pass_sq.SetBinContent(h_pass_sq.GetXaxis().FindBin(m), post)
		h_pass_sq.SetBinError(h_pass_sq.GetXaxis().FindBin(m), poste)
		
		# Info:
		print subprocess
		print post, poste, total, totale
		print post_pre, poste_pre, total, totale
		print
	
	for im, m in enumerate(masses_sg):
		subprocess = "sg{}to5j".format(m)
		
		# Total:
		maod = dataset.fetch_miniaod(subprocess, "moriond17cutht500")
		total = maod.weight*maod.n/maod.preweight
		totale = maod.weight*(maod.n/maod.preweight)**(0.5)
		h_total_sg.SetBinContent(h_total_sg.GetXaxis().FindBin(m), total)
		h_total_sg.SetBinError(h_total_sg.GetXaxis().FindBin(m), totale)
		
		# post pre-selection:
		th1 = plots.get_tobject("{}_pre".format(subprocess))
		poste_pre = ROOT.Double(0.0)
		post_pre = th1.IntegralAndError(0, th1.GetNbinsX() + 1, poste_pre)
		h_pass_pre_sg.SetBinContent(h_pass_pre_sg.GetXaxis().FindBin(m), post_pre)
		h_pass_pre_sg.SetBinError(h_pass_pre_sg.GetXaxis().FindBin(m), poste_pre)
		
		# post signal selection:
		th1 = plots.get_tobject("{}_sig".format(subprocess))
		poste = ROOT.Double(0.0)
		post = th1.IntegralAndError(0, th1.GetNbinsX() + 1, poste)
		h_pass_sg.SetBinContent(h_pass_sg.GetXaxis().FindBin(m), post)
		h_pass_sg.SetBinError(h_pass_sg.GetXaxis().FindBin(m), poste)
		
		# Info:
		print subprocess
		print post, poste, total, totale
		print post_pre, poste_pre, total, totale
		print
	
	tf_out.WriteTObject(h_total_sq)
	tf_out.WriteTObject(h_pass_sq)
	tf_out.WriteTObject(h_pass_pre_sq)
	tf_out.WriteTObject(h_total_sg)
	tf_out.WriteTObject(h_pass_sg)
	tf_out.WriteTObject(h_pass_pre_sg)
	
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

