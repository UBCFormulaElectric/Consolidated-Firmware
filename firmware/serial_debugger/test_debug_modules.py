from google.protobuf.json_format import MessageToDict
try:
    import debug_modules_pb2 as debug_modules
except ModuleNotFoundError:
    print("\n*** ERROR: Some python protobuff files are missing. Run generate_proto_py.sh and try again.\n")

"""
This file is for testting the python encoding and decoding for the serial debugger.
"""

def encode_gpio(pin, write_value):
    gpio = debug_modules.GPIO()
    gpio.pin = pin
    gpio.value = write_value
    gpio_dic = MessageToDict(gpio) # example dic transformation

    serialized_gpio = gpio.SerializeToString()
    print(F"Encoded GPIO Data:\n{gpio}")
    with open("gpio.bin", "wb") as f:
        f.write(serialized_gpio)

def decode_gpio():
    gpio = debug_modules.GPIO()
    with open("gpio.bin", "rb") as f:
        gpio_bin = f.read()
        gpio.ParseFromString(gpio_bin)
        print(f"Decoded GPIO Data:\n{gpio}")

def main():
    encode_gpio(pin=7, write_value=1)
    decode_gpio()

if __name__ == "__main__":
    main()
