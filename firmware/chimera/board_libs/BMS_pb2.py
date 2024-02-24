"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tBMS.proto\x12\x03BMS"M\n\x08GpioRead\x12"\n\x08net_name\x18\x01 \x01(\x0e2\x10.BMS.GpioNetName\x12\x1d\n\x05value\x18\x02 \x01(\x0e2\x0e.BMS.GpioValue"N\n\tGpioWrite\x12"\n\x08net_name\x18\x01 \x01(\x0e2\x10.BMS.GpioNetName\x12\x1d\n\x05value\x18\x02 \x01(\x0e2\x0e.BMS.GpioValue"7\n\x03Adc\x12!\n\x08net_name\x18\x01 \x01(\x0e2\x0f.BMS.AdcNetName\x12\r\n\x05value\x18\x02 \x01(\x02"|\n\x0cDebugMessage\x12"\n\tgpio_read\x18\x01 \x01(\x0b2\r.BMS.GpioReadH\x00\x12$\n\ngpio_write\x18\x02 \x01(\x0b2\x0e.BMS.GpioWriteH\x00\x12\x17\n\x03adc\x18\x03 \x01(\x0b2\x08.BMS.AdcH\x00B\t\n\x07payload*\xc9\x03\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x16\n\x12ACCEL_BRAKE_OK_3V3\x10\x01\x12\x0e\n\nAIRPLUS_EN\x10\x02\x12\x13\n\x0fAUX_TSENSE_MUX0\x10\x03\x12\x13\n\x0fAUX_TSENSE_MUX1\x10\x04\x12\x13\n\x0fAUX_TSENSE_MUX2\x10\x05\x12\x13\n\x0fAUX_TSENSE_MUX3\x10\x06\x12\r\n\tBMS_LATCH\x10\x07\x12\x0e\n\nBMS_OK_3V3\x10\x08\x12\x0e\n\nBSPD_LATCH\x10\t\x12\x0f\n\x0bBSPD_OK_3V3\x10\n\x12\x10\n\x0cBSPD_TEST_EN\x10\x0b\x12\x0f\n\x0bHVD_SHDN_OK\x10\x0c\x12\r\n\tIMD_LATCH\x10\r\x12\x0e\n\nIMD_OK_3V3\x10\x0e\x12\x07\n\x03LED\x10\x0f\x12\x0c\n\x08NCHIMERA\x10\x10\x12\x1a\n\x16NHIGH_CURRENT_BSPD_3V3\x10\x11\x12\x10\n\x0cNPROGRAM_3V3\x10\x12\x12\x11\n\rPRE_CHARGE_EN\x10\x13\x12\x13\n\x0fTS_ILCK_SHDN_OK\x10\x14\x12\x19\n\x15TS_ISENSE_OCSC_OK_3V3\x10\x15\x12\t\n\x05SD_CD\x10\x16\x12\n\n\x06SPI_CS\x10\x17*:\n\tGpioValue\x12\x1a\n\x16GPIO_VALUE_UNSPECIFIED\x10\x00\x12\x07\n\x03LOW\x10\x01\x12\x08\n\x04HIGH\x10\x02*\x83\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0e\n\nAUX_TSENSE\x10\x01\x12\x12\n\x0eTS_ISENSE_400A\x10\x02\x12\x11\n\rTS_ISENSE_50A\x10\x03\x12\x0f\n\x0bTS_VSENSE_N\x10\x04\x12\x0f\n\x0bTS_VSENSE_P\x10\x05b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'BMS_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 361
    _globals['_GPIONETNAME']._serialized_end = 818
    _globals['_GPIOVALUE']._serialized_start = 820
    _globals['_GPIOVALUE']._serialized_end = 878
    _globals['_ADCNETNAME']._serialized_start = 881
    _globals['_ADCNETNAME']._serialized_end = 1012
    _globals['_GPIOREAD']._serialized_start = 18
    _globals['_GPIOREAD']._serialized_end = 95
    _globals['_GPIOWRITE']._serialized_start = 97
    _globals['_GPIOWRITE']._serialized_end = 175
    _globals['_ADC']._serialized_start = 177
    _globals['_ADC']._serialized_end = 232
    _globals['_DEBUGMESSAGE']._serialized_start = 234
    _globals['_DEBUGMESSAGE']._serialized_end = 358