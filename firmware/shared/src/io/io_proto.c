#include io_proto.h

bool decode(uint8_t *buffer, size_t length){

    size_t msg_legnth = 0;
    while (msg_legnth < length && buffer[msg_legnth]!= 0xFF){
        msg_legnth++;
    }
    TEST_TestString message = test_string;
    pb_istream_t = stream = pb_stream_from_buffer(buffer, msg_length);
}