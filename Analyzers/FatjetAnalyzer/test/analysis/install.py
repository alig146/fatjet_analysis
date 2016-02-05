####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
from decortication import dataset
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
# /VARIABLES

# FUNCTIONS:
def make_load_trees():
	script_name = "load_trees.cc"
	
	# Make the script:
	script = "{\n"
	print "Making {} ...".format(script_name)
	dsd = dataset.get_datasets()
	for key, dss in dsd.iteritems():
		script += "\tTChain {}(\"analyzer/events\");\n".format(key)
		for ds in dss:
			script += "\t{}.Add(\"root://cmsxrootd.fnal.gov/{}\");\n".format(key, ds.tuple_path)
		script += "\t\n"
	script += "\tgROOT->SetBatch();\n"
	script += "}"
	
	# Output the script:
	with open(script_name, "w") as out:
		out.write(script)

def make_send_to_lxplus():
	script_name = "send_to_lxplus.sh"
	
	# Make the script:
	print "Making {} ...".format(script_name)
	script = "rsync -rlthv --progress --append-verify {}/*_tuple.root elhughes@lxplus.cern.ch:/afs/cern.ch/user/e/elhughes/work/data/fat\n".format(dataset.tuple_dir)
	script = "rsync -rlthv --progress --append-verify {}/*anatuples.root elhughes@lxplus.cern.ch:/afs/cern.ch/user/e/elhughes/work/data/fat\n".format(dataset.tuple_dir)
	
	# Output the script:
	with open(script_name, "w") as out:
		out.write(script)

def main():
	make_load_trees()
	make_send_to_lxplus()
	return True
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

