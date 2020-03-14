
/**
 * @brief Driver for interfacing with 7 segment display drivers
 * @Settings
 */

static const uint8_t lookup[16] = {
    // 0
    0x03,
    // 1
    0x4F,
    // 2
    0x25,
    // 3
    0x0D,
    // 4
    0x99,
    // 5
    0x49,
    // 6
    0x41,
    // 7
    0x1F,
    // 8
    0x01,
    // 9
    0x09,
    // A
    0x11,
    // B
    0x1C,
    // C
    0x63,
    // D
    0x85,
    // E
    0x61,
    // F
    0x71
};

/* Function Definitions */
/**
 * @brief
 * @param
 * @return
 */

void write7seg(uint8_t command[]);
