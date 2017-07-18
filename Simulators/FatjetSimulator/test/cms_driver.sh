
cmsDriver.py Hadronizer_TuneCUETP8M1_13TeV_generic_LHE_pythia8_cff.py --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT,RAW2DIGI,RECO --mc --eventcontent AODSIM --era Run2_25ns --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --filein file:sg300to5j.lhe --filetype LHE --fileout file:aod.root --datatier AODSIM --no_exec -n 2 --python_filename aod_template_cfg.py --pileup 2016_25ns_Moriond17MC_PoissonOOTPU

cmsDriver.py --step PAT --mc --eventcontent MINIAODSIM --era Run2_25ns --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v8 --runUnscheduled --filein file:aod.root --fileout file:miniaod.root --datatier MINIAODSIM --no_exec -n 2 --python_filename aod_to_miniaod_template_cfg.py
