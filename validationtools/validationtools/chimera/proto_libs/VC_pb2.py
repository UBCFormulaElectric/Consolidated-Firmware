"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x08VC.proto\x12\x02VC*\xe6\x03\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x11\n\rBUZZER_PWR_EN\x10\x01\x12\x12\n\x0eBAT_I_SNS_NFLT\x10\x02\x12\x07\n\x03LED\x10\x03\x12\x10\n\x0cTELEM_PWR_EN\x10\x04\x12\x0b\n\x07NPCM_EN\x10\x05\x12\x14\n\x10ACC_I_SENSE_NFLT\x10\x06\x12\t\n\x05PGOOD\x10\x07\x12\r\n\tLV_PWR_EN\x10\x08\x12\x0e\n\nAUX_PWR_EN\x10\t\x12\x0f\n\x0bPUMP_PWR_EN\x10\n\x12\x0e\n\n_900K_GPIO\x10\x0b\x12\x0f\n\x0bNCHRG_FAULT\x10\x0c\x12\t\n\x05NCHRG\x10\r\x12\x10\n\x0cINV_L_PWR_EN\x10\x0e\x12\x10\n\x0cINV_R_PWR_EN\x10\x0f\x12\x0f\n\x0bSHDN_PWR_EN\x10\x10\x12\x0c\n\x08FR_STBY1\x10\x11\x12\x0c\n\x08FR_STBY2\x10\x12\x12\x0c\n\x08FR_STBY3\x10\x13\x12\x11\n\rINV_L_PROGRAM\x10\x14\x12\x11\n\rINV_R_PROGRAM\x10\x15\x12\x0e\n\nL_SHDN_SNS\x10\x16\x12\x0e\n\nR_SHDN_SNS\x10\x17\x12\x0c\n\x08NCHIMERA\x10\x18\x12\x10\n\x0cNPROGRAM_3V3\x10\x19\x12\x14\n\x10SB_ILCK_SHDN_SNS\x10\x1a\x12\x11\n\rTSMS_SHDN_SNS\x10\x1b*\xfb\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x13\n\x0fINV_R_PWR_I_SNS\x10\x01\x12\x13\n\x0fINV_L_PWR_I_SNS\x10\x02\x12\x11\n\rAUX_PWR_I_SNS\x10\x03\x12\r\n\tBAT_I_SNS\x10\x04\x12\x12\n\x0eSHDN_PWR_I_SNS\x10\x05\x12\x0e\n\nVBAT_SENSE\x10\x06\x12\x12\n\x0e_24V_ACC_SENSE\x10\x07\x12\x14\n\x10_22V_BOOST_SENSE\x10\x08\x12\x10\n\x0cLV_PWR_I_SNS\x10\t\x12\x0f\n\x0bACC_I_SENSE\x10\n\x12\x12\n\x0ePUMP_PWR_I_SNS\x10\x0bb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'VC_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIONETNAME']._serialized_start = 17
    _globals['_GPIONETNAME']._serialized_end = 503
    _globals['_ADCNETNAME']._serialized_start = 506
    _globals['_ADCNETNAME']._serialized_end = 757