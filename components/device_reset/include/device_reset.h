#ifndef DEVICE_RESET_H
#define DEVICE_RESET_H

/**
 * @brief Checks for a Factory Reset trigger during device startup.
 * 
 *        This is a generic device reset utility that can be expanded 
 *        to clear multiple subsystems (Mesh, WiFi, NVS, etc.).
 */
void device_reset_check_trigger(void);

#endif // DEVICE_RESET_H
