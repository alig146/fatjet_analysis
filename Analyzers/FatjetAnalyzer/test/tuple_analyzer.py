import sys
import ROOT

in_path = "tuple.root" if len(sys.argv) == 1 else sys.argv[1] 

tf = ROOT.TFile.Open(in_path)
tt = tf.Get("tuplizer/events")
for ievent, event in enumerate(tt):
#	if ievent in [84, 85, 86]:
	if True:
		print ievent, "\t".join(["{:.2f}".format(pt/jec) for pt, jec in zip(event.ca12_pf_pt, event.ca12_pf_jec)])
		print ievent, "\t".join(["{:.2f}".format(pt) for pt in event.ca12_pf_pt])
		print ievent, "\t".join(["{:.2f}".format(mp/jmc) for mp, jmc in zip(event.ca12_pf_mp, event.ca12_pf_jmc)])
		print ievent, "\t".join(["{:.2f}".format(mp) for mp in event.ca12_pf_mp])
		print ievent, "\t".join(["{:.4f}".format(tau2) for tau2 in event.ca12_pf_tau2])
		print ievent, "\t".join(["{:.4f}".format(tau2p) for tau2p in event.ca12_pf_tau2p])
		print ievent, "\t".join(["{:.4f}".format(tau42) for tau42 in event.ca12_pf_tau42])
		print
		print ievent, sum([pt/jec for pt, jec, eta in zip(event.ak8_pf_pt, event.ak8_pf_jec, event.ak8_pf_eta) if pt > 150 and abs(eta) < 2.5])
		print ievent, event.ak8_pf_ht[0]
		print
		print
