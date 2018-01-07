import ROOT
ROOT.gROOT.SetBatch()
from DataFormats.FWLite import Events, Handle

events = Events('jetworkshop.root')

handle_jets_pruned2 = Handle('std::vector<reco::BasicJet>')
handle_jets_pruned = Handle('std::vector<pat::Jet>')
handle_jets = Handle('std::vector<pat::Jet>')
label_jets_pruned = ("selectedPatJetsCA12CHSPruned")
label_jets_pruned2 = ("jetsBasicPFCA12CHSPruned")
label_jets = ("selectedPatJetsCA12CHS")

for ievent, event in enumerate(events):
	if ievent in [84, 85, 86]:
		event.getByLabel(label_jets, handle_jets)
		event.getByLabel(label_jets_pruned, handle_jets_pruned)
#		event.getByLabel(label_jets_pruned2, handle_jets_pruned2)
		jets = handle_jets.product()
		jets_pruned = handle_jets_pruned.product()
#		jets_pruned2 = handle_jets_pruned2.product()
	
		print ievent, "\t".join(["{:.2f}".format(jet.pt()) for jet in jets])
		print ievent, "\t".join(["{:.2f}".format(jet.mass()) for jet in jets])
		print ievent, "\t".join(["{:.2f}".format(jet.userFloat("massCA12CHSPruned")) for jet in jets])
		print ievent, "\t".join(["{:.2f}".format(jet.userFloat("massCA12CHSTrimmed")) for jet in jets])
		print ievent, "\t".join(["{:.5f}".format(jet.userFloat("tausCA12CHS:tau2")) for jet in jets])
#		print ievent, "\t".join(["{:.5f}".format(jet.userFloat("tausCA12CHSPruned:tau2")) for jet in jets])
#		print
##		print ievent, "\t".join(["{:.2f}".format(jet.pt()) for jet in jets_pruned2])
#		print ievent, "\t".join(["{:.2f}".format(jet.mass()) for jet in jets_pruned2])
		print
		print ievent, "\t".join(["{:.2f}".format(jet.pt()) for jet in jets_pruned])
		print ievent, "\t".join(["{:.2f}".format(jet.mass()) for jet in jets_pruned])
		print ievent, "\t".join(["{:.5f}".format(jet.userFloat("tausCA12CHSPruned:tau2")) for jet in jets_pruned])
	
		print
		print

