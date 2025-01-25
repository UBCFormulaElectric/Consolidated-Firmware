"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 28, 2, '', 'shared.proto')
_sym_db = _symbol_database.Default()
from . import f4dev_pb2 as f4dev__pb2
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cshared.proto\x1a\x0bf4dev.proto"C\n\x0bGpioNetName\x12,\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x12.f4dev.GpioNetNameH\x00B\x06\n\x04name"E\n\x08GpioRead\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\x19\n\x05value\x18\x02 \x01(\x0e2\n.GpioValue"F\n\tGpioWrite\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\x19\n\x05value\x18\x02 \x01(\x0e2\n.GpioValue"A\n\nAdcNetName\x12+\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x11.f4dev.AdcNetNameH\x00B\x06\n\x04name"3\n\x03Adc\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.AdcNetName\x12\r\n\x05value\x18\x02 \x01(\x02"p\n\x0cDebugMessage\x12\x1e\n\tgpio_read\x18\x01 \x01(\x0b2\t.GpioReadH\x00\x12 \n\ngpio_write\x18\x02 \x01(\x0b2\n.GpioWriteH\x00\x12\x13\n\x03adc\x18\x03 \x01(\x0b2\x04.AdcH\x00B\t\n\x07payload*:\n\tGpioValue\x12\x1a\n\x16GPIO_VALUE_UNSPECIFIED\x10\x00\x12\x07\n\x03LOW\x10\x01\x12\x08\n\x04HIGH\x10\x02b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'shared_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIOVALUE']._serialized_start = 475
    _globals['_GPIOVALUE']._serialized_end = 533
    _globals['_GPIONETNAME']._serialized_start = 29
    _globals['_GPIONETNAME']._serialized_end = 96
    _globals['_GPIOREAD']._serialized_start = 98
    _globals['_GPIOREAD']._serialized_end = 167
    _globals['_GPIOWRITE']._serialized_start = 169
    _globals['_GPIOWRITE']._serialized_end = 239
    _globals['_ADCNETNAME']._serialized_start = 241
    _globals['_ADCNETNAME']._serialized_end = 306
    _globals['_ADC']._serialized_start = 308
    _globals['_ADC']._serialized_end = 359
    _globals['_DEBUGMESSAGE']._serialized_start = 361
    _globals['_DEBUGMESSAGE']._serialized_end = 473