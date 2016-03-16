####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys
from decortication import dataset
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
dir_local = "/uscms_data/d3/tote/temp"
cmslpc = True
# /VARIABLES

# FUNCTIONS:
def make_load_trees():
	script_name = "load_trees.cc"
	
	# Make the script:
	script = "{\n"
	print "Making {} ...".format(script_name)
	dsd = dataset.get_datasets(set_info=True)
	for process, dss in dsd.iteritems():
		script += "\tTChain* {} = new TChain(\"analyzer/events\");\n".format(process)
		for ds in dss:
			if ds.analyze:
				for f in ds.tuple_path:
					script += "\t{}->Add(\"{}{}\");\n".format(process, "root://cmsxrootd.fnal.gov/" if cmslpc else "", f)
		script += "\t\n"
	script += "//\tgROOT->SetBatch();\n"
	script += "}"
	
	# Output the script:
	with open(script_name, "w") as out:
		out.write(script)


def make_send_to_lxplus():
	script_name = "send_to_lxplus.sh"
	script = ""
	
	# Make the script:
	print "Making {} ...".format(script_name)
	dsd = dataset.get_datasets(category=["sqto2j", "sqto4j"])
#	print dsd
	for process, dss in dsd.iteritems():
		for ds in dss:
#			print ds.name, ds.tuple_path
			script += "xrdcp -f root://cmseos.fnal.gov/{} {} &&\n".format(ds.tuple_path, dir_local)
			script += "rsync -rlthv --progress {}/{} elhughes@lxplus.cern.ch:/afs/cern.ch/user/e/elhughes/work/data/fat &&\n".format(dir_local, ds.tuple_path.split("/")[-1])
			script += "rm {}/{} &&\n".format(dir_local, ds.tuple_path.split("/")[-1])
	script += "ls {}".format(dir_local)

#	script = "rsync -rlthv --progress --append-verify {}/*_tuple.root elhughes@lxplus.cern.ch:/afs/cern.ch/user/e/elhughes/work/data/fat\n".format(dataset.tuple_dir)
#	script = "rsync -rlthv --progress --append-verify {}/*anatuples.root elhughes@lxplus.cern.ch:/afs/cern.ch/user/e/elhughes/work/data/fat\n".format(dataset.tuple_dir)
	
	# Output the script:
	with open(script_name, "w") as out:
		out.write(script)

def main():
	make_load_trees()
#	make_send_to_lxplus()
	return True
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

