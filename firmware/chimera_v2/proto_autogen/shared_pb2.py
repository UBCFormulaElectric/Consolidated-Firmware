"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 28, 2, '', 'shared.proto')
_sym_db = _symbol_database.Default()
from . import nanopb_pb2 as nanopb__pb2
from . import f4dev_pb2 as f4dev__pb2
from . import ssm_pb2 as ssm__pb2
from . import crit_pb2 as crit__pb2
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cshared.proto\x1a\x0cnanopb.proto\x1a\x0bf4dev.proto\x1a\tssm.proto\x1a\ncrit.proto"\x99\x01\n\x0bGpioNetName\x12,\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x12.f4dev.GpioNetNameH\x00\x12(\n\x0cssm_net_name\x18\x02 \x01(\x0e2\x10.ssm.GpioNetNameH\x00\x12*\n\rcrit_net_name\x18\x03 \x01(\x0e2\x11.crit.GpioNetNameH\x00B\x06\n\x04name"1\n\x0fGpioReadRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName"!\n\x10GpioReadResponse\x12\r\n\x05value\x18\x01 \x01(\x08"A\n\x10GpioWriteRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\r\n\x05value\x18\x02 \x01(\x08"$\n\x11GpioWriteResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"\x95\x01\n\nAdcNetName\x12+\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x11.f4dev.AdcNetNameH\x00\x12\'\n\x0cssm_net_name\x18\x02 \x01(\x0e2\x0f.ssm.AdcNetNameH\x00\x12)\n\rcrit_net_name\x18\x03 \x01(\x0e2\x10.crit.AdcNetNameH\x00B\x06\n\x04name"/\n\x0eAdcReadRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.AdcNetName" \n\x0fAdcReadResponse\x12\r\n\x05value\x18\x01 \x01(\x02"\x95\x01\n\nI2cNetName\x12+\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x11.f4dev.I2cNetNameH\x00\x12\'\n\x0cssm_net_name\x18\x02 \x01(\x0e2\x0f.ssm.I2cNetNameH\x00\x12)\n\rcrit_net_name\x18\x03 \x01(\x0e2\x10.crit.I2cNetNameH\x00B\x06\n\x04name"0\n\x0fI2cReadyRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName"!\n\x10I2cReadyResponse\x12\r\n\x05ready\x18\x01 \x01(\x08"B\n\x11I2cReceiveRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x0e\n\x06length\x18\x02 \x01(\r"*\n\x12I2cReceiveResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x08"I\n\x12I2cTransmitRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x14\n\x04data\x18\x02 \x01(\x0cB\x06\x92?\x03\x08\x80\x08"&\n\x13I2cTransmitResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"]\n\x14I2cMemoryReadRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0ememory_address\x18\x02 \x01(\r\x12\x0e\n\x06length\x18\x03 \x01(\r"-\n\x15I2cMemoryReadResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x08"d\n\x15I2cMemoryWriteRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0ememory_address\x18\x02 \x01(\r\x12\x14\n\x04data\x18\x03 \x01(\x0cB\x06\x92?\x03\x08\x80\x08")\n\x16I2cMemoryWriteResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"\xf7\x02\n\x10ChimeraV2Request\x12%\n\tgpio_read\x18\x01 \x01(\x0b2\x10.GpioReadRequestH\x00\x12\'\n\ngpio_write\x18\x02 \x01(\x0b2\x11.GpioWriteRequestH\x00\x12#\n\x08adc_read\x18\x03 \x01(\x0b2\x0f.AdcReadRequestH\x00\x12%\n\ti2c_ready\x18\x04 \x01(\x0b2\x10.I2cReadyRequestH\x00\x12)\n\x0bi2c_receive\x18\x05 \x01(\x0b2\x12.I2cReceiveRequestH\x00\x12+\n\x0ci2c_transmit\x18\x06 \x01(\x0b2\x13.I2cTransmitRequestH\x00\x120\n\x0fi2c_memory_read\x18\x07 \x01(\x0b2\x15.I2cMemoryReadRequestH\x00\x122\n\x10i2c_memory_write\x18\x08 \x01(\x0b2\x16.I2cMemoryWriteRequestH\x00B\t\n\x07payload"\x80\x03\n\x11ChimeraV2Response\x12&\n\tgpio_read\x18\x01 \x01(\x0b2\x11.GpioReadResponseH\x00\x12(\n\ngpio_write\x18\x02 \x01(\x0b2\x12.GpioWriteResponseH\x00\x12$\n\x08adc_read\x18\x03 \x01(\x0b2\x10.AdcReadResponseH\x00\x12&\n\ti2c_ready\x18\x04 \x01(\x0b2\x11.I2cReadyResponseH\x00\x12*\n\x0bi2c_receive\x18\x05 \x01(\x0b2\x13.I2cReceiveResponseH\x00\x12,\n\x0ci2c_transmit\x18\x06 \x01(\x0b2\x14.I2cTransmitResponseH\x00\x121\n\x0fi2c_memory_read\x18\x07 \x01(\x0b2\x16.I2cMemoryReadResponseH\x00\x123\n\x10i2c_memory_write\x18\x08 \x01(\x0b2\x17.I2cMemoryWriteResponseH\x00B\t\n\x07payloadb\x06proto3')
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
    _globals['_GPIONETNAME']._serialized_start = 67
    _globals['_GPIONETNAME']._serialized_end = 220
    _globals['_GPIOREADREQUEST']._serialized_start = 222
    _globals['_GPIOREADREQUEST']._serialized_end = 271
    _globals['_GPIOREADRESPONSE']._serialized_start = 273
    _globals['_GPIOREADRESPONSE']._serialized_end = 306
    _globals['_GPIOWRITEREQUEST']._serialized_start = 308
    _globals['_GPIOWRITEREQUEST']._serialized_end = 373
    _globals['_GPIOWRITERESPONSE']._serialized_start = 375
    _globals['_GPIOWRITERESPONSE']._serialized_end = 411
    _globals['_ADCNETNAME']._serialized_start = 414
    _globals['_ADCNETNAME']._serialized_end = 563
    _globals['_ADCREADREQUEST']._serialized_start = 565
    _globals['_ADCREADREQUEST']._serialized_end = 612
    _globals['_ADCREADRESPONSE']._serialized_start = 614
    _globals['_ADCREADRESPONSE']._serialized_end = 646
    _globals['_I2CNETNAME']._serialized_start = 649
    _globals['_I2CNETNAME']._serialized_end = 798
    _globals['_I2CREADYREQUEST']._serialized_start = 800
    _globals['_I2CREADYREQUEST']._serialized_end = 848
    _globals['_I2CREADYRESPONSE']._serialized_start = 850
    _globals['_I2CREADYRESPONSE']._serialized_end = 883
    _globals['_I2CRECEIVEREQUEST']._serialized_start = 885
    _globals['_I2CRECEIVEREQUEST']._serialized_end = 951
    _globals['_I2CRECEIVERESPONSE']._serialized_start = 953
    _globals['_I2CRECEIVERESPONSE']._serialized_end = 995
    _globals['_I2CTRANSMITREQUEST']._serialized_start = 997
    _globals['_I2CTRANSMITREQUEST']._serialized_end = 1070
    _globals['_I2CTRANSMITRESPONSE']._serialized_start = 1072
    _globals['_I2CTRANSMITRESPONSE']._serialized_end = 1110
    _globals['_I2CMEMORYREADREQUEST']._serialized_start = 1112
    _globals['_I2CMEMORYREADREQUEST']._serialized_end = 1205
    _globals['_I2CMEMORYREADRESPONSE']._serialized_start = 1207
    _globals['_I2CMEMORYREADRESPONSE']._serialized_end = 1252
    _globals['_I2CMEMORYWRITEREQUEST']._serialized_start = 1254
    _globals['_I2CMEMORYWRITEREQUEST']._serialized_end = 1354
    _globals['_I2CMEMORYWRITERESPONSE']._serialized_start = 1356
    _globals['_I2CMEMORYWRITERESPONSE']._serialized_end = 1397
    _globals['_CHIMERAV2REQUEST']._serialized_start = 1400
    _globals['_CHIMERAV2REQUEST']._serialized_end = 1775
    _globals['_CHIMERAV2RESPONSE']._serialized_start = 1778
    _globals['_CHIMERAV2RESPONSE']._serialized_end = 2162