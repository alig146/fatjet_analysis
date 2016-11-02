####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys
from truculence import root
from decortication import dataset
from ROOT import gROOT, gDirectory
from plotter import get_cuts, get_ttrees
# /IMPORTS

# CLASSES:
#class cut:
#	def __init__(self):
# /CLASSES

# VARIABLES:
anatuple_in = "~/anatuples/anatuple_ca12_fall15_cutpt400.root"
# /VARIABLES

# FUNCTIONS:
#def parse_cuts(cuts_file="cuts_tau21s.txt"):
#	with open(cuts_file) as f:
#		return [line.strip() for line in f.read().strip().split("\n")]

def apply_cut(tcut, tt, v=False):
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

