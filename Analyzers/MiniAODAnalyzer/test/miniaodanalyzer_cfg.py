import FWCore.ParameterSet.Config as cms

process = cms.Process("Analyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(2))

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#        '/store/mc/RunIISummer16MiniAODv2/TT_TuneEE5C_13TeV-powheg-herwigpp/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/0045D9EF-67B5-E611-B736-FA163EEF13C9.root'
#        '/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root'
        '/store/user/elhughes/Sq175To4J/miniaod_sq175to4j_moriond17cutht700_v1/171218_165657/0000/miniaod_sq175to4j_moriond17cutht700_1.root'
#        '/store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_1.root'
#         "file:/uscms_data/d3/tote/data/examples/maod_data_0800_1000.root"
#		"/store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/150/00000/34A57FB8-D819-E611-B0A4-02163E0144EE.root"
#		"file:/uscms_data/d3/tote/data/examples/maod_mc_fall15_1000.root"
    )
)

process.analyzer = cms.EDAnalyzer('MiniAODAnalyzer',
	genParticles=cms.InputTag("prunedGenParticles"),
	genInfo=cms.InputTag("generator"),
	pileupInfo=cms.InputTag("slimmedAddPileupInfo"),
	triggerResults=cms.InputTag("TriggerResults", "", "HLT"),
	triggerPrescales=cms.InputTag("patTrigger", ""),
)


process.p = cms.Path(process.analyzer)
