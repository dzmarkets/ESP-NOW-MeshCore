//
// File Path: Master/components/device_reset/device_reset.c
// Brief:     Source file for the generic Device Reset component.
//

#include "device_reset.h"
#include "shared_config.h"
#include "mesh_manager.h"
#include "blink_led.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "device_reset";

void device_reset_check_trigger(void)
{
    // Initialize the Reset Button GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << FACTORY_RESET_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    if (gpio_get_level(FACTORY_RESET_GPIO) == 0) {
        ESP_LOGW(TAG, "Device Reset Button [GPIO %d] HELD! Wait 3s...", FACTORY_RESET_GPIO);

        vTaskDelay(pdMS_TO_TICKS(3000));

        if (gpio_get_level(FACTORY_RESET_GPIO) == 0) {
            ESP_LOGW(TAG, "Factory Reset Triggered!");
            
            // 1. Clear Mesh Data
            mesh_manager_factory_reset();

            // 2. Future: Clear WiFi credentials or other NVS namespaces here
            // nvs_flash_erase(); 

            // Visual Confirmation: Medium Red Blink (300ms)
            for (int i = 0; i < 6; i++) {
                gpio_set_level(RGB_LED_RED_GPIO, 1);
                gpio_set_level(RGB_LED_GREEN_GPIO, 0);
                gpio_set_level(RGB_LED_BLUE_GPIO, 0);
                vTaskDelay(pdMS_TO_TICKS(300));

                gpio_set_level(RGB_LED_RED_GPIO, 0);
                vTaskDelay(pdMS_TO_TICKS(300));
            }
        } else {
            ESP_LOGI(TAG, "Reset cancelled.");
        }
    }
}
