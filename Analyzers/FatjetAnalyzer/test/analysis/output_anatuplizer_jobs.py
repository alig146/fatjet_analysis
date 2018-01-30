####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
print "[..] Importing packages."
import os, sys
from decortication import variables, eos, site
from truculence import utilities, root
# /IMPORTS

# CLASSES:
# /CLASSES

# VARIABLES:
# /VARIABLES

# FUNCTIONS:
def main():
	## Arguments:
	Site = site.get_site()
	data_dir = Site.get_dir("data")
	a = variables.arguments()
	args = a.args
	tstring = [piece for piece in args.dir.split("/") if piece][-1]
	if not args.output: args.output = "temp"
	
	print "[..] Finding files to hadd."
	data_dir.cd(os.path.join("analyzer_jobs", tstring))
	files = data_dir.ls(most_recent=False, interactive=True)
#	print files
#	print len(files)
#	print data_dir.path
#	return True
#	
#	path = os.path.join(data_dir.path, "analyzer_jobs", tstring)
#	if data_dir.eos: files = eos.listdir(path, root=True, interactive=True)
#	else: files = eo.listdir(eos_path)
##	print files
#	return True
##	print len(files)
	
	print "[..] hadding"
	root.hadd(args.output, files, n=100)
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
