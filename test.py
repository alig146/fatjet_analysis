import ROOT
from DataFormats.FWLite import Events, Handle
events = Events ('root://cms-xrd-global.cern.ch//store/mc/RunIIFall17MiniAODv2/StealthSYY_2t6j_mStop-300_mSo-100_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/50000/E23348BD-C9A3-E811-80C5-1866DA7F94F2.root')

#create handle outside of loop                                                                                                                                                                                 
handle  = Handle ('std::vector<reco::GenParticle>')
label = ("genParticles")
handle2 = Handle ('std::vector<TTTrack< Ref_Phase2TrackerDigi_>>')
label2 =  ("TTTracksFromTracklet")

#loop over events                                                                                                                                                                                              
#for event in events:
#    event.getByLabel (label, handle)
#    print event.object().id().event()

#    gen = handle.product()
#    for g in handle:
#        print g.charge() , g.pdgId()





tracks = Handle("std::vector")
histogram = ROOT.TH1F("histogram", "histogram", 100, 0, 100)
i = 0
for event in events:
    print "Event", i
    event.getByLabel("packedPFCandidates", tracks)
    j = 0
    for track in tracks.product():
        print "    Track", j, track.charge() / track.pt(), track.phi(), track.eta(), track.dxy(), track.dz()
        histogram.Fill(track.pt())
        j += 1
    i += 1
    if i >= 5: break


c = ROOT.TCanvas ( "c" , "c" , 800, 800 )
c.cd()
histogram.Draw()
c.SaveAs("PtFW.png")
