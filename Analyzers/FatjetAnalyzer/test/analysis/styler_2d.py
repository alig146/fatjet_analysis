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
	"ttbar": kAzure-2,
	"sq150to4j": kPink+7,
	"sq200to4j": kRed,
	"sq250to4j": kOrange+7,
	"sq300to4j": kRed, #kRed-9,
	"sq400to4j": kYellow-1,
}
tl_locations = [		# (W, S, E, N) or (x0, y0, x1, y1)
	[0.48, 0.48, 0.68, 0.68],
	[0.23, 0.78, 0.53, 0.98],
	[0.68, 0.78, 0.98, 0.98],
	[0.78, 0.23, 0.98, 0.43],
	[0.23, 0.23, 0.43, 0.43],
]
legend_labels = {
	"qcdp": "QCD Pythia",
	"qcdmg": "QCD MadGraph",
	"ttbar": "t#bar{t}",
	"sq150to4j": "m_{sq} = 150 GeV",
	"sq200to4j": "m_{sq} = 200 GeV",
	"sq250to4j": "m_{sq} = 250 GeV",
	"sq300to4j": "m_{sq} = 300 GeV",
	"sq400to4j": "m_{sq} = 400 GeV",
}
# :VARIABLES

# FUNCTIONS:
def get_plots(f, process=None, var=None, cut=None):
	tf_in = TFile.Open(f)
	SetOwnership(tf_in, 0)
	th2s = analysis.get_tobjects(tf_in, kind="th2f")
	
	th2_dict = {}
	for th2 in th2s:
		pieces = th2.GetName().split("_")
		p = "_".join(pieces[0:-3])		# Process
		if not p:
			p = pieces[0]
		vs = (pieces[-3], pieces[-2])		# Variables
		c = pieces[-1]		# Cut
		cn = int(c[3:])		# Cut number
#		print c,v,p
		
		if (not process or p in process) and (not var or vs in var) and (not cut or c in cut):
			if c not in th2_dict:
				th2_dict[c] = {}
			if vs not in th2_dict[c]:
				th2_dict[c][vs] = {}
			if p not in th2_dict[c][vs]:
				th2_dict[c][vs][p] = {}
		
			th2_dict[c][vs][p] = {
				"th2": th2,
				"process": p,
				"vars": vs,
				"cut": c,
				"ncut": cn,
			}
	
	return th2_dict


def main():
	# Read YAML infos:
	with open("plotter_info.yaml", 'r') as f:
		plot_info = yaml.load(f)
	with open("styler_2d_info.yaml", 'r') as f:
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
	th2s = get_plots(f_in)
	
	# Make ROOT preparations:
	tc = TCanvas("tc", "tc", 1000, 1000)
	gStyle.SetOptStat(0)
	gStyle.SetFrameBorderSize(0)
	gStyle.SetPadBorderSize(0)
	
	# Make plots:
	for i, info in enumerate(make_info):		# For each plot:
		# Basic variables:
#		atts = {key: None for key in ["variable", "process", "cut", "color"]}
#		atts = {key: (value if key not in info else info[key]) for key, value in atts.items()}
#		title = "{}, Cut {}".format(var_label, info["cut"])
		xvar, yvar = info["variables"][::-1]
		ncut = info["cut"]
		
		
		# Prepare:
		tc.Clear()
		
#		# Initialize legend:
#		if "variable" in info:
#			tl_location = tl_locations[plot_info["th1"][info["variable"]]["legend"]]
#		else:
#			tl_location = tl_locations[0]
#		tl = TLegend(tl_location[0], tl_location[1], tl_location[2], tl_location[3])
#		tl.SetBorderSize(0)
		
		for j, copy in enumerate(info["copies"]):
			## Variable:
			var_label_x = plot_info["th1"][xvar]["label"]
			var_unit_x = plot_info["th1"][xvar]["unit"]
			var_label_y = plot_info["th1"][yvar]["label"]
			var_unit_y = plot_info["th1"][yvar]["unit"]
			
			# Fetch hist:
			th2 = th2s["cut{}".format(ncut)][(yvar, xvar)][copy["process"]]["th2"]
			th2.Scale(0.23)		# KLUDGE!
			
			th2.SetTitle("")
			xaxis_label = var_label_x
			if var_unit_x:
				xaxis_label += " ({})".format(var_unit_x)
			yaxis_label = var_label_y
			if var_unit_y:
				yaxis_label += " ({})".format(var_unit_y)
			
			th2.GetXaxis().SetTitle(xaxis_label)
			th2.GetXaxis().CenterTitle()
			th2.GetYaxis().SetTitle(yaxis_label)
			th2.GetYaxis().CenterTitle()
			th2.GetZaxis().SetTitle("Events, weighted to 2.3 fb^{-1}")
			th2.GetZaxis().CenterTitle()
			th2.Draw("colz")
			
			if "rule" in info:
				for xy, value in enumerate(info["rule"]):
					if value != "":
						if xy == 0:
							tline = TLine(value, th2.GetXaxis().GetXmin(), value, th2.GetXaxis().GetXmax())
						elif xy == 1:
							tline = TLine(th2.GetXaxis().GetXmin(), value, th2.GetXaxis().GetXmax(), value)
						tline.SetLineColor(kRed)
						tline.SetLineWidth(2)
						tline.SetLineStyle(2)
						tline.Draw()
			
			# Spacing:
			th2.GetXaxis().SetLabelOffset(th2.GetXaxis().GetLabelOffset()*1.1)
			th2.GetXaxis().SetLabelSize(th2.GetXaxis().GetLabelSize()*1.2)
			th2.GetXaxis().SetTitleOffset(th2.GetXaxis().GetTitleOffset()*1.1)
			th2.GetXaxis().SetTitleSize(th2.GetXaxis().GetTitleSize()*2.0)
			th2.GetYaxis().SetLabelOffset(th2.GetYaxis().GetLabelOffset()*1.1)
			th2.GetYaxis().SetLabelSize(th2.GetYaxis().GetLabelSize()*1.3)
			th2.GetYaxis().SetTitleOffset(th2.GetYaxis().GetTitleOffset()*1.1)
			th2.GetYaxis().SetTitleSize(th2.GetYaxis().GetTitleSize()*1.8)
			th2.GetZaxis().SetLabelOffset(th2.GetZaxis().GetLabelOffset()*1.1)
			th2.GetZaxis().SetLabelSize(th2.GetZaxis().GetLabelSize()*1.3)
			th2.GetZaxis().SetTitleOffset(th2.GetZaxis().GetTitleOffset()*1.1)
			th2.GetZaxis().SetTitleSize(th2.GetZaxis().GetTitleSize()*1.8)
		
			# More styling:
			gPad.SetRightMargin(0.20)
			gPad.SetTopMargin(0.03)
			gPad.SetBottomMargin(0.17)
			gPad.SetLeftMargin(0.15)
		
			# Finalize and save:
			fname = "{}_cut{}_{}".format("-".join(info["variables"]), ncut, copy["process"])
			if "fname" in info:
				fname = info["fname"]
			for logz in range(2):
				tc.SetLogz(logz)
#				tl.Draw()
	#			fname = "{}_cut{}_{}".format(var, info["cut"], "-".join(info["processes"]))
	#			fname = "{}".format(i)
				if logz:
					fname += "_logz"
				for ext in formats:
					tc.SaveAs("{}/{}.{}".format(out_path, fname, ext))
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

