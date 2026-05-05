//
// File Path: ESP-NOW-MeshCore/components/device_reset/device_reset.c
// Brief:     Source file for the generic Device Reset component.
// Author:    M. YOUCEF Yazid (yazid.youcef@gmail.com)
// Version:   0.3.0
// CreateDate: 2026-05-04
// UpdateDate: 2026-05-05
//

#include "device_reset.h"
#include "shared_config.h"
#include "mesh_manager.h"
#include "status_indicator.h"
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

            // 2. Visual Confirmation: Flash the Disconnected (Red) state
            for (int i = 0; i < 6; i++) {
                status_indicator_set_state(LED_STATE_DISCONNECTED);
                vTaskDelay(pdMS_TO_TICKS(300));
                status_indicator_set_state(LED_STATE_OFF);
                vTaskDelay(pdMS_TO_TICKS(300));
            }
        } else {
            ESP_LOGI(TAG, "Reset cancelled.");
        }
    }
}
