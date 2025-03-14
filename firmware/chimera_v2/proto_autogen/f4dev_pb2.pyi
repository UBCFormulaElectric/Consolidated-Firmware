"""
@generated by mypy-protobuf.  Do not edit manually!
isort:skip_file
"""
import builtins
import google.protobuf.descriptor
import google.protobuf.internal.enum_type_wrapper
import sys
import typing
if sys.version_info >= (3, 10):
    import typing as typing_extensions
else:
    import typing_extensions
DESCRIPTOR: google.protobuf.descriptor.FileDescriptor

class _GpioNetName:
    ValueType = typing.NewType('ValueType', builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _GpioNetNameEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_GpioNetName.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    GPIO_NET_NAME_UNSPECIFIED: _GpioNetName.ValueType
    GPIO_6: _GpioNetName.ValueType
    GPIO_5: _GpioNetName.ValueType

class GpioNetName(_GpioNetName, metaclass=_GpioNetNameEnumTypeWrapper):
    ...
GPIO_NET_NAME_UNSPECIFIED: GpioNetName.ValueType
GPIO_6: GpioNetName.ValueType
GPIO_5: GpioNetName.ValueType
global___GpioNetName = GpioNetName

class _AdcNetName:
    ValueType = typing.NewType('ValueType', builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _AdcNetNameEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_AdcNetName.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    ADC_NET_NAME_UNSPECIFIED: _AdcNetName.ValueType

class AdcNetName(_AdcNetName, metaclass=_AdcNetNameEnumTypeWrapper):
    ...
ADC_NET_NAME_UNSPECIFIED: AdcNetName.ValueType
global___AdcNetName = AdcNetName

class _I2cNetName:
    ValueType = typing.NewType('ValueType', builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _I2cNetNameEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_I2cNetName.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    I2C_NET_NAME_UNSPECIFIED: _I2cNetName.ValueType
    I2C_3: _I2cNetName.ValueType

class I2cNetName(_I2cNetName, metaclass=_I2cNetNameEnumTypeWrapper):
    ...
I2C_NET_NAME_UNSPECIFIED: I2cNetName.ValueType
I2C_3: I2cNetName.ValueType
global___I2cNetName = I2cNetName