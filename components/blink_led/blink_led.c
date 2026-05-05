//
// File Path: ESP-NOW-MeshCore/components/blink_led/blink_led.c
// Brief:     Source file for blink_led component (RGB LED driver).
//            Drives a common-cathode 3-pin RGB LED using three independent GPIO pins.
//            GPIO pin numbers are centralised in shared_config.h.
//
//            Colour mapping:
//              RED   (LED_STATE_DISCONNECTED) – no peers, mesh offline.
//              GREEN (LED_STATE_CONNECTED)    – at least one peer reachable.
//              BLUE  (LED_STATE_SENDING)      – transmitting / waiting for ACK.
//
// Author:    M. YOUCEF Yazid (yazid.youcef@gmail.com)
// Version:   0.2.0
// CreateDate: 2026-04-26
// UpdateDate: 2026-04-30
//

#include "blink_led.h"
#include "shared_config.h"
#include "driver/gpio.h"
#include "esp_log.h"

#if !USE_EXTERNAL_RGB_LED
#include "led_strip.h"
static led_strip_handle_t s_led_strip = NULL;
#endif

#ifndef RGB_LED_RED_GPIO
#define RGB_LED_RED_GPIO    4
#endif

#ifndef RGB_LED_GREEN_GPIO
#define RGB_LED_GREEN_GPIO  5
#endif

#ifndef RGB_LED_BLUE_GPIO
#define RGB_LED_BLUE_GPIO   6
#endif

#include "esp_timer.h"

static const char *TAG = "blink_led";

// Track the current logical state
static led_state_t s_current_state = LED_STATE_DISCONNECTED;
static esp_timer_handle_t s_blink_timer = NULL;
static bool s_blink_toggle = false;

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

/** Drive all three colour channels to a specific RGB value (0 or 1 each). */
static void rgb_set(int r, int g, int b)
{
#if USE_EXTERNAL_RGB_LED
    gpio_set_level(RGB_LED_RED_GPIO,   r);
    gpio_set_level(RGB_LED_GREEN_GPIO, g);
    gpio_set_level(RGB_LED_BLUE_GPIO,  b);
#else
    if (s_led_strip) {
        if (r == 0 && g == 0 && b == 0) {
            led_strip_clear(s_led_strip);
        } else {
            // WS2812 is bright, using 50 instead of 255
            led_strip_set_pixel(s_led_strip, 0, r ? 50 : 0, g ? 50 : 0, b ? 50 : 0);
            led_strip_refresh(s_led_strip);
        }
    }
#endif
}

/** Periodic callback to toggle the LED for blinking states. */
static void blink_timer_callback(void* arg)
{
    if (s_current_state == LED_STATE_PARTIAL) {
        s_blink_toggle = !s_blink_toggle;
        if (s_blink_toggle) {
            rgb_set(0, 1, 0); // GREEN ON
        } else {
            rgb_set(0, 0, 0); // OFF
        }
    }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void blink_led_configure(void)
{
#if USE_EXTERNAL_RGB_LED
    ESP_LOGI(TAG, "Configuring EXTERNAL RGB LED  R=%d  G=%d  B=%d",
             RGB_LED_RED_GPIO, RGB_LED_GREEN_GPIO, RGB_LED_BLUE_GPIO);

    // Configure each colour channel as push-pull output, start LOW (off)
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << RGB_LED_RED_GPIO)
                      | (1ULL << RGB_LED_GREEN_GPIO)
                      | (1ULL << RGB_LED_BLUE_GPIO),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
#else
    ESP_LOGI(TAG, "Configuring INTERNAL WS2812B LED on GPIO %d", WS2812B_LED_GPIO);
    
    led_strip_config_t strip_config = {
        .strip_gpio_num = WS2812B_LED_GPIO,
        .max_leds = 1, 
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    if (led_strip_new_rmt_device(&strip_config, &rmt_config, &s_led_strip) == ESP_OK) {
        led_strip_clear(s_led_strip);
    } else {
        ESP_LOGE(TAG, "Failed to initialize WS2812B LED");
    }
#endif

    // Setup blinking timer (500ms toggle)
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &blink_timer_callback,
        .name = "blink_led_timer"
    };
    esp_timer_create(&periodic_timer_args, &s_blink_timer);
    esp_timer_start_periodic(s_blink_timer, 500000); // 500ms

    // Default state: RED (disconnected)
    blink_led_set_state(LED_STATE_DISCONNECTED);
}

void blink_led_set_state(led_state_t state)
{
    if (s_current_state == state) return;
    s_current_state = state;

    switch (state) {
        case LED_STATE_DISCONNECTED:
            rgb_set(1, 0, 0);   // RED
            ESP_LOGI(TAG, "LED -> RED   (No peers online)");
            break;

        case LED_STATE_CONNECTED:
            rgb_set(0, 1, 0);   // GREEN SOLID
            ESP_LOGI(TAG, "LED -> GREEN (All peers online)");
            break;

        case LED_STATE_PARTIAL:
            s_blink_toggle = true;
            rgb_set(0, 1, 0);   // Start GREEN ON
            ESP_LOGI(TAG, "LED -> GREEN BLINK (Partial mesh)");
            break;

        case LED_STATE_SENDING:
            rgb_set(0, 0, 1);   // BLUE
            ESP_LOGI(TAG, "LED -> BLUE  (Sending / Waiting ACK)");
            break;

        default:
            rgb_set(0, 0, 0);   // ALL OFF
            break;
    }
}

led_state_t blink_led_get_state(void)
{
    return s_current_state;
}
