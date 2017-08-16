####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
from truculence import root
#from ROOT import TH1D, TFile
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def main():
	rf = root.rfile("theta_plots_sig15_sb.root")
	params = rf.get_tobject("params")
	data = [
#		("QCD amplitude", params.GetBinContent(1), params.GetBinError(1)),
		("QCD amplitude", "floating", "floating", 41.5, 6.4, "events"),
#		("QCD shift", params.GetBinContent(2), params.GetBinError(2)),
		("QCD shift", 0.0, 20.0, 9.2, 13.0, "GeV"),
#		("QCD stretch", params.GetBinContent(3), params.GetBinError(3)),
		("QCD stretch", 0, 8, 1.92, 5.84, "\%"),
#		("$\\ttbar$ amplitude", params.GetBinContent(4), params.GetBinError(4)),
		("$\\ttbar$ amplitude", 1.0, 0.2, 0.93, 0.19, ""),
		("$\\ttbar$ shift", 0.0, 5.0, 0.65, 4.75, "GeV"),
		("$\\ttbar$ stretch", 0, 9, 0.63, 8.91, "\%"),
#		("Signal amplitude", params.GetBinContent(4), params.GetBinError(4)),
		("Signal amplitude", 1.0, 0.2, 0, 0, ""),
		("Signal shift", "$\\ttbar$ value", "", "$\\ttbar$ value", "", "GeV"),
		("Signal stretch", "$\\ttbar$ value", "", "$\\ttbar$ value", "", "\%"),
	]
	print data
	
	caption = "Nuisance parameters corresponding to each shape and rate parameter of the background and signal distributions"
	table = "\\begin{table}[htbp]\n\t\caption{" + caption + "}\n"
	table += "\t\centering\setlength\doublerulesep{0.1in}\n\t\\resizebox{0.4\\textwidth}{!}{\n"
	tabular = "\t\t\\begin{tabular}{|l|r|r|}\hline\n\t\t\t\\textbf{Parameter} & \\textbf{Pre-fit value} & \\textbf{Post-fit value}\\\\\hline\n"
	for irow, t in enumerate(data):
		unit = t[5]
		if irow == 0: tabular += "\t\t\t" + t[0] + " & " + "{}".format(t[1]) + " & " + "${:.1f} \pm {:.1f}$ {}".format(t[3], t[4], unit) + "\\\\\hline\n"
		elif irow > 0 and irow < 6: 
			if unit == "\%": tabular += "\t\t\t" + t[0] + " & " + "$\left({:.2f}\pm {:.2f}\\right)~{}$".format(t[1], t[2], unit) + " & " + "$\left({:.2f}\pm {:.2f}\\right)~{}$".format(t[3], t[4], unit) + "\\\\\hline\n"
			else: tabular += "\t\t\t" + t[0] + " & " + "${:.2f}\pm {:.2f}$ {}".format(t[1], t[2], unit) + " & " + "${:.2f}\pm {:.2f}$ {}".format(t[3], t[4], unit) + "\\\\\hline\n"
		elif irow == 6: tabular += "\t\t\t" + t[0] + " & " + "${:.2f}\pm {:.2f}$ {}".format(t[1], t[2], unit) + " & " + " - " + "\\\\\hline\n"
		elif irow > 6: tabular += "\t\t\t" + t[0] + " & " + "{}".format(t[1]) + " & " + "{}".format(t[3]) + "\\\\\hline\n"
	tabular += "\t\t\end{tabular}\n"
	table += tabular
	table += "\t}\n"
	table += "\t\label{table:nuisance}\n"
	table += "\end{table}"
	
	print table
	
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

