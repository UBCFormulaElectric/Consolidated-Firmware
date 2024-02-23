"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\nCRIT.proto\x12\x04CRIT"O\n\x08GpioRead\x12#\n\x08net_name\x18\x01 \x01(\x0e2\x11.CRIT.GpioNetName\x12\x1e\n\x05value\x18\x02 \x01(\x0e2\x0f.CRIT.GpioValue"P\n\tGpioWrite\x12#\n\x08net_name\x18\x01 \x01(\x0e2\x11.CRIT.GpioNetName\x12\x1e\n\x05value\x18\x02 \x01(\x0e2\x0f.CRIT.GpioValue"8\n\x03Adc\x12"\n\x08net_name\x18\x01 \x01(\x0e2\x10.CRIT.AdcNetName\x12\r\n\x05value\x18\x02 \x01(\x02"\x7f\n\x0cDebugMessage\x12#\n\tgpio_read\x18\x01 \x01(\x0b2\x0e.CRIT.GpioReadH\x00\x12%\n\ngpio_write\x18\x02 \x01(\x0b2\x0f.CRIT.GpioWriteH\x00\x12\x18\n\x03adc\x18\x03 \x01(\x0b2\t.CRIT.AdcH\x00B\t\n\x07payload*\xc6\x04\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x18\n\x14TORQUE_VECTORING_LED\x10\x01\x12\r\n\tSTART_LED\x10\x02\x12\r\n\tREGEN_LED\x10\x03\x12\x0c\n\x08AUX_DB_R\x10\x04\x12\r\n\tCRIT_DB_R\x10\x05\x12\r\n\tCRIT_DB_G\x10\x06\x12\r\n\tCRIT_DB_B\x10\x07\x12\x0c\n\x08AUX_DB_G\x10\x08\x12\x0c\n\x08AUX_DB_B\x10\t\x12\n\n\x06BSPD_R\x10\n\x12\n\n\x06SHDN_R\x10\x0b\x12\t\n\x05RSM_B\x10\x0c\x12\x08\n\x04VC_R\x10\r\x12\x08\n\x04VC_B\x10\x0e\x12\t\n\x05FSM_R\x10\x0f\x12\t\n\x05BMS_R\x10\x10\x12\t\n\x05BMS_G\x10\x11\x12\t\n\x05BMS_B\x10\x12\x12\t\n\x05AMS_R\x10\x13\x12\x12\n\x0eNDRIVE_MODE_1b\x10\x14\x12\t\n\x05IMD_R\x10\x15\x12\x0c\n\x08SDHN_SEN\x10\x16\x12\x0f\n\x0bINERTIA_SEN\x10\x17\x12\t\n\x05FSM_G\x10\x18\x12\t\n\x05FSM_B\x10\x19\x12\t\n\x05RSM_R\x10\x1a\x12\t\n\x05RSM_G\x10\x1b\x12\x08\n\x04VC_G\x10\x1c\x12\n\n\x06SHDN_G\x10\x1d\x12\x12\n\x0eNDRIVE_MODE_2b\x10\x1e\x12\x12\n\x0eNDRIVE_MODE_3b\x10\x1f\x12\x12\n\x0eNDRIVE_MODE_0b\x10 \x12\x18\n\x14TORQUE_VECTORING_SIG\x10!\x12\r\n\tREGEN_SIG\x10"\x12\x07\n\x03LED\x10#\x12\x10\n\x0cnPROGRAM_3V3\x10$\x12\r\n\tSTART_SIG\x10%\x12\x0c\n\x08nCHIMERA\x10&*:\n\tGpioValue\x12\x1a\n\x16GPIO_VALUE_UNSPECIFIED\x10\x00\x12\x07\n\x03LOW\x10\x01\x12\x08\n\x04HIGH\x10\x02*9\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\r\n\tREGEN_3V3\x10\x01b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'CRIT_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 371
    _globals['_GPIONETNAME']._serialized_end = 953
    _globals['_GPIOVALUE']._serialized_start = 955
    _globals['_GPIOVALUE']._serialized_end = 1013
    _globals['_ADCNETNAME']._serialized_start = 1015
    _globals['_ADCNETNAME']._serialized_end = 1072
    _globals['_GPIOREAD']._serialized_start = 20
    _globals['_GPIOREAD']._serialized_end = 99
    _globals['_GPIOWRITE']._serialized_start = 101
    _globals['_GPIOWRITE']._serialized_end = 181
    _globals['_ADC']._serialized_start = 183
    _globals['_ADC']._serialized_end = 239
    _globals['_DEBUGMESSAGE']._serialized_start = 241
    _globals['_DEBUGMESSAGE']._serialized_end = 368