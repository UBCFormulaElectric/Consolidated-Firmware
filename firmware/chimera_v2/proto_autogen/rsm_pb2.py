from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='rsm.proto', package='rsm', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\trsm.proto\x12\x03rsm*\xd5\x01\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0c\n\x08GPIO_LED\x10\x01\x12\x17\n\x13GPIO_BRAKE_LIGHT_EN\x10\x02\x12\x0f\n\x0bGPIO_RL_INT\x10\x03\x12\x11\n\rGPIO_IMU_INT1\x10\x04\x12\x11\n\rGPIO_IMU_INT2\x10\x05\x12\r\n\tGPIO_BOOT\x10\x06\x12\x1c\n\x18GPIO_SUSP_TRAVEL_RR_OCSC\x10\x07\x12\x1c\n\x18GPIO_SUSP_TRAVEL_RL_OCSC\x10\x08*\x84\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bADC_LC3_OUT\x10\x01\x12\x1a\n\x16ADC_SUSP_TRAVEL_RL_3V3\x10\x02\x12\x1a\n\x16ADC_SUSP_TRAVEL_RR_3V3\x10\x03\x12\x0f\n\x0bADC_BPS_3V3\x10\x04*O\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bI2C_IMU_I2C\x10\x01\x12\x12\n\x0eI2C_R_PUMP_I2C\x10\x02b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='rsm.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BRAKE_LIGHT_EN', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_RL_INT', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMU_INT1', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_IMU_INT2', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BOOT', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SUSP_TRAVEL_RR_OCSC', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SUSP_TRAVEL_RL_OCSC', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=19, serialized_end=232)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
_ADCNETNAME = _descriptor.EnumDescriptor(name='AdcNetName', full_name='rsm.AdcNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='ADC_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_LC3_OUT', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_SUSP_TRAVEL_RL_3V3', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_SUSP_TRAVEL_RR_3V3', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='ADC_BPS_3V3', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=235, serialized_end=367)
_sym_db.RegisterEnumDescriptor(_ADCNETNAME)
AdcNetName = enum_type_wrapper.EnumTypeWrapper(_ADCNETNAME)
_I2CNETNAME = _descriptor.EnumDescriptor(name='I2cNetName', full_name='rsm.I2cNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='I2C_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_IMU_I2C', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_R_PUMP_I2C', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=369, serialized_end=448)
_sym_db.RegisterEnumDescriptor(_I2CNETNAME)
I2cNetName = enum_type_wrapper.EnumTypeWrapper(_I2CNETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_LED = 1
GPIO_BRAKE_LIGHT_EN = 2
GPIO_RL_INT = 3
GPIO_IMU_INT1 = 4
GPIO_IMU_INT2 = 5
GPIO_BOOT = 6
GPIO_SUSP_TRAVEL_RR_OCSC = 7
GPIO_SUSP_TRAVEL_RL_OCSC = 8
ADC_NET_NAME_UNSPECIFIED = 0
ADC_LC3_OUT = 1
ADC_SUSP_TRAVEL_RL_3V3 = 2
ADC_SUSP_TRAVEL_RR_3V3 = 3
ADC_BPS_3V3 = 4
I2C_NET_NAME_UNSPECIFIED = 0
I2C_IMU_I2C = 1
I2C_R_PUMP_I2C = 2
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
DESCRIPTOR.enum_types_by_name['AdcNetName'] = _ADCNETNAME
DESCRIPTOR.enum_types_by_name['I2cNetName'] = _I2CNETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)