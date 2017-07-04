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
TT_names = {
	"sig": ["sq100to4j", "sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j"],
	"bkg": ["qcdmg", "qcdp", "ttbar", "wjets", "zjets"],
}
proper_names = {
	"sq100to4j": "$m_{\\tilde{q}} = 100$~\\text{GeV}",
	"sq150to4j": "$m_{\\tilde{q}} = 150$~\\text{GeV}",
	"sq200to4j": "$m_{\\tilde{q}} = 200$~\\text{GeV}",
	"sq250to4j": "$m_{\\tilde{q}} = 250$~\\text{GeV}",
	"sq300to4j": "$m_{\\tilde{q}} = 300$~\\text{GeV}",
	"sq400to4j": "$m_{\\tilde{q}} = 400$~\\text{GeV}",
	"sq500to4j": "$m_{\\tilde{q}} = 500$~\\text{GeV}",
	"qcdp": "$\\QCDP$",
	"qcdmg": "$\\QCDMG$",
	"ttbar": "\\ttbar",
	"wjets": "$\\WJETS$",
	"zjets": "$\\ZJETS$",
}
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

def read_cutflow(tt_names, name):
	rf = root.rfile("cutflow_plots.root")
#	ttrees = rf.get_ttrees()
	with open("cutflow.yaml") as f:
		info = yaml.load(f)
	
	tt_names = tt_names["sig"] + tt_names["bkg"]
	results = OrderedDict()
	for tt_name in tt_names:
		results[tt_name] = []
		for icut, cut in enumerate(info[name]):
			cut_info = {"label": cut}
			for version in ["n", "w"]:
				h = rf.tf.Get("{}_{}_{}".format(tt_name, name, version))
				value = h.GetBinContent(icut + 1)
				if version == "n": value = int(value)
				cut_info[version] = value
			if icut == 0: cut_info["p"] = 100
			else: cut_info["p"] = cut_info["w"]/results[tt_name][0]["w"]*100
			results[tt_name].append(cut_info)
	
	return results


def make_table(results, name, tt_names, caption=""):
	path = "cutflow_results/tables"
	if not os.path.exists(path): os.makedirs(path)
	
#	tt_names = results.keys()
	cuts = [d["label"] for d in results[tt_names["sig"][0]]]
	
	table_name = "table_cutflow_{}".format(name)
	label_name = "table:cutflow_{}".format(name)
	
	# Open the table:
	width = 0.9
	table = "\\begin{table}[htbp]\n"
	table += "\t\\centering\\resizebox{" + str(width) + "\\textwidth}{!}{\n\t\\begin{tabular}{|l|"
	for cut in cuts:
		table += "r|r|"		# One for each subcolumn
	table += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\\textbf{Cut $\\rightarrow$}}"
	for cut in cuts:
		table += " & \multicolumn{2}{c|}{" + cut + "}"
	table += " \\\\ \cline{1-" + str(2*len(cuts) + 1) + "}\n\t\t\multicolumn{1}{|c|}{\\textbf{Sample $\\downarrow$}}"
	for cut in cuts:
		table += " & \multicolumn{1}{c|}{$N$} & \multicolumn{1}{c|}{\%}"
	table += " \\\\ \hline\hline\n"

	# Make rows:
	rows = []
	groups = ["sig", "bkg"]
	for igroup, group in enumerate(groups):
		for tt_name in tt_names[group]:
			row = proper_names[tt_name] + " & "
			for values_dict in results[tt_name]:
				percent = "{:.1f}".format(values_dict["p"])
				if percent == "0.0": percent = "$<$ 0.1"
				N = "{:.1f}".format(values_dict["w"])
				if N == "0.0": N = "$<$ 0.1"
				row += "{} & {} \% & ".format(N, percent)
			row = row[:-2]		# Remove trailing "&".
			row += "\\\\ \hline\n"
			rows.append(row)
		if igroup < len(groups) - 1: rows.append("\hline\n")
	
	# Add the rows to the table:
	for row in rows:
		table += "\t\t" + row

	# Close the table:
	table += "\t\end{tabular}}\n"
	table += "\t\caption{" + caption + "}\n"
	table += "\t\label{" + label_name + "}\n"
	table += "\end{table}"
	
	with open("{}/{}.tex".format(path, table_name), "w") as out:
		out.write(table)
	return table

def main():
	results = read_cutflow(TT_names, "sig")
	table = make_table(results, "sig", TT_names, caption="MC sample cutflow for the signal region")
	print table + "\n"
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

