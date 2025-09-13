"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 4, '', 'bms.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tbms.proto\x12\x03bms*\xaa\x05\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\r\n\tGPIO_LEDB\x10\x01\x12\x10\n\x0cGPIO_BSPD_OK\x10\x02\x12\x17\n\x13GPIO_ACCEL_BRAKE_OK\x10\x03\x12\x19\n\x15GPIO_nBRAKE_PRESS_3V3\x10\x04\x12\x1b\n\x17GPIO_nHIGH_CURRENT_BSPD\x10\x05\x12\x15\n\x11GPIO_MSD_SHDN_SNS\x10\x06\x12\x18\n\x14GPIO_HV_P_INTLCK_SNS\x10\x07\x12\x18\n\x14GPIO_HV_N_INTLCK_SNS\x10\x08\x12\x12\n\x0eGPIO_IMD_LATCH\x10\t\x12\r\n\tGPIO_DIAG\x10\n\x12\x0e\n\nGPIO_SD_CD\x10\x0b\x12\r\n\tGPIO_LEDR\x10\x0c\x12\x12\n\x0eGPIO_SPI_CS_LS\x10\r\x12\x14\n\x10GPIO_TSENSE_SEL0\x10\x0e\x12\x14\n\x10GPIO_TSENSE_SEL1\x10\x0f\x12\x14\n\x10GPIO_TSENSE_SEL2\x10\x10\x12\x15\n\x11GPIO_BSPD_TEST_EN\x10\x11\x12\x1e\n\x1aGPIO_TS_ISENSE_OCSC_OK_3V3\x10\x12\x12\x12\n\x0eGPIO_SPI_CS_HS\x10\x13\x12\x0f\n\x0bGPIO_BMS_OK\x10\x14\x12\x19\n\x15GPIO_nEVSE_I_LIM_GPIO\x10\x15\x12\x10\n\x0cGPIO_IR_P_EN\x10\x16\x12\x10\n\x0cGPIO_SHDN_EN\x10\x17\x12\x12\n\x0eGPIO_BMS_LATCH\x10\x18\x12\x0f\n\x0bGPIO_IMD_OK\x10\x19\x12\x13\n\x0fGPIO_BSPD_LATCH\x10\x1a\x12\x16\n\x12GPIO_PRE_CHARGE_EN\x10\x1b\x12\x15\n\x11GPIO_FB_STBY_SHDN\x10\x1c\x12\x0f\n\x0bGPIO_FAN_EN\x10\x1d\x12\x12\n\x0eGPIO_FAN_PGOOD\x10\x1e*\xbe\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x10\n\x0cADC_FAN_ISNS\x10\x01\x12\x13\n\x0fADC_TS_VSENSE_P\x10\x02\x12\x13\n\x0fADC_TS_VSENSE_N\x10\x03\x12\x11\n\rADC_SHDN_ISNS\x10\x04\x12\x18\n\x14ADC_EMETER_THERM_SNS\x10\x05\x12\x14\n\x10ADC_TS_ISNS_400A\x10\x06\x12\x13\n\x0fADC_TS_ISNS_50A\x10\x07*R\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00\x12\x12\n\x0eSPI_LTC6813_LS\x10\x01\x12\x12\n\x0eSPI_LTC6813_HS\x10\x02*;\n\nPwmNetName\x12\x1c\n\x18PWM_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bPWM_CHARGER\x10\x01b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'bms_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 701
    _globals['_ADCNETNAME']._serialized_start = 704
    _globals['_ADCNETNAME']._serialized_end = 894
    _globals['_SPINETNAME']._serialized_start = 896
    _globals['_SPINETNAME']._serialized_end = 978
    _globals['_PWMNETNAME']._serialized_start = 980
    _globals['_PWMNETNAME']._serialized_end = 1039