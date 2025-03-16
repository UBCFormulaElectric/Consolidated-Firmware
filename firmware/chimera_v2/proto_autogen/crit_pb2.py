"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\ncrit.proto\x12\x04crit*\xe4\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x08\n\x04BOOT\x10\x01\x12\x07\n\x03LED\x10\x02\x12\r\n\tTELEM_SIG\x10\x03\x12\x12\n\x0ePUSH_DRIVE_SIG\x10\x04\x12\t\n\x05ROT_S\x10\x05\x12\t\n\x05ROT_B\x10\x06\x12\t\n\x05ROT_A\x10\x07\x12\x16\n\x12LAUNCH_CONTROL_SIG\x10\x08\x12\x18\n\x14TORQUE_VECTORING_SIG\x10\t\x12\r\n\tREGEN_SIG\x10\n\x12\x0b\n\x07LED_RCK\x10\x0b\x12\x11\n\rSEVEN_SEG_RCK\x10\x0c**\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00**\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00*d\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00\x12\x18\n\x14SPI_NET_NAME_LED_SPI\x10\x01\x12\x1e\n\x1aSPI_NET_NAME_SEVEN_SEG_SPI\x10\x02*_\n\x10PwmOutputNetName\x12#\n\x1fPWM_OUTPUT_NET_NAME_UNSPECIFIED\x10\x00\x12\x15\n\x11SEVEN_SEG_DIMMING\x10\x01\x12\x0f\n\x0bLED_DIMMING\x10\x02b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'crit_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 21
    _globals['_GPIONETNAME']._serialized_end = 249
    _globals['_ADCNETNAME']._serialized_start = 251
    _globals['_ADCNETNAME']._serialized_end = 293
    _globals['_I2CNETNAME']._serialized_start = 295
    _globals['_I2CNETNAME']._serialized_end = 337
    _globals['_SPINETNAME']._serialized_start = 339
    _globals['_SPINETNAME']._serialized_end = 439
    _globals['_PWMOUTPUTNETNAME']._serialized_start = 441
    _globals['_PWMOUTPUTNETNAME']._serialized_end = 536