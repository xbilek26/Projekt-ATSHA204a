/*
 * sha204_i2c.h
 *
 * Created: 3/26/2024 14:21:00
 *  Author: Student
 */ 


#ifndef SHA204_I2C_H_
#define SHA204_I2C_H_

#define SHA204_BUFFER_POS_COUNT      (0)             //!< buffer index of count byte in command or response
#define SHA204_BUFFER_POS_DATA       (1)             //!< buffer index of data in response
#define SHA204_RESPONSE_TIMEOUT     ((uint16_t) 37)

#include <stdio.h>
#include "../../i2c/i2c.h"
#include "../sha204/sha204.h"

void sha204p_set_device_id(uint8_t id);
void sha204p_init(void);
uint8_t sha204p_wakeup(void);
uint8_t sha204p_send_slave_address(uint8_t read);
uint8_t sha204p_i2c_send(uint8_t word_address, uint8_t count, uint8_t *buffer);
uint8_t sha204p_send_command(uint8_t count, uint8_t *command);
uint8_t sha204p_idle(void);
uint8_t sha204p_sleep(void);
uint8_t sha204p_reset_io(void);
uint8_t sha204p_receive_response(uint8_t size, uint8_t *response);
uint8_t sha204p_resync(uint8_t size, uint8_t *response);

#endif /* SHA204_I2C_H_ */
