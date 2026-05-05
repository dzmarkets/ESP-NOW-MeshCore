#
# File Path: ESP-NOW-MeshCore/components/device_reset/CMakeLists.txt
# Brief:     CMake build configuration file for device_reset component.
# Author:    M. YOUCEF Yazid (yazid.youcef@gmail.com)
# Version:   0.1.0
# CreateDate: 2026-05-04
# UpdateDate: 2026-05-04
#
idf_component_register(SRCS "device_reset.c"
                      INCLUDE_DIRS "include"
                      REQUIRES "shared_config" "mesh_manager" "blink_led" "driver")
