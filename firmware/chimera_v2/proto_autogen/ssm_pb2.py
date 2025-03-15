"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 0, '-rc2', 'ssm.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tssm.proto\x12\x03ssm*z\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bINDICATOR_1\x10\x01\x12\x0f\n\x0bINDICATOR_2\x10\x02\x12\x0f\n\x0bINDICATOR_3\x10\x03\x12\n\n\x06DOUT_4\x10\x04\x12\r\n\tDEBUG_LED\x10\x05**\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00**\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'ssm_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 18
    _globals['_GPIONETNAME']._serialized_end = 140
    _globals['_ADCNETNAME']._serialized_start = 142
    _globals['_ADCNETNAME']._serialized_end = 184
    _globals['_I2CNETNAME']._serialized_start = 186
    _globals['_I2CNETNAME']._serialized_end = 228