####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import os
from decortication import dataset, eos
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
dir_local = "/uscms_data/d3/tote/temp"
dir_eos = dataset.data_dir
# /VARIABLES

# FUNCTIONS:
def main():
	files = [f for f in os.listdir(dir_local) if ".root" in f]
	tuples = [f for f in files if "_tuple" in f]
	tuples = [t for t in tuples if "QCD" not in t]
	if tuples:
		for t in tuples:
			name = t.split("_tuple")[0]
			ds = dataset.get_datasets(name=name).values()[0][0]
			if ds:
				dir_store = "{}/{}/{}_tuples".format(dir_eos, ds.name, ds.subprocess)
				eos.mkdir(dir_store)
				eos.cp("{}/{}".format(dir_local, t), dir_store)
				print "{} went into {}".format(t, dir_store)
			else:
				print "I don't know what DS this belongs to."
	else:
		print "There aren't any tuples to organize in {}.".format(dir_local)
	return True
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

