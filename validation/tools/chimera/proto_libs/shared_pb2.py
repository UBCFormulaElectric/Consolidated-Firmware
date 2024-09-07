"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_sym_db = _symbol_database.Default()
from . import VC_pb2 as VC__pb2
from . import BMS_pb2 as BMS__pb2
from . import FSM_pb2 as FSM__pb2
from . import RSM_pb2 as RSM__pb2
from . import CRIT_pb2 as CRIT__pb2
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cshared.proto\x1a\x08VC.proto\x1a\tBMS.proto\x1a\tFSM.proto\x1a\tRSM.proto\x1a\nCRIT.proto"\xe7\x01\n\x0bGpioNetName\x12&\n\x0bvc_net_name\x18\x01 \x01(\x0e2\x0f.VC.GpioNetNameH\x00\x12(\n\x0cbms_net_name\x18\x02 \x01(\x0e2\x10.BMS.GpioNetNameH\x00\x12(\n\x0cfsm_net_name\x18\x03 \x01(\x0e2\x10.FSM.GpioNetNameH\x00\x12(\n\x0crsm_net_name\x18\x04 \x01(\x0e2\x10.RSM.GpioNetNameH\x00\x12*\n\rcrit_net_name\x18\x05 \x01(\x0e2\x11.CRIT.GpioNetNameH\x00B\x06\n\x04name"E\n\x08GpioRead\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\x19\n\x05value\x18\x02 \x01(\x0e2\n.GpioValue"F\n\tGpioWrite\x12\x1e\n\x08net_name\x18\x01 \x01(\x0b2\x0c.GpioNetName\x12\x19\n\x05value\x18\x02 \x01(\x0e2\n.GpioValue"\xe1\x01\n\nAdcNetName\x12%\n\x0bvc_net_name\x18\x01 \x01(\x0e2\x0e.VC.AdcNetNameH\x00\x12\'\n\x0cbms_net_name\x18\x02 \x01(\x0e2\x0f.BMS.AdcNetNameH\x00\x12\'\n\x0cfsm_net_name\x18\x03 \x01(\x0e2\x0f.FSM.AdcNetNameH\x00\x12\'\n\x0crsm_net_name\x18\x04 \x01(\x0e2\x0f.RSM.AdcNetNameH\x00\x12)\n\rcrit_net_name\x18\x05 \x01(\x0e2\x10.CRIT.AdcNetNameH\x00B\x06\n\x04name"3\n\x03Adc\x12\x1d\n\x08net_name\x18\x01 \x01(\x0b2\x0b.AdcNetName\x12\r\n\x05value\x18\x02 \x01(\x02"p\n\x0cDebugMessage\x12\x1e\n\tgpio_read\x18\x01 \x01(\x0b2\t.GpioReadH\x00\x12 \n\ngpio_write\x18\x02 \x01(\x0b2\n.GpioWriteH\x00\x12\x13\n\x03adc\x18\x03 \x01(\x0b2\x04.AdcH\x00B\t\n\x07payload*:\n\tGpioValue\x12\x1a\n\x16GPIO_VALUE_UNSPECIFIED\x10\x00\x12\x07\n\x03LOW\x10\x01\x12\x08\n\x04HIGH\x10\x02b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'shared_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals['_GPIOVALUE']._serialized_start = 843
    _globals['_GPIOVALUE']._serialized_end = 901
    _globals['_GPIONETNAME']._serialized_start = 72
    _globals['_GPIONETNAME']._serialized_end = 303
    _globals['_GPIOREAD']._serialized_start = 305
    _globals['_GPIOREAD']._serialized_end = 374
    _globals['_GPIOWRITE']._serialized_start = 376
    _globals['_GPIOWRITE']._serialized_end = 446
    _globals['_ADCNETNAME']._serialized_start = 449
    _globals['_ADCNETNAME']._serialized_end = 674
    _globals['_ADC']._serialized_start = 676
    _globals['_ADC']._serialized_end = 727
    _globals['_DEBUGMESSAGE']._serialized_start = 729
    _globals['_DEBUGMESSAGE']._serialized_end = 841