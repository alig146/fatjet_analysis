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
			cut_results[ttree.GetName()] = (int(th1.GetEntries()), th1.Integral())
		results.append(cut_results)
	
	# Make tables:
	print "Making table ..."
	print make_table(results)
	
#	for cut in cuts:
#		if cut_full:
#			cut_full += "&&({})".format(cut)
#		else:
#			cut_full += "({})".format(cut)
#		result = {}
#		
#		for process, tt in ttd.items():
#			dss = dsd[process]
#			signal = dss[0].signal
#			th = apply_cut(cut_full, tt)
#			
#			ns = {}
#			ns[0] = th.Integral(0, th.GetNbinsX() + 1)
#			if signal:
#				m = dss[0].m
#				ns[m] = th.Integral(th.GetXaxis().FindBin(m) - 1, th.GetXaxis().FindBin(m))
#			else:
#				for m in ms:
#					ns[m] = th.Integral(th.GetXaxis().FindBin(m) - 1, th.GetXaxis().FindBin(m))
#			result[process] = ns
#		results.append(result)
#	print results
#	
#	# Analyze results:
#	
##	## Make totals table:
##	n_cut_columns = 5
##	cut_groups = [cuts[x:x+n_cut_columns] for x in range(0, len(cuts), n_cut_columns)]
##	if len(cut_groups[-1]) == 1:		# Don't allow a single-column table, tack it on to the previous one.
##		cut_groups[-2:] = [cut_groups[-2] + cut_groups[-1]]

##	for i, cut_group in enumerate(cut_groups):
##		table_totals = "\\begin{table}[H]\n\t\centering\n\t\\tiny\n\t\\begin{tabular}{|l|"
##		for cut in cut_group:
##			table_totals += "r|r|"
##		table_totals += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\multirow{2}{*}{\\textbf{Sample}}}"
##		for cut in cut_group:
##			table_totals += " & \multicolumn{2}{c|}{\\textbf{" + cut.split()[0].replace("$", "\\$").replace("_", "\\_") + (" ${}$".format(" ".join(cut.split()[1:]).replace("$", "\\$")), "")[cut == "1"] + "}}"
##		table_totals += " \\\\ \cline{2-" + str(2*len(cut_group) + 1) + "}\n\t\t\multicolumn{1}{|c|}{}"
##		for cut in cut_group:
##			table_totals += " & \\textbf{passed} & \multicolumn{1}{c|}{\\textbf{\%}}"
##		table_totals += " \\\\ \hline\n"
##	
##		for process in ttd.keys():
##			row = process + " & "
##			for j, cut in enumerate(cut_group):
##				i_cut = i*n_cut_columns + j
###				print i_cut
##				n_base = results[0][process][0]
##				n = results[i_cut][process][0]
##				row += str(int(n)) + " & " + "{:.1f}".format(n/n_base*100) + " \%"
##				if j < len(cut_group)-1:
##					row += " & "
##				else:
##					row += " \\\\ \hline"
##			table_totals += "\t\t{}\n".format(row)
##	
##		table_totals += "\t\end{tabular}\n\end{table}"
##		print table_totals
#	
#	## Make SB table:
#	n_cut_columns = 3
#	cut_groups = [cuts[x:x+n_cut_columns] for x in range(0, len(cuts), n_cut_columns)]
#	if len(cut_groups[-1]) == 1:		# Don't allow a single-column table, tack it on to the previous one.
#		cut_groups[-2:] = [cut_groups[-2] + cut_groups[-1]]
#	
#	for i, cut_group in enumerate(cut_groups):
#		table_sb = "\\begin{table}[H]\n\t\centering\n\t\\tiny\n\t\\begin{tabular}{|l|l|"
#		for cut in cut_group:
#			table_sb += "r|r|r|"
#		table_sb += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\multirow{2}{*}{\\textbf{signal}}} & \multicolumn{1}{|c|}{\multirow{2}{*}{\\textbf{back.}}}"
#		for cut in cut_group:
#			table_sb += " & \multicolumn{3}{c|}{\\textbf{" + format_cut(cut) + "}}"
#		table_sb += " \\\\ \cline{3-" + str(3*len(cut_group) + 2) + "}\n\t\t\multicolumn{1}{|c|}{} & \multicolumn{1}{|c|}{}"
#		for cut in cut_group:
#			table_sb += " & \multicolumn{1}{|c|}{\\textbf{s}} & \multicolumn{1}{c|}{\\textbf{b}} & \multicolumn{1}{c|}{s/$\sqrt{\\textbf{b}}$}"
#		table_sb += " \\\\ \hline\n"

#		for process in ttd.keys():
#			ds = dsd[process][0]
#			if ds.signal:
#				m = ds.m
#				
#				for bckg in ["qcdmg", "qcdp"]:
#					row = process + " & " + bckg + " & "
#					for j, cut in enumerate(cut_group):
#						i_cut = i*n_cut_columns + j
#						b = results[i_cut][bckg][m]
#						s = results[i_cut][process][m]*0.132		# KLUDGE!
#						row += str(int(s)) + " & " + str(int(b)) + " & " + "{:.1f}".format(s/(b**0.5))
#						if j < len(cut_group)-1:
#							row += " & "
#						else:
#							row += " \\\\ \hline"
#					table_sb += "\t\t{}\n".format(row)

#		table_sb += "\t\end{tabular}\n\end{table}"
#		print table_sb
#	return True
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

