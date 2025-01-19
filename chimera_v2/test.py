import chimera_v2;

dev = chimera_v2.UsbDevice(0x0483, 0x5740)
while True:
    dev.write(bytes(input() + "\n", "ascii"))