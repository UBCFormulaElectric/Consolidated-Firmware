from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        name="logfs",
        sources=["../logfs.c", "../logfs_util.c", "logfs.pyx"],
        include_dirs=[".."],
        
    )
]

setup(
    name="logfs-python",
    package_dir=find_packages(),
    zip_safe=False,
    ext_modules=cythonize(ext_modules, language_level=3),
    install_requires=[],
)
