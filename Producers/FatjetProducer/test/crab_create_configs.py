import os
from decortication import dataset, production

# VARIABLES:
generation = "spring15"
cut_pt = 400
# /VARIABLES

if __name__ == "__main__":
	# Prepare:
	path = "crab_configs"
	if not os.path.exists(path):
		os.makedirs(path)
	dss_dict = dataset.get_datasets(set_info=False, generation=generation)
	
	# Signal:
	## SqTo4J
	masses = [150, 200, 250, 300]
	for mass in masses:
		subprocess = "sq{}to4j".format(mass)
		dss = dss_dict[subprocess]
		for ds in dss:
			config = production.get_crab_config(dataset=ds, cut_pt=cut_pt)
			with open(path + "/{}.py".format(subprocess), "w") as out:
				out.write(config)
	## SqTo2J
	masses = [100, 200, 350, 800]
	for mass in masses:
		subprocess = "sq{}to2j".format(mass)
		dss = dss_dict[subprocess]
		for ds in dss:
			config = production.get_crab_config(dataset=ds, cut_pt=cut_pt)
			with open(path + "/{}.py".format(subprocess), "w") as out:
				out.write(config)
	
	# Background:
	## QCD:
	### Pythia 8:
	process = "qcdp"
	dss = dss_dict[process]
	for ds in dss:
		subprocess = ds.subprocess
		config = production.get_crab_config(dataset=ds, cut_pt=cut_pt)
		with open(path + "/{}.py".format(subprocess), "w") as out:
			out.write(config)
	## MadGraph:
	process = "qcdmg"
	dss = dss_dict[process]
	for ds in dss:
		subprocess = ds.subprocess
		config = production.get_crab_config(dataset=ds, cut_pt=cut_pt)
		with open(path + "/{}.py".format(subprocess), "w") as out:
			out.write(config)
