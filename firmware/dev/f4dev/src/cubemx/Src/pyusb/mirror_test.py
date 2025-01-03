import sys
import usb.core
import usb.util
import time
import importlib
module = importlib.import_module("/root/Consolidated-Firmware/validationtools/validationtools/chimera/chimera.py")

devboard = usb.core.find(idVendor=0x0483, idProduct=0x5740)
interface = devboard[0][(1,0)]

# #and this is resulting in err2 


endpointW = interface[0]
endpointR = interface[1]

while True:
    if devboard.is_kernel_driver_active(interface.bInterfaceNumber):
        try:

            devboard.detach_kernel_driver(interface.bInterfaceNumber)
            usb.util.claim_interface(devboard, interface.bInterfaceNumber)
        except usb.core.USBError as e:
            sys.exit("Could not detatch kernel driver from interface({0}): {1}".format(i, str(e)))

    print("input your data, if you want to end the program input: exit ")
    dataIn = input()
    if dataIn == "exit":
        break
    bytesWritten = devboard.write(endpointW.bEndpointAddress, dataIn)
    print(f"Endpoint Address (Read): {endpointR.bEndpointAddress}")
    print(f"Endpoint Attributes: {endpointR.bmAttributes}")
    print(f"Endpoint Address (Read): {endpointW.bEndpointAddress}")
    print(f"Endpoint Attributes: {endpointW.bmAttributes}")

    print(f"Wrote {bytesWritten} bytes")
    time.sleep(1)
    buf = devboard.read(endpointR.bEndpointAddress, len(dataIn))

    res = bytes(buf).decode()
    print(res)
    #this is resulting in error number 2 so if you get err2 its cuz of the util stuff 
    
usb.util.release_interface(devboard, interface.bInterfaceNumber)
#I get resource busy errors when trying to reattach the kernel I think this will be an issue later on but right now its good? 
#devboard.attach_kernel_driver(interface.bInterfaceNumber)
