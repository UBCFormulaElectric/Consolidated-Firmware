from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(name='ssm.proto', package='ssm', syntax='proto3', serialized_options=None, create_key=_descriptor._internal_create_key, serialized_pb=b'\n\tssm.proto\x12\x03ssm*\xc7\x02\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bGPIO_DOUT_1\x10\x01\x12\x0f\n\x0bGPIO_DOUT_2\x10\x02\x12\x0f\n\x0bGPIO_DOUT_3\x10\x03\x12\x0f\n\x0bGPIO_DOUT_4\x10\x04\x12\x12\n\x0eGPIO_DEBUG_LED\x10\x05\x12\x11\n\rGPIO_BOOT_LED\x10\x06\x12\x14\n\x10GPIO_INDICATOR_1\x10\x07\x12\x14\n\x10GPIO_INDICATOR_2\x10\x08\x12\x14\n\x10GPIO_INDICATOR_3\x10\t\x12\x14\n\x10GPIO_INTERLOCK_1\x10\n\x12\x14\n\x10GPIO_INTERLOCK_2\x10\x0b\x12\x14\n\x10GPIO_INTERLOCK_3\x10\x0c\x12\x14\n\x10GPIO_INTERLOCK_4\x10\r\x12\x14\n\x10GPIO_DAC_N_CLEAR\x10\x0e*]\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00\x12\x11\n\rSPI_ISOSPI_LS\x10\x01\x12\x11\n\rSPI_ISOSPI_HS\x10\x02\x12\x0b\n\x07SPI_DAC\x10\x03b\x06proto3')
_GPIONETNAME = _descriptor.EnumDescriptor(name='GpioNetName', full_name='ssm.GpioNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='GPIO_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DOUT_1', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DOUT_2', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DOUT_3', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DOUT_4', index=4, number=4, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DEBUG_LED', index=5, number=5, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_BOOT_LED', index=6, number=6, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INDICATOR_1', index=7, number=7, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INDICATOR_2', index=8, number=8, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INDICATOR_3', index=9, number=9, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INTERLOCK_1', index=10, number=10, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INTERLOCK_2', index=11, number=11, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INTERLOCK_3', index=12, number=12, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_INTERLOCK_4', index=13, number=13, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='GPIO_DAC_N_CLEAR', index=14, number=14, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=19, serialized_end=346)
_sym_db.RegisterEnumDescriptor(_GPIONETNAME)
GpioNetName = enum_type_wrapper.EnumTypeWrapper(_GPIONETNAME)
_SPINETNAME = _descriptor.EnumDescriptor(name='SpiNetName', full_name='ssm.SpiNetName', filename=None, file=DESCRIPTOR, create_key=_descriptor._internal_create_key, values=[_descriptor.EnumValueDescriptor(name='SPI_NET_NAME_UNSPECIFIED', index=0, number=0, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='SPI_ISOSPI_LS', index=1, number=1, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='SPI_ISOSPI_HS', index=2, number=2, serialized_options=None, type=None, create_key=_descriptor._internal_create_key), _descriptor.EnumValueDescriptor(name='SPI_DAC', index=3, number=3, serialized_options=None, type=None, create_key=_descriptor._internal_create_key)], containing_type=None, serialized_options=None, serialized_start=348, serialized_end=441)
_sym_db.RegisterEnumDescriptor(_SPINETNAME)
SpiNetName = enum_type_wrapper.EnumTypeWrapper(_SPINETNAME)
GPIO_NET_NAME_UNSPECIFIED = 0
GPIO_DOUT_1 = 1
GPIO_DOUT_2 = 2
GPIO_DOUT_3 = 3
GPIO_DOUT_4 = 4
GPIO_DEBUG_LED = 5
GPIO_BOOT_LED = 6
GPIO_INDICATOR_1 = 7
GPIO_INDICATOR_2 = 8
GPIO_INDICATOR_3 = 9
GPIO_INTERLOCK_1 = 10
GPIO_INTERLOCK_2 = 11
GPIO_INTERLOCK_3 = 12
GPIO_INTERLOCK_4 = 13
GPIO_DAC_N_CLEAR = 14
SPI_NET_NAME_UNSPECIFIED = 0
SPI_ISOSPI_LS = 1
SPI_ISOSPI_HS = 2
SPI_DAC = 3
DESCRIPTOR.enum_types_by_name['GpioNetName'] = _GPIONETNAME
DESCRIPTOR.enum_types_by_name['SpiNetName'] = _SPINETNAME
_sym_db.RegisterFileDescriptor(DESCRIPTOR)