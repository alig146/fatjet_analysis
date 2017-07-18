####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
print "Importing packages ..."
import sys
from decortication import dataset, analyzer, variables
from truculence import condor
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
# /VARIABLES

# FUNCTIONS:
def main():
	## Arguments:
	a = variables.arguments()
	args = a.args
	tuples = dataset.fetch_entries("tuple", a.query)
	tuples = dataset.sort_datasets(tuples, collapse=True)
	tuples = {key.split("_")[0]: value for key, value in tuples.items()}		# Kludge to keep only "process" as the key.
	
	if not tuples:
		print "ERROR: Your query resulted in no tuples:"
		print a.query
		sys.exit()
	
	## Print an introduction:
	print "Making anatuplizer jobs for the following tuples:"
	if isinstance(tuples, dict):
		for key, tups in tuples.items():
			print "\t* {}:".format(key)
			for tup in tups:
				print "\t\t* {}".format(tup)
	else:
		print "\t{}".format(tuples)
	
	ana = analyzer.analyzer(tuples, v=args.verbose, count=False)
	path = ana.create_jobs(cmd="python anatuplizer.py --condor %%N%% -f %%FILE%% -p %%PROCESS%%", input_files=["anatuplizer.py", "anatuple.yaml"])
	condor.tar_cmssw(path)
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
