import os
from setuptools import setup, find_packages
import glob

PATTERNS = ["logfs_src*.so", "logfs_src*.pyd"]

setup_dir = os.path.dirname(os.path.abspath(__file__))
glob_pattern = os.path.join(setup_dir, "logfs_src*.so")
files = glob.glob(glob_pattern)

for pattern in PATTERNS:
    glob_pattern = os.path.join(setup_dir, pattern)
    files = glob.glob(glob_pattern)

    if len(files) != 0:
        break

if len(files) == 0:
    raise Exception("logfs_src shared library not found")
else:
    logfs_src = files[0]

setup(
    name="logfs-python",
    packages=find_packages(),
    entry_points={
        "console_scripts": [
            "logfs=logfs.shell:main",
        ],
    },
    install_requires=[],
    package_data={"logfs": [logfs_src]},
    include_package_data=True,
)
