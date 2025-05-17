from setuptools import find_packages, setup

setup(
    name="jsoncan",
    version="0.1.0",
    packages=find_packages(),
    package_dir={"jsoncan": "src"},
    zip_safe=False,
)
