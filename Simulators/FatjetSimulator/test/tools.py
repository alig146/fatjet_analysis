####################################################################
# Type: MODULE                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
# :IMPORTS

# VARIABLES:
# :VARIABLES

# CLASSES:
# :CLASSES

# FUNCTIONS:
def get_options():
	from FWCore.ParameterSet.VarParsing import VarParsing		# Module used to pass arguments from the command line.
	options = VarParsing('analysis')
	# Define options:
	options.register ('inFile',
		'',
		VarParsing.multiplicity.singleton,
		VarParsing.varType.string,
		"Input file"
	)
	options.register ('outFile',
		'',
		VarParsing.multiplicity.singleton,
		VarParsing.varType.string,
		"Output file"
	)
	options.register('subprocess',
		'',
		VarParsing.multiplicity.singleton,
		VarParsing.varType.string,
		"What is the dataset's subprocess (sq100to2j, qcdp80, etc.)?"
	)
	options.register('generation',
		'',
		VarParsing.multiplicity.singleton,
		VarParsing.varType.string,
		"What is the dataset's generation (spring15, 0706, etc.)?"
	)
	options.maxEvents = 2
	options.parseArguments()
	return options
# :FUNCTIONS
