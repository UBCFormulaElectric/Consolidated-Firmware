from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='vc.proto', package='vc', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\x08vc.proto\x12\x02vc*\xc6\x05\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x18\n\x14GPIO_SB_SHDN_3V3_OUT\x10\x01\x12\x18\n\x14GPIO_INTERIA_3V3_OUT\x10\x02\x12\x14\n\x10GPIO_FR_STBY_INV\x10\x03\x12\x15\n\x11GPIO_TSMS_3V3_OUT\x10\x04\x12\x18\n\x14GPIO_RR_ILCK_3V3_OUT\x10\x05\x12\x11\n\rGPIO_R_INV_EN\x10\x06\x12\x0c\n\x08GPIO_LED\x10\x07\x12\x11\n\rGPIO_BOOT_LED\x10\x08\x12\x0f\n\x0bGPIO_BMS_EN\x10\t\x12\x16\n\x12GPIO_FR_STBY_FRONT\x10\n\x12\x0e\n\nGPIO_PGOOD\x10\x0b\x12\x16\n\x12GPIO_BAT_CHRG_MODE\x10\x0c\x12\x17\n\x13GPIO_BAT_CHRG_nSHDN\x10\r\x12\x17\n\x13GPIO_BAT_MTR_nALERT\x10\x0e\x12\x15\n\x11GPIO_FR_STBY_REAR\x10\x0f\x12\x11\n\rGPIO_FRONT_EN\x10\x10\x12\x0f\n\x0bGPIO_RSM_EN\x10\x11\x12\x11\n\rGPIO_IMU_INT1\x10\x12\x12\x11\n\rGPIO_IMU_INT2\x10\x13\x12\x18\n\x14GPIO_RAD_FAN_FR_STBY\x10\x14\x12\x17\n\x13GPIO_PWR_MTR_nALERT\x10\x15\x12\x16\n\x12GPIO_RL_PUMP_PGOOD\x10\x16\x12\x11\n\rGPIO_F_INV_EN\x10\x17\x12\x0f\n\x0bGPIO_DAM_EN\x10\x18\x12\x13\n\x0fGPIO_RR_PUMP_EN\x10\x19\x12\x16\n\x12GPIO_RR_PUMP_PGOOD\x10\x1a\x12\x15\n\x11GPIO_F_PUMP_PGOOD\x10\x1b\x12\x0f\n\x0bGPIO_PCM_EN\x10\x1c\x12\x15\n\x11GPIO_R_RAD_FAN_EN\x10\x1d\x12\x13\n\x0fGPIO_RL_PUMP_EN\x10\x1e\x12\x15\n\x11GPIO_L_RAD_FAN_EN\x10\x1f*\xd6\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0e\n\nADC_F_PUMP\x10\x01\x12\x0f\n\x0bADC_RR_PUMP\x10\x02\x12\x0b\n\x07ADC_DAM\x10\x03\x12\x11\n\rADC_L_RAD_FAN\x10\x04\x12\x11\n\rADC_R_RAD_FAN\x10\x05\x12\x0f\n\x0bADC_RL_PUMP\x10\x06\x12\r\n\tADC_F_INV\x10\x07\x12\r\n\tADC_R_INV\x10\x08\x12\x0b\n\x07ADC_BMS\x10\t\x12\r\n\tADC_FRONT\x10\n\x12\x0b\n\x07ADC_RSM\x10\x0b*q\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bI2C_IMU_I2C\x10\x01\x12\x12\n\x0eI2C_R_PUMP_I2C\x10\x02\x12\x0f\n\x0bI2C_PWR_MTR\x10\x03\x12\x0f\n\x0bI2C_BAT_MTR\x10\x04b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='vc.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SB_SHDN_3V3_OUT', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INTERIA_3V3_OUT', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FR_STBY_INV', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TSMS_3V3_OUT', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RR_ILCK_3V3_OUT', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_R_INV_EN', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BOOT_LED', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BMS_EN', index=9, number=9, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FR_STBY_FRONT', index=10, number=10, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_PGOOD', index=11, number=11, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BAT_CHRG_MODE', index=12, number=12, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BAT_CHRG_nSHDN', index=13, number=13, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BAT_MTR_nALERT', index=14, number=14, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FR_STBY_REAR', index=15, number=15, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FRONT_EN', index=16, number=16, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RSM_EN', index=17, number=17, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMU_INT1', index=18, number=18, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMU_INT2', index=19, number=19, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RAD_FAN_FR_STBY', index=20, number=20, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_PWR_MTR_nALERT', index=21, number=21, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RL_PUMP_PGOOD', index=22, number=22, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_F_INV_EN', index=23, number=23, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DAM_EN', index=24, number=24, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RR_PUMP_EN', index=25, number=25, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RR_PUMP_PGOOD', index=26, number=26, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_F_PUMP_PGOOD', index=27, number=27, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_PCM_EN', index=28, number=28, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_R_RAD_FAN_EN', index=29, number=29, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RL_PUMP_EN', index=30, number=30, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_L_RAD_FAN_EN', index=31, number=31, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=17, serialized_end=727)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
_ADCNETNAME = _descriptor.EnumDescriptor(name='AdcNetName', full_name='vc.AdcNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='ADC_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_F_PUMP', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_RR_PUMP', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_DAM', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_L_RAD_FAN', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_R_RAD_FAN', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_RL_PUMP', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_F_INV', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_R_INV', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_BMS', index=9, number=9, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_FRONT', index=10, number=10, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_RSM', index=11, number=11, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=730, serialized_end=944)
_sym_db.RegisterEnumDescriptor(_ADCNETNAME)
AdcNetName = enum_type_wrapper.EnumTypeWrapper(_ADCNETNAME)
_I2CNETNAME = _descriptor.EnumDescriptor(name='I2cNetName', full_name='vc.I2cNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='I2C_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_IMU_I2C', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_R_PUMP_I2C', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_PWR_MTR', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_BAT_MTR', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=946, serialized_end=1059)
_sym_db.RegisterEnumDescriptor(_I2CNETNAME)
I2cNetName = enum_type_wrapper.EnumTypeWrapper(_I2CNETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_SB_SHDN_3V3_OUT = 1
GPIO_INTERIA_3V3_OUT = 2
GPIO_FR_STBY_INV = 3
GPIO_TSMS_3V3_OUT = 4
GPIO_RR_ILCK_3V3_OUT = 5
GPIO_R_INV_EN = 6
GPIO_LED = 7
GPIO_BOOT_LED = 8
GPIO_BMS_EN = 9
GPIO_FR_STBY_FRONT = 10
GPIO_PGOOD = 11
GPIO_BAT_CHRG_MODE = 12
GPIO_BAT_CHRG_nSHDN = 13
GPIO_BAT_MTR_nALERT = 14
GPIO_FR_STBY_REAR = 15
GPIO_FRONT_EN = 16
GPIO_RSM_EN = 17
GPIO_IMU_INT1 = 18
GPIO_IMU_INT2 = 19
GPIO_RAD_FAN_FR_STBY = 20
GPIO_PWR_MTR_nALERT = 21
GPIO_RL_PUMP_PGOOD = 22
GPIO_F_INV_EN = 23
GPIO_DAM_EN = 24
GPIO_RR_PUMP_EN = 25
GPIO_RR_PUMP_PGOOD = 26
GPIO_F_PUMP_PGOOD = 27
GPIO_PCM_EN = 28
GPIO_R_RAD_FAN_EN = 29
GPIO_RL_PUMP_EN = 30
GPIO_L_RAD_FAN_EN = 31
ADC_NET_NAME_UNSPECIFIED = 0
ADC_F_PUMP = 1
ADC_RR_PUMP = 2
ADC_DAM = 3
ADC_L_RAD_FAN = 4
ADC_R_RAD_FAN = 5
ADC_RL_PUMP = 6
ADC_F_INV = 7
ADC_R_INV = 8
ADC_BMS = 9
ADC_FRONT = 10
ADC_RSM = 11
I2C_NET_NAME_UNSPECIFIED = 0
I2C_IMU_I2C = 1
I2C_R_PUMP_I2C = 2
I2C_PWR_MTR = 3
I2C_BAT_MTR = 4
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
DESCRIPTOR.enum_types_by_name['AdcNetName'] = _ADCNETNAME
DESCRIPTOR.enum_types_by_name['I2cNetName'] = _I2CNETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)