####################################################################
# Type: SCRIPT                                                     #
#                                                                  #
# Description: [description]                                       #
####################################################################

# IMPORTS:
# :IMPORTS

# CLASSES:
# :CLASSES

# VARIABLES:
# :VARIABLES

# FUNCTIONS:
def main():
	lines = open("events_raw.txt").readlines()
	# Filter lines for data:
	lines = [l for l in lines if l[0] == "*" and "**" not in l]
	# Filter our column title line:
	lines = lines[1:]
	
	# Format:
	lines_formatted = []
	for l in lines:
		pieces = [thing.strip() for thing in l.split("*") if thing.strip()]
		lines_formatted.append(":".join(pieces[1:4]))
	with open("events.txt", "w") as out:
		out.write("\n".join(lines_formatted) + "\n")
	return True
# :FUNCTIONS

# MAIN:
if __name__ == "__main__":
	main()
# :MAIN

