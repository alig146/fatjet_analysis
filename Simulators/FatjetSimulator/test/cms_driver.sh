# Step 0
cmsDriver.py Hadronizer_TuneCUETP8M1_13TeV_generic_LHE_pythia8_cff.py --step GEN,SIM --mc --eventcontent RAWSIM --era Run2_2016 --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --filein /store/user/tote/lhe/sq300to4j.lhe --filetype LHE --fileout file:step0.root --datatier GEN-SIM --no_exec -n 2 --python_filename step0_template_cfg.py

# Step 1
cmsDriver.py step1 --step DIGI,L1,DIGI2RAW,HLT:@frozen2016 --mc --eventcontent RAWSIM --era Run2_2016 --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --filein file:step0.root --fileout file:step1.root --datatier GEN-SIM-RAW --no_exec -n 2 --python_filename step1_template_cfg.py --pileup 2016_25ns_Moriond17MC_PoissonOOTPU --pileup_input dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer15GS-MCRUN2_71_V1_ext1-v1/GEN-SIM

# Step 01
cmsDriver.py Hadronizer_TuneCUETP8M1_13TeV_generic_LHE_pythia8_cff.py --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@frozen2016 --mc --eventcontent RAWSIM --era Run2_2016 --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --filein /store/user/tote/lhe/sq300to4j.lhe --filetype LHE --fileout file:step01.root --datatier GEN-SIM-RAW --no_exec -n 2 --python_filename step01_template_cfg.py --pileup 2016_25ns_Moriond17MC_PoissonOOTPU --pileup_input dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer15GS-MCRUN2_71_V1_ext1-v1/GEN-SIM

# Step 012
cmsDriver.py Hadronizer_TuneCUETP8M1_13TeV_generic_LHE_pythia8_cff.py --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@frozen2016,RAW2DIGI,L1Reco,RECO --mc --eventcontent AODSIM --era Run2_2016 --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --filein /store/user/tote/lhe/sq300to4j.lhe --filetype LHE --fileout file:step0123.root --datatier AODSIM --no_exec -n 2 --python_filename step012_template_cfg.py --pileup 2016_25ns_Moriond17MC_PoissonOOTPU --pileup_input dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer15GS-MCRUN2_71_V1_ext1-v1/GEN-SIM

# Step 0123 - I can't get this to work.
cmsDriver.py Hadronizer_TuneCUETP8M1_13TeV_generic_LHE_pythia8_cff.py --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@frozen2016,RAW2DIGI,L1Reco,RECO,EI,PAT --mc --eventcontent MINIAODSIM --era Run2_2016 --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --filein /store/user/tote/lhe/sq300to4j.lhe --filetype LHE --fileout file:step0123.root --datatier MINIAODSIM --no_exec -n 2 --python_filename step0123_template_cfg.py --pileup 2016_25ns_Moriond17MC_PoissonOOTPU --pileup_input dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer15GS-MCRUN2_71_V1_ext1-v1/GEN-SIM --runUnscheduled

# Step 3
cmsDriver.py Hadronizer_TuneCUETP8M1_13TeV_generic_LHE_pythia8_cff.py --step PAT --mc --eventcontent MINIAODSIM --era Run2_2016 --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --filein file:step012.root --fileout file:step3.root --datatier MINIAODSIM --no_exec -n 2 --python_filename step3_template_cfg.py --runUnscheduled
