# PCAN-Explorer 6

user manual : https://www.peak-system.com/produktcd/Pdf/English/PCAN-USB_UserMan_eng.pdf

## Setup
1. Download and extract http://www.peak-system.com/cupd2/PE6Setup6_2_1_1946.zip
1. Install PCAN-Explorer 6 using **PE6-Setup.exe**
1. The installer will ask for a license file which you can find in this folder as **PcanExpl.lic**
2. Install the apropriate PCAN USB drivers from https://www.peak-system.com/Drivers.523.0.html?&L=1 run the application


## Usage
- to have PCAN explorer cross-reference incoming signals with our `.sym` file (which holds the meaning and format of each of our CAN messages), right click on the `.sym` and select "Apply", as per the below image
1. Open PCAN with the PCAN USB device connected to your machine, aswell as the red PCAN-Explorer USB stick (the application will not run with out this)
2. Start a new project and upload the necessary dbc files
3. On the bottom of the screen, add a new connection, when the window opens right click on whitespace under the available nets header. Select add a new net. Then select edit nets and change the bitrate to 500 kbits/s
4. You should now see some can messages in hex coming in on the main window, click on the dbc file and select apply this will decode the hex messages into human readable format

![image here](../../../images/sym_file_click_apply.png?raw=true "Click Apply")
