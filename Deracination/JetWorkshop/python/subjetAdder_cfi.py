import FWCore.ParameterSet.Config as cms

subjet_variables = ["px", "py", "pz", "e", "pt", "m", "eta", "phi"]

Subjetter = cms.EDProducer("SubjetProducer",
	src=cms.InputTag("ca12PFJetsCHS"),
	nSubjets=cms.uint32(4),
)
