import os

CLANG_FORMAT_VERSION = "4.0"
CLANG_FORMAT_COMMAND = "clang-format-" + CLANG_FORMAT_VERSION

for root, dirnames, filenames in os.walk('../src'):
    for filename in filenames:
        if filename.endswith(('.c', '.cc', '.cpp', '.h', '.hpp', '.hh')):
        	print((CLANG_FORMAT_COMMAND + " -i --style=file " + os.path.join(root, filename)))
        	os.system((os.path.abspath(CLANG_FORMAT_COMMAND) + " -i --style=file " + os.path.join(root, filename)))
 


