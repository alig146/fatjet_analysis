import ROOT
ROOT.gROOT.SetBatch()
from DataFormats.FWLite import Events, Handle

events = Events('jettoolbox.root')

handle_jets_pruned = Handle('std::vector<reco::BasicJet>')
handle_jets_pruned2 = Handle('std::vector<pat::Jet>')
handle_jets = Handle('std::vector<pat::Jet>')
label_jets_pruned = ("ca12PFJetsCHSPruned")
label_jets_pruned2 = ("selectedPatJetsCA12PFCHSPruned")
label_jets = ("selectedPatJetsCA12PFCHS")

for ievent, event in enumerate(events):
	if ievent in [84, 85, 86]:
		event.getByLabel(label_jets, handle_jets)
		event.getByLabel(label_jets_pruned, handle_jets_pruned)
#		event.getByLabel(label_jets_pruned2, handle_jets_pruned2)
		jets = handle_jets.product()
		jets_pruned = handle_jets_pruned.product()
#		jets_pruned2 = handle_jets_pruned2.product()
	
		print ievent, "\t".join(["{:.2f}".format(jet.pt()) for jet in jets])
		print ievent, "\t".join(["{:.2f}".format(jet.userFloat("ca12PFJetsCHSPrunedMass")) for jet in jets])
		print ievent, "\t".join(["{:.2f}".format(jet.userFloat("ca12PFJetsCHSTrimmedMass")) for jet in jets])
		print
		print ievent, "\t".join(["{:.2f}".format(jet.pt()) for jet in jets_pruned])
		print ievent, "\t".join(["{:.2f}".format(jet.mass()) for jet in jets_pruned])
#		print
#		print ievent, "\t".join(["{:.2f}".format(jet.pt()) for jet in jets_pruned2])
#		print ievent, "\t".join(["{:.2f}".format(jet.mass()) for jet in jets_pruned2])
	
		print
		print

