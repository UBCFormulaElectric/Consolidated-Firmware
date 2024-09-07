"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tRSM.proto\x12\x03RSM*\x9a\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0c\n\x08NCHIMERA\x10\x01\x12\x07\n\x03LED\x10\x02\x12\x0e\n\nRAD_FAN_EN\x10\x03\x12\x0b\n\x07FR_STBY\x10\x04\x12\x16\n\x12BRAKE_LIGHT_EN_3V3\x10\x05\x12\x0e\n\nACC_FAN_EN\x10\x06\x12\x10\n\x0cNProgram_3V3\x10\x07*\xfa\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x11\n\rACC_FAN_I_SNS\x10\x01\x12\x11\n\rRAD_FAN_I_SNS\x10\x02\x12\x18\n\x14CoolantPressure1_3V3\x10\x03\x12\x18\n\x14CoolantPressure2_3V3\x10\x04\x12\x0b\n\x07LC3_OUT\x10\x05\x12\x16\n\x12SUSP_TRAVEL_RR_3V3\x10\x06\x12\x16\n\x12SUSP_TRAVEL_RL_3V3\x10\x07\x12\x14\n\x10CoolantTemp2_3V3\x10\x08\x12\x14\n\x10CoolantTemp1_3V3\x10\t\x12\x0b\n\x07LC4_OUT\x10\nb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'RSM_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 173
    _globals['_ADCNETNAME']._serialized_start = 176
    _globals['_ADCNETNAME']._serialized_end = 426