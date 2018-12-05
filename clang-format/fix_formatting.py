import os

def runClangFormat():
	success = 0
	
	# Construct the command line input
	CLANG_FORMAT_VERSION = "4.0"
	CLANG_FORMAT_COMMAND = "clang-format-" + CLANG_FORMAT_VERSION
	CLANG_FORMAT_OPTIONS = " -i --style=file "

	#If the OS is Windows
	if os.name == 'nt':
		# Append the requisite .exe file ending
		CLANG_FORMAT_COMMAND += ".exe" 

	SOURCE_DIR = os.path.join("..", "src")

	for root, dirnames, filenames in os.walk(SOURCE_DIR):
	    for filename in filenames:
	        if filename.endswith(('.c', '.cc', '.cpp', '.h', '.hpp', '.hh')):
	        	print((CLANG_FORMAT_COMMAND + CLANG_FORMAT_OPTIONS + os.path.join(root, filename)))
	        	success |= os.system((os.path.abspath(CLANG_FORMAT_COMMAND) + CLANG_FORMAT_OPTIONS + os.path.join(root, filename)))

	return success
 


