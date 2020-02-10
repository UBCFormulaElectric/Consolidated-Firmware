
/**
 * @brief Driver for interfacing with 7 segment display drivers
 * @Settings TODO
 * 			 Efuse max SPI transfer rate: 8MHz
 *           SPI Clock Polarity: 0 (SCK low-level idle state)
 *           SPI Clock Phase: 2nd edge (slave samples MOSI on SCK falling edge)
 *           Slave Select: Active Low (Must be toggle between SPI messages)
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
 * @brief      Configures the LSM6DS33 IMU Gyro/Accelerometer chip
 * @param      i2c_handle The handle for the I2C device the IMU is connected to
 * @return     None
 */

void write7seg(uint8_t command[]);
