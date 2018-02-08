from truculence import root
from decortication import dataset
import os, sys, yaml, math
from ROOT import gROOT, gDirectory, TFile, TH1D, TH2D


tuples = {
	"sq100to4j": dataset.fetch_entries("tuple", dict(process="sq100to4j", generation=["moriond17cutht700"], suffix="cutpt0etax")),
	"sq200to4j": dataset.fetch_entries("tuple", dict(process="sq200to4j", generation=["moriond17cutht700"], suffix="cutpt0etax")),
	"sq300to4j": dataset.fetch_entries("tuple", dict(process="sq300to4j", generation=["moriond17cutht700"], suffix="cutpt0etax")),
	"sq400to4j": dataset.fetch_entries("tuple", dict(process="sq400to4j", generation=["moriond17cutht700"], suffix="cutpt0etax")),
	"sq500to4j": dataset.fetch_entries("tuple", dict(process="sq500to4j", generation=["moriond17cutht700"], suffix="cutpt0etax")),
#	"qcdmg": dataset.fetch_entries("tuple", dict(process="qcdmg", generation=["moriond17", "moriond17ext"], suffix="cutpt300eta20")),
}

tf = TFile.Open("jets_plots.root", "recreate")
gROOT.SetBatch()

for p, tups in tuples.items():
	print p
	files = []
	for tup in tups: files += tup.files
#	if not isinstance(paths, list): list(paths)
#	files = []
#	for path in paths: files += ["/".join([path, f]) for f in os.listdir(path)]
	tc = root.make_tc(files, "tuplizer/events")
	print tc.GetEntries()
	
	hs = dict(
		njets = TH1D("njets_{}".format(p), "", 6,-0.5,5.5),
		njets_fjsigl = TH1D("njets_fjsigl_{}".format(p), "", 6,-0.5,5.5),
		pt0_jec0 = TH2D("pt0_jec0_{}".format(p), "", 1200, 0, 2400, 1200, 0.8, 1.2),
		pt0ak8_jec0ak8 = TH2D("pt0ak8_jec0ak8_{}".format(p), "", 1200, 0, 2400, 1200, 0.8, 1.2),
		jec0ak8_jec0 = TH2D("jec0ak8_jec0_{}".format(p), "", 1200, 0.8, 1.2, 1200, 0.8, 1.2),
		jec0ak8_jec0_pt300to500 = TH2D("jec0ak8_jec0_pt300to500_{}".format(p), "", 1200, 0.8, 1.2, 1200, 0.8, 1.2),
		mp0 = TH1D("mp0_{}".format(p), "", 1200, 0, 1200),
		mp0ak8 = TH1D("mp0ak8_{}".format(p), "", 1200, 0, 1200),
		masyp0 = TH1D("masyp0_{}".format(p), "", 1200, -1, 1),
		dm0 = TH1D("dm0_{}".format(p), "", 1200, -240, 240),
		mp0ak8_mp0 = TH2D("mp0ak8_mp0_{}".format(p), "", 1200, 0, 1200, 1200, 0, 1200),
		mp0ak8_masyp0 = TH2D("mp0ak8_masyp0_{}".format(p), "", 1200, 0, 1200, 1200, -1, 1),
		mp0_masyp0 = TH2D("mp0_masyp0_{}".format(p), "", 1200, 0, 1200, 1200, -1, 1),
		mp0ak8_dm0 = TH2D("mp0ak8_dm0_{}".format(p), "", 1200, 0, 1200, 1200, -240, 240),
		mp0_dm0 = TH2D("mp0_dm0_{}".format(p), "", 1200, 0, 1200, 1200, -240, 240),
		dR0 = TH1D("dR0_{}".format(p), "", 1200, 0, 3.2),
		mp0_pre = TH1D("mp0_pre_{}".format(p), "", 1200, 0, 1200),
		ms0_pre = TH1D("ms0_pre_{}".format(p), "", 1200, 0, 1200),
		mavgp_pre = TH1D("mavgp_pre_{}".format(p), "", 1200, 0, 1200),
		mavgs_pre = TH1D("mavgs_pre_{}".format(p), "", 1200, 0, 1200),
		mp0_sig = TH1D("mp0_sig_{}".format(p), "", 1200, 0, 1200),
		ms0_sig = TH1D("ms0_sig_{}".format(p), "", 1200, 0, 1200),
		mavgp_sig = TH1D("mavgp_sig_{}".format(p), "", 1200, 0, 1200),
		mavgs_sig = TH1D("mavgs_sig_{}".format(p), "", 1200, 0, 1200),
	)

	for ievent, event in enumerate(tc):
		if ievent%1000000 == 0 and ievent != 0: print "event", ievent
		
#		ht = event.ak8_pf_ht[0]
		ht = sum([pt/jec for pt, jec, eta in zip(getattr(event, "ak8_pf_pt"), getattr(event, "ak8_pf_jec"), getattr(event, "ak8_pf_eta")) if pt/jec > 150 and abs(eta) < 2.5])
		w = event.wpu[0]*event.w[0]
		
		pt0 = event.ca12_pf_pt[0]
		pt1 = event.ca12_pf_pt[1]
		jec0 = event.ca12_pf_jec[0]
		eta0 = event.ca12_pf_eta[0]
		eta1 = event.ca12_pf_eta[1]
		deta = abs(eta0 - eta1)
		phi0 = event.ca12_pf_phi[0]
		phi1 = event.ca12_pf_phi[1]
		mp0 = event.ca12_pf_mp[0]
		mp1 = event.ca12_pf_mp[1]
		mavgp = (mp0 + mp1)/2
		masyp = abs(mp0 - mp1)/(mp0 + mp1)
		
		ms0 = event.ca12_pf_ms[0]
		ms1 = event.ca12_pf_ms[1]
		mavgs = (ms0 + ms1)/2
		masys = abs(ms0 - ms1)/(ms0 + ms1)
		
		pt0ak8 = event.ak8_pf_pt[0]
		jec0ak8 = event.ak8_pf_jec[0]
		mp0ak8 = event.ak8_pf_mp[0]
		eta0ak8 = event.ak8_pf_eta[0]
		phi0ak8 = event.ak8_pf_phi[0]
		dR0 = (abs(eta0 - eta0ak8)**2 + (math.pi - abs(math.pi - abs(phi0 - phi0ak8)))**2)**0.5
		dm0 = mp0 - mp0ak8
		try: masyp0 = dm0/(mp0 + mp0ak8)
		except: masyp0 = 1000
		
		tau210 = event.ca12_pf_tau21[0]
		tau211 = event.ca12_pf_tau21[1]
		tau420 = event.ca12_pf_tau42[0]
		tau421 = event.ca12_pf_tau42[1]
		tau430 = event.ca12_pf_tau43[0]
		tau431 = event.ca12_pf_tau43[1]
		
		if dR0 < 1.0:
			hs["jec0ak8_jec0"].Fill(jec0ak8, jec0)
			if 300 < pt0 < 500: hs["jec0ak8_jec0_pt300to500"].Fill(jec0ak8, jec0)
		
		
		if ht > 900:
#			if ievent < 100:
#				print ievent, event.ca12_pf_pt[1], event.ca12_pf_eta[1], event.ca12_pf_phi[1]
#				print ievent, event.ca12_pfp_pt[1], event.ca12_pfp_eta[1], event.ca12_pfp_phi[1]
			hs["njets"].Fill(len([pt for pt, eta in zip(event.ca12_pf_pt, event.ca12_pf_eta) if pt > 400 and abs(eta) < 2.0]), event.wpu[0])
			
			hs["mp0"].Fill(mp0, w)
			hs["mp0ak8"].Fill(mp0ak8, w)
			hs["dR0"].Fill(dR0, w)
			hs["pt0_jec0"].Fill(pt0, jec0, w)
			hs["pt0ak8_jec0ak8"].Fill(pt0ak8, jec0ak8, w)
			if dR0 < 1.0:
				hs["dm0"].Fill(masyp0, w)
				hs["masyp0"].Fill(masyp0, w)
				hs["mp0ak8_mp0"].Fill(mp0ak8, mp0, w)
				hs["mp0ak8_masyp0"].Fill(mp0ak8, masyp0, w)
				hs["mp0_masyp0"].Fill(mp0, masyp0, w)
				hs["mp0ak8_dm0"].Fill(mp0ak8, dm0, w)
				hs["mp0_dm0"].Fill(mp0, dm0, w)
			
			# Pre-selection:
			if min(pt0, pt1) > 400 and max(abs(eta0), abs(eta1)) < 2.0:
				hs["mp0_pre"].Fill(mp0, w)
				hs["ms0_pre"].Fill(ms0, w)
				hs["mavgp_pre"].Fill(mavgp, w)
				hs["mavgs_pre"].Fill(mavgs, w)
				
				# Loose FJ signal selection:
				if tau210 < 0.75 and tau420 < 0.50 and tau430 < 0.90:
					hs["njets_fjsigl"].Fill(len([pt for pt, eta in zip(event.ca12_pf_pt, event.ca12_pf_eta) if pt > 400 and abs(eta) < 2.0]), event.wpu[0])
				
				# Signal selection:
				if deta < 1.0 and max(tau210, tau211) < 0.75 and max(tau420, tau421) < 0.50 and max(tau430, tau431) < 0.80:
					if masyp < 0.1:
						hs["mp0_sig"].Fill(mp0, w)
						hs["mavgp_sig"].Fill(mavgp, w)
					if masys < 0.1:
						hs["ms0_sig"].Fill(ms0, w)
						hs["mavgs_sig"].Fill(mavgs, w)
					
	for key, h in hs.items(): tf.WriteTObject(h)




