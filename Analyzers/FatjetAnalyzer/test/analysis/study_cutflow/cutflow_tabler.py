####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys, os
import yaml
from collections import OrderedDict
from decortication.samples import names_latex
from truculence import root
from ROOT import TFile, TCut, TH1F, gROOT
import cutflow
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
# /VARIABLES

# FUNCTIONS:
def get_ttnames():
	return {
		"sig": ["sq100to4j", "sq150to4j", "sq200to4j", "sq250to4j", "sq300to4j", "sq400to4j", "sq500to4j"],
		"bkg": ["ttbar", "qcdmg", "qcdp", "wjets", "zjets"],
	}


def read_cutflow(name):
	rf = root.rfile("cutflow_plots.root")
#	ttrees = rf.get_ttrees()
	info = cutflow.get_cutnames(style="latex")
	tt_names = get_ttnames()
	tt_names = tt_names["sig"] + tt_names["bkg"]
	results = OrderedDict()
	for tt_name in tt_names:
		results[tt_name] = []
		for icut, cut in enumerate(info[name]):
			cut_info = {"label": cut}
			for version in ["n", "w", "p"]:
				h = rf.tf.Get("{}_{}_{}".format(tt_name, name, version))
				value = h.GetBinContent(icut + 1)
				if version == "n": value = int(value)
				cut_info[version] = value
#			if icut == 0: cut_info["p"] = 100
#			else: cut_info["p"] = cut_info["w"]/results[tt_name][0]["w"]*100
			results[tt_name].append(cut_info)
	
	return results


def make_table(results, name, caption="", norm_qcd=True):
#	path = "cutflow_results/tables"
	path = "."
	if not os.path.exists(path): os.makedirs(path)
	
	tt_names = get_ttnames()
	cuts = [d["label"] for d in results[tt_names["sig"][0]]]
	
	table_name = "table_cutflow_{}".format(name)
	tabular_name = "tabular_cutflow_{}".format(name)
	label_name = "table:cutflow_{}".format(name)
	
	# Open the table:
	width = 0.9
	table = "\\begin{table}[htbp]\n"
	table += "\t\caption{" + caption + "}\n"
	table += "\t\\centering\\resizebox{" + str(width) + "\\textwidth}{!}{\n"
	tabular = "\t\\begin{tabular}{|l|"
	for cut in cuts:
		tabular += "r|r|"		# One for each subcolumn
	tabular += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\\textbf{Cut $\\rightarrow$}}"
	for cut in cuts:
		tabular += " & \multicolumn{2}{c|}{" + cut + "}"
	tabular += " \\\\ \cline{1-" + str(2*len(cuts) + 1) + "}\n\t\t\multicolumn{1}{|c|}{\\textbf{Sample $\\downarrow$}}"
	for cut in cuts:
		tabular += " & \multicolumn{1}{c|}{$N$} & \multicolumn{1}{c|}{\%}"
	tabular += " \\\\ \hline\n"

	# Make rows:
	rows = []
	groups = ["sig", "bkg"]
	for igroup, group in enumerate(groups):
		for tt_name in tt_names[group]:
			row = names_latex[tt_name] + " & "
			norm_w = results[tt_name][1]["w"]
			for icut, values_dict in enumerate(results[tt_name]):
				if norm_qcd and tt_name in ["qcdp", "qcdmg", "wjets", "zjets"] and icut > 0: values_dict["p"] = values_dict["w"]/norm_w*100
				if not values_dict["p"]: percent = "-"
				else:
					if values_dict["p"] >= 10: percent = "{:.1f}".format(values_dict["p"])
					elif 10 > values_dict["p"] >= 0.1: percent = "{:.2f}".format(values_dict["p"])
					elif 0.1 > values_dict["p"] >= 0.01: percent = "{:.3f}".format(values_dict["p"])
					elif 0.01 > values_dict["p"]: percent = "$<$ 0.001".format(values_dict["p"])
					percent += " \%"
				if not values_dict["w"]: N = "-"
				else:
					N = "{:.1f}".format(values_dict["w"])
					if N == "0.0": N = "$<$ 0.1"
				row += "{} & {} & ".format(N, percent)
			row = row[:-2]		# Remove trailing "&".
			row += "\\\\ \hline\n"
			rows.append(row)
		if igroup < len(groups) - 1: rows.append("\hline\n")
	
	# Add the rows to the table:
	for row in rows:
		tabular += "\t\t" + row

	# Close the table:
	tabular += "\t\end{tabular}\n"
	table += tabular
	table += "\t}\n\t\label{" + label_name + "}\n"
	table += "\end{table}"
	
	with open("{}/{}.tex".format(path, tabular_name), "w") as out:
		out.write(tabular)
	with open("{}/{}.tex".format(path, table_name), "w") as out:
		out.write(table)
	return table

def main():
	results = read_cutflow("sig")
	table = make_table(results, "sig", caption="The MC sample cutflow for the signal region. Percentages after the generator-level cut refer to the cumulative acceptance with respect to pre-selection.")
	print table + "\n"
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

