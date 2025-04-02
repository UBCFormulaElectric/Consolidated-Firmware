from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='f4dev.proto', package='f4dev', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\x0bf4dev.proto\x12\x05f4dev*D\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\n\n\x06GPIO_6\x10\x01\x12\n\n\x06GPIO_5\x10\x02b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='f4dev.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_6', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_5', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=22, serialized_end=90)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_6 = 1
GPIO_5 = 2
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)