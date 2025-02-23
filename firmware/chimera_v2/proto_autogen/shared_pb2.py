"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 28, 2, '', 'shared.proto')
_sym_db = _symbol_database.Default()
from . import f4dev_pb2 as f4dev__pb2
from . import nanopb_pb2 as nanopb__pb2
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cshared.proto\x1a\x0bf4dev.proto\x1a\x0cnanopb.proto"C\n\x0bGpioNetName\x12,\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x12.f4dev.GpioNetNameH\x00B\x06\n\x04name"1\n\x0fGpioReadRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName"!\n\x10GpioReadResponse\x12\r\n\x05value\x18\x01 \x01(\x08"A\n\x10GpioWriteRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\r\n\x05value\x18\x02 \x01(\x08"$\n\x11GpioWriteResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"A\n\nAdcNetName\x12+\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x11.f4dev.AdcNetNameH\x00B\x06\n\x04name"/\n\x0eAdcReadRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.AdcNetName" \n\x0fAdcReadResponse\x12\r\n\x05value\x18\x01 \x01(\x02"A\n\nI2cNetName\x12+\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x11.f4dev.I2cNetNameH\x00B\x06\n\x04name"\\\n\x0fI2cReadyRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0edevice_address\x18\x02 \x01(\r\x12\x12\n\ntimeout_ms\x18\x03 \x01(\r"!\n\x10I2cReadyResponse\x12\r\n\x05ready\x18\x01 \x01(\x08"n\n\x11I2cReceiveRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0edevice_address\x18\x02 \x01(\r\x12\x0e\n\x06length\x18\x03 \x01(\r\x12\x12\n\ntimeout_ms\x18\x04 \x01(\r"*\n\x12I2cReceiveResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x08"u\n\x12I2cTransmitRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0edevice_address\x18\x02 \x01(\r\x12\x14\n\x04data\x18\x03 \x01(\x0cB\x06\x92?\x03\x08\x80\x08\x12\x12\n\ntimeout_ms\x18\x04 \x01(\r"&\n\x13I2cTransmitResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"\x89\x01\n\x14I2cMemoryReadRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0edevice_address\x18\x02 \x01(\r\x12\x16\n\x0ememory_address\x18\x03 \x01(\r\x12\x0e\n\x06length\x18\x04 \x01(\r\x12\x12\n\ntimeout_ms\x18\x05 \x01(\r"-\n\x15I2cMemoryReadResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x08"\x90\x01\n\x15I2cMemoryWriteRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0edevice_address\x18\x02 \x01(\r\x12\x16\n\x0ememory_address\x18\x03 \x01(\r\x12\x14\n\x04data\x18\x04 \x01(\x0cB\x06\x92?\x03\x08\x80\x08\x12\x12\n\ntimeout_ms\x18\x05 \x01(\r")\n\x16I2cMemoryWriteResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"\xf7\x02\n\x10ChimeraV2Request\x12%\n\tgpio_read\x18\x01 \x01(\x0b2\x10.GpioReadRequestH\x00\x12\'\n\ngpio_write\x18\x02 \x01(\x0b2\x11.GpioWriteRequestH\x00\x12#\n\x08adc_read\x18\x03 \x01(\x0b2\x0f.AdcReadRequestH\x00\x12%\n\ti2c_ready\x18\x04 \x01(\x0b2\x10.I2cReadyRequestH\x00\x12)\n\x0bi2c_receive\x18\x05 \x01(\x0b2\x12.I2cReceiveRequestH\x00\x12+\n\x0ci2c_transmit\x18\x06 \x01(\x0b2\x13.I2cTransmitRequestH\x00\x120\n\x0fi2c_memory_read\x18\x07 \x01(\x0b2\x15.I2cMemoryReadRequestH\x00\x122\n\x10i2c_memory_write\x18\x08 \x01(\x0b2\x16.I2cMemoryWriteRequestH\x00B\t\n\x07payload"\x80\x03\n\x11ChimeraV2Response\x12&\n\tgpio_read\x18\x01 \x01(\x0b2\x11.GpioReadResponseH\x00\x12(\n\ngpio_write\x18\x02 \x01(\x0b2\x12.GpioWriteResponseH\x00\x12$\n\x08adc_read\x18\x03 \x01(\x0b2\x10.AdcReadResponseH\x00\x12&\n\ti2c_ready\x18\x04 \x01(\x0b2\x11.I2cReadyResponseH\x00\x12*\n\x0bi2c_receive\x18\x05 \x01(\x0b2\x13.I2cReceiveResponseH\x00\x12,\n\x0ci2c_transmit\x18\x06 \x01(\x0b2\x14.I2cTransmitResponseH\x00\x121\n\x0fi2c_memory_read\x18\x07 \x01(\x0b2\x16.I2cMemoryReadResponseH\x00\x123\n\x10i2c_memory_write\x18\x08 \x01(\x0b2\x17.I2cMemoryWriteResponseH\x00B\t\n\x07payloadb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'shared_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_I2CRECEIVERESPONSE'].fields_by_name['data']._loaded_options = None
    _globals['_I2CRECEIVERESPONSE'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x08'
    _globals['_I2CTRANSMITREQUEST'].fields_by_name['data']._loaded_options = None
    _globals['_I2CTRANSMITREQUEST'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x08'
    _globals['_I2CMEMORYREADRESPONSE'].fields_by_name['data']._loaded_options = None
    _globals['_I2CMEMORYREADRESPONSE'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x08'
    _globals['_I2CMEMORYWRITEREQUEST'].fields_by_name['data']._loaded_options = None
    _globals['_I2CMEMORYWRITEREQUEST'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x08'
    _globals['_GPIONETNAME']._serialized_start = 43
    _globals['_GPIONETNAME']._serialized_end = 110
    _globals['_GPIOREADREQUEST']._serialized_start = 112
    _globals['_GPIOREADREQUEST']._serialized_end = 161
    _globals['_GPIOREADRESPONSE']._serialized_start = 163
    _globals['_GPIOREADRESPONSE']._serialized_end = 196
    _globals['_GPIOWRITEREQUEST']._serialized_start = 198
    _globals['_GPIOWRITEREQUEST']._serialized_end = 263
    _globals['_GPIOWRITERESPONSE']._serialized_start = 265
    _globals['_GPIOWRITERESPONSE']._serialized_end = 301
    _globals['_ADCNETNAME']._serialized_start = 303
    _globals['_ADCNETNAME']._serialized_end = 368
    _globals['_ADCREADREQUEST']._serialized_start = 370
    _globals['_ADCREADREQUEST']._serialized_end = 417
    _globals['_ADCREADRESPONSE']._serialized_start = 419
    _globals['_ADCREADRESPONSE']._serialized_end = 451
    _globals['_I2CNETNAME']._serialized_start = 453
    _globals['_I2CNETNAME']._serialized_end = 518
    _globals['_I2CREADYREQUEST']._serialized_start = 520
    _globals['_I2CREADYREQUEST']._serialized_end = 612
    _globals['_I2CREADYRESPONSE']._serialized_start = 614
    _globals['_I2CREADYRESPONSE']._serialized_end = 647
    _globals['_I2CRECEIVEREQUEST']._serialized_start = 649
    _globals['_I2CRECEIVEREQUEST']._serialized_end = 759
    _globals['_I2CRECEIVERESPONSE']._serialized_start = 761
    _globals['_I2CRECEIVERESPONSE']._serialized_end = 803
    _globals['_I2CTRANSMITREQUEST']._serialized_start = 805
    _globals['_I2CTRANSMITREQUEST']._serialized_end = 922
    _globals['_I2CTRANSMITRESPONSE']._serialized_start = 924
    _globals['_I2CTRANSMITRESPONSE']._serialized_end = 962
    _globals['_I2CMEMORYREADREQUEST']._serialized_start = 965
    _globals['_I2CMEMORYREADREQUEST']._serialized_end = 1102
    _globals['_I2CMEMORYREADRESPONSE']._serialized_start = 1104
    _globals['_I2CMEMORYREADRESPONSE']._serialized_end = 1149
    _globals['_I2CMEMORYWRITEREQUEST']._serialized_start = 1152
    _globals['_I2CMEMORYWRITEREQUEST']._serialized_end = 1296
    _globals['_I2CMEMORYWRITERESPONSE']._serialized_start = 1298
    _globals['_I2CMEMORYWRITERESPONSE']._serialized_end = 1339
    _globals['_CHIMERAV2REQUEST']._serialized_start = 1342
    _globals['_CHIMERAV2REQUEST']._serialized_end = 1717
    _globals['_CHIMERAV2RESPONSE']._serialized_start = 1720
    _globals['_CHIMERAV2RESPONSE']._serialized_end = 2104