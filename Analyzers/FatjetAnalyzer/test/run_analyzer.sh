# TESTING:
#cmsRun jetanalyzer_cfg.py maxEvents=2 dataset="QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"
#cmsRun contuplizer_cfg.py maxEvents=100 subprocess="sq200to4j"
#cmsRun contuplizer_cfg.py maxEvents=100 subprocess="qcdmg1000"
#cmsRun tuplizer_cfg.py maxEvents=10 cutPtFilter=400 subprocess="qcdmg500" generation="spring15" suffix="pt400" crab=False
cmsRun tuplizer_cfg.py maxEvents=10 cutPtFilter=400 subprocess="jethtd" generation="0706" suffix="pt400" crab=False data=False
#cmsRun tuplizer_cfg.py subprocess="sq200to4j" inFile="/store/user/tote/Sq150To4J/Sq150To4J/160504_092455/0000/sq150to4j_07040101_aodsim_56.root","/store/user/tote/Sq150To4J/Sq150To4J/160504_092455/0000/sq150to4j_07040101_aodsim_57.root"

# RUN OVER BACKGROUND
## QCD:
### Pythia
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8" &

#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp50.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp80.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp120.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp170.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp300.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp470.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp600.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp800.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp1000.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8" > logs/jetanalyzer_qcdp1400.log 2>&1 &

### MadGraph
#cmsRun jetanalyzer_cfg.py maxEvents=2 dataset="QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg100.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg200.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg300.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg500.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg700.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg1000.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg1500.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8" > logs/jetanalyzer_qcdmg2000.log 2>&1 &

## TTbar:
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" nevents=200000 &

# RUN OVER SqTo4J:
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="SqToJJJJ_150" > logs/jetanalyzer_sq150to4j.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="SqToJJJJ_200" > logs/jetanalyzer_sq200to4j.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="SqToJJJJ_250" > logs/jetanalyzer_sq250to4j.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="SqToJJJJ_300" > logs/jetanalyzer_sq300to4j.log 2>&1 &

# RUN OVER SqTO2J:
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="RPVStopStopToJets_UDD312_M-100-madgraph" > logs/jetanalyzer_sq100to2j.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="RPVStopStopToJets_UDD312_M-200-madgraph" > logs/jetanalyzer_sq200to2j.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="RPVStopStopToJets_UDD312_M-350-madgraph" > logs/jetanalyzer_sq350to2j.log 2>&1 &
#cmsRun jetanalyzer_cfg.py maxEvents=-1 dataset="RPVStopStopToJets_UDD312_M-800-madgraph" > logs/jetanalyzer_sq800to2j.log 2>&1 &

# RUN OVER LqToUE:
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQToUE_M-200_BetaOne_TuneCUETP8M1_13TeV-pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQToUE_M-250_BetaOne_TuneCUETP8M1_13TeV-pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQToUE_M-300_BetaOne_TuneCUETP8M1_13TeV-pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQToUE_M-450_BetaOne_TuneCUETP8M1_13TeV-pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQToUE_M-500_BetaOne_TuneCUETP8M1_13TeV-pythia8" &

# RUN OVER LqToTTau:
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQLQToTopTau_M-200_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQLQToTopTau_M-300_TuneCUETP8M1_13TeV_pythia8" &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="LQLQToTopTau_M-500_TuneCUETP8M1_13TeV_pythia8" &

# RUN OVER NON-EOS:
#cmsRun fatjetanalyzer_cfg.py maxEvents=1 inDir="/uscms_data/d3/tote/CMSSW_7_4_1_patch3/src/Producers/FatjetProducer/test" inFile="sqtojjjj_200gev_7413_fatjets.root" verbose=True cutPt=50
#cmsRun fatjetanalyzer_cfg.py maxEvents=1 inDir="/eos/uscms/store/user/tote/SqToJJJJ_200/sqtojjjj_200_fatjets/151021_155231/0000" inFile="sqtojjjj_200gev_7413_fatjets_1.root" verbose=True cutPt=50


# RUN OVER OLD FATJETS:
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_R8_ak_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ak8_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_R8_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ca8_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_R10_ak_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ak10_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_R10_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ca10_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_R12_ak_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ak12_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ca12_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_R14_ak_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ak14_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_200gev_chs_30000_7322_R14_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_200_ca14_ntuple.root" sigma=647.123 &
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=2 inDir="/eos/uscms/store/user/tote/data/fat/old" inFile="sqtojjjj_300gev_chs_50000_7322_fatjets.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="old_300_ca12_ntuple.root" sigma=85.6773 &


# JUNK:
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=1 inDir="file:." inFile="sqtojjjj_200gev_7413_fatjets.root" outDir="."
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=1 inDir="/eos/uscms/store/user/tote/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/qcd_pythia300to470_fatjets/150625_081543/0000" inFile="qcd_pythia300to470_7413_fatjets_10.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="ntuple_qcd_300_470.root"
#cmsRun fatjetanalyzer_cfg.py maxEvents=2 inType=1 inDir="/eos/uscms/store/user/tote/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/qcd_pythia800to1000_fatjets/150625_081623/0000" inFile="qcd_pythia800to1000_7413_fatjets_10.root"		# Seg fault
#cmsRun fatjetanalyzer_cfg.py maxEvents=2 inType=1 inDir="/eos/uscms/store/user/tote/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/qcd_pythia50to80_fatjets/150625_081438/0000" inFile="qcd_pythia50to80_7413_fatjets_10.root"		
