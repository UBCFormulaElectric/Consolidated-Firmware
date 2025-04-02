from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='bms.proto', package='bms', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\tbms.proto\x12\x03bms*\xaa\x05\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\r\n\tGPIO_LEDB\x10\x01\x12\x10\n\x0cGPIO_BSPD_OK\x10\x02\x12\x17\n\x13GPIO_ACCEL_BRAKE_OK\x10\x03\x12\x19\n\x15GPIO_nBRAKE_PRESS_3V3\x10\x04\x12\x1b\n\x17GPIO_nHIGH_CURRENT_BSPD\x10\x05\x12\x15\n\x11GPIO_MSD_SHDN_SNS\x10\x06\x12\x18\n\x14GPIO_HV_P_INTLCK_SNS\x10\x07\x12\x18\n\x14GPIO_HV_N_INTLCK_SNS\x10\x08\x12\x12\n\x0eGPIO_IMD_LATCH\x10\t\x12\r\n\tGPIO_DIAG\x10\n\x12\x0e\n\nGPIO_SD_CD\x10\x0b\x12\r\n\tGPIO_LEDR\x10\x0c\x12\x12\n\x0eGPIO_SPI_CS_LS\x10\r\x12\x14\n\x10GPIO_TSENSE_SEL0\x10\x0e\x12\x14\n\x10GPIO_TSENSE_SEL1\x10\x0f\x12\x14\n\x10GPIO_TSENSE_SEL2\x10\x10\x12\x15\n\x11GPIO_BSPD_TEST_EN\x10\x11\x12\x1e\n\x1aGPIO_TS_ISENSE_OCSC_OK_3V3\x10\x12\x12\x12\n\x0eGPIO_SPI_CS_HS\x10\x13\x12\x0f\n\x0bGPIO_BMS_OK\x10\x14\x12\x19\n\x15GPIO_nEVSE_I_LIM_GPIO\x10\x15\x12\x10\n\x0cGPIO_IR_P_EN\x10\x16\x12\x10\n\x0cGPIO_SHDN_EN\x10\x17\x12\x12\n\x0eGPIO_BMS_LATCH\x10\x18\x12\x0f\n\x0bGPIO_IMD_OK\x10\x19\x12\x13\n\x0fGPIO_BSPD_LATCH\x10\x1a\x12\x16\n\x12GPIO_PRE_CHARGE_EN\x10\x1b\x12\x15\n\x11GPIO_FB_STBY_SHDN\x10\x1c\x12\x0f\n\x0bGPIO_FAN_EN\x10\x1d\x12\x12\n\x0eGPIO_FAN_PGOOD\x10\x1e*\xbe\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x10\n\x0cADC_FAN_ISNS\x10\x01\x12\x13\n\x0fADC_TS_VSENSE_P\x10\x02\x12\x13\n\x0fADC_TS_VSENSE_N\x10\x03\x12\x11\n\rADC_SHDN_ISNS\x10\x04\x12\x18\n\x14ADC_EMETER_THERM_SNS\x10\x05\x12\x14\n\x10ADC_TS_ISNS_400A\x10\x06\x12\x13\n\x0fADC_TS_ISNS_50A\x10\x07*R\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00\x12\x12\n\x0eSPI_LTC6813_LS\x10\x01\x12\x12\n\x0eSPI_LTC6813_HS\x10\x02b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='bms.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LEDB', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BSPD_OK', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_ACCEL_BRAKE_OK', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_nBRAKE_PRESS_3V3', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_nHIGH_CURRENT_BSPD', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_MSD_SHDN_SNS', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_HV_P_INTLCK_SNS', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_HV_N_INTLCK_SNS', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMD_LATCH', index=9, number=9, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DIAG', index=10, number=10, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SD_CD', index=11, number=11, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LEDR', index=12, number=12, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SPI_CS_LS', index=13, number=13, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TSENSE_SEL0', index=14, number=14, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TSENSE_SEL1', index=15, number=15, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TSENSE_SEL2', index=16, number=16, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BSPD_TEST_EN', index=17, number=17, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TS_ISENSE_OCSC_OK_3V3', index=18, number=18, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SPI_CS_HS', index=19, number=19, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BMS_OK', index=20, number=20, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_nEVSE_I_LIM_GPIO', index=21, number=21, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IR_P_EN', index=22, number=22, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SHDN_EN', index=23, number=23, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BMS_LATCH', index=24, number=24, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMD_OK', index=25, number=25, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BSPD_LATCH', index=26, number=26, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_PRE_CHARGE_EN', index=27, number=27, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FB_STBY_SHDN', index=28, number=28, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FAN_EN', index=29, number=29, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FAN_PGOOD', index=30, number=30, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=19, serialized_end=701)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
_ADCNETNAME = _descriptor.EnumDescriptor(name='AdcNetName', full_name='bms.AdcNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='ADC_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_FAN_ISNS', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_TS_VSENSE_P', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_TS_VSENSE_N', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_SHDN_ISNS', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_EMETER_THERM_SNS', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_TS_ISNS_400A', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_TS_ISNS_50A', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=704, serialized_end=894)
_sym_db.RegisterEnumDescriptor(_ADCNETNAME)
AdcNetName = enum_type_wrapper.EnumTypeWrapper(_ADCNETNAME)
_SPINETNAME = _descriptor.EnumDescriptor(name='SpiNetName', full_name='bms.SpiNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='SPI_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='SPI_LTC6813_LS', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='SPI_LTC6813_HS', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=896, serialized_end=978)
_sym_db.RegisterEnumDescriptor(_SPINETNAME)
SpiNetName = enum_type_wrapper.EnumTypeWrapper(_SPINETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_LEDB = 1
GPIO_BSPD_OK = 2
GPIO_ACCEL_BRAKE_OK = 3
GPIO_nBRAKE_PRESS_3V3 = 4
GPIO_nHIGH_CURRENT_BSPD = 5
GPIO_MSD_SHDN_SNS = 6
GPIO_HV_P_INTLCK_SNS = 7
GPIO_HV_N_INTLCK_SNS = 8
GPIO_IMD_LATCH = 9
GPIO_DIAG = 10
GPIO_SD_CD = 11
GPIO_LEDR = 12
GPIO_SPI_CS_LS = 13
GPIO_TSENSE_SEL0 = 14
GPIO_TSENSE_SEL1 = 15
GPIO_TSENSE_SEL2 = 16
GPIO_BSPD_TEST_EN = 17
GPIO_TS_ISENSE_OCSC_OK_3V3 = 18
GPIO_SPI_CS_HS = 19
GPIO_BMS_OK = 20
GPIO_nEVSE_I_LIM_GPIO = 21
GPIO_IR_P_EN = 22
GPIO_SHDN_EN = 23
GPIO_BMS_LATCH = 24
GPIO_IMD_OK = 25
GPIO_BSPD_LATCH = 26
GPIO_PRE_CHARGE_EN = 27
GPIO_FB_STBY_SHDN = 28
GPIO_FAN_EN = 29
GPIO_FAN_PGOOD = 30
ADC_NET_NAME_UNSPECIFIED = 0
ADC_FAN_ISNS = 1
ADC_TS_VSENSE_P = 2
ADC_TS_VSENSE_N = 3
ADC_SHDN_ISNS = 4
ADC_EMETER_THERM_SNS = 5
ADC_TS_ISNS_400A = 6
ADC_TS_ISNS_50A = 7
SPI_NET_NAME_UNSPECIFIED = 0
SPI_LTC6813_LS = 1
SPI_LTC6813_HS = 2
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
DESCRIPTOR.enum_types_by_name['AdcNetName'] = _ADCNETNAME
DESCRIPTOR.enum_types_by_name['SpiNetName'] = _SPINETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)