# Imports:
import os, sys
from decortication import dataset, production, site
from truculence import lhe
# :Imports

# Functions:
# :Functions

if __name__ == "__main__":
	# Prepare:
	Site = site.get_site()
	dir_lhe = Site.get_dir("lhe")
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	
	# Write configs:
	miniaods = dataset.fetch_entries("miniaod", {"category": "sqto4j", "generation": ["moriond17", "moriond17cutht700"]})
	
#	for miniaod in miniaods: print miniaod.generation, miniaod.aod
	
	for miniaod in miniaods:
		sample = miniaod.get_sample()
		# Make MiniAOD CRAB config file:
		config = production.get_crab_config(
			sample=sample,
			kind="miniaod",
			dataset_in=miniaod.aod,
			generation=miniaod.generation,
			dataset_out=sample.name,
			instance="phys03",
			cmssw_config='aod_to_miniaod_cfg.py',
#			cmssw_params={
#				"inFile": "root://cmseos.fnal.gov//store/user/tote/lhe/{}".format(lhe_file),
#			},
			unitsper=1000
		)
		with open(path + "/{}_{}_miniaod.py".format(miniaod.subprocess, miniaod.generation), "w") as out:
			out.write(config)

