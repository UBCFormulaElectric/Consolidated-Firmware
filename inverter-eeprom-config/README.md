# Configuring PM100DZ Inverter EEPROM Parameters

## RMS GUI.exe

This provides the main application to monitor data and also to reprogram EEPROM parameters. Please see "Section 5 RMS GUI – EEPROM Parameters Guide" in the [Software User Manual (V3_6)](https://app.box.com/s/vf9259qlaadhzxqiqrt5cco8xpsn84hk/file/23982240179) for more information on usage.

## gtk+-2.8.9-setup-1.exe

This executable **must** be run once prior to using the RMS GUI to install the necessary library files. The computer needs to be rebooted after the installation.

## Saved_EEPROM_Configs folder

This folder contains the most recent EEPROM configurations programmed onto the left and right inverter. They are labelled "left_inv" (serial #1846) and "right_inv" (serial #1847), respectively. Please save the most recent EEPROM values to this folder after programming them onto the left/right inverter.

## Conf folder

The RMS GUI will automatically create a folder named “conf”. The folder will
contain a record of any EEPROM changes that have been made. Each time the EEPROM
is programmed a new file is created. Note that this folder is not tracked by Git.

## defsyms_20211209.txt, defsyms_20211209_G3, defsyms20211209_G2

These are the default symbols file that includes the parameters to be monitored and reprogrammed
depending on which version of hardware is being used. Please do not move these files from their default location.
