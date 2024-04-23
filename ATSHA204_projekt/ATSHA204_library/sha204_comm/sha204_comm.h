/*
 * sha204_comm.h
 *
 * Created: 4/9/2024 15:20:18
 *  Author: Student
 */ 


#ifndef SHA204_COMM_H_
#define SHA204_COMM_H_

#include <stddef.h>

#include "../sha204_i2c/sha204_i2c.h"

//! maximum command delay
#define SHA204_COMMAND_EXEC_MAX      ((uint8_t) (69.0 * CPU_CLOCK_DEVIATION_POSITIVE + 0.5))

//! minimum number of bytes in command (from count byte to second CRC byte)
#define SHA204_CMD_SIZE_MIN          ((uint8_t)  7)

//! maximum size of command packet (CheckMac)
#define SHA204_CMD_SIZE_MAX          ((uint8_t) 84)

//! number of CRC bytes
#define SHA204_CRC_SIZE              ((uint8_t)  2)

//! buffer index of status byte in status response
#define SHA204_BUFFER_POS_STATUS     (1)

//! buffer index of first data byte in data response
#define SHA204_BUFFER_POS_DATA       (1)

//! status byte after wake-up
#define SHA204_STATUS_BYTE_WAKEUP    ((uint8_t) 0x11)

//! command parse error
#define SHA204_STATUS_BYTE_PARSE     ((uint8_t) 0x03)

//! command execution error
#define SHA204_STATUS_BYTE_EXEC      ((uint8_t) 0x0F)

//! communication error
#define SHA204_STATUS_BYTE_COMM      ((uint8_t) 0xFF)


void sha204c_calculate_crc(uint8_t length, uint8_t *data, uint8_t *crc);
uint8_t sha204c_wakeup(uint8_t *response);
uint8_t sha204c_send_and_receive(uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer,
				uint8_t execution_delay, uint8_t execution_timeout);

#endif /* SHA204_COMM_H_ */