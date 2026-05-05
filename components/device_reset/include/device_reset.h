//
// File Path: ESP-NOW-MeshCore/components/device_reset/include/device_reset.h
// Brief:     Header file for the generic Device Reset component.
// Author:    M. YOUCEF Yazid (yazid.youcef@gmail.com)
// Version:   0.3.0
// CreateDate: 2026-05-04
// UpdateDate: 2026-05-05
//

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
