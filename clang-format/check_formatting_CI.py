import os
import fix_formatting

if __name__ == '__main__':
	if fix_formatting.runClangFormat() != 0:
		print("ERROR: Clang-Format encountered issues!")
	else:
		print("PASS: Clang-Format")
