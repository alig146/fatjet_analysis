####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import os, sys, subprocess
from condor_status import *
from truculence import utilities, condor
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
force_flag = 0
# :VARIABLES

# FUNCTIONS:
#def retar_cmssw(indir):
#	try:
#		subprocess.check_call("tar --exclude-caches-all -zcf " + indir + "/${CMSSW_VERSION}.tar.gz -C ${CMSSW_BASE}/.. ${CMSSW_VERSION}", shell=True)
#	except Exception as ex:
#		print ex
#		return False
#	return True

def submit_jobs(indir, n_jobs, prefix):
	os.chdir(indir)
	jlist = ["{}_{}.jdl".format(prefix, n) for n in n_jobs]
	FNULL = open(os.devnull, 'w')
	for j in jlist:
		subprocess.Popen(["condor_submit", j], stdout=FNULL, stderr=subprocess.STDOUT)
#		print ["condor_submit", j]

def main():
	a = variables.arguments()
	miniaods = dataset.fetch_entries("miniaod", a.query)
	suffix = a.suffix
	if not suffix:
		suffix = "cutpt400eta25"
	indate = a.args.dir
	if not indate:
		indate = "161118_062506"
	
	for miniaod in miniaods:
		indir = "condor_jobs/tuplizer/{}/{}_{}_{}".format(indate, miniaod.subprocess, miniaod.generation, suffix)
		print "\n[..] Remaking the CMSSW tarball."
		if not condor.tar_cmssw(indir):
			print "[!!] Something went wrong making the tarball."
			continue
		print "Resubmitting jobs for {}".format(miniaod.Name)
		print "[..] Figuring out what jobs to resubmit."
		results = check_stderr_logs(indir)
		results_queue = check_queue(miniaod)
		jobs_error = sorted(utilities.flatten_list([jobs_list for code, jobs_list in {k: v for k, v in results.items() if k > 0}.items()]))
		jobs_unsub_or_queue = sorted(utilities.flatten_list([jobs_list for code, jobs_list in {k: v for k, v in results.items() if k < 0}.items()]))
		jobs_queue = sorted(utilities.flatten_list([jobs_list for code, jobs_list in results_queue.items()]))
		jobs_unsub = sorted(list(set(jobs_unsub_or_queue).difference(set(jobs_queue))))
		jobs_resubmit = jobs_error + jobs_unsub
		print "[..] Resubmitting {} jobs:".format(len(jobs_resubmit))
		print jobs_resubmit
		submit_jobs(indir, jobs_resubmit, prefix="job_{}_{}_{}".format(miniaod.subprocess, miniaod.generation, suffix))
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

