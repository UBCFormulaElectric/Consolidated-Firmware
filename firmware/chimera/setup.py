from setuptools import setup, find_packages

setup(
    name="chimera",
    version="0.1",
    packages=find_packages(),
    install_requires=["pyserial", "protobuf"],
)
