"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 3, '', 'dam.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tdam.proto\x12\x03dam*\xe3\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x10\n\x0cTELEM_PWR_EN\x10\x01\x12\x11\n\rBUZZER_PWR_EN\x10\x02\x12\x0f\n\x0bTSIM_RED_EN\x10\x03\x12\x10\n\x0cNTSIM_GRN_EN\x10\x04\x12\x10\n\x0cR_SHDN_SENSE\x10\x05\x12\x10\n\x0cL_SHDN_SENSE\x10\x06\x12\x07\n\x03LED\x10\x07\x12\x08\n\x04BOOT\x10\x08\x12\x0e\n\nSD_PRESENT\x10\t\x12\x0e\n\nN_RTC_INT1\x10\n\x12\t\n\x05_900M\x10\x0b\x12\x0b\n\x07SD_FAIL\x10\x0c**\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00*7\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07RTC_I2C\x10\x01**\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'dam_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 246
    _globals['_ADCNETNAME']._serialized_start = 248
    _globals['_ADCNETNAME']._serialized_end = 290
    _globals['_I2CNETNAME']._serialized_start = 292
    _globals['_I2CNETNAME']._serialized_end = 347
    _globals['_SPINETNAME']._serialized_start = 349
    _globals['_SPINETNAME']._serialized_end = 391