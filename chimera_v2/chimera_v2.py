import proto_autogen
import usb

class UsbDevice:
    def __init__(self):
        self._device = usb.core.find(idVendor=0x0483, idProduct=0x5740)
        self._interface = self.device[0][(1,0)]
        self._endpoint_write = self.interface[0]
        self._endpoint_read = self.interface[1]

        # If the kernel is _attachted to another driver, detach it.
        if self._device.is_kernel_driver_active(self._interface.bInterfaceNumber):
            self._device.detach_kernel_driver(self._interface.bInterfaceNumber)

            # TODO: Investigate if claiming the interface explictly is needed.
            usb.util.claim_interface(self._device, self._interface.bInterfaceNumber)

    def __exit__(self):
        # Release the interface explictly.
        # TODO: Investigate if releasing the interface explictly is needed.
        usb.util.release_interface(self._device, self._interface.bInterfaceNumber)

    def write(self, buffer):
        self._device.write(self._endpoint_write.bEndpointAddress, buffer)

    def read(self, length):
        return bytes(self._device.read(self._endpoint_write.bEndpointAddress, length)).decode()
