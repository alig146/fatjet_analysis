# TESTING:
#cmsRun jetanalyzer_cfg.py maxEvents=2 dataset="QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"
#cmsRun contuplizer_cfg.py maxEvents=100 subprocess="sq200to4j"
#cmsRun tuplizer_cfg.py maxEvents=100 cutPtFilter=400 subprocess="ttbar" generation="spring16" suffix="cutpt400eta25" data=False crab=False
cmsRun tuplizer_cfg.py maxEvents=100 cutPtFilter=400 subprocess="qcdmg2000" generation="fall15" suffix="cutpt400eta25" data=False crab=False
#cmsRun tuplizer_cfg.py maxEvents=100 cutPtFilter=0 cutEtaFilter=-1 subprocess="smu16b" generation="feb17" suffix="cutpt400eta25" data=True crab=False mask="https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt" inFile="/store/data/Run2016B/SingleMuon/MINIAOD/03Feb2017_ver2-v2/100000/52956F28-A1EC-E611-B034-0CC47A7C35D2.root"
#cmsRun tuplizer_cfg.py maxEvents=10000 cutPtFilter=400 subprocess="jetht16b" generation="0800" suffix="cutpt400eta25" data=True crab=False #inFile="file:/uscms_data/d3/tote/data/examples/maod_data_0800_1000.root"
#cmsRun tuplizer_cfg.py maxEvents=10 cutPtFilter=400 subprocess="sq200to4j" generation="0706" suffix="pt400" crab=False inFile="root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_1.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_2.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_3.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_4.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_5.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_6.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_7.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_8.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_9.root,root://cmseos.fnal.gov//store/user/elhughes/Sq200To4J/sq200to4j-miniaod/161129_000557/0000/miniaod_sq200to4j_0706_10.root"
#cmsRun tuplizer_cfg.py maxEvents=100 cutPtFilter=400 subprocess="ttbar" generation="fall15" suffix="pt400" crab=False inFile="file:/uscms_data/d3/tote/data/examples/maod_mc_fall15_1000.root"
#cmsRun tuplizer_cfg.py maxEvents=1000 cutPtFilter=400 subprocess="sq200to4j" generation="0706" suffix="pt400" crab=False
#cmsRun tuplizer_cfg.py maxEvents=1000 cutPtFilter=400 subprocess="jetht16d" generation="0800" suffix="pt400" crab=False data=True # mask="https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON.txt"
#cmsRun tuplizer_cfg.py maxEvents=2 cutPtFilter=400 subprocess="sq150to4j" generation="0706" suffix="pt400" crab=False data=False
#cmsRun tuplizer_cfg.py subprocess="sq200to4j" inFile="/store/user/tote/Sq150To4J/Sq150To4J/160504_092455/0000/sq150to4j_07040101_aodsim_56.root","/store/user/tote/Sq150To4J/Sq150To4J/160504_092455/0000/sq150to4j_07040101_aodsim_57.root"

# RUN OVER BACKGROUND
## QCD:
### Pythia:

### MadGraph

#cmsRun tuplizer_cfg.py maxEvents=10000 cutPtFilter=400 subprocess="qcdmg500" generation="fall15" suffix="pt400" crab=False > logs/tuplizer_qcdmg500.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=10000 cutPtFilter=400 subprocess="qcdmg700" generation="fall15" suffix="pt400" crab=False > logs/tuplizer_qcdmg700.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=10000 cutPtFilter=400 subprocess="qcdmg1000" generation="fall15" suffix="pt400" crab=False > logs/tuplizer_qcdmg1000.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=10000 cutPtFilter=400 subprocess="qcdmg1500" generation="fall15" suffix="pt400" crab=False > logs/tuplizer_qcdmg1500.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=10000 cutPtFilter=400 subprocess="qcdmg2000" generation="fall15" suffix="pt400" crab=False > logs/tuplizer_qcdmg2000.log 2>&1 &

## TTbar:
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 dataset="TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" nevents=200000 &

### JetHT

#cmsRun tuplizer_cfg.py maxEvents=50000 cutPtFilter=400 subprocess="jethtd" generation="0706" suffix="pt400" crab=False data=True> logs/tuplizer_jethtd.log 2>&1 &

## RUN OVER SqTo4J:		(161115)
#cmsRun tuplizer_cfg.py maxEvents=30000 cutPtFilter=400 subprocess="sq100to4j" generation="0706" suffix="pt400" crab=False> logs/tuplizer_sq100to4j.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=30000 cutPtFilter=400 subprocess="sq150to4j" generation="0706" suffix="pt400" crab=False> logs/tuplizer_sq150to4j.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=30000 cutPtFilter=400 subprocess="sq200to4j" generation="0706" suffix="pt400" crab=False> logs/tuplizer_sq200to4j.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=30000 cutPtFilter=400 subprocess="sq250to4j" generation="0706" suffix="pt400" crab=False> logs/tuplizer_sq250to4j.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=30000 cutPtFilter=400 subprocess="sq300to4j" generation="0706" suffix="pt400" crab=False> logs/tuplizer_sq300to4j.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=30000 cutPtFilter=400 subprocess="sq400to4j" generation="0706" suffix="pt400" crab=False> logs/tuplizer_sq400to4j.log 2>&1 &
#cmsRun tuplizer_cfg.py maxEvents=30000 cutPtFilter=400 subprocess="sq500to4j" generation="0706" suffix="pt400" crab=False> logs/tuplizer_sq500to4j.log 2>&1 &


# JUNK:
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=1 inDir="file:." inFile="sqtojjjj_200gev_7413_fatjets.root" outDir="."
#cmsRun fatjetanalyzer_cfg.py maxEvents=-1 inType=1 inDir="/eos/uscms/store/user/tote/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/qcd_pythia300to470_fatjets/150625_081543/0000" inFile="qcd_pythia300to470_7413_fatjets_10.root" outDir="/eos/uscms/store/user/tote/data/fat" outFile="ntuple_qcd_300_470.root"
#cmsRun fatjetanalyzer_cfg.py maxEvents=2 inType=1 inDir="/eos/uscms/store/user/tote/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/qcd_pythia800to1000_fatjets/150625_081623/0000" inFile="qcd_pythia800to1000_7413_fatjets_10.root"		# Seg fault
#cmsRun fatjetanalyzer_cfg.py maxEvents=2 inType=1 inDir="/eos/uscms/store/user/tote/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/qcd_pythia50to80_fatjets/150625_081438/0000" inFile="qcd_pythia50to80_7413_fatjets_10.root"		
