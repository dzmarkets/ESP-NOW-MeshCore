//
// File Path: Master/components/sensors/include/sensors.h
// Brief:     Header file for sensors component.
//

#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize all hardware sensors (I2C, ADC, GPIO, etc.)
 */
void sensors_init(void);

/**
 * @brief Read data from all sensors and format it as a string
 * @param buffer Buffer to hold the formatted string
 * @param max_len Maximum length of the buffer
 */
void sensors_read(char *buffer, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif // SENSORS_H
