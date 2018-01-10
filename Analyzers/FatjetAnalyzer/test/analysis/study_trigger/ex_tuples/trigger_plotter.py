from truculence import root
import os, yaml, math
from ROOT import gROOT, gDirectory, TFile, TH1D, TH2D

# Configure:
gROOT.SetBatch()
#nevents_process = 1000000
nevents_process = -1

# Read input:
path = "/cms/tote/store/SingleMuon/tuple_smu16f_feb17_cutpt0etax/180108_201610"
files = ["/".join([path, f]) for f in os.listdir(path)]
tc = root.make_tc(files, "tuplizer/events")
nevents = tc.GetEntries()
if nevents_process < 0: nevents_process = nevents

# Prepare output:
tf = TFile.Open("trigger_plots.root", "recreate")
## Construct histograms:
hs = dict(
	ht_mp0_total = TH2D("ht_mp0_total", "", 200, 0, 2000, 50, 0, 500),
	ht_mp0_pass = TH2D("ht_mp0_pass", "", 200, 0, 2000, 50, 0, 500),
	ht_mp0ak8_total = TH2D("ht_mp0ak8_total", "", 200, 0, 2000, 50, 0, 500),
	ht_mp0ak8_pass = TH2D("ht_mp0ak8_pass", "", 200, 0, 2000, 50, 0, 500),
#	ht_total = TH1D("ht_total", "", 200, 0, 2000),
#	ht_pass = TH1D("ht_pass", "", 200, 0, 2000),
#	mp0_total = TH1D("mp0_total", "", 50, 0, 500),
#	mp0_pass = TH1D("mp0_pass", "", 50, 0, 500),
)

# Loop over events:
print "[..] Processing {}/{} ({:.1f}%) events in the tuple.".format(nevents_process, nevents, float(nevents_process)/nevents*100)
for ievent, event in enumerate(tc):
	if nevents_process > 0 and ievent >= nevents_process: break
	if ievent%10000 == 0: print "[..] Processing event {}/{} ({:.1f}%)".format(ievent + 1, nevents, float(ievent + 1)/nevents_process*100)
	if ievent < 10:
		print event.trig_mupt50[0], "\t", event.trig_pfht900[0], event.trig_pfak8ht700mt50[0], event.trig_pfpt450[0], event.trig_pfak8pt360mt30[0], bool(event.trig_pfht900[0] or event.trig_pfak8ht700mt50[0] or event.trig_pfpt450[0] or event.trig_pfak8pt360mt30[0])
	
#	if len(event.ca12_pf_mp) == 0:
#		print "[--] WARNING: Event {} has no pruned masses.".format(ievent + 1)
#		print event.ak8_pf_pt[0]
		
	
	if not event.trig_mupt50[0]:
		print "[--] WARNING: Event {} didn't pass the muon trigger. Do you expect that?".format(ievent + 1)
	elif event.trig_mupt50[0] and len([pt for pt, eta in zip(event.ak8_pf_pt, event.ak8_pf_eta) if pt > 170 and abs(eta) < 2.5]) >= 2:
		ht = event.ak8_pf_ht[0]
		mp0 = event.ca12_pf_mp[0]
		mp0ak8 = event.ak8_pf_mp[0]
		hs["ht_mp0_total"].Fill(ht, mp0)
		hs["ht_mp0ak8_total"].Fill(ht, mp0ak8)
		if event.trig_pfht900[0] or event.trig_pfak8ht700mt50[0] or event.trig_pfpt450[0] or event.trig_pfak8pt360mt30[0]:
			hs["ht_mp0_pass"].Fill(ht, mp0)
			hs["ht_mp0ak8_pass"].Fill(ht, mp0ak8)

# Write out histograms:
for key, h in hs.items(): tf.WriteTObject(h)



