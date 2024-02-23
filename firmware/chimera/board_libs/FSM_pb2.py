"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tFSM.proto\x12\x03FSM"M\n\x08GpioRead\x12"\n\x08net_name\x18\x01 \x01(\x0e2\x10.FSM.GpioNetName\x12\x1d\n\x05value\x18\x02 \x01(\x0e2\x0e.FSM.GpioValue"N\n\tGpioWrite\x12"\n\x08net_name\x18\x01 \x01(\x0e2\x10.FSM.GpioNetName\x12\x1d\n\x05value\x18\x02 \x01(\x0e2\x0e.FSM.GpioValue"7\n\x03Adc\x12!\n\x08net_name\x18\x01 \x01(\x0e2\x0f.FSM.AdcNetName\x12\r\n\x05value\x18\x02 \x01(\x02"|\n\x0cDebugMessage\x12"\n\tgpio_read\x18\x01 \x01(\x0b2\r.FSM.GpioReadH\x00\x12$\n\ngpio_write\x18\x02 \x01(\x0b2\x0e.FSM.GpioWriteH\x00\x12\x17\n\x03adc\x18\x03 \x01(\x0b2\x08.FSM.AdcH\x00B\t\n\x07payload*\x97\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x15\n\x11BRAKE_OCSC_OK_3V3\x10\x01\x12\x0c\n\x08NCHIMERA\x10\x02\x12\x07\n\x03LED\x10\x03\x12\x1b\n\x17NBSPD_BRAKE_PRESSED_3V3\x10\x04\x12\x10\n\x0cNPROGRAM_3V3\x10\x05\x12\x0c\n\x08FSM_SHDN\x10\x06*:\n\tGpioValue\x12\x1a\n\x16GPIO_VALUE_UNSPECIFIED\x10\x00\x12\x07\n\x03LOW\x10\x01\x12\x08\n\x04HIGH\x10\x02*\xd7\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x16\n\x12SUSP_TRAVEL_FR_3V3\x10\x01\x12\x16\n\x12SUSP_TRAVEL_FL_3V3\x10\x02\x12\x13\n\x0fLOAD_CELL_2_3V3\x10\x03\x12\r\n\tAPPS2_3V3\x10\x04\x12\r\n\tBPS_F_3V3\x10\x05\x12\r\n\tBPS_B_3V3\x10\x06\x12\x13\n\x0fLOAD_CELL_1_3V3\x10\x07\x12\r\n\tAPPS1_3V3\x10\x08\x12\x15\n\x11SteeringAngle_3V3\x10\tb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'FSM_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 361
    _globals['_GPIONETNAME']._serialized_end = 512
    _globals['_GPIOVALUE']._serialized_start = 514
    _globals['_GPIOVALUE']._serialized_end = 572
    _globals['_ADCNETNAME']._serialized_start = 575
    _globals['_ADCNETNAME']._serialized_end = 790
    _globals['_GPIOREAD']._serialized_start = 18
    _globals['_GPIOREAD']._serialized_end = 95
    _globals['_GPIOWRITE']._serialized_start = 97
    _globals['_GPIOWRITE']._serialized_end = 175
    _globals['_ADC']._serialized_start = 177
    _globals['_ADC']._serialized_end = 232
    _globals['_DEBUGMESSAGE']._serialized_start = 234
    _globals['_DEBUGMESSAGE']._serialized_end = 358