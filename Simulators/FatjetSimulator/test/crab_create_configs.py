# Imports:
import os, sys
from decortication import dataset, production
from truculence import lhe
# :Imports

# Variables:
generation = "moriond17"
lhe_cut = "pt200"
# :Variables

# Functions:
# :Functions

if __name__ == "__main__":
	# Prepare:
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	
	# Write configs:
	samples = dataset.fetch_samples(category="sqto4j")
	
	for sample in samples:
		lhe_file = "{}_cut{}.lhe".format(sample.subprocess, lhe_cut)
		try:
			lhe_n = lhe.get_info("{}/{}".format(dataset.lhe_dir, lhe_file))["nevents"]
		except Exception as ex:
			print "Didn't make the config for {}:".format(sample.subprocess)
			print ex
			break
		config = production.get_crab_config(
			sample=sample,
			kind="miniaod",
			generation=generation,
			cmssw_config='fatjetsimulator_cfg.py',
			cmssw_params={
				"crab": True,
				"inFile": "root://cmseos.fnal.gov//store/user/tote/lhe/{}_cutpt200.lhe".format(sample.subprocess),
			},
			mass=sample.parameter1,
			units=lhe_n,
		)
		with open(path + "/{}_{}.py".format(sample.subprocess, generation), "w") as out:
			out.write(config)

