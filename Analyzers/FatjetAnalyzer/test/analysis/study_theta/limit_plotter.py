import sys
from limit_tools import *
from ROOT import TFile

name = "sig15_sb"

tg_c = make_cross_graphs()
tg_e = make_limit_graphs("expected_{}.txt".format(name))
tg_o = make_limit_graphs("observed_{}.txt".format(name))

out = TFile("theta_plots_{}_output.root".format(name), "recreate")
for tg in tg_c.values(): out.WriteTObject(tg)
for tg in tg_e.values(): out.WriteTObject(tg)
for tg in tg_o.values(): out.WriteTObject(tg)
