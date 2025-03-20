"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 3, '', 'crit.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\ncrit.proto\x12\x04crit*\xd2\x02\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\r\n\tGPIO_BOOT\x10\x01\x12\x0c\n\x08GPIO_LED\x10\x02\x12\x12\n\x0eGPIO_TELEM_SIG\x10\x03\x12\x17\n\x13GPIO_PUSH_DRIVE_SIG\x10\x04\x12\x0e\n\nGPIO_ROT_S\x10\x05\x12\x0e\n\nGPIO_ROT_B\x10\x06\x12\x0e\n\nGPIO_ROT_A\x10\x07\x12\x1b\n\x17GPIO_LAUNCH_CONTROL_SIG\x10\x08\x12\x1d\n\x19GPIO_TORQUE_VECTORING_SIG\x10\t\x12\x12\n\x0eGPIO_REGEN_SIG\x10\n\x12\x10\n\x0cGPIO_LED_RCK\x10\x0b\x12\x16\n\x12GPIO_SEVEN_SEG_RCK\x10\x0c\x12\x14\n\x10GPIO_LED_DIMMING\x10\r\x12\x1a\n\x16GPIO_SEVEN_SEG_DIMMING\x10\x0e**\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00**\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00*J\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07SPI_LED\x10\x01\x12\x11\n\rSPI_SEVEN_SEG\x10\x02*7\n\x10PwmOutputNetName\x12#\n\x1fPWM_OUTPUT_NET_NAME_UNSPECIFIED\x10\x00b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'crit_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 21
    _globals['_GPIONETNAME']._serialized_end = 359
    _globals['_ADCNETNAME']._serialized_start = 361
    _globals['_ADCNETNAME']._serialized_end = 403
    _globals['_I2CNETNAME']._serialized_start = 405
    _globals['_I2CNETNAME']._serialized_end = 447
    _globals['_SPINETNAME']._serialized_start = 449
    _globals['_SPINETNAME']._serialized_end = 523
    _globals['_PWMOUTPUTNETNAME']._serialized_start = 525
    _globals['_PWMOUTPUTNETNAME']._serialized_end = 580