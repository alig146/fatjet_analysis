####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import os, sys, subprocess
from condor_status import *
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
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
		suffix = "cutpt400"
	indate = a.args.dir
	if not indate:
		indate = "161118_062506"
	
	for miniaod in miniaods:
		indir = "/uscms/home/tote/8_0_20/Analyzers/FatjetAnalyzer/test/condor_jobs/tuplizer/{}/{}_{}_{}".format(indate, miniaod.subprocess, miniaod.generation, suffix)
		print "\nResubmitting jobs for {}".format(miniaod.Name)
		print "[..] Figuring out what jobs to resubmit."
		jobs_bad, jobs_error, jobs_unsubmitted = check_stderr_logs(indir)
		jobs_resubmit = list(set(jobs_bad + jobs_unsubmitted))
		print "[..] Resubmitting {} jobs:".format(len(jobs_resubmit))
		print jobs_resubmit
		submit_jobs(indir, jobs_resubmit)
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

