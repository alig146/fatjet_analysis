####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
from truculence import root
from decortication import dataset
from ROOT import gROOT, gDirectory
# /IMPORTS

# CLASSES:
#class cut:
#	def __init__(self):
# /CLASSES

# VARIABLES:
anatuple_in = "~/anatuples/anatuple_ca12_cutpt400.root"
# /VARIABLES

# FUNCTIONS:
def parse_cuts(cuts_file="cuts.txt"):
	with open(cuts_file) as f:
		return [line.strip() for line in f.read().strip().split("\n")]

def apply_cut(cut, tt, v=True):
	if v: print "{}, cut = {}:".format(tt.GetName(), cut)
	tt.Draw("mavg_p>>h(10, 0, 500)", "w[0]*({})".format(cut))
	h = gDirectory.Get("h")
	return h

def main():
	gROOT.SetBatch()
	rf = root.rfile(anatuple_in)
	ttd = rf.get_ttrees()
	dsd = dataset.get_datasets(process=ttd.keys())
	ms = [dss[0].m for process, dss in dsd.items() if dss[0].signal]
	cuts = parse_cuts()
	
	# Apply cuts to each process:
	results = []
	cut_full = ""
	for cut in cuts:
		if cut_full:
			cut_full += "&&({})".format(cut)
		else:
			cut_full += "({})".format(cut)
		result = {}
		
		for process, tt in ttd.items():
			dss = dsd[process]
			signal = dss[0].signal
			th = apply_cut(cut_full, tt)
			
			ns = {}
			ns[0] = th.Integral(0, th.GetNbinsX() + 1)
			if signal:
				m = dss[0].m
				ns[m] = th.Integral(th.GetXaxis().FindBin(m) - 1, th.GetXaxis().FindBin(m))
			else:
				for m in ms:
					ns[m] = th.Integral(th.GetXaxis().FindBin(m) - 1, th.GetXaxis().FindBin(m))
			result[process] = ns
		results.append(result)
#	print results
	
	# Analyze results:
	
	## Make totals table:
	table_totals = "\\begin{table}[H]\n\t\label{my-label}\n\t\centering\n\t\\tiny\n\t\\begin{tabular}{|l|"
	for cut in cuts:
		table_totals += "r|r|"
	table_totals += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\multirow{2}{*}{\\textbf{Sample}}}"
	for cut in cuts:
		table_totals += " & \multicolumn{2}{c|}{\\textbf{" + cut.split()[0].replace("$", "\\$").replace("_", "\\_") + (" ${}$".format(" ".join(cut.split()[1:]).replace("$", "\\$")), "")[cut == "1"] + "}}"
	table_totals += " \\\\ \cline{2-" + str(2*len(cuts) + 1) + "}\n\t\t\multicolumn{1}{|c|}{}"
	for cut in cuts:
		table_totals += " & \\textbf{passed} & \multicolumn{1}{c|}{\\textbf{\%}}"
	table_totals += " \\\\ \hline\n"
	
	for process in ttd.keys():
		row = process + " & "
		for i, cut in enumerate(cuts):
			n_base = results[0][process][0]
			n = results[i][process][0]
			row += str(int(n)) + " & " + "{:.1f}".format(n/n_base*100) + " \%"
			if i < len(cuts)-1:
				row += " & "
			else:
				row += " \\\\ \hline"
		table_totals += "\t\t{}\n".format(row)
	
	table_totals += "\t\end{tabular}\n\t\caption{My caption}\n\end{table}"
	print table_totals
	
	## Make SB table:
	table_sb = "\\begin{table}[H]\n\t\label{my-label}\n\t\centering\n\t\\tiny\n\t\\begin{tabular}{|l|"
	for cut in cuts:
		table_sb += "r|r|r|"
	table_sb += "}\n\t\t\hline\n\t\t\multicolumn{1}{|c|}{\multirow{2}{*}{\\textbf{Signal}}}"
	for cut in cuts:
		table_sb += " & \multicolumn{3}{c|}{\\textbf{" + cut.split()[0].replace("$", "\\$").replace("_", "\\_") + (" ${}$".format(" ".join(cut.split()[1:]).replace("$", "\\$")), "")[cut == "1"] + "}}"
	table_sb += " \\\\ \cline{2-" + str(3*len(cuts) + 1) + "}\n\t\t\multicolumn{1}{|c|}{}"
	for cut in cuts:
		table_sb += " & \multicolumn{1}{|c|}{\\textbf{s}} & \multicolumn{1}{c|}{\\textbf{b}} & \multicolumn{1}{c|}{s/$\sqrt{\\textbf{b}}$}"
	table_sb += " \\\\ \hline\n"
	
	for process in ttd.keys():
		ds = dsd[process][0]
		if ds.signal:
			m = ds.m
			row = process + " & "
			for i, cut in enumerate(cuts):
				b = results[i]["qcdmg"][m]
				s = results[i][process][m]*0.132
				row += str(int(s)) + " & " + str(int(b)) + " & " + "{:.1f}".format(s/(b**0.5))
				if i < len(cuts)-1:
					row += " & "
				else:
					row += " \\\\ \hline"
			table_sb += "\t\t{}\n".format(row)
	
	table_sb += "\t\end{tabular}\n\t\caption{My caption}\n\end{table}"
	print table_sb
	return True
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

