####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
print "Importing packages ..."
import sys
from decortication import dataset, analyzer, variables
from truculence import condor, utilities
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
# /VARIABLES

# FUNCTIONS:
def main():
	## Arguments:
	eos = dataset.Site.get_dir("data").eos
	a = variables.arguments()
	args = a.args
	tstring = [piece for piece in args.dir.split("/") if piece][-1]
	
	njobs = condor.get_job_info(args.dir, v=True)
	if not njobs: return False
	
	print "[..] Checking jobs."
	log_results = condor.check_stderr_logs(args.dir)
	jobs_error = sorted(utilities.flatten_list([jobs_list for code, jobs_list in {k: v for k, v in log_results.items() if k > 0}.items()]))
#	queue_results = check_queue(miniaod)
	if eos:
		if 0 in log_results: print "{}/{} jobs completed successfully.".format(len(log_results[0]), njobs)
		if -1 in log_results: print "{}/{} jobs are unsubmitted or running.".format(len(log_results[-1]), njobs)
	else:
		keys = [key for key in log_results if key in [0, -1]]
		if keys:
			print "{}/{} jobs completed successfully.".format(sum([len(log_results[key]) for key in keys]), njobs)
#	print log_results
#	for key, nlist in queue_results.items(): print "\t{}/{} jobs are {}.".format(len(nlist), len(log_results[-1]), queue_codes[key] if key in queue_codes else "?")
	for code, jobs in {key: value for key, value in log_results. items() if key > 0}.items():
		print "{}/{} jobs have error code {}: {}".format(len(log_results[code]), njobs, code, condor.error_codes[code])
		print jobs
	
	# Resubmit stuff if toggle on (I use the "j" switch because I'm lazy.)
	if args.json:
		jobs_resubmit = jobs_error #+ jobs_unsub
		print "[..] Resubmitting {} jobs:".format(len(jobs_resubmit))
		job_files = ["analyzer_job{}_{}.jdl".format(n, tstring) for n in jobs_resubmit]
#		print job_files
		condor.tar_cmssw(args.dir)
		condor.submit_jobs_by_name(args.dir, job_files)
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
