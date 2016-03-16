####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys
from subprocess import Popen, PIPE
from decortication import dataset
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
ds_generation = "spring15"
out_dir = "/uscms_data/d3/tote/temp"
# /VARIABLES

# FUNCTIONS:
def main():
	result = dataset.get_datasets(generation=ds_generation, set_info=True)
	result = {k: v for k, v in result.iteritems() if k == "qcdmg"}
	for process, dss in result.iteritems():
		print process
		
		files = []
		for ds in dss:
			if ds.tuple_path:
				files += ds.tuple_path
		
		n = 200
		groups = [files[i:i+n] for i in xrange(0, len(files), n)]
		if len(groups) > 500:
			print "ERROR: So many files!"
			sys.exit()
		if groups:
			for i, group in enumerate(groups):
				print i, len(group)
				if i > 21: #in [14, 19]:
					out_file = "{}/{}_tuple_{}.root".format(out_dir, process, i)
					cmd = "hadd -f {}".format(out_file)            # "-f" forces overwrite of output file.
					for f in group:
						if "_1405.root" not in f and "_1323.root" not in f and "_1443.root" not in f and "_1199.root" not in f and "_1464.root" not in f:
							cmd += " root://cmseos.fnal.gov/{}".format(f)
					raw_output = Popen([cmd], shell=True, stdout=PIPE, stderr=PIPE).communicate()
					if raw_output[1]:
						print raw_output[0]
						print raw_output[1]
						sys.exit()
					else:
						print out_file
		else:
			print "There were no tuple files to combine."
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

