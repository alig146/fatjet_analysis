####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import os
import argparse       # For commandline options
from decortication import dataset, variables, site
from truculence import utilities, cmssw, condor
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
cut_pt_filter = 300
cut_eta_filter = 2.0
#cut_pt_filter = 0
#cut_eta_filter = -1
cut_smu_filter = False
n_per = 5000
Site = site.get_site()
Dir = Site.get_dir("data")
# /VARIABLES

# FUNCTIONS:
def main():
	# Arguments:
	a = variables.arguments()
	miniaods = dataset.fetch_entries("miniaod", a.query)
	if not miniaods:
		print "[!!] There were no miniaods to run over based on your query:"
		print a.query
		return False
	tstring = utilities.time_string()[:-4]
	suffix = "cutpt{}".format(cut_pt_filter)
	if cut_eta_filter < 0: suffix += "etax"
	else: suffix += "eta{}".format(int(cut_eta_filter*10))
	cmssw_version = cmssw.get_version(parsed=False)
#	condor.unclean()		# A weird bug makes this necessary if there's more than one sample. (WHY?!)
	
	for miniaod in miniaods:
		print "Making condor setup for {} ...".format(miniaod.Name)
		sample = miniaod.get_sample()

		# Create groups of input files:
		groups = []
		group = []
		n_group = 0
	#	print miniaod.ns
		for i, n in enumerate(miniaod.ns):
			n_group += n
			fname = miniaod.files[i]
#			if not Dir.eos: fname = "file:" + fname
			if fname[:7] != "/store/": fname = "file:" + fname
			group.append(fname)
			if (n_group >= n_per) or (i == len(miniaod.ns) - 1):
				groups.append(group)
				group = []
				n_group = 0
		print "\tCreating {} jobs ...".format(len(groups))
	
		# Prepare directories:
		path = "condor_jobs/tuplizer/{}/{}_{}_{}".format(tstring, miniaod.subprocess, miniaod.generation, suffix)
		log_path = path + "/logs"
		if not os.path.exists(path):
			os.makedirs(path)
		if not os.path.exists(log_path):
			os.makedirs(log_path)
		# Define an output path (output is moved here at the end of each job):
		out_path = Dir.path + "/{}/tuple_{}_{}_{}/{}".format(sample.name, miniaod.subprocess, miniaod.generation, suffix, tstring)
	
		# Create job scripts:
		for i, group in enumerate(groups):
			job_name = "job_{}_{}_{}_{}".format(miniaod.subprocess, miniaod.generation, suffix, i + 1)
			job_script = "#!/bin/bash\n"
			job_script += "\n"
			job_script += "# Untar CMSSW area:\n"
			job_script += "tar -xhzf {}.tar.gz\n".format(cmssw_version)
			job_script += "cd {}/src/Analyzers/FatjetAnalyzer/test\n".format(cmssw_version)
			job_script += "\n"
			job_script += "# Setup CMSSW:\n"
			job_script += "source /cvmfs/cms.cern.ch/cmsset_default.sh\n"
			job_script += "scramv1 b ProjectRename\n"
			job_script += "eval `scramv1 runtime -sh`		#cmsenv\n"
			job_script += "\n"
			job_script += "# Run CMSSW:\n"
			list_str = ",".join(['"{}"'.format(g) for g in group])
			out_file = "tuple_{}_{}_{}_{}.root".format(miniaod.subprocess, miniaod.generation, suffix, i + 1)
			job_script += 'cmsRun tuplizer_cfg.py subprocess="{}" generation="{}" cutPtFilter={} cutEtaFilter={} cutSmuFilter={} outDir="." outFile="{}" inFile={}'.format(miniaod.subprocess, miniaod.generation, cut_pt_filter, cut_eta_filter, cut_smu_filter, out_file, list_str)
			if sample.data:
				job_script += ' data=True'.format(sample.data)
			if miniaod.mask:
				job_script += ' mask="{}"'.format(miniaod.mask)
			job_script += " &&\n"
			if Dir.eos:
				job_script += "xrdcp -f {} root://cmseos.fnal.gov/{} &&\n".format(out_file, out_path)
				job_script += "rm {}\n".format(out_file)
			else: job_script += "mv {} {}\n".format(out_file, out_path)
			with open("{}/{}.sh".format(path, job_name), "w") as out:
				out.write(job_script)
	
		# Create condor configs:
		for i, group in enumerate(groups):
			job_name = "job_{}_{}_{}_{}".format(miniaod.subprocess, miniaod.generation, suffix, i + 1)
			job_config = "universe = vanilla\n"
			job_config += "Executable = {}.sh\n".format(job_name)
			job_config += "Should_Transfer_Files = YES\n"
			job_config += "WhenToTransferOutput = ON_EXIT\n"
			job_config += "Transfer_Input_Files = {}.tar.gz\n".format(cmssw_version)
			job_config += "Transfer_Output_Files = \"\"\n"
			job_config += "Output = logs/{}.stdout\n".format(job_name)
			job_config += "Error = logs/{}.stderr\n".format(job_name)
			job_config += "Log = logs/{}.log\n".format(job_name)
			if Site.name == "cmslpc":
				job_config += "notify_user = ${LOGNAME}@FNAL.GOV\n"
				job_config += "x509userproxy = $ENV(X509_USER_PROXY)\n"
			elif Site.name == "hexcms":
				job_config += "x509userproxy = $ENV(HOME)/myproxy\n"
			job_config += "request_memory = 3500\n"
			job_config += "Queue 1\n"
		
			with open("{}/{}.jdl".format(path, job_name), "w") as out:
				out.write(job_config)
	
	
		# Create run script:
		run_script = "# Update cache info:\n"
		run_script += "bash $CMSSW_BASE/src/Deracination/Straphanger/test/decortication/scripts/cache.sh\n"
		run_script += "\n"
		run_script += "# Grid proxy existence & expiration check:\n"
		run_script += "PCHECK=`voms-proxy-info -timeleft`\n"
		run_script += "if [[ ($? -ne 0) || (\"$PCHECK\" -eq 0) ]]; then\n"
		if Site.name == "hexcms": run_script += "\tvoms-proxy-init -voms cms --valid 168:00 --out $HOME/myproxy\n"
		else: run_script += "\tvoms-proxy-init -voms cms --valid 168:00\n"
		run_script += "fi\n"
		run_script += "\n"
#		run_script += "# Copy python packages to CMSSW area:\n"
#		run_script += "cp -r $HOME/decortication/decortication $CMSSW_BASE/python\n"
#		run_script += "cp -r $HOME/decortication/resources $CMSSW_BASE/python\n"
#		run_script += "cp -r $HOME/truculence/truculence $CMSSW_BASE/python\n"
#		run_script += "\n"
#		run_script += "# Make tarball:\n"
#		run_script += "echo 'Making a tarball of the CMSSW area ...'\n"
#		run_script += "tar --exclude-caches-all -zcf ${CMSSW_VERSION}.tar.gz -C ${CMSSW_BASE}/.. ${CMSSW_VERSION}\n"
#		run_script += "\n"
		run_script += "# Prepare EOS:\n"
		if Dir.eos: run_script += "eos root://cmseos.fnal.gov mkdir -p {}\n".format(out_path)
		else: run_script += "mkdir -p {}\n".format(out_path)
		run_script += "\n"
		run_script += "# Submit condor jobs:\n"
		for i, group in enumerate(groups):
			job_name = "job_{}_{}_{}_{}".format(miniaod.subprocess, miniaod.generation, suffix, i + 1)
			run_script += "condor_submit {}.jdl\n".format(job_name)
		run_script += "\n"
	
		with open("{}/run.sh".format(path), "w") as out:
			out.write(run_script)
		
		condor.tar_cmssw(path)
		
		print "\tThe jobs are in {}".format(path)
	
#	condor.clean()
	return True
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN

