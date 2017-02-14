####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import os, sys, subprocess
from condor_status import *
from truculence import utilities
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
force_flag = 0
# :VARIABLES

# FUNCTIONS:
def retar_cmssw(indir):
	try:
		subprocess.check_call("tar --exclude-caches-all -zcf " + indir + "/${CMSSW_VERSION}.tar.gz -C ${CMSSW_BASE}/.. ${CMSSW_VERSION}", shell=True)
	except Exception as ex:
		print ex
		return False
	return True

def submit_jobs(indir, n_jobs):
	os.chdir(indir)
	jlist = ["job_{}.jdl".format(n) for n in n_jobs]
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
		indir = "/uscms/home/tote/8_0_20/Analyzers/FatjetAnalyzer/test/condor_jobs/tuplizer/{}/{}_{}_{}".format(indate, miniaod.subprocess, miniaod.generation, suffix)
		print "\n[..] Remaking the CMSSW tarball."
		if not retar_cmssw(indir):
			print "[!!] Something went wrong."
			return False
		print "Resubmitting jobs for {}".format(miniaod.Name)
		print "[..] Figuring out what jobs to resubmit."
		results = check_stderr_logs(indir)
		jobs_error = sorted(utilities.flatten_list([jobs_list for code, jobs_list in {k: v for k, v in results.items() if k > 0}.items()]))
		jobs_unsuborrun = sorted(utilities.flatten_list([jobs_list for code, jobs_list in {k: v for k, v in results.items() if k < 0}.items()]))
		jobs_resubmit = jobs_error + jobs_unsuborrun if force_flag else jobs_error
		print "[..] Resubmitting {} jobs:".format(len(jobs_resubmit))
		print jobs_resubmit
		submit_jobs(indir, jobs_resubmit)
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

