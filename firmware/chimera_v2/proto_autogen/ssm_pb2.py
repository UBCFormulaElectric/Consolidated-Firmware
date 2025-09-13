"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 4, '', 'ssm.proto')
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\tssm.proto\x12\x03ssm*\xc7\x02\n\x0bGpioNetName\x12\x1d\n\x19GPIO_NET_NAME_UNSPECIFIED\x10\x00\x12\x0f\n\x0bGPIO_DOUT_1\x10\x01\x12\x0f\n\x0bGPIO_DOUT_2\x10\x02\x12\x0f\n\x0bGPIO_DOUT_3\x10\x03\x12\x0f\n\x0bGPIO_DOUT_4\x10\x04\x12\x12\n\x0eGPIO_DEBUG_LED\x10\x05\x12\x11\n\rGPIO_BOOT_LED\x10\x06\x12\x14\n\x10GPIO_INDICATOR_1\x10\x07\x12\x14\n\x10GPIO_INDICATOR_2\x10\x08\x12\x14\n\x10GPIO_INDICATOR_3\x10\t\x12\x14\n\x10GPIO_INTERLOCK_1\x10\n\x12\x14\n\x10GPIO_INTERLOCK_2\x10\x0b\x12\x14\n\x10GPIO_INTERLOCK_3\x10\x0c\x12\x14\n\x10GPIO_INTERLOCK_4\x10\r\x12\x14\n\x10GPIO_DAC_N_CLEAR\x10\x0e*]\n\nSpiNetName\x12\x1c\n\x18SPI_NET_NAME_UNSPECIFIED\x10\x00\x12\x11\n\rSPI_ISOSPI_LS\x10\x01\x12\x11\n\rSPI_ISOSPI_HS\x10\x02\x12\x0b\n\x07SPI_DAC\x10\x03b\x06proto3')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'ssm_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    DESCRIPTOR._loaded_options = None
    _globals['_GPIONETNAME']._serialized_start = 19
    _globals['_GPIONETNAME']._serialized_end = 346
    _globals['_SPINETNAME']._serialized_start = 348
    _globals['_SPINETNAME']._serialized_end = 441