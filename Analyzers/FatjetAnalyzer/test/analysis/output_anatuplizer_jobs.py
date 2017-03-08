####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
print "[..] Importing packages."
import sys
from decortication import variables, eos
from truculence import utilities, root
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
	tstring = [piece for piece in args.dir.split("/") if piece][-1]
	
	print "[..] Finding files to hadd."
	eos_path = "/store/user/tote/analyzer_jobs/{}".format(tstring)
	files = eos.listdir(eos_path, root=True, interactive=True)
	
	print "[..] hadding"
	root.hadd(args.output, files)
# /FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
