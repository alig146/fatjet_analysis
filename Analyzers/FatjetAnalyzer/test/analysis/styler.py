####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys, os, yaml
from ROOT import *
from truculence import analysis
from re import search
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
formats = ["pdf", "png", "svg"]
colors_by_process = {
	"jetht": kBlack,
	"qcdmg": kBlue,
	"qcdp": kCyan,
	"ttbar": kSpring,
	"sq150to4j": kPink+7,
	"sq200to4j": kRed,
	"sq250to4j": kOrange+7,
	"sq300to4j": kOrange,
	"sq400to4j": kYellow-1,
}
tl_locations = [		# (W, S, E, N) or (x0, y0, x1, y1)
	[0.4, 0.4, 0.6, 0.6],
	[0.15, 0.7, 0.35, 0.9],
	[0.7, 0.7, 0.9, 0.9],
	[0.7, 0.15, 0.9, 0.35],
	[0.15, 0.15, 0.35, 0.35],
]
# :VARIABLES

# FUNCTIONS:
def get_plots(f, process=None, var=None, cut=None):
	tf_in = TFile.Open(f)
	SetOwnership(tf_in, 0)
	th1s = analysis.get_tobjects(tf_in, kind="th1f")
	
	th1_dict = {}
	for th1 in th1s:
		pieces = th1.GetName().split("_")
		p = "_".join(pieces[0:-2])		# Process
		if not p:
			p = pieces[0]
		v = pieces[-2]		# Variable
		c = pieces[-1]		# Cut
		cn = int(pieces[2][3:])		# Cut number
#		print c,v,p
		
		if (not process or p in process) and (not var or v in var) and (not cut or c in cut):
			if c not in th1_dict:
				th1_dict[c] = {}
			if v not in th1_dict[c]:
				th1_dict[c][v] = {}
			if p not in th1_dict[c][v]:
				th1_dict[c][v][p] = {}
		
			th1_dict[c][v][p] = {
				"th1": th1,
				"process": p,
				"var": v,
				"cut": c,
				"ncut": cn,
			}
	
	return th1_dict


def main():
	# Read YAML infos:
	with open("plotter_info.yaml", 'r') as f:
		plot_info = yaml.load(f)
	with open("styler_info.yaml", 'r') as f:
		make_info = yaml.load(f)
	
	# Prepare input:
	gROOT.SetBatch()
	f_in = "plots_fall15.root"
	if len(sys.argv) > 1:
		f_in = sys.argv[1]
	
	# Prepare output path:
	out_path = "plots/{}".format(f_in.split(".")[0])
	try:
		os.makedirs(out_path)
	except OSError as ex:
		import errno
		if ex.errno != errno.EEXIST:
			raise
	
	# Fetch histograms from input:
	th1s = get_plots(f_in)
	
	# Make ROOT preparations:
	tc = TCanvas("tc", "tc")
	gStyle.SetOptStat(0)
	
	# Make plots:
	for i, info in enumerate(make_info):		# For each plot:
		# Basic variables:
		atts = {key: None for key in ["variable", "process", "cut", "color"]}
		atts = {key: (value if key not in info else info[key]) for key, value in atts.items()}
#		title = "{}, Cut {}".format(var_label, info["cut"])
		
		# Prepare:
		tc.Clear()
		
		# Initialize legend:
		if "variable" in info:
			tl_location = tl_locations[plot_info["th1"][info["variable"]]["legend"]]
		else:
			tl_location = tl_locations[0]
		tl = TLegend(tl_location[0], tl_location[1], tl_location[2], tl_location[3])
		
		for j, line in enumerate(info["lines"]):
			l = atts
			# Set attributes:
			for key, value in atts.items():
				if key in line:
					l[key] = line[key]
			## Color:
			color = l["color"]
			if not color and l["process"]:
				color = colors_by_process[l["process"]]
			if not color:
				print "ERROR (styler.py): I don't know what color this line is supposed to be:"
				print line
				sys.exit()
			else:
				if isinstance(color, str):
					color = getattr(ROOT, color)
			## Variable:
			var_label = plot_info["th1"][l["variable"]]["label"]
			var_unit = plot_info["th1"][l["variable"]]["unit"]
			
			# Fetch line:
			th1 = th1s["cut{}".format(atts["cut"])][atts["variable"]][atts["process"]]["th1"]
			
			# Add th1 to legend:
			tl.AddEntry(th1, l["process"], "l")
			
			# Set basic plot attributes:
			th1.SetLineWidth(2)
			th1.SetLineColor(color)
			th1.SetMarkerColor(color)
			if j == 0:
				th1.SetMinimum(0.01)
#				th1.SetTitle(title)
				xaxis_label = var_label
				if var_unit:
					xaxis_label += " ({})".format(var_unit)
				th1.GetXaxis().SetTitle(xaxis_label)
				th1.GetXaxis().CenterTitle()
				th1.GetYaxis().SetTitle("Events, weighted to 10 fb^{-1}")
				th1.GetYaxis().CenterTitle()
				th1.Draw("histe")
			else:
				th1.Draw("histe,same")
		
		# Finalize and save:
		for logy in range(2):
			tc.SetLogy(logy)
			tl.Draw()
#			fname = "{}_cut{}_{}".format(var, info["cut"], "-".join(info["processes"]))
			fname = "{}".format(i)
			if logy:
				fname += "_logy"
			for ext in formats:
				tc.SaveAs("{}/{}.{}".format(out_path, fname, ext))
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

