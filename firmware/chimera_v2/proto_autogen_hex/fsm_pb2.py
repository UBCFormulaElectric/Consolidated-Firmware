"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 2, '', 'fsm.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tfsm.proto\x12\x03fsm*\xb1\x02\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x11\n\rGPIO_BOTS_3v3\x10\x01\x12\x19\n\x15GPIO_COCKPIT_SHDN_3v3\x10\x02\x12\x10\n\x0cGPIO_IMU_NSS\x10\x03\x12\x12\n\x0eGPIO_IMU_FSYNC\x10\x04\x12\x16\n\x12GPIO_NSUSP_FL_OCSC\x10\x05\x12\x16\n\x12GPIO_NSUSP_FR_OCSC\x10\x06\x12\x13\n\x0fGPIO_FR_INT_3v3\x10\x07\x12\x13\n\x0fGPIO_FL_INT_3v3\x10\x08\x12\x12\n\x0eGPIO_DEBUG_LED\x10\t\x12\x18\n\x14GPIO_NSTR_ANGLE_OCSC\x10\n\x12\x14\n\x10GPIO_NBPS_F_OCSC\x10\x0b\x12\x11\n\rGPIO_BOOT_LED\x10\x0c*\x8c\x01\n\nAdcNetName\x12\x1c\n\x18ADC_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bADC_SUSP_FL\x10\x01\x12\x0f\n\x0bADC_SUSP_FR\x10\x02\x12\r\n\tADC_APPS2\x10\x03\x12\r\n\tADC_BPS_F\x10\x04\x12\x11\n\rADC_STR_ANGLE\x10\x05\x12\r\n\tADC_APPS1\x10\x06*7\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00\x12\x0b\n\x07SPI_IMU\x10\x01b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'fsm_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 324
    _globals['_ADCNETNAME']._serialized_start = 327
    _globals['_ADCNETNAME']._serialized_end = 467
    _globals['_SPINETNAME']._serialized_start = 469
    _globals['_SPINETNAME']._serialized_end = 524