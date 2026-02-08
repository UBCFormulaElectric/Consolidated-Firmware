"""Generated protocol buffer code."""

from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 6, 32, 1, '', 'shared.proto')
_sym_db = _symbol_database.Default()
from . import nanopb_pb2 as nanopb__pb2
from . import rsm_pb2 as rsm__pb2
from . import f4dev_pb2 as f4dev__pb2
from . import crit_pb2 as crit__pb2
from . import bms_pb2 as bms__pb2
from . import dam_pb2 as dam__pb2
from . import fsm_pb2 as fsm__pb2
from . import vc_pb2 as vc__pb2
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cshared.proto\x1a\x0cnanopb.proto\x1a\trsm.proto\x1a\x0bf4dev.proto\x1a\ncrit.proto\x1a\tbms.proto\x1a\tdam.proto\x1a\tfsm.proto\x1a\x08vc.proto"\xbf\x02\n\x0bGpioNetName\x12,\n\x0ef4dev_net_name\x18\x01 \x01(\x0e2\x12.f4dev.GpioNetNameH\x00\x12*\n\rcrit_net_name\x18\x02 \x01(\x0e2\x11.crit.GpioNetNameH\x00\x12(\n\x0cdam_net_name\x18\x03 \x01(\x0e2\x10.dam.GpioNetNameH\x00\x12(\n\x0crsm_net_name\x18\x04 \x01(\x0e2\x10.rsm.GpioNetNameH\x00\x12(\n\x0cfsm_net_name\x18\x05 \x01(\x0e2\x10.fsm.GpioNetNameH\x00\x12(\n\x0cbms_net_name\x18\x06 \x01(\x0e2\x10.bms.GpioNetNameH\x00\x12&\n\x0bvc_net_name\x18\x07 \x01(\x0e2\x0f.vc.GpioNetNameH\x00B\x06\n\x04name"1\n\x0fGpioReadRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName"!\n\x10GpioReadResponse\x12\r\n\x05value\x18\x01 \x01(\x08"A\n\x10GpioWriteRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\r\n\x05value\x18\x02 \x01(\x08"$\n\x11GpioWriteResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"\xb6\x01\n\nAdcNetName\x12\'\n\x0crsm_net_name\x18\x01 \x01(\x0e2\x0f.rsm.AdcNetNameH\x00\x12\'\n\x0cfsm_net_name\x18\x02 \x01(\x0e2\x0f.fsm.AdcNetNameH\x00\x12\'\n\x0cbms_net_name\x18\x03 \x01(\x0e2\x0f.bms.AdcNetNameH\x00\x12%\n\x0bvc_net_name\x18\x04 \x01(\x0e2\x0e.vc.AdcNetNameH\x00B\x06\n\x04name"/\n\x0eAdcReadRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.AdcNetName" \n\x0fAdcReadResponse\x12\r\n\x05value\x18\x01 \x01(\x02"d\n\nI2cNetName\x12\'\n\x0crsm_net_name\x18\x01 \x01(\x0e2\x0f.rsm.I2cNetNameH\x00\x12%\n\x0bvc_net_name\x18\x02 \x01(\x0e2\x0e.vc.I2cNetNameH\x00B\x06\n\x04name"0\n\x0fI2cReadyRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName"!\n\x10I2cReadyResponse\x12\r\n\x05ready\x18\x01 \x01(\x08"B\n\x11I2cReceiveRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x0e\n\x06length\x18\x02 \x01(\r"*\n\x12I2cReceiveResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"I\n\x12I2cTransmitRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x14\n\x04data\x18\x02 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"&\n\x13I2cTransmitResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"]\n\x14I2cMemoryReadRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0ememory_address\x18\x02 \x01(\r\x12\x0e\n\x06length\x18\x03 \x01(\r"-\n\x15I2cMemoryReadResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"d\n\x15I2cMemoryWriteRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.I2cNetName\x12\x16\n\x0ememory_address\x18\x02 \x01(\r\x12\x14\n\x04data\x18\x03 \x01(\x0cB\x06\x92?\x03\x08\x80\x01")\n\x16I2cMemoryWriteResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"\xe1\x01\n\nSpiNetName\x12\'\n\x0cbms_net_name\x18\x01 \x01(\x0e2\x0f.bms.SpiNetNameH\x00\x12)\n\rcrit_net_name\x18\x02 \x01(\x0e2\x10.crit.SpiNetNameH\x00\x12\'\n\x0crsm_net_name\x18\x03 \x01(\x0e2\x0f.rsm.SpiNetNameH\x00\x12\'\n\x0cfsm_net_name\x18\x04 \x01(\x0e2\x0f.fsm.SpiNetNameH\x00\x12%\n\x0bvc_net_name\x18\x05 \x01(\x0e2\x0e.vc.SpiNetNameH\x00B\x06\n\x04name"I\n\x12SpiTransmitRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.SpiNetName\x12\x14\n\x04data\x18\x02 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"&\n\x13SpiTransmitResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"B\n\x11SpiReceiveRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.SpiNetName\x12\x0e\n\x06length\x18\x02 \x01(\r"*\n\x12SpiReceiveResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"b\n\x15SpiTransactionRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.SpiNetName\x12\x17\n\x07tx_data\x18\x02 \x01(\x0cB\x06\x92?\x03\x08\x80\x01\x12\x11\n\trx_length\x18\x03 \x01(\r"1\n\x16SpiTransactionResponse\x12\x17\n\x07rx_data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"?\n\x0bUartNetName\x12(\n\x0cdam_net_name\x18\x03 \x01(\x0e2\x10.dam.UartNetNameH\x00B\x06\n\x04name"K\n\x13UartTransmitRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.UartNetName\x12\x14\n\x04data\x18\x02 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"\'\n\x14UartTransmitResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"D\n\x12UartRecieveRequest\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.UartNetName\x12\x0e\n\x06length\x18\x02 \x01(\r"+\n\x13UartReceiveResponse\x12\x14\n\x04data\x18\x01 \x01(\x0cB\x06\x92?\x03\x08\x80\x01"?\n\nPwmNetName\x12)\n\rcrit_net_name\x18\x01 \x01(\x0e2\x10.crit.PwmNetNameH\x00B\x06\n\x04name"R\n\rPwmSetRequest\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.PwmNetName\x12\x0e\n\x06enable\x18\x02 \x01(\x08\x12\x12\n\nduty_cycle\x18\x03 \x01(\x02"!\n\x0ePwmSetResponse\x12\x0f\n\x07success\x18\x01 \x01(\x08"\x81\x05\n\x10ChimeraV2Request\x12%\n\tgpio_read\x18\x01 \x01(\x0b2\x10.GpioReadRequestH\x00\x12\'\n\ngpio_write\x18\x02 \x01(\x0b2\x11.GpioWriteRequestH\x00\x12#\n\x08adc_read\x18\x03 \x01(\x0b2\x0f.AdcReadRequestH\x00\x12%\n\ti2c_ready\x18\x04 \x01(\x0b2\x10.I2cReadyRequestH\x00\x12)\n\x0bi2c_receive\x18\x05 \x01(\x0b2\x12.I2cReceiveRequestH\x00\x12+\n\x0ci2c_transmit\x18\x06 \x01(\x0b2\x13.I2cTransmitRequestH\x00\x120\n\x0fi2c_memory_read\x18\x07 \x01(\x0b2\x15.I2cMemoryReadRequestH\x00\x122\n\x10i2c_memory_write\x18\x08 \x01(\x0b2\x16.I2cMemoryWriteRequestH\x00\x12)\n\x0bspi_receive\x18\t \x01(\x0b2\x12.SpiReceiveRequestH\x00\x12+\n\x0cspi_transmit\x18\n \x01(\x0b2\x13.SpiTransmitRequestH\x00\x121\n\x0fspi_transaction\x18\x0b \x01(\x0b2\x16.SpiTransactionRequestH\x00\x12+\n\x0cuart_receive\x18\x0c \x01(\x0b2\x13.UartRecieveRequestH\x00\x12-\n\ruart_transmit\x18\r \x01(\x0b2\x14.UartTransmitRequestH\x00\x12!\n\x07pwm_set\x18\x0e \x01(\x0b2\x0e.PwmSetRequestH\x00B\t\n\x07payload"\x90\x05\n\x11ChimeraV2Response\x12&\n\tgpio_read\x18\x01 \x01(\x0b2\x11.GpioReadResponseH\x00\x12(\n\ngpio_write\x18\x02 \x01(\x0b2\x12.GpioWriteResponseH\x00\x12$\n\x08adc_read\x18\x03 \x01(\x0b2\x10.AdcReadResponseH\x00\x12&\n\ti2c_ready\x18\x04 \x01(\x0b2\x11.I2cReadyResponseH\x00\x12*\n\x0bi2c_receive\x18\x05 \x01(\x0b2\x13.I2cReceiveResponseH\x00\x12,\n\x0ci2c_transmit\x18\x06 \x01(\x0b2\x14.I2cTransmitResponseH\x00\x121\n\x0fi2c_memory_read\x18\x07 \x01(\x0b2\x16.I2cMemoryReadResponseH\x00\x123\n\x10i2c_memory_write\x18\x08 \x01(\x0b2\x17.I2cMemoryWriteResponseH\x00\x12*\n\x0bspi_receive\x18\t \x01(\x0b2\x13.SpiReceiveResponseH\x00\x12,\n\x0cspi_transmit\x18\n \x01(\x0b2\x14.SpiTransmitResponseH\x00\x122\n\x0fspi_transaction\x18\x0b \x01(\x0b2\x17.SpiTransactionResponseH\x00\x12,\n\x0cuart_receive\x18\x0c \x01(\x0b2\x14.UartReceiveResponseH\x00\x12.\n\ruart_transmit\x18\r \x01(\x0b2\x15.UartTransmitResponseH\x00\x12"\n\x07pwm_set\x18\x0e \x01(\x0b2\x0f.PwmSetResponseH\x00B\t\n\x07payloadb\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, "shared_pb2", _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_I2CRECEIVERESPONSE'].fields_by_name['data']._loaded_options = None
    _globals['_I2CRECEIVERESPONSE'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_I2CTRANSMITREQUEST'].fields_by_name['data']._loaded_options = None
    _globals['_I2CTRANSMITREQUEST'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_I2CMEMORYREADRESPONSE'].fields_by_name['data']._loaded_options = None
    _globals['_I2CMEMORYREADRESPONSE'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_I2CMEMORYWRITEREQUEST'].fields_by_name['data']._loaded_options = None
    _globals['_I2CMEMORYWRITEREQUEST'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_SPITRANSMITREQUEST'].fields_by_name['data']._loaded_options = None
    _globals['_SPITRANSMITREQUEST'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_SPIRECEIVERESPONSE'].fields_by_name['data']._loaded_options = None
    _globals['_SPIRECEIVERESPONSE'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_SPITRANSACTIONREQUEST'].fields_by_name['tx_data']._loaded_options = None
    _globals['_SPITRANSACTIONREQUEST'].fields_by_name['tx_data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_SPITRANSACTIONRESPONSE'].fields_by_name['rx_data']._loaded_options = None
    _globals['_SPITRANSACTIONRESPONSE'].fields_by_name['rx_data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_UARTTRANSMITREQUEST'].fields_by_name['data']._loaded_options = None
    _globals['_UARTTRANSMITREQUEST'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_UARTRECEIVERESPONSE'].fields_by_name['data']._loaded_options = None
    _globals['_UARTRECEIVERESPONSE'].fields_by_name['data']._serialized_options = b'\x92?\x03\x08\x80\x01'
    _globals['_GPIONETNAME']._serialized_start = 110
    _globals['_GPIONETNAME']._serialized_end = 429
    _globals['_GPIOREADREQUEST']._serialized_start = 431
    _globals['_GPIOREADREQUEST']._serialized_end = 480
    _globals['_GPIOREADRESPONSE']._serialized_start = 482
    _globals['_GPIOREADRESPONSE']._serialized_end = 515
    _globals['_GPIOWRITEREQUEST']._serialized_start = 517
    _globals['_GPIOWRITEREQUEST']._serialized_end = 582
    _globals['_GPIOWRITERESPONSE']._serialized_start = 584
    _globals['_GPIOWRITERESPONSE']._serialized_end = 620
    _globals['_ADCNETNAME']._serialized_start = 623
    _globals['_ADCNETNAME']._serialized_end = 805
    _globals['_ADCREADREQUEST']._serialized_start = 807
    _globals['_ADCREADREQUEST']._serialized_end = 854
    _globals['_ADCREADRESPONSE']._serialized_start = 856
    _globals['_ADCREADRESPONSE']._serialized_end = 888
    _globals['_I2CNETNAME']._serialized_start = 890
    _globals['_I2CNETNAME']._serialized_end = 990
    _globals['_I2CREADYREQUEST']._serialized_start = 992
    _globals['_I2CREADYREQUEST']._serialized_end = 1040
    _globals['_I2CREADYRESPONSE']._serialized_start = 1042
    _globals['_I2CREADYRESPONSE']._serialized_end = 1075
    _globals['_I2CRECEIVEREQUEST']._serialized_start = 1077
    _globals['_I2CRECEIVEREQUEST']._serialized_end = 1143
    _globals['_I2CRECEIVERESPONSE']._serialized_start = 1145
    _globals['_I2CRECEIVERESPONSE']._serialized_end = 1187
    _globals['_I2CTRANSMITREQUEST']._serialized_start = 1189
    _globals['_I2CTRANSMITREQUEST']._serialized_end = 1262
    _globals['_I2CTRANSMITRESPONSE']._serialized_start = 1264
    _globals['_I2CTRANSMITRESPONSE']._serialized_end = 1302
    _globals['_I2CMEMORYREADREQUEST']._serialized_start = 1304
    _globals['_I2CMEMORYREADREQUEST']._serialized_end = 1397
    _globals['_I2CMEMORYREADRESPONSE']._serialized_start = 1399
    _globals['_I2CMEMORYREADRESPONSE']._serialized_end = 1444
    _globals['_I2CMEMORYWRITEREQUEST']._serialized_start = 1446
    _globals['_I2CMEMORYWRITEREQUEST']._serialized_end = 1546
    _globals['_I2CMEMORYWRITERESPONSE']._serialized_start = 1548
    _globals['_I2CMEMORYWRITERESPONSE']._serialized_end = 1589
    _globals['_SPINETNAME']._serialized_start = 1592
    _globals['_SPINETNAME']._serialized_end = 1817
    _globals['_SPITRANSMITREQUEST']._serialized_start = 1819
    _globals['_SPITRANSMITREQUEST']._serialized_end = 1892
    _globals['_SPITRANSMITRESPONSE']._serialized_start = 1894
    _globals['_SPITRANSMITRESPONSE']._serialized_end = 1932
    _globals['_SPIRECEIVEREQUEST']._serialized_start = 1934
    _globals['_SPIRECEIVEREQUEST']._serialized_end = 2000
    _globals['_SPIRECEIVERESPONSE']._serialized_start = 2002
    _globals['_SPIRECEIVERESPONSE']._serialized_end = 2044
    _globals['_SPITRANSACTIONREQUEST']._serialized_start = 2046
    _globals['_SPITRANSACTIONREQUEST']._serialized_end = 2144
    _globals['_SPITRANSACTIONRESPONSE']._serialized_start = 2146
    _globals['_SPITRANSACTIONRESPONSE']._serialized_end = 2195
    _globals['_UARTNETNAME']._serialized_start = 2197
    _globals['_UARTNETNAME']._serialized_end = 2260
    _globals['_UARTTRANSMITREQUEST']._serialized_start = 2262
    _globals['_UARTTRANSMITREQUEST']._serialized_end = 2337
    _globals['_UARTTRANSMITRESPONSE']._serialized_start = 2339
    _globals['_UARTTRANSMITRESPONSE']._serialized_end = 2378
    _globals['_UARTRECIEVEREQUEST']._serialized_start = 2380
    _globals['_UARTRECIEVEREQUEST']._serialized_end = 2448
    _globals['_UARTRECEIVERESPONSE']._serialized_start = 2450
    _globals['_UARTRECEIVERESPONSE']._serialized_end = 2493
    _globals['_PWMNETNAME']._serialized_start = 2495
    _globals['_PWMNETNAME']._serialized_end = 2558
    _globals['_PWMSETREQUEST']._serialized_start = 2560
    _globals['_PWMSETREQUEST']._serialized_end = 2642
    _globals['_PWMSETRESPONSE']._serialized_start = 2644
    _globals['_PWMSETRESPONSE']._serialized_end = 2677
    _globals['_CHIMERAV2REQUEST']._serialized_start = 2680
    _globals['_CHIMERAV2REQUEST']._serialized_end = 3321
    _globals['_CHIMERAV2RESPONSE']._serialized_start = 3324
    _globals['_CHIMERAV2RESPONSE']._serialized_end = 3980
