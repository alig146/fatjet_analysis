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
	[0.68, 0.23, 0.98, 0.43],
	[0.23, 0.23, 0.53, 0.43],
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
	tc = TCanvas("tc", "tc", 1000, 1000)
	gStyle.SetOptStat(0)
	gStyle.SetFrameBorderSize(0)
	gStyle.SetPadBorderSize(0)
	
	# Make plots:
	for i, info in enumerate(make_info):		# For each plot:
		# Basic variables:
		atts = {key: None for key in ["variable", "process", "cut", "color", "style", "label"]}
		atts = {key: (value if key not in info else info[key]) for key, value in atts.items()}
#		title = "{}, Cut {}".format(var_label, info["cut"])
		norm = False
		if "normalized" in info:
			norm = bool(info["normalized"])
		if "residual" not in info:
			info["residual"] = []
		
		# Prepare:
		tc.Clear()
		if info["residual"]:
			tps = [
				TPad("tp0", "tp0", 0, 0.35, 1, 1),
				TPad("tp1", "tp1", 0, 0, 1, 0.35),
			]
			[tp.Draw() for tp in tps]
			residual_th1s = []
			tps[0].cd()
		
		# Initialize legend:
		if "legend" in info:
			tl_location = tl_locations[info["legend"]]
		else:
			if "variable" in info:
				tl_location = tl_locations[plot_info["th1"][info["variable"]]["legend"]]
			else:
				tl_location = tl_locations[0]
		tl = TLegend(tl_location[0], tl_location[1], tl_location[2], tl_location[3])
		tl.SetBorderSize(0)
		
		nlines = len(info["lines"])
		for j, line in enumerate(info["lines"]):
			l = atts
			# Set attributes:
			for key, value in atts.items():
				if key in line:
					l[key] = line[key]
			processes = l["process"].split("+")
			
			## Color:
			color = l["color"]
			if not color and processes:
				color = colors_by_process[processes[0]]
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
			th1 = th1s["cut{}".format(atts["cut"])][atts["variable"]][processes[0]]["th1"]
#			th1.Sumw2()
			for process in processes[1:]:
				th1_temp = th1s["cut{}".format(atts["cut"])][atts["variable"]][process]["th1"]
				th1.Add(th1_temp)
			if norm:
				th1.Scale(1/th1.Integral())
			else:
				th1.Scale(0.23)		# KLUDGE!
			
			# Add th1 to legend:
			legend_label = l["label"]
			if not legend_label:
				legend_label = " + ".join([legend_labels[process] for process in processes])
			tl.AddEntry(th1, legend_label, "l")
			
			# Set basic plot attributes:
			th1.SetLineWidth(2)
			if l["style"]:
				th1.SetLineStyle(l["style"])
			th1.SetLineColor(color)
			th1.SetMarkerColor(color)
			if j == 0:
				th1.SetMinimum(0.01)
#				th1.SetTitle(title)
				th1.SetTitle("")
				xaxis_label = var_label
				if var_unit:
					xaxis_label += " ({})".format(var_unit)
				th1.GetXaxis().SetTitle(xaxis_label)
				th1.GetXaxis().CenterTitle()
				if norm:
					th1.GetYaxis().SetTitle("Events, normalized to unity")
				else:
					th1.GetYaxis().SetTitle("Events, weighted to 2.3 fb^{-1}")
				th1.GetYaxis().CenterTitle()
				th1.Draw("histe")
				
				if "rule" in info:		# BROKEN: Horizontal lines?!
					for xy in info["rule"]:
						if xy != "":
							tline = TLine(xy, th1.GetMinimum(), xy, th1.GetMaximum())
							tline.SetLineColor(kRed)
							tline.SetLineWidth(2)
							tline.SetLineStyle(2)
							tline.Draw()
				
				# Spacing:		# This doesn't work with "DrawNormlized" for some reason ...
				th1.GetXaxis().SetLabelOffset(th1.GetXaxis().GetLabelOffset()*1.1)
				th1.GetXaxis().SetLabelSize(th1.GetXaxis().GetLabelSize()*1.2)
				th1.GetXaxis().SetTitleOffset(th1.GetXaxis().GetTitleOffset()*1.1)
				th1.GetXaxis().SetTitleSize(th1.GetXaxis().GetTitleSize()*2.0)
				th1.GetYaxis().SetLabelOffset(th1.GetYaxis().GetLabelOffset()*1.1)
				th1.GetYaxis().SetLabelSize(th1.GetYaxis().GetLabelSize()*1.3)
				th1.GetYaxis().SetTitleOffset(th1.GetYaxis().GetTitleOffset()*1.1)
				th1.GetYaxis().SetTitleSize(th1.GetYaxis().GetTitleSize()*2.0)
			else:
				th1.Draw("histe,same")
			if j in info["residual"]:
				residual_th1s.append(th1)
		# Draw legend:
		tl.Draw()
		
		# Draw residual if applicable:
		if info["residual"]:
			if len(info["residual"]) == 2:
				tps[1].cd()
#				residual_plot = TH1F("h", "h", 11, 25, 575)
				residual_plot = residual_th1s[0].Clone()
				residual_plot.Add(residual_th1s[1], -1)
				residual_plot.Divide(residual_th1s[0])
#				residual_plot.Reset()
				residual_plot.SetMaximum(2)
				residual_plot.SetMinimum(-2)
#				for ibin in range(1, residual_plot.GetNbinsX() + 1):
#					values = [residual_th1s[ihist].GetBinContent(ibin) for ihist in info["residual"]]
#					errors = [residual_th1s[ihist].GetBinError(ibin) for ihist in info["residual"]]
##					print values
##					print (values[0]-values[1])/values[0]
#					r = (values[0]-values[1])/values[0]
#					re = r*((errors[0]**2+errors[1]**2)/(values[0]-values[1])**2 + (errors[0]**2/values[0]**2))**0.5
#					residual_plot.SetBinContent(ibin, r)
#					residual_plot.SetBinError(ibin, re)
				residual_plot.GetYaxis().SetTitle("Weighted residual")
				residual_plot.GetYaxis().SetNdivisions(8)
				
				residual_plot.GetXaxis().SetLabelOffset(0.008)
				residual_plot.GetXaxis().SetLabelSize(0.15)
				residual_plot.GetXaxis().SetTitleOffset(0.95)
				residual_plot.GetXaxis().SetTitleSize(0.16)
				residual_plot.GetYaxis().SetLabelOffset(0.006)
				residual_plot.GetYaxis().SetLabelSize(0.060)
				residual_plot.GetYaxis().SetTitleOffset(0.6)
				residual_plot.GetYaxis().SetTitleSize(0.084)
				
				
#				print residual_plot.GetXaxis().GetLabelOffset()
#				print residual_plot.GetXaxis().GetLabelSize()
#				print residual_plot.GetXaxis().GetTitleOffset()
#				print residual_plot.GetXaxis().GetTitleSize()
#				print residual_plot.GetYaxis().GetLabelOffset()
#				print residual_plot.GetYaxis().GetLabelSize()
#				print residual_plot.GetYaxis().GetTitleOffset()
#				print residual_plot.GetYaxis().GetTitleSize()
				
				residual_plot.Draw("e")
				
				# Draw "0 rule":
				tline = TLine(residual_plot.GetXaxis().GetXmin(), 0, residual_plot.GetXaxis().GetXmax(), 0)
				tline.SetLineColor(kBlack)
				tline.SetLineWidth(2)
				tline.SetLineStyle(2)
				tline.Draw()
			else:
				print "ERROR making residual plot."
		
		# More styling:
		if info["residual"]:
			tps[0].SetRightMargin(0.01)
			tps[1].SetRightMargin(0.01)
			tps[0].SetTopMargin(0.01)
			tps[1].SetTopMargin(0.02)
			tps[0].SetBottomMargin(0.01)
			tps[1].SetBottomMargin(0.35)
			tps[0].SetLeftMargin(0.17)
			tps[1].SetLeftMargin(0.17)
		else:
			gPad.SetRightMargin(0.01)
			gPad.SetTopMargin(0.01)
			gPad.SetBottomMargin(0.17)
			gPad.SetLeftMargin(0.17)
		
		
		# Finalize and save:
		fname = "{}_cut{}_{}".format(info["variable"], info["cut"], "-".join([line["process"] for line in info["lines"]]))
		if "fname" in info:
			fname = info["fname"]
		for logy in range(2):
			tc.SetLogy(logy)
#			fname = "{}_cut{}_{}".format(var, info["cut"], "-".join(info["processes"]))
#			fname = "{}".format(i)
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

