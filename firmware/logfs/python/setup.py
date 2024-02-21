import os
from pathlib import Path
from setuptools import setup

logfs_lib = os.getenv("LOGFS_LIB")
logfs_lib = os.fspath(Path(logfs_lib).resolve())

setup(
    name="logfs-python",
    packages=["."],
    install_requires=[],
    package_data={".": [logfs_lib]},
    include_package_data=True,
)
