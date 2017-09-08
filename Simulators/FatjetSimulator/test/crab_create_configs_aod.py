# Imports:
import os, sys
from decortication import dataset, production, site
from truculence import lhe
# :Imports

# Variables:
generation = "moriond17"
lhe_cut = "cutht700"
#lhe_cut = ""
# :Variables

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
	samples = dataset.fetch_samples(category="sqto4j")
	
	for sample in samples:
		dataset_name = "Sq{}To4J".format(int(sample.parameter1))
		lhe_file = sample.subprocess
		if lhe_cut: lhe_file += "_" + lhe_cut
		lhe_file += ".lhe"
		# Get info from LHE file:
		lhe_info = lhe.get_info("{}/{}".format(dir_lhe.path, lhe_file), v=True)
		if lhe_info: lhe_n = lhe_info["nevents"]
		else:
			print "Didn't make the config for {}:".format(sample.subprocess)
			break
		# Make AOD CRAB config file:
		config = production.get_crab_config(
			subprocess=lhe_file.replace(".lhe", ""),
			sample=sample,
			kind="aod",
			generation=generation,
			dataset_out=dataset_name,
			cmssw_config='aod_cfg.py',
			cmssw_params={
				"inFile": "root://cmseos.fnal.gov//store/user/tote/lhe/{}".format(lhe_file),
			},
			units=lhe_n,
			unitsper=100
		)
		with open(path + "/{}_{}_aod.py".format(lhe_file.replace(".lhe", ""), generation), "w") as out:
			out.write(config)
#		# Make MiniAOD CRAB config file:
#		config = production.get_crab_config(
#			sample=sample,
#			kind="miniaod",
#			dataset_in="",
#			generation=generation,
#			dataset_out=dataset_name,
#			instance="phys03",
#			cmssw_config='aod_to_miniaod_cfg.py',
##			cmssw_params={
##				"inFile": "root://cmseos.fnal.gov//store/user/tote/lhe/{}".format(lhe_file),
##			},
#			units=lhe_n,
#			unitsper=100
#		)
#		with open(path + "/{}_{}_miniaod.py".format(sample.subprocess, generation), "w") as out:
#			out.write(config)

