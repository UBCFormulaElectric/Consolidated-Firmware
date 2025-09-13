"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(_runtime_version.Domain.PUBLIC, 5, 29, 4, '', 'nanopb.proto')
_sym_db = _symbol_database.Default()
from google.protobuf import descriptor_pb2 as google_dot_protobuf_dot_descriptor__pb2
DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cnanopb.proto\x1a google/protobuf/descriptor.proto"\x99\t\n\rNanoPBOptions\x12\x10\n\x08max_size\x18\x01 \x01(\x05\x12\x12\n\nmax_length\x18\x0e \x01(\x05\x12\x11\n\tmax_count\x18\x02 \x01(\x05\x12&\n\x08int_size\x18\x07 \x01(\x0e2\x08.IntSize:\nIS_DEFAULT\x12*\n\x0cenum_intsize\x18" \x01(\x0e2\x08.IntSize:\nIS_DEFAULT\x12$\n\x04type\x18\x03 \x01(\x0e2\n.FieldType:\nFT_DEFAULT\x12\x18\n\nlong_names\x18\x04 \x01(\x08:\x04true\x12\x1c\n\rpacked_struct\x18\x05 \x01(\x08:\x05false\x12\x1a\n\x0bpacked_enum\x18\n \x01(\x08:\x05false\x12\x1b\n\x0cskip_message\x18\x06 \x01(\x08:\x05false\x12\x18\n\tno_unions\x18\x08 \x01(\x08:\x05false\x12\r\n\x05msgid\x18\t \x01(\r\x12\x1e\n\x0fanonymous_oneof\x18\x0b \x01(\x08:\x05false\x12\x15\n\x06proto3\x18\x0c \x01(\x08:\x05false\x12#\n\x14proto3_singular_msgs\x18\x15 \x01(\x08:\x05false\x12\x1d\n\x0eenum_to_string\x18\r \x01(\x08:\x05false\x12\x1c\n\renum_validate\x18  \x01(\x08:\x05false\x12\x1b\n\x0cfixed_length\x18\x0f \x01(\x08:\x05false\x12\x1a\n\x0bfixed_count\x18\x10 \x01(\x08:\x05false\x12\x1e\n\x0fsubmsg_callback\x18\x16 \x01(\x08:\x05false\x12/\n\x0cmangle_names\x18\x11 \x01(\x0e2\x11.TypenameMangling:\x06M_NONE\x12(\n\x11callback_datatype\x18\x12 \x01(\t:\rpb_callback_t\x124\n\x11callback_function\x18\x13 \x01(\t:\x19pb_default_field_callback\x120\n\x0edescriptorsize\x18\x14 \x01(\x0e2\x0f.DescriptorSize:\x07DS_AUTO\x12\x1a\n\x0bdefault_has\x18\x17 \x01(\x08:\x05false\x12\x0f\n\x07include\x18\x18 \x03(\t\x12\x0f\n\x07exclude\x18\x1a \x03(\t\x12\x0f\n\x07package\x18\x19 \x01(\t\x12A\n\rtype_override\x18\x1b \x01(\x0e2*.google.protobuf.FieldDescriptorProto.Type\x12C\n\x0elabel_override\x18\x1f \x01(\x0e2+.google.protobuf.FieldDescriptorProto.Label\x12\x19\n\x0bsort_by_tag\x18\x1c \x01(\x08:\x04true\x12.\n\rfallback_type\x18\x1d \x01(\x0e2\n.FieldType:\x0bFT_CALLBACK\x12\x13\n\x0binitializer\x18\x1e \x01(\t\x12,\n\x1ediscard_unused_automatic_types\x18! \x01(\x08:\x04true\x12!\n\x12discard_deprecated\x18# \x01(\x08:\x05false*i\n\tFieldType\x12\x0e\n\nFT_DEFAULT\x10\x00\x12\x0f\n\x0bFT_CALLBACK\x10\x01\x12\x0e\n\nFT_POINTER\x10\x04\x12\r\n\tFT_STATIC\x10\x02\x12\r\n\tFT_IGNORE\x10\x03\x12\r\n\tFT_INLINE\x10\x05*D\n\x07IntSize\x12\x0e\n\nIS_DEFAULT\x10\x00\x12\x08\n\x04IS_8\x10\x08\x12\t\n\x05IS_16\x10\x10\x12\t\n\x05IS_32\x10 \x12\t\n\x05IS_64\x10@*Z\n\x10TypenameMangling\x12\n\n\x06M_NONE\x10\x00\x12\x13\n\x0fM_STRIP_PACKAGE\x10\x01\x12\r\n\tM_FLATTEN\x10\x02\x12\x16\n\x12M_PACKAGE_INITIALS\x10\x03*E\n\x0eDescriptorSize\x12\x0b\n\x07DS_AUTO\x10\x00\x12\x08\n\x04DS_1\x10\x01\x12\x08\n\x04DS_2\x10\x02\x12\x08\n\x04DS_4\x10\x04\x12\x08\n\x04DS_8\x10\x08:E\n\x0enanopb_fileopt\x12\x1c.google.protobuf.FileOptions\x18\xf2\x07 \x01(\x0b2\x0e.NanoPBOptions:G\n\rnanopb_msgopt\x12\x1f.google.protobuf.MessageOptions\x18\xf2\x07 \x01(\x0b2\x0e.NanoPBOptions:E\n\x0enanopb_enumopt\x12\x1c.google.protobuf.EnumOptions\x18\xf2\x07 \x01(\x0b2\x0e.NanoPBOptions:>\n\x06nanopb\x12\x1d.google.protobuf.FieldOptions\x18\xf2\x07 \x01(\x0b2\x0e.NanoPBOptionsB\x1a\n\x18fi.kapsi.koti.jpa.nanopb')
_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'nanopb_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
    _globals['DESCRIPTOR']._loaded_options = None
    _globals['DESCRIPTOR']._serialized_options = b'\n\x18fi.kapsi.koti.jpa.nanopb'
    _globals['_FIELDTYPE']._serialized_start = 1230
    _globals['_FIELDTYPE']._serialized_end = 1335
    _globals['_INTSIZE']._serialized_start = 1337
    _globals['_INTSIZE']._serialized_end = 1405
    _globals['_TYPENAMEMANGLING']._serialized_start = 1407
    _globals['_TYPENAMEMANGLING']._serialized_end = 1497
    _globals['_DESCRIPTORSIZE']._serialized_start = 1499
    _globals['_DESCRIPTORSIZE']._serialized_end = 1568
    _globals['_NANOPBOPTIONS']._serialized_start = 51
    _globals['_NANOPBOPTIONS']._serialized_end = 1228