####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
import sys
from decortication import analyzer, dataset
from truculence import root, utilities, physics
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def get_variables(f="anatuple_dalitz.yaml"):
	with open(f, 'r') as f_in:
		info = utilities.ordered_load(f_in)
	return info["variables"]


def treat_event(loop, event, args):		# Where "loop" refers to an event_loop object
	branches = loop.branches
	
	# Empty branches:
	for key in branches.keys():
#		print branches[key]
		for i in xrange(len(branches[key])):
			branches[key][i] = -1
	
	# Variables:
	variables = loop.ana.variables            # list of dicts
	n_events_tc = loop.n                      # The total number of events in the TChain
	n_events = loop.n_run                     # The number of events to anatuplize
	
	vars_calc = {}
	for ifj in range(2):
#		print ifj
		# Make jets for each subjet:
		subjets = [physics.jet(getattr(event, "spx{}".format(isj))[ifj], getattr(event, "spy{}".format(isj))[ifj], getattr(event, "spz{}".format(isj))[ifj], getattr(event, "se{}".format(isj))[ifj]) for isj in range(4)]
#			for isj in range(4):
#				print subjets[isj].m
#				print getattr(event, "sm{}".format(isj))[ifj]
#			totaljet = subjets[0] + subjets[1] + subjets[2] + subjets[3]
		
		# Calculate Dalitz variables:
		## N = 4
		totaljet = sum(subjets)
#		print totaljet.m
		mijhat2_denominator = totaljet.m**2 + 2*sum([subjet.m**2 for subjet in subjets])
#		print mijhat2_denominator
		vars_dalitz = []
		for isj in range(4):
			for jsj in range(isj + 1, 4):
#					print isj, jsj
				sumjet = subjets[isj] + subjets[jsj]
				mijhat2 = sumjet.m**2/mijhat2_denominator
				vars_dalitz.append(((isj, jsj), mijhat2))
		vars_dalitz.sort(key=lambda x: x[1], reverse=True)
		for isjp in range(6):
			var_name = "sm{}hat".format(isjp)
			if var_name not in vars_calc: vars_calc[var_name] = []
			vars_calc[var_name].append(vars_dalitz[isjp][1])
		sd = sum([(dalitz[1]**0.5 - 6**-0.5)**2 for dalitz in vars_dalitz])
		if "sd" not in vars_calc: vars_calc["sd"] = []
		vars_calc["sd"].append(sd)
		
		## Merge lightest pair (N = 3):
		subjets_merged = [sj for isj, sj in enumerate(subjets) if isj not in vars_dalitz[-1][0]]
		subjets_merged.append(subjets[vars_dalitz[-1][0][0]] + subjets[vars_dalitz[-1][0][1]])
		totaljet_merged = sum(subjets_merged)
		denominator_merged = totaljet_merged.m**2 + sum([sj.m**2 for sj in subjets_merged])
		vars_dalitz_merged = []
		for isj in range(3):
			for jsj in range(isj + 1, 3):
#					print isj, jsj
				sumjet = subjets_merged[isj] + subjets_merged[jsj]
				mijhat2 = sumjet.m**2/denominator_merged
				vars_dalitz_merged.append(((isj, jsj), mijhat2))
		vars_dalitz_merged.sort(key=lambda x: x[1], reverse=True)
		for isjp in range(3):
			var_name = "smm{}hat".format(isjp)
			if var_name not in vars_calc: vars_calc[var_name] = []
			vars_calc[var_name].append(vars_dalitz_merged[isjp][1])
		smd = sum([(dalitz[1]**0.5 - 3**-0.5)**2 for dalitz in vars_dalitz_merged])
		if "smd" not in vars_calc: vars_calc["smd"] = []
		vars_calc["smd"].append(smd)
	
	
	
	for variable in variables:
		var_name = variable["name"]
		var_dim = variable["dimension"]
#		print var_name, event
		if hasattr(event, var_name):
			if var_dim == 1: branches[var_name][0] = getattr(event, var_name)
			else:
				for ifj in range(var_dim): branches[var_name][ifj] = getattr(event, var_name)[ifj]
		elif var_name in vars_calc:
			for ifj in range(var_dim): branches[var_name][ifj] = vars_calc[var_name][ifj]
		else:
			print "[x]", var_name
#	vars_calc = {};
#	for variable in 
#		for groomer in groomers:
#			suffix = groomer if not groomer else "_" + groomer
#			
#			vars_calc["jec" + suffix] = [getattr(event, "{}_pf{}_jec".format(alg, groomer))[i] for i in range(2)]
#			vars_calc["jmc" + suffix] = [getattr(event, "{}_pf{}_jmc".format(alg, groomer))[i] for i in range(2)]
#			# Apply JEC and JMC: (I apply the ungroomed quantity to groomed and ungroomed jets.)
#			vars_calc["m" + suffix] = [getattr(event, "{}_pf{}_m".format(alg, groomer))[i]*vars_calc["jmc"][i] for i in range(2)]
#			vars_calc["px" + suffix] = [getattr(event, "{}_pf{}_px".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
#			vars_calc["py" + suffix] = [getattr(event, "{}_pf{}_py".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
#			vars_calc["pz" + suffix] = [getattr(event, "{}_pf{}_pz".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
#			vars_calc["e" + suffix] = [getattr(event, "{}_pf{}_e".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
#			vars_calc["pt" + suffix] = [getattr(event, "{}_pf{}_pt".format(alg, groomer))[i]*vars_calc["jec"][i] for i in range(2)]
#			vars_calc["eta" + suffix] = [getattr(event, "{}_pf{}_eta".format(alg, groomer))[i] for i in range(2)]
#			vars_calc["phi" + suffix] = [getattr(event, "{}_pf{}_phi".format(alg, groomer))[i] for i in range(2)]
#			vars_calc["tau1" + suffix] = [getattr(event, "{}_pf{}_tau1".format(alg, groomer))[i] for i in range(2)]
#			vars_calc["tau2" + suffix] = [getattr(event, "{}_pf{}_tau2".format(alg, groomer))[i] for i in range(2)]
#			vars_calc["tau3" + suffix] = [getattr(event, "{}_pf{}_tau3".format(alg, groomer))[i] for i in range(2)]
#			vars_calc["tau4" + suffix] = [getattr(event, "{}_pf{}_tau4".format(alg, groomer))[i] for i in range(2)]
#			vars_calc["tau5" + suffix] = [getattr(event, "{}_pf{}_tau5".format(alg, groomer))[i] for i in range(2)]
#		
#			# Calculated variables (not in the tuple):
#			vars_calc["mavg" + suffix] = [(vars_calc["m" + suffix][0] + vars_calc["m" + suffix][1])/2]
#			vars_calc["dm" + suffix] = [abs(vars_calc["m" + suffix][0] - vars_calc["m" + suffix][1])]
#			vars_calc["masy" + suffix] = [vars_calc["dm" + suffix][0]/vars_calc["mavg" + suffix][0]/2]
#			vars_calc["ptavg" + suffix] = [(vars_calc["pt" + suffix][0] + vars_calc["pt" + suffix][1])/2]
#			vars_calc["dpt" + suffix] = [abs(vars_calc["pt" + suffix][0] - vars_calc["pt" + suffix][1])]
#			vars_calc["ptasy" + suffix] = [vars_calc["dpt" + suffix][0]/vars_calc["ptavg" + suffix][0]/2]
#			vars_calc["deta" + suffix] = [abs(vars_calc["eta" + suffix][0] - vars_calc["eta" + suffix][1])]
#			vars_calc["dphi" + suffix] = [math.pi - abs(math.pi - abs(vars_calc["phi" + suffix][0] - vars_calc["phi" + suffix][1]))]
#			vars_calc["dr" + suffix] = [(vars_calc["deta" + suffix][0]**2 + vars_calc["dphi" + suffix][0]**2)**0.5]
#			try: vars_calc["tau43" + suffix] = [vars_calc["tau4" + suffix][i]/vars_calc["tau3" + suffix][i] for i in range(2)]
#			except: vars_calc["tau43" + suffix] = [100 for i in range(2)]		# Division by 0.
#			try: vars_calc["tau42" + suffix] = [vars_calc["tau4" + suffix][i]/vars_calc["tau2" + suffix][i] for i in range(2)]
#			except: vars_calc["tau42" + suffix] = [100 for i in range(2)]		# Division by 0.
#			try: vars_calc["tau41" + suffix] = [vars_calc["tau4" + suffix][i]/vars_calc["tau1" + suffix][i] for i in range(2)]
#			except: vars_calc["tau41" + suffix] = [100 for i in range(2)]		# Division by 0.
#			try: vars_calc["tau32" + suffix] = [vars_calc["tau3" + suffix][i]/vars_calc["tau2" + suffix][i] for i in range(2)]
#			except: vars_calc["tau32" + suffix] = [100 for i in range(2)]		# Division by 0.
#			try: vars_calc["tau31" + suffix] = [vars_calc["tau3" + suffix][i]/vars_calc["tau1" + suffix][i] for i in range(2)]
#			except: vars_calc["tau31" + suffix] = [100 for i in range(2)]		# Division by 0.
#			try: vars_calc["tau21" + suffix] = [vars_calc["tau2" + suffix][i]/vars_calc["tau1" + suffix][i] for i in range(2)]
#			except: vars_calc["tau21" + suffix] = [100 for i in range(2)]		# Division by 0.
#	
#	
#		for variable in variables:
#			var_name = variable["name"]
#			var_dim = variable["dimension"]
#			var_type = variable["type"]
#	#		print var_name, var_dim, var_type
#		
#			if var_name == "w": branches[var_name][0] = getattr(event, var_name)[0]*n_events_tc/n_events
#			elif var_name == "njets": branches[var_name][0] = getattr(event, "ak4_pf_njets")[0]
#	#		elif var_name == "pthat": branches[var_name][0] = getattr(event, "pt_hat")[0]
#			elif var_name == "htak4": branches[var_name][0] = getattr(event, "ak4_pf_ht")[0]
#			elif var_name == "htak8": branches[var_name][0] = getattr(event, "ak8_pf_ht")[0]
#			elif var_name == "wtt": branches[var_name][0] = (getattr(event, "q_gn_sf")[0]*getattr(event, "q_gn_sf")[1])**0.5 if len(getattr(event, "q_gn_sf")) == 2 else 1
#			elif var_name == "bd":
#				for i in range(var_dim): branches[var_name][i] = getattr(event, "ca12_pf_bd_csv")[i]
#			elif var_name == "jetid":
#				for i in range(var_dim): branches[var_name][i] = getattr(event, "ca12_pf_jetid_l")[i]
#			elif var_name in vars_calc:
#				for i in range(var_dim): branches[var_name][i] = vars_calc[var_name][i]
#			else:
#				pieces = var_name.split("_")
#				var_stem = pieces[0]
#				if var_type == "event":
#					for i in range(var_dim): 
#						try: branches[var_name][i] = getattr(event, var_name)[i]		# Sometimes a trigger (for example) might have an empty branch.
#						except: pass
#				if var_type == "jet":
#					groomer = pieces[1] if len(pieces) > 1 else ""
#					for i in range(var_dim): branches[var_name][i] = getattr(event, "{}_pf{}_{}".format(alg, groomer, var_stem))[i]
#	
	loop.tt_out.Fill()

def main():
#	ana_in = root.rfile("~/anatuples/anatuple_cutpt400eta25_updated_predeta.root")
	ana_in = root.rfile("~/anatuples/anatuple_cutpt400eta25_predeta.root")
	anas = {tt_name: [ana_in.path] for tt_name in ana_in.get_ttree_names()}
	ana = analyzer.analyzer(anas, out_file="anatuple_dalitz_predeta.root", out_dir=".", v=True, tt_names=None)
	
	ana.define_branches(get_variables())
#	print ana.branches
#	print ana.loops
#	print ana.variables
#	print ana.loops["ttbar"].tt_in

#	loop = ana.loops["ttbar"]
#	loop.progress = False
#	loop.treatment = treat_event
#	loop.run(5)

	for key, loop in ana.loops.items():
		loop.treatment = treat_event
		loop.run(n=-1)
	ana.write()
	
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN
























