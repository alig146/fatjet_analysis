import os
from decortication import *
from truculence import *

# VARIABLES:
generation = "spring15"
units = 20000000
# /VARIABLES

if __name__ == "__main__":
	# Prepare:
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	datasets = samples.get_datasets()
	
	# Signal:
	for d in datasets["sqtojjjj"]["miniaod"]:
		config = crab.get_config(dataset=d, cmssw_config="fatjetproducer_cfg.py", units=units)
		with open(path + "/sqtojjjj_{0}.py".format(d.m), "w") as out:
			out.write(config)
	
	# Background:
	## QCD:
	### Pythia 8:
	for d in datasets["qcd"]["miniaod"]:
		if d.generation == generation:
			config = crab.get_config(dataset=d, cmssw_config="fatjetproducer_cfg.py", units=units)
			with open(path + "/qcd_pt{0}to{1}.py".format(d.pts[0], d.pts[1]), "w") as out:
				out.write(config)
	### MadGraph:
	for d in datasets["qcdmg"]["miniaod"]:
		if d.generation == generation:
			config = crab.get_config(dataset=d, cmssw_config="fatjetproducer_cfg.py", units=units)
			with open(path + "/qcdmg_ht{0}to{1}.py".format(d.hts[0], d.hts[1]), "w") as out:
				out.write(config)

