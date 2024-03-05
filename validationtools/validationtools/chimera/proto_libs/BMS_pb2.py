"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tBMS.proto\x12\x03BMS*\xc7\x03\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x16\n\x12ACCEL_BRAKE_OK_3V3\x10\x01\x12\x0c\n\x08AIR_P_EN\x10\x02\x12\x13\n\x0fAUX_TSENSE_MUX0\x10\x03\x12\x13\n\x0fAUX_TSENSE_MUX1\x10\x04\x12\x13\n\x0fAUX_TSENSE_MUX2\x10\x05\x12\x13\n\x0fAUX_TSENSE_MUX3\x10\x06\x12\r\n\tBMS_LATCH\x10\x07\x12\x0e\n\nBMS_OK_3V3\x10\x08\x12\x0e\n\nBSPD_LATCH\x10\t\x12\x0f\n\x0bBSPD_OK_3V3\x10\n\x12\x10\n\x0cBSPD_TEST_EN\x10\x0b\x12\x0f\n\x0bHVD_SHDN_OK\x10\x0c\x12\r\n\tIMD_LATCH\x10\r\x12\x0e\n\nIMD_OK_3V3\x10\x0e\x12\x07\n\x03LED\x10\x0f\x12\x0c\n\x08NCHIMERA\x10\x10\x12\x1a\n\x16NHIGH_CURRENT_BSPD_3V3\x10\x11\x12\x10\n\x0cNPROGRAM_3V3\x10\x12\x12\x11\n\rPRE_CHARGE_EN\x10\x13\x12\x13\n\x0fTS_ILCK_SHDN_OK\x10\x14\x12\x19\n\x15TS_ISENSE_OCSC_OK_3V3\x10\x15\x12\t\n\x05SD_CD\x10\x16\x12\n\n\x06SPI_CS\x10\x17*p\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0e\n\nAUX_TSENSE\x10\x01\x12\x12\n\x0eTS_ISENSE_400A\x10\x02\x12\x11\n\rTS_ISENSE_50A\x10\x03\x12\r\n\tTS_VSENSE\x10\x04b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'BMS_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 474
    _globals['_ADCNETNAME']._serialized_start = 476
    _globals['_ADCNETNAME']._serialized_end = 588