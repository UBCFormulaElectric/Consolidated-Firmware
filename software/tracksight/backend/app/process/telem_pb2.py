"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0btelem.proto"\xca\x01\n\x0cTelemMessage\x12\x0e\n\x06can_id\x18\x01 \x01(\x05\x12\x11\n\tmessage_0\x18\x02 \x01(\x05\x12\x11\n\tmessage_1\x18\x03 \x01(\x05\x12\x11\n\tmessage_2\x18\x04 \x01(\x05\x12\x11\n\tmessage_3\x18\x05 \x01(\x05\x12\x11\n\tmessage_4\x18\x06 \x01(\x05\x12\x11\n\tmessage_5\x18\x07 \x01(\x05\x12\x11\n\tmessage_6\x18\x08 \x01(\x05\x12\x11\n\tmessage_7\x18\t \x01(\x05\x12\x12\n\ntime_stamp\x18\n \x01(\x05b\x06proto3')
_TELEMMESSAGE = DESCRIPTOR.message_types_by_name['TelemMessage']
TelemMessage = _reflection.GeneratedProtocolMessageType('TelemMessage', (_message.Message,), {'DESCRIPTOR': _TELEMMESSAGE, '__module__': 'telem_pb2'})
_sym_db.RegisterMessage(TelemMessage)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _TELEMMESSAGE._serialized_start = 16
    _TELEMMESSAGE._serialized_end = 218