####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import os, sys, re
from subprocess import Popen, PIPE, check_output
from decortication import dataset, variables
from truculence import utilities, condor
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
error_codes = {
	-1: "empty",
	0: "job completed successfully",
	1: "unknown",
	2: "error detected",
	3: "last two lines empty",
}
queue_codes = {
	1: "idle",
	2: "running",
}
# :VARIABLES

# FUNCTIONS:
def list_logs(indir):
	results = {}
	logs = os.listdir(indir + "/logs")
	results["log"] = [f for f in logs if ".log" in f]
	results["stdout"] = [f for f in logs if ".stdout" in f]
	results["stderr"] = [f for f in logs if ".stderr" in f]
	return results


#def parse_job_name(name):
#	name = name.split("/")[-1]
#	result = {}
#	info, result["ext"] = name.split(".")
#	name_pieces = info.split("_")[1:]
#	if (len(name_pieces) == 1): result["n"] = name_pieces[0]
#	elif (len(name_pieces) == 4): result["subprocess"], result["generation"], result["suffix"], result["n"] = name_pieces
#	result["n"] = int(result["n"])
#	return result


def analyze_log_stderr(path):
	n_lines = utilities.wcl(path)
	if n_lines == 0: return {"code": -1, "n": n_lines}
	last = check_output(["tail", "-n1", path]).strip()
	if not last: last = check_output(["tail", "-n1", path]).strip()
	
	if not last: return {"code": 3, "n": n_lines}
	if "[100%][==================================================]" in last: return {"code": 0, "n": n_lines}
	if "error" in last.lower() or "End Fatal Exception" in last or "system" in last.lower() or "%MSG" in last: return {"code": 2, "n": n_lines}
	else: return {"code": 1, "n": n_lines}


def check_stderr_logs(indir, logs_stderr=None):
	if not logs_stderr: logs_stderr = list_logs(indir)["stderr"]
	
	results = {}
	for l in logs_stderr:
		path = indir + "/logs/" + l
		match = re.search("[a-zA-Z0-9_]+_(\d+).stderr", l)
		njob = int(match.group(1))
		
		stderr_analysis = analyze_log_stderr(path)
		code = stderr_analysis["code"]
		if code not in results: results[code] = []
		results[code].append(njob)
	
	return results


def check_queue(miniaod, user="tote"):
	results = {}
	
	raw_output = Popen(['condor_q -submitter {} -format "%s " jobstatus -format "%s\\n" cmd'.format(user)], shell=True, stdout=PIPE, stderr=PIPE).communicate()
	if raw_output[1]: return False
	lines = raw_output[0].split("\n")
	for line in lines:
		if not line: continue
		pieces = line.split()
		status = int(pieces[0])
#		if status == "<": status = "R"
		info = condor.parse_job_name(pieces[1])
		if "subprocess" in info and "generation" in info:
			if info["subprocess"] == miniaod.subprocess and info["generation"] == miniaod.generation:
				if status not in results: results[status] = []
				results[status].append(info["n"])
	return results

def main():
	a = variables.arguments()
	miniaods = dataset.fetch_entries("miniaod", a.query)
	suffix = a.suffix
	if not suffix:
		suffix = "cutpt400eta25"
	indate = a.args.dir
	if not indate:
		indate = "170126_203418"
	
	for miniaod in miniaods:
		indir = "condor_jobs/tuplizer/{}/{}_{}_{}".format(indate, miniaod.subprocess, miniaod.generation, suffix)
		print "\nStatusing {}".format(miniaod.Name)
		print "[..] Scanning the condor directory."
		try:
			jdls = [f for f in os.listdir(indir) if ".jdl" in f]
		except Exception as ex:
			print ex
			continue
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
		log_results = check_stderr_logs(indir, logs_stderr)
		queue_results = check_queue(miniaod)
#		print log_results
#		print
#		print queue_results
#		print queue_results[1]
#		print len(queue_results[1])
		
		if 0 in log_results: print "{}/{} jobs completed successfully.".format(len(log_results[0]), njobs)
		if -1 in log_results:
			print "{}/{} jobs are unsubmitted or running.".format(len(log_results[-1]), njobs)
			for key, nlist in queue_results.items(): print "\t{}/{} jobs are {}.".format(len(nlist), len(log_results[-1]), queue_codes[key] if key in queue_codes else "?")
		for code, jobs in {key: value for key, value in log_results. items() if key > 0}.items():
			print "{}/{} jobs have error code {}: {}".format(len(log_results[code]), njobs, code, error_codes[code])
			print jobs
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

