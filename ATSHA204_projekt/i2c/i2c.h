/*
 * i2c.h
 *
 * Created: 3/26/2024 14:33:15
 *  Author: Student
 */ 

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "../ATSHA204_library/sha204_conf/sha204_config.h"

#define SHA204_RSP_SIZE_MIN          ((uint8_t)  4)  //!< minimum number of bytes in response
#define SHA204_RSP_SIZE_MAX          ((uint8_t) 35)  //!< maximum size of response packet
#define I2C_CLOCK  (400000.0)
#define I2C_BYTE_TIMEOUT  ((uint8_t) 200)
#define I2C_STOP_TIMEOUT  ((uint8_t) 250)
#define I2C_START_TIMEOUT  ((uint8_t) 250)
#define SHA204_BUFFER_POS_COUNT      (0)             //!< buffer index of count byte in command or response
#define SHA204_BUFFER_POS_DATA       (1)             //!< buffer index of data in response
#define SHA204_WAKEUP_PULSE_WIDTH    (uint8_t) (6.0 * CPU_CLOCK_DEVIATION_POSITIVE + 0.5)
#define SHA204_WAKEUP_DELAY          (uint8_t) (3.0 * CPU_CLOCK_DEVIATION_POSITIVE + 0.5)

void i2c_enable(void);
void i2c_disable(void);
uint8_t i2c_send_start(void);
uint8_t i2c_send_stop(void);
uint8_t i2c_send_bytes(uint8_t count, uint8_t *data);
uint8_t i2c_receive_byte(uint8_t *data);
uint8_t i2c_receive_bytes(uint8_t count, uint8_t *data);

#endif /* I2C_H_ */
