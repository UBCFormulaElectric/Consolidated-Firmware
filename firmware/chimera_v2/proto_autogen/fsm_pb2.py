"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tfsm.proto\x12\x03fsm*\xf5\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0c\n\x08BOTS_3v3\x10\x01\x12\x14\n\x10COCKPIT_SHDN_3v3\x10\x02\x12\x0c\n\x08IMU_INT1\x10\x03\x12\x0c\n\x08IMU_INT2\x10\x04\x12\x11\n\rNSUSP_FL_OCSC\x10\x05\x12\x11\n\rNSUSP_FR_OCSC\x10\x06\x12\x0e\n\nFR_INT_3v3\x10\x07\x12\x0e\n\nFL_INT_3v3\x10\x08\x12\r\n\tDEBUG_LED\x10\t\x12\x13\n\x0fNSTR_ANGLE_OCSC\x10\n\x12\x0f\n\x0bNBPS_F_OCSC\x10\x0b\x12\x0c\n\x08BOOT_LED\x10\x0c*t\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07SUSP_FL\x10\x01\x12\x0b\n\x07SUSP_FR\x10\x02\x12\t\n\x05APPS2\x10\x03\x12\t\n\x05BPS_F\x10\x04\x12\r\n\tSTR_ANGLE\x10\x05\x12\t\n\x05APPS1\x10\x06*7\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07IMU_I2C\x10\x01b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'fsm_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 264
    _globals['_ADCNETNAME']._serialized_start = 266
    _globals['_ADCNETNAME']._serialized_end = 382
    _globals['_I2CNETNAME']._serialized_start = 384
    _globals['_I2CNETNAME']._serialized_end = 439