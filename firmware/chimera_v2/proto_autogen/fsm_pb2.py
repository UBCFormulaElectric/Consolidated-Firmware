from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='fsm.proto', package='fsm', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\tfsm.proto\x12\x03fsm*\xb1\x02\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x11\n\rGPIO_BOTS_3v3\x10\x01\x12\x19\n\x15GPIO_COCKPIT_SHDN_3v3\x10\x02\x12\x11\n\rGPIO_IMU_INT1\x10\x03\x12\x11\n\rGPIO_IMU_INT2\x10\x04\x12\x16\n\x12GPIO_NSUSP_FL_OCSC\x10\x05\x12\x16\n\x12GPIO_NSUSP_FR_OCSC\x10\x06\x12\x13\n\x0fGPIO_FR_INT_3v3\x10\x07\x12\x13\n\x0fGPIO_FL_INT_3v3\x10\x08\x12\x12\n\x0eGPIO_DEBUG_LED\x10\t\x12\x18\n\x14GPIO_NSTR_ANGLE_OCSC\x10\n\x12\x14\n\x10GPIO_NBPS_F_OCSC\x10\x0b\x12\x11\n\rGPIO_BOOT_LED\x10\x0c*\x8c\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bADC_SUSP_FL\x10\x01\x12\x0f\n\x0bADC_SUSP_FR\x10\x02\x12\r\n\tADC_APPS2\x10\x03\x12\r\n\tADC_BPS_F\x10\x04\x12\x11\n\rADC_STR_ANGLE\x10\x05\x12\r\n\tADC_APPS1\x10\x06*7\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07I2C_IMU\x10\x01b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='fsm.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BOTS_3v3', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_COCKPIT_SHDN_3v3', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMU_INT1', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMU_INT2', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_NSUSP_FL_OCSC', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_NSUSP_FR_OCSC', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FR_INT_3v3', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_FL_INT_3v3', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DEBUG_LED', index=9, number=9, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_NSTR_ANGLE_OCSC', index=10, number=10, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_NBPS_F_OCSC', index=11, number=11, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BOOT_LED', index=12, number=12, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=19, serialized_end=324)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
_ADCNETNAME = _descriptor.EnumDescriptor(name='AdcNetName', full_name='fsm.AdcNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='ADC_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_SUSP_FL', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_SUSP_FR', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_APPS2', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_BPS_F', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_STR_ANGLE', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_APPS1', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=327, serialized_end=467)
_sym_db.RegisterEnumDescriptor(_ADCNETNAME)
AdcNetName = enum_type_wrapper.EnumTypeWrapper(_ADCNETNAME)
_I2CNETNAME = _descriptor.EnumDescriptor(name='I2cNetName', full_name='fsm.I2cNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='I2C_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_IMU', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=469, serialized_end=524)
_sym_db.RegisterEnumDescriptor(_I2CNETNAME)
I2cNetName = enum_type_wrapper.EnumTypeWrapper(_I2CNETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_BOTS_3v3 = 1
GPIO_COCKPIT_SHDN_3v3 = 2
GPIO_IMU_INT1 = 3
GPIO_IMU_INT2 = 4
GPIO_NSUSP_FL_OCSC = 5
GPIO_NSUSP_FR_OCSC = 6
GPIO_FR_INT_3v3 = 7
GPIO_FL_INT_3v3 = 8
GPIO_DEBUG_LED = 9
GPIO_NSTR_ANGLE_OCSC = 10
GPIO_NBPS_F_OCSC = 11
GPIO_BOOT_LED = 12
ADC_NET_NAME_UNSPECIFIED = 0
ADC_SUSP_FL = 1
ADC_SUSP_FR = 2
ADC_APPS2 = 3
ADC_BPS_F = 4
ADC_STR_ANGLE = 5
ADC_APPS1 = 6
I2C_NET_NAME_UNSPECIFIED = 0
I2C_IMU = 1
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
DESCRIPTOR.enum_types_by_name['AdcNetName'] = _ADCNETNAME
DESCRIPTOR.enum_types_by_name['I2cNetName'] = _I2CNETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)