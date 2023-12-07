#ifndef SERIALPORTIMPL_H
#define SERIALPORTIMPL_H 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Initialize serial port
 */
void SerialPort_Constructor(void);

/**
 * Open serial connection
 * @param baudrate The baud rate to use for communication
 * @return Strata error code: E_FAILED, if applying the configuration fails
 */
sr_t SerialPort_open(uint32_t baudrate);

/**
 * Check if the serial port has been opened
 *
 * @return true is the serial port is open. Otherwise, it returns false
 */
bool SerialPort_isOpened(void);

/**
 * Close serial connection
 * @return Strata error code
 */
sr_t SerialPort_close(void);

/**
* Clear the input buffer
* Discards any content in the serial port's input buffer
*/
void SerialPort_clearInputBuffer(void);

/**
* Flush the output buffer
* Wait to send out any remaining content in the serial port's output buffer
*/
void SerialPort_flushOutputBuffer(void);

/**
 * Write data to the remote device.
 * The write operation completes when:
 * 	- The specified data is written.
 * 	- The time specified by an internal Timeout passes (e.g. TimeConst_100s)
 *
 * @param data a buffer of the specified length
 * @param length number of bytes to be written
 *
 * @return Strata error code: E_TIMEOUT, if the write timeout occurs
 */
sr_t SerialPort_send(const uint8_t data[], uint16_t length);

/**
 * Read data from the remote device.
 * The read operation completes when:
 * 	- The specified number of bytes are read
 *  - The time specified by 'timeout' passes
 *  - If 'returnImmediately' is true and there is no data available right away
 *
 * @param data a buffer of the specified maximum length
 * @param length maximum number of bytes to be read (size of data)
 * @param timeout in milliseconds to wait for data to arrive to fill the buffer
 * @param returnImmediately if true, return immediately if no data is available
 * @param count out parameter receiving the amount of bytes that have been read.
 * @return Strata error code: E_TIMEOUT, if the write timeout occurs
 */
sr_t SerialPort_receive(uint8_t data[], uint16_t length, uint16_t timeout, bool returnImmediately, uint16_t *count);

/**
* Send null-terminated string to the remote device.
* @param data Null-terminated string to send
* @return Strata error code
*/
sr_t SerialPort_sendString(const char data[]);

#endif /* SERIALPORTIMPL_H */
