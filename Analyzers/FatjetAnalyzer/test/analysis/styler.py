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
	"qcdmg": kBlue,
	"qcdp": kCyan,
	"ttbar": kSpring,
	"sq150to4j": kPink+7,
	"sq200to4j": kRed,
	"sq250to4j": kOrange+7,
	"sq300to4j": kOrange,
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
	with open("plotter_info.yaml", 'r') as f:
		plot_info = yaml.load(f)
	
	
	gROOT.SetBatch()
	f_in = "plots.root"
	if len(sys.argv) > 1:
		f_in = sys.argv[1]
	
#	th1s = get_plots(f_in, process=["qcdmg", "ttbar", "sq150to4j_spring15cutht700_pt400", "sq300to4j_spring15_pt400"])
	th1s = get_plots(f_in, process=["qcdmg", "ttbar", "sq200to4j", "sq300to4j_spring15_pt400"])
	
	out_path = "plots/{}".format(f_in.split(".")[0])
	try:
		os.makedirs(out_path)
	except OSError as ex:
		import errno
		if ex.errno != errno.EEXIST:
			raise
	
	tc = TCanvas("tc", "tc")
	gStyle.SetOptStat(0)
	
#	for cut, variables in th1s.items():
##		print cut
#		for var_name, processes in variables.items():
#			info = plot_info["th1"][var_name]
#			for logy in range(2):
#				tc.Clear()
#				first = processes["qcdmg"]
#				name = first["var"] + "_" + first["cut"]
#				
#				# Initialize legend:
#				tl_localtion = tl_locations[info["legend"]]
#				tl = TLegend(tl_localtion[0], tl_localtion[1], tl_localtion[2], tl_localtion[3])
#				
#				# Style primary plot:
#				first["th1"].SetLineWidth(2)
#				first["th1"].SetLineColor(colors_by_process[first["process"]])
#				first["th1"].SetMarkerColor(colors_by_process[first["process"]])
#				first["th1"].SetMinimum(0.01)
#				first["th1"].SetTitle("{}, {}".format(info["label"], first["cut"]))
#				xaxis_label = info["label"]
#				if info["unit"]:
#					xaxis_label += " ({})".format(info["unit"])
#				first["th1"].GetXaxis().SetTitle(xaxis_label)
#				first["th1"].GetXaxis().CenterTitle()
#				first["th1"].GetYaxis().SetTitle("Events, weighted to 10 fb^{-1}")
#				first["th1"].GetYaxis().CenterTitle()
#				first["th1"].Draw("histe")
#				
#				tl.AddEntry(first["th1"], "qcdmg", "l")
#				
#				# Style secondary plots:
#				for p, th1_dict in processes.items():
#					p = p.split("_")[0]
#					if p != "qcdmg":
#						th1_dict["th1"].SetLineWidth(2)
#						th1_dict["th1"].SetLineColor(colors_by_process[p])
#						th1_dict["th1"].SetMarkerColor(colors_by_process[p])
#						th1_dict["th1"].Draw("histe,same")
#						
#						tl.AddEntry(th1_dict["th1"], p, "l")
#				tc.SetLogy(logy)
#				tl.Draw()
#				title = name
#				if logy:
#					title += "_logy"
#				for ext in formats:
#					tc.SaveAs("{}/{}.{}".format(out_path, title, ext))
	
	for cut, variables in th1s.items():
		tc.Clear()
		tc.SetLogy(0)
		ths = THStack()
		processes = variables["mavgp"]
#		print processes.keys()
		for p, th1_dict in processes.items():
			p = p.split("_")[0]
			th1_dict["th1"].SetFillColor(colors_by_process[p])
			th1_dict["th1"].SetLineWidth(0)
#			th1_dict["th1"].SetLineColor(colors_by_process[p])
		ths.Add(processes["qcdmg"]["th1"])
		ths.Add(processes["ttbar"]["th1"])
#		ths.Add(processes["sq150to4j_spring15cutht700_pt400"]["th1"])
		ths.Add(processes["sq300to4j_spring15_pt400"]["th1"])
		ths.Add(processes["sq200to4j"]["th1"])
		ths.SetMaximum(1.2*ths.GetMaximum())
		ths.Draw("hist")
		ths.GetXaxis().SetTitle("#bar{m}_{pruned} (GeV)")
		ths.GetXaxis().CenterTitle()
		ths.GetYaxis().SetTitle("Events, weighted to 10 fb^{-1}")
		ths.GetYaxis().CenterTitle()
#		ths.SetTitle("Average pruned mass, {}".format(cut))
		ths.SetTitle("")
		tl = TLegend(0.7, 0.7, 0.9, 0.9)
		tl.AddEntry(processes["qcdmg"]["th1"], "QCD MadGraph", "f")
		tl.AddEntry(processes["ttbar"]["th1"], "TTbar", "f")
#		tl.AddEntry(processes["sq150to4j_spring15cutht700_pt400"]["th1"], "sq150to4j", "f")
		tl.AddEntry(processes["sq300to4j_spring15_pt400"]["th1"], "m_{sq} = 300 GeV", "f")
		tl.AddEntry(processes["sq200to4j"]["th1"], "m_{sq} = 200 GeV", "f")
		tl.Draw()
		
		title = "stack_{}".format(cut)
		for logy in range(2):
			if logy:
				tc.SetLogy(1)
				title += "_logy"
			for ext in formats:
				tc.SaveAs("{}/{}.{}".format(out_path, title, ext))
	
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

