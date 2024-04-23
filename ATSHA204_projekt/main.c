#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "ATSHA204_library/sha204_comm/sha204_comm.h"
#include "ATSHA204_library/sha204_comm_marshaling/sha204_comm_marshaling.h"
#include "ATSHA204_library/sha204_examples/sha204_examples.h"
#include "ATSHA204_library/sha204_i2c/sha204_i2c.h"
#include "ATSHA204_library/sha204_return_codes/sha204_return_codes.h"
#include "uart/uart.h"

FILE uart_str =
	FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);	// soubor pro stdout

void board_init();

void board_init()
{
	cli();
	UART_init(38400);
	UCSR1B |= (1 << RXCIE1);	// UART receive interrupt enable
	stdout = &uart_str;

	sei();
}

int main(void)
{
	board_init();

	UART_SendChar(27);	// escape
	UART_SendString("[2J");	// clear and home
	UART_SendChar(27);	// escape
	UART_SendString("[0;32;40m");	// barva pozadi a textu

	sha204p_init();

	uint8_t response[SHA204_RSP_SIZE_MAX];
	uint8_t wakeup_status = sha204c_wakeup(response);

	// *********************WAKEUP *********************

	if (wakeup_status != SHA204_SUCCESS)
	{
		printf("Error: SHA204 wakeup failed! Return code: %d\n\r", wakeup_status);
	}
	else
	{
		printf("SHA204 device woken up successfully!\n\r");
	}

	// *********************COMMAND *********************

	uint8_t my_command[] = { 0x03, 0x00, 0x00, 0x00
	};

	uint8_t send_status = sha204p_send_command(sizeof(my_command), my_command);
	if (send_status != SHA204_SUCCESS)
	{
		printf(			"Error: Failed to send command to SHA204 device! Return code:  %d\n\r",
			send_status);
	}
	else
	{
		printf("Command sent successfully to SHA204 device!\n\r");
	}

	// *********************RESPONSE *********************

	uint8_t receive_status = sha204p_receive_response(sizeof(response), response);
	if (receive_status != SHA204_SUCCESS)
	{
		printf(			"Error: Failed to receive response from SHA204 device! Return code:  "
			"%d\n\r",
			receive_status);
		return 1;
	}
	else
	{
		printf("Response received successfully from SHA204 device!\n\r");
	}

	// *********************CTENI SERIOVEHO CISLA *********************

	uint8_t tx_buffer[10];
	uint8_t sn[9];

	volatile uint8_t sn_read_status = sha204e_read_serial_number(tx_buffer, sn);

	if (sn_read_status == SHA204_SUCCESS)
	{
		printf("Serial number read successfully: ");
		for (int i = 0; i < 9; i++)
		{
			printf("%02X ", sn[i]);
		}
	}
	else
	{
		printf("Error reading serial number! Return code:  %d", sn_read_status);
	}

	printf("\n\r");

	// *********************RANDOM *********************

	uint8_t command[SHA204_CMD_SIZE_MAX];
	uint8_t response_random[RANDOM_RSP_SIZE];
	uint8_t *random_number = &response_random[SHA204_BUFFER_POS_DATA];

	volatile uint8_t rand_gen_status =
		sha204m_random(command, response_random, RANDOM_NO_SEED_UPDATE);

	if (rand_gen_status == SHA204_SUCCESS)
	{
		printf("Random number generated successfully: ");
		for (int i = 0; i < RANDOM_RSP_SIZE - SHA204_BUFFER_POS_DATA; i++)
		{
			printf("%02X ", random_number[i]);
		}

		sha204e_sleep();
	}
	else
	{
		printf("Error generating random number! %d", rand_gen_status);
	}

	printf("\n\r");

	// *********************CRC *********************

	uint8_t data[] = { 0x02, 0x02, 0x03, 0x07
	};

	uint8_t crc[2];

	sha204c_calculate_crc(sizeof(data), data, crc);
	printf("CRC: 0x%02X%02X", crc[1], crc[0]);

	// overeni: http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
	// (CRC-16, Custom, Input reflected, Poly 0x8005, Init/Final value 0x0)

	printf("\n\r");

	// *********************READ CONFIG ZONE *********************

	uint8_t config_data[88];
	uint8_t device_id = 0xC8;
	uint8_t read_config_status = sha204e_read_config_zone(device_id, config_data);

	printf("Config zone:\n\r");
	if (read_config_status == SHA204_SUCCESS)
	{
		for (int i = 0; i < 88; i++)
		{
			printf("Byte %d: 0x%02X\n\r", i, config_data[i]);
		}
	}
	else
	{
		printf("Error reading config zone! %d", read_config_status);
	}

	// *********************WRITE DATA *********************

	uint8_t write_tx_buffer[SHA204_COUNT_IDX + SHA204_ZONE_ACCESS_32 +
		WRITE_MAC_SIZE + SHA204_CRC_SIZE];
	uint8_t write_rx_buffer[WRITE_RSP_SIZE];
	uint8_t write_zone = SHA204_ZONE_DATA;
	uint16_t write_address = 0x0000;
	uint8_t mac[WRITE_MAC_SIZE] = { 0 };

	uint8_t write_data_status =
		sha204m_write(write_tx_buffer, write_rx_buffer, write_zone, write_address,
			random_number, mac);

	if (write_data_status == SHA204_SUCCESS)
	{
		printf("Success writing data!");
	}
	else
	{
		printf("Error writing data zone! %d\n\r", write_data_status);
	}

	// *********************READ DATA *********************

	uint8_t read_tx_buffer[5];
	uint8_t read_rx_buffer[READ_32_RSP_SIZE];
	uint8_t read_zone = SHA204_ZONE_DATA;
	uint16_t read_address = 0x0000;

	uint8_t read_data_status =
		sha204m_read(read_tx_buffer, read_rx_buffer, read_zone, read_address);

	if (read_data_status == SHA204_SUCCESS)
	{
		for (int i = 0; i < READ_32_RSP_SIZE; i++)
			printf("Byte %d: 0x%02X\n", i, read_rx_buffer[i]);
	}
	else
	{
		printf("Error reading data zone! %d\n\r", read_data_status);
	}

	return 0;
}
