import os
from decortication import dataset, production

# VARIABLES:
cut_pt_filter = 400		# The eta 2.5 cut is the default.
# /VARIABLES

if __name__ == "__main__":
	# Prepare:
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	
	# Write configs:
	samples = dataset.fetch_samples()
	for sample in samples:
		for miniaod in sample.miniaods:
			print "Making a configuration file for {} ...".format(miniaod.Name)
			config = production.get_crab_config(
				kind="tuple",
				miniaod=miniaod,
				cmssw_config="tuplizer_cfg.py",
				cut_pt_filter=cut_pt_filter
			)
			with open(path + "/{}.py".format(miniaod.Name), "w") as out:
				out.write(config)
