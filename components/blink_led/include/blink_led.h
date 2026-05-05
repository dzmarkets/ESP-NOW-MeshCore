//
// File Path: ESP-NOW-MeshCore/components/blink_led/include/blink_led.h
// Brief:     Header file for blink_led component (RGB LED driver).
//            States: Red = Disconnected, Green = Connected, Blue = Sending/Waiting ACK.
// Author:    M. YOUCEF Yazid (yazid.youcef@gmail.com)
// Version:   0.2.0
// CreateDate: 2026-04-26
// UpdateDate: 2026-04-30
//

#ifndef BLINK_LED_H
#define BLINK_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief LED state definitions mapped to RGB colours.
 *
 *  LED_STATE_DISCONNECTED  – RED   : No peer connected yet.
 *  LED_STATE_CONNECTED     – GREEN : At least one peer is reachable.
 *  LED_STATE_SENDING       – BLUE  : Actively sending a message or waiting for ACK.
 */
typedef enum {
    LED_STATE_DISCONNECTED = 0, /**< RED   – no peer / offline */
    LED_STATE_CONNECTED,        /**< GREEN – all peers online  */
    LED_STATE_PARTIAL,          /**< GREEN BLINK – some peers offline */
    LED_STATE_SENDING,          /**< BLUE  – TX in progress    */
} led_state_t;

/**
 * @brief Configure the RGB LED GPIO pins and set initial state (disconnected/red).
 *        GPIO pin numbers are read from shared_config.h (RGB_LED_*_GPIO).
 */
void blink_led_configure(void);

/**
 * @brief Set the RGB LED to a specific logical state.
 * @param state One of LED_STATE_DISCONNECTED, LED_STATE_CONNECTED, LED_STATE_SENDING.
 */
void blink_led_set_state(led_state_t state);

/**
 * @brief Get the current LED logical state.
 * @return Current led_state_t value.
 */
led_state_t blink_led_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // BLINK_LED_H
