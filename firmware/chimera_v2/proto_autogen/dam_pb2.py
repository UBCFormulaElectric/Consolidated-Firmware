"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 4, '', 'dam.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tdam.proto\x12\x03dam*\x9e\x02\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x15\n\x11GPIO_TELEM_PWR_EN\x10\x01\x12\x16\n\x12GPIO_BUZZER_PWR_EN\x10\x02\x12\x14\n\x10GPIO_TSIM_RED_EN\x10\x03\x12\x15\n\x11GPIO_NTSIM_GRN_EN\x10\x04\x12\x15\n\x11GPIO_R_SHDN_SENSE\x10\x05\x12\x15\n\x11GPIO_L_SHDN_SENSE\x10\x06\x12\x0c\n\x08GPIO_LED\x10\x07\x12\r\n\tGPIO_BOOT\x10\x08\x12\x13\n\x0fGPIO_SD_PRESENT\x10\t\x12\x13\n\x0fGPIO_N_RTC_INT1\x10\n\x12\r\n\tGPIO_900M\x10\x0b\x12\x10\n\x0cGPIO_SD_FAIL\x10\x0c*7\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07I2C_RTC\x10\x01b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'dam_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 305
    _globals['_I2CNETNAME']._serialized_start = 307
    _globals['_I2CNETNAME']._serialized_end = 362