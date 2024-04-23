/*
 * sha204_comm_marshaling.c
 *
 * Created: 4/9/2024 16:25:25
 *  Author: Student
 */ 


#include <string.h>                    // needed for memcpy()
#include "../sha204_return_codes/sha204_return_codes.h"   // declarations of function return codes
#include "sha204_comm_marshaling.h"    // definitions and declarations for the Command Marshaling module

/** \brief This function sends a Random command to the device.
 *
 * \param[in]  tx_buffer pointer to transmit buffer
 * \param[out] rx_buffer pointer to receive buffer
 * \param[in]  mode 0: update seed; 1: no seed update
 * \return status of the operation
 */
uint8_t sha204m_random(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t mode)
{
	if (!tx_buffer || !rx_buffer || (mode > RANDOM_NO_SEED_UPDATE))
		// no null pointers allowed
		// mode has to match an allowed Random mode.
		return SHA204_BAD_PARAM;

	tx_buffer[SHA204_COUNT_IDX] = RANDOM_COUNT;
	tx_buffer[SHA204_OPCODE_IDX] = SHA204_RANDOM;
	tx_buffer[RANDOM_MODE_IDX] = mode & RANDOM_SEED_UPDATE;

	// 2. parameter is 0.
	tx_buffer[RANDOM_PARAM2_IDX] =
	tx_buffer[RANDOM_PARAM2_IDX + 1] = 0;

	return sha204c_send_and_receive(&tx_buffer[0], RANDOM_RSP_SIZE, &rx_buffer[0],
				RANDOM_DELAY, RANDOM_EXEC_MAX - RANDOM_DELAY);
}


/** \brief This function sends a Read command to the device.
 *
 * \param[in]  tx_buffer pointer to transmit buffer
 * \param[out] rx_buffer pointer to receive buffer
 * \param[in]  zone 0: Configuration; 1: OTP; 2: Data
 * \param[in]  address address to read from
 * \return status of the operation
 */
uint8_t sha204m_read(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t zone, uint16_t address)
{
	uint8_t rx_size;

	if (!tx_buffer || !rx_buffer || (zone & ~READ_ZONE_MASK)
				|| ((zone & READ_ZONE_MODE_32_BYTES) && (zone == SHA204_ZONE_OTP)))
		// no null pointers allowed
		// zone has to match a valid param1 value.
		// Reading a 32-byte from the OTP zone is not allowed.
		return SHA204_BAD_PARAM;

	address >>= 2;
	if ((zone & SHA204_ZONE_MASK) == SHA204_ZONE_CONFIG) {
		if (address > SHA204_ADDRESS_MASK_CONFIG)
			return SHA204_BAD_PARAM;
	}
	else if ((zone & SHA204_ZONE_MASK) == SHA204_ZONE_OTP) {
		if (address > SHA204_ADDRESS_MASK_OTP)
			return SHA204_BAD_PARAM;
	}
	else if ((zone & SHA204_ZONE_MASK) == SHA204_ZONE_DATA) {
		if (address > SHA204_ADDRESS_MASK)
			return SHA204_BAD_PARAM;
	}

	tx_buffer[SHA204_COUNT_IDX] = READ_COUNT;
	tx_buffer[SHA204_OPCODE_IDX] = SHA204_READ;
	tx_buffer[READ_ZONE_IDX] = zone;
	tx_buffer[READ_ADDR_IDX] = (uint8_t) (address & SHA204_ADDRESS_MASK);
	tx_buffer[READ_ADDR_IDX + 1] = 0;

	rx_size = (zone & SHA204_ZONE_COUNT_FLAG) ? READ_32_RSP_SIZE : READ_4_RSP_SIZE;

	return sha204c_send_and_receive(&tx_buffer[0], rx_size, &rx_buffer[0],
				READ_DELAY, READ_EXEC_MAX - READ_DELAY);
}

/**\brief This function sends a Write command to the device.
 *
 * \param[in]  tx_buffer pointer to transmit buffer
 * \param[out] rx_buffer pointer to receive buffer
 * \param[in]  zone 0: Configuration; 1: OTP; 2: Data
 * \param[in]  address address to write to
 * \param[in]  new_value pointer to 32 (zone bit 7 set) or 4 bytes of data
 * \param[in]  mac pointer to MAC (ignored if zone is unlocked)
 * \return status of the operation
 */
uint8_t sha204m_write(uint8_t *tx_buffer, uint8_t *rx_buffer,
			uint8_t zone, uint16_t address, uint8_t *new_value, uint8_t *mac)
{
	uint8_t *p_command;
	uint8_t count;

	if (!tx_buffer || !rx_buffer || !new_value || (zone & ~WRITE_ZONE_MASK))
		// no null pointers allowed
		// zone has to match a valid param1 value.
		return SHA204_BAD_PARAM;

	address >>= 2;
	if ((zone & SHA204_ZONE_MASK) == SHA204_ZONE_CONFIG) {
		if (address > SHA204_ADDRESS_MASK_CONFIG)
			return SHA204_BAD_PARAM;
	}
	else if ((zone & SHA204_ZONE_MASK) == SHA204_ZONE_OTP) {
		if (address > SHA204_ADDRESS_MASK_OTP)
			return SHA204_BAD_PARAM;
	}
	else if ((zone & SHA204_ZONE_MASK) == SHA204_ZONE_DATA) {
		if (address > SHA204_ADDRESS_MASK)
			return SHA204_BAD_PARAM;
	}

	p_command = &tx_buffer[SHA204_OPCODE_IDX];
	*p_command++ = SHA204_WRITE;
	*p_command++ = zone;
	*p_command++ = (uint8_t) (address & SHA204_ADDRESS_MASK);
	*p_command++ = 0;

	count = (zone & SHA204_ZONE_COUNT_FLAG) ? SHA204_ZONE_ACCESS_32 : SHA204_ZONE_ACCESS_4;
	memcpy(p_command, new_value, count);
	p_command += count;

	if (mac != NULL)
	{
		memcpy(p_command, mac, WRITE_MAC_SIZE);
		p_command += WRITE_MAC_SIZE;
	}

	// Supply count.
	tx_buffer[SHA204_COUNT_IDX] = (uint8_t) (p_command - &tx_buffer[0] + SHA204_CRC_SIZE);

	return sha204c_send_and_receive(&tx_buffer[0], WRITE_RSP_SIZE, &rx_buffer[0],
				WRITE_DELAY, WRITE_EXEC_MAX - WRITE_DELAY);
}
