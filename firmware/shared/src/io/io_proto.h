#pragma once

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "hello.pb.h"
#include "io_log.h"


int packet_sizes(pb_callback_t message){

}

bool read_ints(pb_istream_t *stream, const pb_field_iter_t *field, void **arg)
{
    while (stream->bytes_left)
    {
        uint64_t value;
        if (!pb_decode_varint(stream, &value))
            return false;
        LOG_INFO("%lld\n", value);
    }
    return true;
}



typedef struct _TEST_TestString {
    pb_callback_t message;
} TEST_TestString



