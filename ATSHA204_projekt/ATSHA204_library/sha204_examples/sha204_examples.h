/*
 * sha204_examples.h
 *
 * Created: 4/9/2024 16:21:25
 *  Author: Student
 */ 


#ifndef SHA204_EXAMPLES_H_
#define SHA204_EXAMPLES_H_

#include <stdint.h>                   // data type definitions


/** \ingroup  sha204_examples Example Definitions
@{ */

/** \brief This definition selects a simple MAC / CheckMac example using
 * an ATSHA204 as the host (key storage and SHA-256 calculation).
*/
#define SHA204_EXAMPLE_CHECKMAC_DEVICE    1

/** \brief This definition selects a simple MAC / CheckMac example using
 * firmware as the host (key storage and SHA-256 calculation).
 */
#define SHA204_EXAMPLE_CHECKMAC_FIRMWARE  2

/** \brief This definition selects an advanced MAC / CheckMac example using
 *  a derived key. This example runs only with two devices.
 */
#define SHA204_EXAMPLE_DERIVE_KEY         3

/** \brief This definition selects an advanced MAC / CheckMac example using
 *  a diversified key. This example runs only with two devices.
 */
#define SHA204_EXAMPLE_DIVERSIFY_KEY      4

/** \brief This definition selects a utility that changes the I2C default
 *  address of the device to SHA204_HOST_ADDRESS.
 *
 *  You need to change the address on one device from its default
 *  in order to run the advanced MAC / CheckMac examples.
 */
#define SHA204_EXAMPLE_CHANGE_I2C_ADDRESS 5

/** \brief This definition selects a utility that reads all 88 bytes from
 *  the configuration zone.
 *
 *  This gives you easy access to the device configuration
 *  (e.g. serial number, lock status, configuration of keys).
 */
#define SHA204_EXAMPLE_READ_CONFIG_ZONE   6

/** -------------------- Define an example. --------------------------
 *
 */
#define SHA204_EXAMPLE   SHA204_EXAMPLE_CHECKMAC_DEVICE

/** \brief Use this definition if you like to lock the configuration zone
of the host during personalization.

Once the configuration zone is locked you cannot modify
the configuration zone anymore, but the ATSHA204 device will then generate
true random numbers instead of a 0xFFFF0000FFFF0000... sequence.
The example assumes that the data line of the host is
much less accessible by an adversary than the data line of the client.
Therefore, the example requests a random number from the host and not
the client, since an adversary could take over the data line and
inject a number of her choice.
*/
#define SHA204_EXAMPLE_CONFIG_WITH_LOCK   0

/** @} */


#ifdef SHA204_I2C
/** \brief I2C address for client device
If you have two devices at your disposal you can run an example as a real-world
host / client scenario. You have to change the address of one of the devices
by writing it to configuration zone address 16.
Be aware that bit 3 of the I2C address is also used to configure the input level
reference (see data sheet table 2-1).
To change the address you can run the \ref SHA204_EXAMPLE_READ_CONFIG_ZONE example.
*/
#   define SHA204_CLIENT_ADDRESS        (0xC8)
/** \brief I2C address for host device
To make the simple Mac / CheckMac I2C examples work out-of-the-box without 
changing the I2C address for the host device, you can make the host address the
same as the client address. See \ref SHA204_CLIENT_ADDRESS.
*/
//#   define SHA204_HOST_ADDRESS          SHA204_CLIENT_ADDRESS
#   define SHA204_HOST_ADDRESS          (0xCA)
#else
/** \ingroup  sha204_examples Device Selectors
These settings have an effect only when using bit-banging where the SDA of every 
device is connected to its own GPIO pin. When using only one UART the SDA of both 
devices is connected to the same GPIO pin. In that case you have create a
version of \ref sha204p_set_device_id that would use a Pause command. (Refer
to data sheet about the Pause command.)
@{ */
#   define SHA204_CLIENT_ADDRESS        (0x00)
#   define SHA204_HOST_ADDRESS          (0x01)
/** @} */
#endif

// Check example selection against project selection.
#if (SHA204_EXAMPLE == SHA204_EXAMPLE_DERIVE_KEY || SHA204_EXAMPLE == SHA204_EXAMPLE_DIVERSIFY_KEY)
#   ifdef SHA204_SWI_UART
#      error The selected example will not run under the UART project.
#   elif SHA204_CLIENT_ADDRESS == SHA204_HOST_ADDRESS
#      error The selected example needs different addresses for client and host.
#   endif
#endif
#if (SHA204_EXAMPLE == SHA204_EXAMPLE_CHANGE_I2C_ADDRESS && !defined(SHA204_I2C))
#   error The selected example will only run under the I2C project.
#endif


/** \ingroup  sha204_examples Key Identifiers Used by the Examples
Do not change these key identifiers since related values (configuration addresses)
are hard-coded in associated functions.
@{ */
#define SHA204_KEY_ID           ( 0)
#define SHA204_KEY_PARENT       (13)
#define SHA204_KEY_CHILD        (10)
/** @} */

#define sha204e_wakeup_sleep()   {sha204p_wakeup(); sha204p_sleep();}

uint8_t sha204e_checkmac_device(void);
uint8_t sha204e_checkmac_firmware(void);
uint8_t sha204e_checkmac_derived_key(void);
uint8_t sha204e_checkmac_diversified_key(void);
uint8_t sha204e_change_i2c_address(void);
uint8_t sha204e_read_config_zone(uint8_t device_id, uint8_t *config_data);

#endif /* SHA204_EXAMPLES_H_ */