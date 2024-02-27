"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tFSM.proto\x12\x03FSM*\x97\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x15\n\x11BRAKE_OCSC_OK_3V3\x10\x01\x12\x0c\n\x08NCHIMERA\x10\x02\x12\x07\n\x03LED\x10\x03\x12\x1b\n\x17NBSPD_BRAKE_PRESSED_3V3\x10\x04\x12\x10\n\x0cNPROGRAM_3V3\x10\x05\x12\x0c\n\x08FSM_SHDN\x10\x06*\xd7\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x16\n\x12SUSP_TRAVEL_FR_3V3\x10\x01\x12\x16\n\x12SUSP_TRAVEL_FL_3V3\x10\x02\x12\x13\n\x0fLOAD_CELL_2_3V3\x10\x03\x12\r\n\tAPPS2_3V3\x10\x04\x12\r\n\tBPS_F_3V3\x10\x05\x12\r\n\tBPS_B_3V3\x10\x06\x12\x13\n\x0fLOAD_CELL_1_3V3\x10\x07\x12\r\n\tAPPS1_3V3\x10\x08\x12\x15\n\x11SteeringAngle_3V3\x10\tb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'FSM_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 170
    _globals['_ADCNETNAME']._serialized_start = 173
    _globals['_ADCNETNAME']._serialized_end = 388