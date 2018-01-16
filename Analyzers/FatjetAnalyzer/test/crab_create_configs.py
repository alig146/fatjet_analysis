import os, sys
from decortication import variables, dataset, production

# VARIABLES:
cut_pt_filter = 300
cut_eta_filter = 2.0
cut_smu_filter = False
# /VARIABLES

if __name__ == "__main__":
	# Prepare:
	a = variables.arguments()
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	suffix = "cut"
	if not cut_smu_filter: suffix += "pt{}eta{}".format(cut_pt_filter, int(cut_eta_filter*10) if cut_eta_filter > 0 else "x")
	else: suffix += "smu"
	
	# Write configs:
	miniaods = dataset.fetch_entries("miniaod", a.query)
	for miniaod in miniaods:
		print "[..] Making a configuration file for {}_{}.".format(miniaod.Name, suffix)
		config = production.get_crab_config(
			kind="tuple",
			miniaod=miniaod,
			cmssw_config="tuplizer_cfg.py",
			cut_pt_filter=cut_pt_filter,
			cut_eta_filter=cut_eta_filter,
			cut_smu_filter=cut_smu_filter,
			suffix=suffix,
			unitsper=50000,
			publish=False,
		)
		with open(path + "/{}_{}_cfg.py".format(miniaod.Name, suffix), "w") as out:
			out.write(config)
