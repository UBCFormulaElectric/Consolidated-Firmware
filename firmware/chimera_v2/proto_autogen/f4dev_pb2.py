"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 3, '', 'f4dev.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0bf4dev.proto\x12\x05f4dev*D\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\n\n\x06GPIO_6\x10\x01\x12\n\n\x06GPIO_5\x10\x02**\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00*5\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\t\n\x05I2C_3\x10\x01**\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'f4dev_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 22
    _globals['_GPIONETNAME']._serialized_end = 90
    _globals['_ADCNETNAME']._serialized_start = 92
    _globals['_ADCNETNAME']._serialized_end = 134
    _globals['_I2CNETNAME']._serialized_start = 136
    _globals['_I2CNETNAME']._serialized_end = 189
    _globals['_SPINETNAME']._serialized_start = 191
    _globals['_SPINETNAME']._serialized_end = 233