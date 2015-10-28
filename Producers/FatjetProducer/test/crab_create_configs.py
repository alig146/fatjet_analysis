import os
from decortication import *
from truculence import *

# VARIABLES:
generation = "spring15"
# /VARIABLES

if __name__ == "__main__":
	# Prepare:
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	datasets = samples.get_datasets()
	
	# Signal:
	for d in datasets["sqtojjjj"]["miniaod"]:
		config = crab.get_config(dataset=d, cmssw_config="fatjetproducer_cfg.py")
		with open(path + "/sqtojjjj_{0}.py".format(d.m), "w") as out:
			out.write(config)
	
	# Background:
	for d in datasets["qcd"]["miniaod"]:
		if d.generation == generation:
			config = crab.get_config(dataset=d, cmssw_config="fatjetproducer_cfg.py")
			with open(path + "/qcd_pythia{0}to{1}.py".format(d.pts[0], d.pts[1]), "w") as out:
				out.write(config)

