import FWCore.ParameterSet.Config as cms

process = cms.Process("Analyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    	'/store/mc/RunIISummer16MiniAODv2/ZZTo2L2Q_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0C12A530-18DE-E611-A759-047D7B881D04.root', '/store/mc/RunIISummer16MiniAODv2/ZZTo2L2Q_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/1CAC8ED6-BBDD-E611-AFBD-008CFAF5592A.root', '/store/mc/RunIISummer16MiniAODv2/ZZTo2L2Q_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/405D1E29-05DD-E611-923B-001E67792576.root', '/store/mc/RunIISummer16MiniAODv2/ZZTo2L2Q_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/682226C4-EDDC-E611-B3ED-20CF3027A5B7.root'		# ~220,000
#    	'/store/mc/RunIISummer16MiniAODv2/WWToLNuQQ_13TeV-powheg/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/060EC6F6-5AB9-E611-B3CA-002590D0B0B6.root'		# 100,000
#    	'/store/mc/RunIISummer16MiniAODv2/WWTo4Q_13TeV-powheg/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0298A3BB-3CC3-E611-80F2-0CC47A7FC6E6.root',		# ~120,000
#    	'/store/mc/RunIISummer16MiniAODv2/ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/0A34656B-70C5-E611-B7E2-B499BAAC0414.root',		# ~120,000
#		'/store/mc/RunIISummer16MiniAODv2/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0E5C4A43-04B7-E611-B3D9-A0000420FE80.root',		# ~90,000
#    	'/store/mc/RunIISummer16MiniAODv2/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/002BD1CD-24B5-E611-A29C-0025907254C8.root',		# ~70,000
#		'/store/mc/RunIISummer16MiniAODv2/WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/007F0D37-9ABE-E611-9EE6-002590E3A212.root', '/store/mc/RunIISummer16MiniAODv2/WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/0CFB4A2B-9FBE-E611-AB28-FA163E0889DC.root'		# ~100,000
#    	'/store/mc/RunIISpring16MiniAODv2/WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/30000/0EA1D6CA-931A-E611-BFCD-BCEE7B2FE01D.root', '/store/mc/RunIISpring16MiniAODv2/WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/30000/2C5E7A70-831A-E611-BC3B-F46D0450CEA0.root'		# ~100,000
#    	'/store/mc/RunIISummer16MiniAODv2/ZJetsToQQ_HT600toInf_13TeV-madgraph/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/16DC0526-F4FA-E611-938E-6CC2173BBA40.root', '/store/mc/RunIISummer16MiniAODv2/ZJetsToQQ_HT600toInf_13TeV-madgraph/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/18DFBF7B-B5FC-E611-80D2-002481DE49B6.root', '/store/mc/RunIISummer16MiniAODv2/ZJetsToQQ_HT600toInf_13TeV-madgraph/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/20C84FF5-11FB-E611-AA3C-C4346BC70B58.root'		# ~100,000
#    	'/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root'		# ~100,000
#        '/store/mc/RunIIFall15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/00547C97-2FCC-E511-8D75-002590DB91D2.root'
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

process.xsec = cms.EDAnalyzer("GenXSecAnalyzer")

process.p = cms.Path(process.analyzer*process.xsec)
