"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 2, '', 'vc.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x08vc.proto\x12\x02vc*\xc6\x05\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x18\n\x14GPIO_SB_SHDN_3V3_OUT\x10\x01\x12\x18\n\x14GPIO_INTERIA_3V3_OUT\x10\x02\x12\x14\n\x10GPIO_FR_STBY_INV\x10\x03\x12\x15\n\x11GPIO_TSMS_3V3_OUT\x10\x04\x12\x18\n\x14GPIO_RR_ILCK_3V3_OUT\x10\x05\x12\x11\n\rGPIO_R_INV_EN\x10\x06\x12\x0c\n\x08GPIO_LED\x10\x07\x12\x11\n\rGPIO_BOOT_LED\x10\x08\x12\x0f\n\x0bGPIO_BMS_EN\x10\t\x12\x16\n\x12GPIO_FR_STBY_FRONT\x10\n\x12\x0e\n\nGPIO_PGOOD\x10\x0b\x12\x16\n\x12GPIO_BAT_CHRG_MODE\x10\x0c\x12\x17\n\x13GPIO_BAT_CHRG_nSHDN\x10\r\x12\x17\n\x13GPIO_BAT_MTR_nALERT\x10\x0e\x12\x15\n\x11GPIO_FR_STBY_REAR\x10\x0f\x12\x11\n\rGPIO_FRONT_EN\x10\x10\x12\x0f\n\x0bGPIO_RSM_EN\x10\x11\x12\x11\n\rGPIO_IMU_INT1\x10\x12\x12\x11\n\rGPIO_IMU_INT2\x10\x13\x12\x18\n\x14GPIO_RAD_FAN_FR_STBY\x10\x14\x12\x17\n\x13GPIO_PWR_MTR_nALERT\x10\x15\x12\x16\n\x12GPIO_RL_PUMP_PGOOD\x10\x16\x12\x11\n\rGPIO_F_INV_EN\x10\x17\x12\x0f\n\x0bGPIO_DAM_EN\x10\x18\x12\x13\n\x0fGPIO_RR_PUMP_EN\x10\x19\x12\x16\n\x12GPIO_RR_PUMP_PGOOD\x10\x1a\x12\x15\n\x11GPIO_F_PUMP_PGOOD\x10\x1b\x12\x0f\n\x0bGPIO_PCM_EN\x10\x1c\x12\x15\n\x11GPIO_R_RAD_FAN_EN\x10\x1d\x12\x13\n\x0fGPIO_RL_PUMP_EN\x10\x1e\x12\x15\n\x11GPIO_L_RAD_FAN_EN\x10\x1f*\xd6\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0e\n\nADC_F_PUMP\x10\x01\x12\x0f\n\x0bADC_RR_PUMP\x10\x02\x12\x0b\n\x07ADC_DAM\x10\x03\x12\x11\n\rADC_L_RAD_FAN\x10\x04\x12\x11\n\rADC_R_RAD_FAN\x10\x05\x12\x0f\n\x0bADC_RL_PUMP\x10\x06\x12\r\n\tADC_F_INV\x10\x07\x12\r\n\tADC_R_INV\x10\x08\x12\x0b\n\x07ADC_BMS\x10\t\x12\r\n\tADC_FRONT\x10\n\x12\x0b\n\x07ADC_RSM\x10\x0b*q\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bI2C_IMU_I2C\x10\x01\x12\x12\n\x0eI2C_R_PUMP_I2C\x10\x02\x12\x0f\n\x0bI2C_PWR_MTR\x10\x03\x12\x0f\n\x0bI2C_BAT_MTR\x10\x04b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'vc_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 17
    _globals['_GPIONETNAME']._serialized_end = 727
    _globals['_ADCNETNAME']._serialized_start = 730
    _globals['_ADCNETNAME']._serialized_end = 944
    _globals['_I2CNETNAME']._serialized_start = 946
    _globals['_I2CNETNAME']._serialized_end = 1059