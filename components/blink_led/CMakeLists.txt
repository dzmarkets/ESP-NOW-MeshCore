#
# File Path: ESP-NOW-MeshCore/components/blink_led/CMakeLists.txt
# Brief:     CMake build configuration file for blink_led component.
# Author:    M. YOUCEF Yazid (yazid.youcef@gmail.com)
# Version:   0.2.0
# CreateDate: 2026-04-26
# UpdateDate: 2026-05-02
#
idf_component_register(SRCS "blink_led.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_hw_support shared_config led_strip esp_timer)
