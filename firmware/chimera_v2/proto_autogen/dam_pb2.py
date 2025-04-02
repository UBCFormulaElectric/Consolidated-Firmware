from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='dam.proto', package='dam', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\tdam.proto\x12\x03dam*\x9e\x02\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x15\n\x11GPIO_TELEM_PWR_EN\x10\x01\x12\x16\n\x12GPIO_BUZZER_PWR_EN\x10\x02\x12\x14\n\x10GPIO_TSIM_RED_EN\x10\x03\x12\x15\n\x11GPIO_NTSIM_GRN_EN\x10\x04\x12\x15\n\x11GPIO_R_SHDN_SENSE\x10\x05\x12\x15\n\x11GPIO_L_SHDN_SENSE\x10\x06\x12\x0c\n\x08GPIO_LED\x10\x07\x12\r\n\tGPIO_BOOT\x10\x08\x12\x13\n\x0fGPIO_SD_PRESENT\x10\t\x12\x13\n\x0fGPIO_N_RTC_INT1\x10\n\x12\r\n\tGPIO_900M\x10\x0b\x12\x10\n\x0cGPIO_SD_FAIL\x10\x0c*7\n\nI2cNetName\x12\x1c\n\x18I2C_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07I2C_RTC\x10\x01b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='dam.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TELEM_PWR_EN', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BUZZER_PWR_EN', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TSIM_RED_EN', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_NTSIM_GRN_EN', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_R_SHDN_SENSE', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_L_SHDN_SENSE', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BOOT', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SD_PRESENT', index=9, number=9, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_N_RTC_INT1', index=10, number=10, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_900M', index=11, number=11, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SD_FAIL', index=12, number=12, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=19, serialized_end=305)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
_I2CNETNAME = _descriptor.EnumDescriptor(name='I2cNetName', full_name='dam.I2cNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='I2C_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='I2C_RTC', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=307, serialized_end=362)
_sym_db.RegisterEnumDescriptor(_I2CNETNAME)
I2cNetName = enum_type_wrapper.EnumTypeWrapper(_I2CNETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_TELEM_PWR_EN = 1
GPIO_BUZZER_PWR_EN = 2
GPIO_TSIM_RED_EN = 3
GPIO_NTSIM_GRN_EN = 4
GPIO_R_SHDN_SENSE = 5
GPIO_L_SHDN_SENSE = 6
GPIO_LED = 7
GPIO_BOOT = 8
GPIO_SD_PRESENT = 9
GPIO_N_RTC_INT1 = 10
GPIO_900M = 11
GPIO_SD_FAIL = 12
I2C_NET_NAME_UNSPECIFIED = 0
I2C_RTC = 1
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
DESCRIPTOR.enum_types_by_name['I2cNetName'] = _I2CNETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)