####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import os, sys, subprocess, re
from decortication import dataset, variables
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def list_logs(indir):
	results = {}
	logs = os.listdir(indir + "/logs")
	results["log"] = [f for f in logs if ".log" in f]
	results["stdout"] = [f for f in logs if ".stdout" in f]
	results["stderr"] = [f for f in logs if ".stderr" in f]
	return results

def check_stderr_logs(indir, logs_stderr=None):
	if not logs_stderr: logs_stderr = list_logs(indir)["stderr"]
	
	jobs_bad = []
	jobs_error = []
	jobs_unsubmitted = []
	for l in logs_stderr:
		path = indir + "/logs/" + l
		match = re.search("\w+_(\d+).stderr", l)
		njob = int(match.group(1))
		
		last = subprocess.check_output(["tail", "-n1", path])
		if not last.strip():
			last = subprocess.check_output(["tail", "-n2", path]).strip()
#		if not last:
#			print path
#			sys.exit()
		if "[100%][==================================================]" not in last:
			jobs_bad.append(njob)
		if "error" in last.lower() or "End Fatal Exception" in last:
			jobs_error.append(njob)
		if not last:		# Kludgy! If a non-empty log has two blank lines at the end, this triggers.
			jobs_unsubmitted.append(njob)
	return jobs_bad, jobs_error, jobs_unsubmitted

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
		print "\nStatusing {}".format(miniaod.Name)
		print "[..] Scanning the condor directory."
		jdls = [f for f in os.listdir(indir) if ".jdl" in f]
		njobs = len(jdls)
	
		log_dict = list_logs(indir)
		logs_log = log_dict["log"]
		logs_stdout = log_dict["stdout"]
		logs_stderr = log_dict["stderr"]
	
		print "[OK] Total jobs: {}".format(njobs)
		good = True
		if len(logs_log) != njobs:
			print "[!!] There are only {} .log logs".format(len(logs_log))
			good = False
		if len(logs_stdout) != njobs:
			print "[!!] There are only {} .stdout logs".format(len(logs_stdout))
			good = False
		if len(logs_stderr) != njobs:
			print "[!!] There are only {} .stderr logs".format(len(logs_stderr))
			good = False
		if good: print "[OK] All logs accounted for."
		else: sys.exit()
	
		print "[..] Checking jobs."
		jobs_bad, jobs_error, jobs_unsubmitted = check_stderr_logs(indir, logs_stderr)
		if jobs_bad:
			print "[!!] There were problems with {} jobs:".format(len(jobs_bad))
			print jobs_bad
			good = False
		if jobs_error:
			print "[!!] There were errors in {} of these jobs:".format(len(jobs_error))
			print jobs_error
			good = False
		if jobs_unsubmitted:
			print "[!!] There are {} unsubmitted jobs:".format(len(jobs_unsubmitted))
			print jobs_unsubmitted
			good = False
		if good:
			print "[OK] All jobs completed successfully."
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

