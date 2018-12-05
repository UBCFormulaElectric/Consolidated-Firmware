import os
import fix_formatting

if __name__ == '__main__':
	if fix_formatting.runClangFormat() != 0:
		print("ERROR: Clang-Format encountered issues!")
	else:
		print("PASS: Clang-Format")

	#If the git command exits with a 1
	if os.system("git diff-index --quiet HEAD --;"):
		#then there are differences and the user forgot to run formatting
		print("Formatting check FAILED, see below for what changed!!")
		os.system("git diff --color")
	else:
		print("Formatting check PASSED, great work!")