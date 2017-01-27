import os, sys
from decortication import variables, dataset, production

# VARIABLES:
cut_pt_filter = 400
cut_eta_filter = 2.5		# Default.
# /VARIABLES

if __name__ == "__main__":
	# Prepare:
	a = variables.arguments()
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	
	# Write configs:
	miniaods = dataset.fetch_entries("miniaod", a.query)
	for miniaod in miniaods:
		print "Making a configuration file for {} ...".format(miniaod.Name)
		config = production.get_crab_config(
			kind="tuple",
			miniaod=miniaod,
			cmssw_config="tuplizer_cfg.py",
			cut_pt_filter=cut_pt_filter,
			cut_eta_filter=cut_eta_filter
		)
		with open(path + "/{}.py".format(miniaod.Name), "w") as out:
			out.write(config)
