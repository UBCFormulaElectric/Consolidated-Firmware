"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tRSM.proto\x12\x03RSM"M\n\x08GpioRead\x12"\n\x08net_name\x18\x01 \x01(\x0e2\x10.RSM.GpioNetName\x12\x1d\n\x05value\x18\x02 \x01(\x0e2\x0e.RSM.GpioValue"N\n\tGpioWrite\x12"\n\x08net_name\x18\x01 \x01(\x0e2\x10.RSM.GpioNetName\x12\x1d\n\x05value\x18\x02 \x01(\x0e2\x0e.RSM.GpioValue"7\n\x03Adc\x12!\n\x08net_name\x18\x01 \x01(\x0e2\x0f.RSM.AdcNetName\x12\r\n\x05value\x18\x02 \x01(\x02"|\n\x0cDebugMessage\x12"\n\tgpio_read\x18\x01 \x01(\x0b2\r.RSM.GpioReadH\x00\x12$\n\ngpio_write\x18\x02 \x01(\x0b2\x0e.RSM.GpioWriteH\x00\x12\x17\n\x03adc\x18\x03 \x01(\x0b2\x08.RSM.AdcH\x00B\t\n\x07payload*\x9a\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0c\n\x08NCHIMERA\x10\x01\x12\x07\n\x03LED\x10\x02\x12\x0e\n\nRAD_FAN_EN\x10\x03\x12\x0b\n\x07FR_STBY\x10\x04\x12\x16\n\x12BRAKE_LIGHT_EN_3V3\x10\x05\x12\x0e\n\nACC_FAN_EN\x10\x06\x12\x10\n\x0cNProgram_3V3\x10\x07*:\n\tGpioValue\x12\x1a\n\x16GPIO_VALUE_UNSPECIFIED\x10\x00\x12\x07\n\x03LOW\x10\x01\x12\x08\n\x04HIGH\x10\x02*\xfa\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x11\n\rACC_FAN_I_SNS\x10\x01\x12\x11\n\rRAD_FAN_I_SNS\x10\x02\x12\x18\n\x14CoolantPressure1_3V3\x10\x03\x12\x18\n\x14CoolantPressure2_3V3\x10\x04\x12\x0b\n\x07LC3_OUT\x10\x05\x12\x16\n\x12SUSP_TRAVEL_RR_3V3\x10\x06\x12\x16\n\x12SUSP_TRAVEL_RL_3V3\x10\x07\x12\x14\n\x10CoolantTemp2_3V3\x10\x08\x12\x14\n\x10CoolantTemp1_3V3\x10\t\x12\x0b\n\x07LC4_OUT\x10\nb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'RSM_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 361
    _globals['_GPIONETNAME']._serialized_end = 515
    _globals['_GPIOVALUE']._serialized_start = 517
    _globals['_GPIOVALUE']._serialized_end = 575
    _globals['_ADCNETNAME']._serialized_start = 578
    _globals['_ADCNETNAME']._serialized_end = 828
    _globals['_GPIOREAD']._serialized_start = 18
    _globals['_GPIOREAD']._serialized_end = 95
    _globals['_GPIOWRITE']._serialized_start = 97
    _globals['_GPIOWRITE']._serialized_end = 175
    _globals['_ADC']._serialized_start = 177
    _globals['_ADC']._serialized_end = 232
    _globals['_DEBUGMESSAGE']._serialized_start = 234
    _globals['_DEBUGMESSAGE']._serialized_end = 358