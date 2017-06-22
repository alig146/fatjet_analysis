####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys, os
import yaml
from collections import OrderedDict
from truculence import root
from ROOT import TFile, TCut, TH1F, gROOT
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
names_sig = ["sq100to4j", "sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j"]
names_bkg = [
	"qcdmg",
#	"qcdp",
	"ttbar",
	"wjets",
]
# /VARIABLES

# FUNCTIONS:
#def make_table_old(results):
#	# Open the table:
#	n_cuts = len(results)
#	table = "\\begin{table}[H]\n\t\centering\n\t\\tiny\n\t\\begin{tabular}{|l|"
#	for icut in range(n_cuts):
#		table += "r|r|r|"		# One for each subcolumn
#	table += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\multirow{2}{*}{\\textbf{Sample}}}"
#	for icut in range(n_cuts):
#		table += " & \multicolumn{3}{c|}{\\textbf{Cut " + str(icut) + "}}"
#	table += " \\\\ \cline{2-" + str(3*n_cuts + 1) + "}\n\t\t\multicolumn{1}{|c|}{}"
#	for icut in range(n_cuts):
#		table += " & \\textbf{events (!w)} & \\textbf{events} & \multicolumn{1}{c|}{\\textbf{\%}}"
#	table += " \\\\ \hline\n"
#	
#	# Make rows:
#	## Reformat the input:
#	results_new = {}
#	for icut, cut_results in enumerate(results):
#		for process, values in cut_results.items():
#			if process not in results_new:
#				results_new[process] = []
#			results_new[process].append(values) 
#	## Format the rows:
#	rows = []
#	for process, values_list in results_new.items():
#		row = "{} & ".format(process)
#		values_initial = values_list[0]
#		for values in values_list:
#			try:
#				percent = float(values[0])/values_initial[0]*100
#			except:
#				percent = 100
#			row += "{} & {:.2f} & {:.2f} \% & ".format(values[0], values[1], percent)
#		row = row[:-2]		# Remove trailing "&".
#		row += "\\\\ \hline\n"
#		rows.append(row)
#	
#	# Add the rows to the table:
#	for row in rows:
#		table += "\t\t" + row
#	
#	# Close the table:
#	table += "\t\end{tabular}\n\end{table}"
#	return table

def read_cutflow(tt_names):
	rf = root.rfile("cutflow_plots.root")
#	ttrees = rf.get_ttrees()
	with open("cutflow.yaml") as f:
		info = yaml.load(f)
	
	results = OrderedDict()
	for tt_name in tt_names:
		results[tt_name] = OrderedDict()
		for cut in info["sig"]:
			results[tt_name][cut] = []
		for version in ["n", "w"]:
			h = rf.tf.Get("{}_sig_{}".format(tt_name, version))
			for icut, cut in enumerate(info["sig"]):
				value = h.GetBinContent(icut + 1)
				if version == "n": value = int(value)
				results[tt_name][cut].append(value)
		for cut in info["sig"]:
			results[tt_name][cut].append(results[tt_name][cut][1]/results[tt_name]["1"][1]*100)
	
	return results


def make_tables(results, name, caption="", n=3):
	path = "cutflow_results/tables"
	if not os.path.exists(path): os.makedirs(path)
	
	tt_names = results.keys()
	
	tables = OrderedDict()
	groups = [tt_names[i:n+i] for i in range(0, len(tt_names), n)]
	for igroup, group in enumerate(groups):
		table_name = "table_cutflow_{}_{}".format(name, igroup)
		label_name = "table:cutflow_{}_{}".format(name, igroup)
		
		# Open the table:
		n_ds = len(group)
		width = 0.9 if n_ds == 4 else 0.7
		table = "\\begin{table}[H]\n"
		if igroup == 0 and caption: table += "\t\caption{" + caption + "}\n"
		table += "\t\\centering\\resizebox{" + str(width) + "\\textwidth}{!}{\n\t\\begin{tabular}{|l|"
		for tt_name in group:
			table += "r|r|r|"		# One for each subcolumn
		table += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\\textbf{Sample $\\rightarrow$}}"
		for tt_name in group:
			table += " & \multicolumn{3}{c|}{\\textbf{" + tt_name + "}}"
		table += " \\\\ \cline{1-" + str(3*n_ds + 1) + "}\n\t\t\multicolumn{1}{|c|}{\\textbf{Cut $\\downarrow$}}"
		for tt_name in group:
			table += " & \multicolumn{1}{|c|}{\\textbf{N}} & \multicolumn{1}{|c|}{\\textbf{n}} & \multicolumn{1}{c|}{\\textbf{\%}}"
		table += " \\\\ \hline\n"

		# Make rows:
		cuts = results[group[0]].keys()
		rows = []
		for cut in cuts:
			row = "\\Q{" + cut + "} & "
			for tt_name in group:
				values = results[tt_name][cut]
				percent = "{:.1f}".format(values[2])
				if percent == "0.0": percent = "$<$ 0.1"
				row += "{} & {:.1f} & {} \% & ".format(values[0], values[1], percent)
			row = row[:-2]		# Remove trailing "&".
			row += "\\\\ \hline\n"
			rows.append(row)
		
		# Add the rows to the table:
		for row in rows:
			table += "\t\t" + row

		# Close the table:
		table += "\t\end{tabular}}\n"
		table += "\t\label{" + label_name + "}\n"
		table += "\end{table}"
		tables[table_name] = table
		
		with open("{}/{}.tex".format(path, table_name), "w") as out:
			out.write(table)
	return tables

def main():
	results_sig = read_cutflow(names_sig)
	tables_sig = make_tables(results_sig, "signal", caption="Signal MonteCarlo cutflow", n=4)
	for table in tables_sig.values(): print table + "\n"
	
	results_bkg = read_cutflow(names_bkg)
	tables_bkg = make_tables(results_bkg, "background", caption="Background MonteCarlo cutflow", n=4)
	for table in tables_bkg.values(): print table + "\n"
	
	
	
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

