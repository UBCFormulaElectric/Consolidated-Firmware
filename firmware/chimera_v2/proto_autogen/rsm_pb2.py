"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 3, '', 'rsm.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\trsm.proto\x12\x03rsm*\xd5\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0c\n\x08GPIO_LED\x10\x01\x12\x17\n\x13GPIO_BRAKE_LIGHT_EN\x10\x02\x12\x0f\n\x0bGPIO_RL_INT\x10\x03\x12\x11\n\rGPIO_IMU_INT1\x10\x04\x12\x11\n\rGPIO_IMU_INT2\x10\x05\x12\r\n\tGPIO_BOOT\x10\x06\x12\x1c\n\x18GPIO_SUSP_TRAVEL_RR_OCSC\x10\x07\x12\x1c\n\x18GPIO_SUSP_TRAVEL_RL_OCSC\x10\x08*\x84\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bADC_LC3_OUT\x10\x01\x12\x1a\n\x16ADC_SUSP_TRAVEL_RL_3V3\x10\x02\x12\x1a\n\x16ADC_SUSP_TRAVEL_RR_3V3\x10\x03\x12\x0f\n\x0bADC_BPS_3V3\x10\x04*O\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bI2C_IMU_I2C\x10\x01\x12\x12\n\x0eI2C_R_PUMP_I2C\x10\x02b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'rsm_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 232
    _globals['_ADCNETNAME']._serialized_start = 235
    _globals['_ADCNETNAME']._serialized_end = 367
    _globals['_I2CNETNAME']._serialized_start = 369
    _globals['_I2CNETNAME']._serialized_end = 448