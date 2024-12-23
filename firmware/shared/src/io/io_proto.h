#pragma once

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "hello.pb.h"
#include "io_log.h"

bool decode(uint8_t *buffer, size_t length);
int packet_sizes(pb_callback_t message);