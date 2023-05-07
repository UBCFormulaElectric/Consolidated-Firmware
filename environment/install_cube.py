"""
Install STM32CubeMX to the specified directory.
"""
import sys
import os
import argparse
import subprocess

CUBE_AUTO_INSTALL_XML = '''\
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<AutomatedInstallation langpack="eng">
    <com.st.microxplorer.install.MXHTMLHelloPanel id="readme"/>
    <com.st.microxplorer.install.MXLicensePanel id="licence.panel"/>
    <com.st.microxplorer.install.MXAnalyticsPanel id="analytics.panel"/>
    <com.st.microxplorer.install.MXTargetPanel id="target.panel">
        <installpath>{CUBE_INSTALL_DIR}</installpath>
    </com.st.microxplorer.install.MXTargetPanel>
    <com.st.microxplorer.install.MXShortcutPanel id="shortcut.panel"/>
    <com.st.microxplorer.install.MXInstallPanel id="install.panel"/>
    <com.st.microxplorer.install.MXFinishPanel id="finish.panel"/>
</AutomatedInstallation>
'''

def _isWindows():
    """
    Check if the operating system is Windows
    :return: True if using Windows, false otherwise
    """
    return os.name == 'nt'

def install_stm32cubemx(install_dir, installer_zip):
    """
    Install STM32CubeMX to the specified install directory on the command line

    @param install_dir: STM32CubeMX install directory
    @param installer_zip: STM32CubeMX installer zip file
    """
    # Return early if the installer extension is not we expect
    installer_extension = '.zip'
    if not installer_zip.lower().endswith(installer_extension):
        raise Exception('Expect: STM32CubeMX installer ({}) => Actual: {}'.format(installer_extension, installer_zip))

    # Generate output folder if they don't exist already
    auto_install_xml_dir = os.path.join('.', 'auto_generated')
    if not os.path.exists(auto_install_xml_dir):
        os.mkdir(auto_install_xml_dir)

    # Generate .xml that specifies the STM32CubeMX installation path
    auto_install_xml = os.path.join(auto_install_xml_dir, 'cube-auto-install.xml')
    auto_install_xml_f = open(auto_install_xml, 'w')
    auto_install_xml_f.write(CUBE_AUTO_INSTALL_XML.format(CUBE_INSTALL_DIR=install_dir))
    auto_install_xml_f.close()

    # Extract STM32CubeMX
    extract_dir = os.path.join('/tmp', 'STM32CubeMXSetup')
    child = subprocess.Popen(['unzip', '-o', installer_zip, '-d', extract_dir])
    child.wait()
    if child.returncode is not 0:
        Exception('Failed to unzip {}'.format(installer_zip))

    # Find STM32CubeMX installer
    cube_installer = ''
    cube_installer_extension = '.exe'
    for f in os.listdir(extract_dir):
        if f.endswith(cube_installer_extension):
            cube_installer = os.path.join(extract_dir, f)
    if cube_installer == '':
        Exception('Did not find a STM32CubeMX installer ({})'.format(cube_installer_extension))

    # Run STM32CubeMX installer
    child = subprocess.Popen(['java', '-jar', cube_installer, auto_install_xml_f.name])
    child.wait()
    if child.returncode is not 0:
        Exception('Failed to install {}'.format(installer_zip))

    try:
        if _isWindows():
            subprocess.call(["where", "STM32CubeMX"])
        else:
            subprocess.call(["which", "STM32CubeMX"])
    except OSError as e:
        Exception('Could not verifiy that STM32CubeMX was installed correctly...' )

    print('Verified that STM32CubeMX was installed correctly!')

if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('install_dir', help='STM32CubeMX installation directory')
    parser.add_argument('cube_zip', help='STM32CubeMX installer zip')
    args = parser.parse_args()

    install_stm32cubemx(args.install_dir, args.cube_zip)
