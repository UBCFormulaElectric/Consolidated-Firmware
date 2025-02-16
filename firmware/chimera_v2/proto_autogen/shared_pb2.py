"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 28, 2, '', 'shared.proto')
_sym_db = _symbol_database.Default()
from . import f4dev_pb2 as f4dev__pb2
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cshared.proto\x1a\x0bf4dev.proto"C\n\x0bGpioNetName\x12,\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x12.f4dev.GpioNetNameH\x00B\x06\n\x04name"1\n\x0fGpioReadRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName"A\n\x10GpioReadResponse\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\r\n\x05value\x18\x02 \x01(\x08"A\n\x10GpioWriteRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\r\n\x05value\x18\x02 \x01(\x08"D\n\x11GpioWriteResponse\x12/\n\x14request_confirmation\x18\x02 \x01(\x0b2\x11.GpioWriteRequest"A\n\nAdcNetName\x12+\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x11.f4dev.AdcNetNameH\x00B\x06\n\x04name"/\n\x0eAdcReadRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.AdcNetName"?\n\x0fAdcReadResponse\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.AdcNetName\x12\r\n\x05value\x18\x02 \x01(\x02"A\n\nI2cNetName\x12+\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x11.f4dev.I2cNetNameH\x00B\x06\n\x04name"\x92\x01\n\x10ChimeraV2Request\x12%\n\tgpio_read\x18\x01 \x01(\x0b2\x10.GpioReadRequestH\x00\x12\'\n\ngpio_write\x18\x02 \x01(\x0b2\x11.GpioWriteRequestH\x00\x12#\n\x08adc_read\x18\x03 \x01(\x0b2\x0f.AdcReadRequestH\x00B\t\n\x07payload"\x96\x01\n\x11ChimeraV2Response\x12&\n\tgpio_read\x18\x01 \x01(\x0b2\x11.GpioReadResponseH\x00\x12(\n\ngpio_write\x18\x02 \x01(\x0b2\x12.GpioWriteResponseH\x00\x12$\n\x08adc_read\x18\x03 \x01(\x0b2\x10.AdcReadResponseH\x00B\t\n\x07payloadb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'shared_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 29
    _globals['_GPIONETNAME']._serialized_end = 96
    _globals['_GPIOREADREQUEST']._serialized_start = 98
    _globals['_GPIOREADREQUEST']._serialized_end = 147
    _globals['_GPIOREADRESPONSE']._serialized_start = 149
    _globals['_GPIOREADRESPONSE']._serialized_end = 214
    _globals['_GPIOWRITEREQUEST']._serialized_start = 216
    _globals['_GPIOWRITEREQUEST']._serialized_end = 281
    _globals['_GPIOWRITERESPONSE']._serialized_start = 283
    _globals['_GPIOWRITERESPONSE']._serialized_end = 351
    _globals['_ADCNETNAME']._serialized_start = 353
    _globals['_ADCNETNAME']._serialized_end = 418
    _globals['_ADCREADREQUEST']._serialized_start = 420
    _globals['_ADCREADREQUEST']._serialized_end = 467
    _globals['_ADCREADRESPONSE']._serialized_start = 469
    _globals['_ADCREADRESPONSE']._serialized_end = 532
    _globals['_I2CNETNAME']._serialized_start = 534
    _globals['_I2CNETNAME']._serialized_end = 599
    _globals['_CHIMERAV2REQUEST']._serialized_start = 602
    _globals['_CHIMERAV2REQUEST']._serialized_end = 748
    _globals['_CHIMERAV2RESPONSE']._serialized_start = 751
    _globals['_CHIMERAV2RESPONSE']._serialized_end = 901