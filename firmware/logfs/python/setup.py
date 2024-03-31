import os
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext

setup_dir = os.path.dirname(os.path.abspath(__file__))

logfs_src = Pybind11Extension(
    "logfs_src",
    sources=[os.path.join(setup_dir, "bindings.cpp")],
    include_dirs=[os.path.join(setup_dir, "../src")],
    extra_link_args=["-llogfs_src"],
    library_dirs=[setup_dir],
)

setup(
    name="logfs-python",
    packages=find_packages(),
    entry_points={
        "console_scripts": [
            "logfs=logfs.shell:main",
        ],
    },
    cmdclass={"build_ext": build_ext},
    ext_modules=[logfs_src],
    install_requires=["pytest"],
)
