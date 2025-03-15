"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 28, 1, '', 'rsm.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\trsm.proto\x12\x03rsm*\xcd\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07LED_PIN\x10\x01\x12\x16\n\x12BRAKE_LIGHT_EN_PIN\x10\x02\x12\x0e\n\nRL_INT_PIN\x10\x03\x12\x10\n\x0cIMU_INT1_PIN\x10\x04\x12\x10\n\x0cIMU_INT2_PIN\x10\x05\x12\x0c\n\x08BOOT_PIN\x10\x06\x12\x1b\n\x17SUSP_TRAVEL_RR_OCSC_PIN\x10\x07\x12\x1b\n\x17SUSP_TRAVEL_RL_OCSC_PIN\x10\x08*t\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07LC3_OUT\x10\x01\x12\x16\n\x12SUSP_TRAVEL_RL_3V3\x10\x02\x12\x16\n\x12SUSP_TRAVEL_RR_3V3\x10\x03\x12\x0b\n\x07BPS_3V3\x10\x04*G\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07IMU_I2C\x10\x01\x12\x0e\n\nR_PUMP_I2C\x10\x02b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'rsm_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 224
    _globals['_ADCNETNAME']._serialized_start = 226
    _globals['_ADCNETNAME']._serialized_end = 342
    _globals['_I2CNETNAME']._serialized_start = 344
    _globals['_I2CNETNAME']._serialized_end = 415