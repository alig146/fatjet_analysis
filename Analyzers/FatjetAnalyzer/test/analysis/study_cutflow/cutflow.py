####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys
import yaml
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
weight_factor = 38.180/2.183
# /VARIABLES

# FUNCTIONS:
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

def make_table(results):
	# Open the table:
	n_cuts = len(results)
	table = "\\begin{table}[H]\n\t\centering\n\t\\tiny\n\t\\begin{tabular}{|l|"
	for icut in range(n_cuts):
		table += "r|r|r|"		# One for each subcolumn
	table += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\multirow{2}{*}{\\textbf{Sample}}}"
	for icut in range(n_cuts):
		table += " & \multicolumn{3}{c|}{\\textbf{Cut " + str(icut) + "}}"
	table += " \\\\ \cline{2-" + str(3*n_cuts + 1) + "}\n\t\t\multicolumn{1}{|c|}{}"
	for icut in range(n_cuts):
		table += " & \\textbf{events (!w)} & \\textbf{events} & \multicolumn{1}{c|}{\\textbf{\%}}"
	table += " \\\\ \hline\n"
	
	# Make rows:
	## Reformat the input:
	results_new = {}
	for icut, cut_results in enumerate(results):
		for process, values in cut_results.items():
			if process not in results_new:
				results_new[process] = []
			results_new[process].append(values) 
	## Format the rows:
	rows = []
	for process, values_list in results_new.items():
		row = "{} & ".format(process)
		values_initial = values_list[0]
		for values in values_list:
			try:
				percent = float(values[0])/values_initial[0]*100
			except:
				percent = 100
			row += "{} & {:.2f} & {:.2f} \% & ".format(values[0], values[1], percent)
		row = row[:-2]		# Remove trailing "&".
		row += "\\\\ \hline\n"
		rows.append(row)
	
	# Add the rows to the table:
	for row in rows:
		table += "\t\t" + row
	
	# Close the table:
	table += "\t\end{tabular}\n\end{table}"
	return table

def main():
	gROOT.SetBatch()
	rf = root.rfile(anatuple_in)
	ttrees = rf.get_ttrees()
	with open("cutflow.yaml") as f:
		info = yaml.load(f)
	
	tf = TFile("cutflow_plots.root", "recreate")		# This prevents new TTrees from giving an error about being memory residents.
	ths = {(tt_name, cut_key, metric): TH1F("{}_{}_{}".format(tt_name, cut_key, metric), "", len(cuts), 0, len(cuts)) for tt_name in ttrees.keys() for cut_key, cuts in info.items() for metric in ["n", "w"]}
	for th in ths.values():
		for key, cuts in info.items():
			for icut, cut in enumerate(cuts):
				th.GetXaxis().SetBinLabel(icut + 1, cut)
	
	for tt_name, ttree in ttrees.items():
		print "[..] Making cutflow for {}.".format(tt_name)
		wf = 1 if "jetht" in tt_name else weight_factor
		print "\tWill apply a weight factor of {:.2f}".format(wf)
		for ievent, event in enumerate(ttree):
			if ievent%10000 == 0: print "\t[..] Processing event {}.".format(ievent)
			if event.htak8 > 900 and max(event.m_t) > 50 and min(event.pt) > 400 and max([abs(v) for v in event.eta]) < 2.0:
				ths[tt_name, "sig", "n"].Fill(0)
				ths[tt_name, "sig", "w"].Fill(0, event.w*wf)
				if event.deta < 1.0:
					ths[tt_name, "sig", "n"].Fill(1)
					ths[tt_name, "sig", "w"].Fill(1, event.w*wf)
					if event.masy_p < 0.1:
						ths[tt_name, "sig", "n"].Fill(2)
						ths[tt_name, "sig", "w"].Fill(2, event.w*wf)
						if max(event.tau43) < 0.80 and max(event.tau42) < 0.45 and max(event.tau21) < 0.75:
							ths[tt_name, "sig", "n"].Fill(3)
							ths[tt_name, "sig", "w"].Fill(3, event.w*wf)
	for th in ths.values(): th.Write()
	sys.exit()
	
	
	for key, cuts in info.items():
		ncuts = len(cuts)
		th = TH1F(key, "", ncuts, 0, ncuts)
		for icut, cut in enumerate(cuts):
			tcut = TCut(cut, "w*({})".format(cut))
			for tt_name in ttrees.keys():
				ths[tt_name, key, "n"].GetXaxis().SetBinLabel(icut+1, cut)
				ths[tt_name, key, "w"].GetXaxis().SetBinLabel(icut+1, cut)
#				if tt_name == "sq500to4j": continue
#				if tt_name == "sq400to4j": continue
				ttrees[tt_name] = ttrees[tt_name].CopyTree(tcut.GetTitle())
				ttrees[tt_name].Draw("mavg_p>>h", "w")
				h = tf.Get("h")
				print tt_name, tcut.GetTitle(), ttrees[tt_name].GetEntries(), h.GetEntries(), h.Integral()
				ths[tt_name, key, "n"].SetBinContent(icut+1, h.GetEntries())
				ths[tt_name, key, "w"].SetBinContent(icut+1, h.Integral())
#				return True;
	for th in ths.values(): th.Write()
	
	sys.exit()
	# Set up ROOT stuff:
	gROOT.SetBatch()
	
	# Prepare:
	tcuts = get_cuts("cutflow_cuts.txt")
	ttrees = get_ttrees(anatuple_in)
	
	# Apply cuts to each process:
	results = []
	cut_full = ""
	for tcut in tcuts:
		cut_results = {}
		for ttree in ttrees:
			print "Applying {} to {} ...".format(tcut.GetName(), ttree.GetName())
			th1 = apply_cut(tcut, ttree)
			nweighted = th1.Integral()*0.2161
			if ttree.GetName() == "jetht":
				nweighted = th1.GetEntries()
			cut_results[ttree.GetName()] = (int(th1.GetEntries()), nweighted)
		results.append(cut_results)
	
	# Make tables:
	print "Making table ..."
	print make_table(results)
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

