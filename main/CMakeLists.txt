#
# File Path: ESP-NOW-MeshCore/main/CMakeLists.txt
# Brief:     CMake build configuration file for main component.
# Author:    M. YOUCEF Yazid (yazid.youcef@gmail.com)
# Version:   0.2.2
# CreateDate: 2026-04-25
# UpdateDate: 2026-05-04
#
idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES espnow_control blink_led message_provider
                                message_processor mesh_manager sensors
                                shared_config esp_timer device_reset)
