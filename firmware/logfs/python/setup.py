import os
from pathlib import Path
from setuptools import setup, find_packages

logfs_src = os.getenv("LOGFS_SRC")
logfs_src = os.fspath(Path(logfs_src).resolve())

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
