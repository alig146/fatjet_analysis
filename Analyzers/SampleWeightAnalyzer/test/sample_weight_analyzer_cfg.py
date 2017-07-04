import FWCore.ParameterSet.Config as cms

process = cms.Process("Analyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/mc/RunIIFall15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/00547C97-2FCC-E511-8D75-002590DB91D2.root'
#        '/store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_1.root'
#         "file:/uscms_data/d3/tote/data/examples/maod_data_0800_1000.root"
#		"/store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/150/00000/34A57FB8-D819-E611-B0A4-02163E0144EE.root"
#		"file:/uscms_data/d3/tote/data/examples/maod_mc_fall15_1000.root"
    )
)

process.analyzer = cms.EDAnalyzer('SampleWeightAnalyzer',
	genInfo=cms.InputTag("generator"),
)

process.TFileService = cms.Service("TFileService", fileName=cms.string('sample_weights.root'))

process.p = cms.Path(process.analyzer)
