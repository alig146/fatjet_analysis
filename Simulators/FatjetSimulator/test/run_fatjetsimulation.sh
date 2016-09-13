#cmsRun fatjetsimulation_step0_cfg.py maxEvents=5 dataset="sqtojjjj" m=300 outDir="." inFile="sqtojjjj_300.lhe"
#cmsRun fatjetsimulation_cfg.py maxEvents=5 dataset="sqtojjjj" m=300 outDir="." inFile="sqtojjjj_300.lhe"
#cmsRun fatjetsimulation_cfg.py maxEvents=2 dataset="sqtojjjj" m=200 outDir="." inDir="file:/uscms_data/d3/tote/CMSSW_7_4_1_patch1/src/Simulation/FatjetSimulation/test" inFile="test.lhe"

cmsRun fatjetsimulator_cfg.py maxEvents=2 subprocess="sq200to4j" inFile="sq200to4j_cutpt200.lhe"
