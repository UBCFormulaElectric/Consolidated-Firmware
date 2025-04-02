from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='crit.proto', package='crit', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\ncrit.proto\x12\x04crit*\xac\x03\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\r\n\tGPIO_BOOT\x10\x01\x12\x0c\n\x08GPIO_LED\x10\x02\x12\x12\n\x0eGPIO_TELEM_SIG\x10\x03\x12\x17\n\x13GPIO_PUSH_DRIVE_SIG\x10\x04\x12\x0e\n\nGPIO_ROT_S\x10\x05\x12\x0e\n\nGPIO_ROT_B\x10\x06\x12\x0e\n\nGPIO_ROT_A\x10\x07\x12\x1b\n\x17GPIO_LAUNCH_CONTROL_SIG\x10\x08\x12\x1d\n\x19GPIO_TORQUE_VECTORING_SIG\x10\t\x12\x12\n\x0eGPIO_REGEN_SIG\x10\n\x12\x10\n\x0cGPIO_LED_RCK\x10\x0b\x12\x16\n\x12GPIO_SEVEN_SEG_RCK\x10\x0c\x12\x14\n\x10GPIO_LED_DIMMING\x10\r\x12\x1a\n\x16GPIO_SEVEN_SEG_DIMMING\x10\x0e\x12\x12\n\x0eGPIO_LED_SERIN\x10\x0f\x12\x18\n\x14GPIO_SEVEN_SEG_SERIN\x10\x10\x12\x11\n\rGPIO_LED_SRCK\x10\x11\x12\x17\n\x13GPIO_SEVEN_SEG_SRCK\x10\x12b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='crit.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BOOT', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TELEM_SIG', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_PUSH_DRIVE_SIG', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_ROT_S', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_ROT_B', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_ROT_A', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LAUNCH_CONTROL_SIG', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_TORQUE_VECTORING_SIG', index=9, number=9, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_REGEN_SIG', index=10, number=10, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED_RCK', index=11, number=11, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SEVEN_SEG_RCK', index=12, number=12, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED_DIMMING', index=13, number=13, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SEVEN_SEG_DIMMING', index=14, number=14, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED_SERIN', index=15, number=15, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SEVEN_SEG_SERIN', index=16, number=16, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_LED_SRCK', index=17, number=17, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_SEVEN_SEG_SRCK', index=18, number=18, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=21, serialized_end=449)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_BOOT = 1
GPIO_LED = 2
GPIO_TELEM_SIG = 3
GPIO_PUSH_DRIVE_SIG = 4
GPIO_ROT_S = 5
GPIO_ROT_B = 6
GPIO_ROT_A = 7
GPIO_LAUNCH_CONTROL_SIG = 8
GPIO_TORQUE_VECTORING_SIG = 9
GPIO_REGEN_SIG = 10
GPIO_LED_RCK = 11
GPIO_SEVEN_SEG_RCK = 12
GPIO_LED_DIMMING = 13
GPIO_SEVEN_SEG_DIMMING = 14
GPIO_LED_SERIN = 15
GPIO_SEVEN_SEG_SERIN = 16
GPIO_LED_SRCK = 17
GPIO_SEVEN_SEG_SRCK = 18
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)