#cmsRun fatjetproducer_cfg.py dataset="sqtojjjj" maxEvents=10 inFile="sqtojjjj_300gev_7411_aodsim_10.root" inDir="root://xrootd.unl.edu//store/user/elhughes/SqToJJJJ/SqToJJJJ_300/150901_215515/0000" outDir="/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Producers/FatjetProducer/test" cmssw=$CMSSW_RELEASE_BASE

#cmsRun fatjetproducer_cfg.py dataset="sqtojjjj" maxEvents=5 inFile="sqtojjjj_200gev_7411_aodsim_10.root" inDir="root://xrootd.unl.edu//store/user/elhughes/SqToJJJJ_200/SqToJJJJ_200/151008_081406/0000" outDir="/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Producers/FatjetProducer/test" cmssw=$CMSSW_RELEASE_BASE m=200

#cmsRun fatjetproducer_cfg.py dataset="ttbar" maxEvents=10 inFile="022B08C4-C702-E511-9995-D4856459AC30.root" inDir="root://xrootd.unl.edu//store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000" outDir="/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Producers/FatjetProducer/test" cmssw=$CMSSW_RELEASE_BASE

#cmsRun fatjetproducer_cfg_new.py dataset="sqtojjjj" m="200" maxEvents=2 outDir="/uscms_data/d3/tote/CMSSW_7_3_2_patch2/src/Producers/FatjetProducer/test" cmssw=$CMSSW_RELEASE_BASE auto=1
#cmsRun fatjetproducer_cfg.py dataset="qcd" method="pythia50to80" maxEvents=10 inFile="02071298-1415-E511-A143-0025905B8572.root" inDir="root://xrootd.unl.edu//store/mc/RunIISpring15DR74/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/00000" outDir="/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Producers/FatjetProducer/test" cmssw=$CMSSW_RELEASE_BASE
#cmsRun fatjetproducer_cfg.py dataset="sqtojjjj" m=200 maxEvents=100 inFile="sqtojjjj_200gev_7411_aodsim.root" inDir="file:." outDir="." cmssw=$CMSSW_RELEASE_BASE


#cmsRun fatjetproducer_cfg.py inFile="02071298-1415-E511-A143-0025905B8572.root" inDir="root://xrootd.unl.edu//store/mc/RunIISpring15DR74/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/00000" maxEvents=2 cmssw=$CMSSW_RELEASE_BASE outDir="." outFile="test.root"
#cmsRun fatjetproducer_cfg.py dataset="SqToJJJJ_250" maxEvents=2 cmssw=$CMSSW_RELEASE_BASE
cmsRun fatjetproducer_cfg.py dataset="QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" maxEvents=2 cmssw=$CMSSW_RELEASE_BASE
