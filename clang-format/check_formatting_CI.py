import os
import fix_formatting
import sys

if __name__ == '__main__':
    if input("THIS WILL FORMAT YOUR CODE! CONTINUE (Y | N)? ") == 'N':
        sys.exit()

    if fix_formatting.runClangFormat() != 0:
        print("ERROR: Clang-Format encountered issues!")
        sys.exit()
    else:
        print("PASS: Clang-Format")

    #If the git command exits with a non-zero return value
    if os.system("git diff --quiet"):
        #then there are differences and the user forgot to run formatting
        print("Formatting check FAILED, see below for what changed!!")
        os.system("git diff --color")
    else:
        print("Formatting check PASSED, great work!")
