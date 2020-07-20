# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# compile C with /home/johann/.espressif/tools/xtensa-esp32-elf/esp-2020r1-8.2.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
C_FLAGS = -mlongcalls -Wno-frame-address  

C_DEFINES = -DHAVE_CONFIG_H -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\" -DUNITY_INCLUDE_CONFIG_H -DWITH_POSIX

C_INCLUDES = -I/home/johann/.esp/esp-idf/components/xtensa/include -I/home/johann/.esp/esp-idf/components/xtensa/esp32/include -I/home/johann/github/motion_glove/espidf-arduino-blink/build/config -I/home/johann/.esp/esp-idf/components/newlib/platform_include -I/home/johann/.esp/esp-idf/components/freertos/include -I/home/johann/.esp/esp-idf/components/freertos/xtensa/include -I/home/johann/.esp/esp-idf/components/heap/include -I/home/johann/.esp/esp-idf/components/log/include -I/home/johann/.esp/esp-idf/components/lwip/include/apps -I/home/johann/.esp/esp-idf/components/lwip/include/apps/sntp -I/home/johann/.esp/esp-idf/components/lwip/lwip/src/include -I/home/johann/.esp/esp-idf/components/lwip/port/esp32/include -I/home/johann/.esp/esp-idf/components/lwip/port/esp32/include/arch -I/home/johann/.esp/esp-idf/components/soc/src/esp32/. -I/home/johann/.esp/esp-idf/components/soc/src/esp32/include -I/home/johann/.esp/esp-idf/components/soc/include -I/home/johann/.esp/esp-idf/components/esp_rom/include -I/home/johann/.esp/esp-idf/components/esp_common/include -I/home/johann/.esp/esp-idf/components/esp_system/include -I/home/johann/.esp/esp-idf/components/esp32/include -I/home/johann/.esp/esp-idf/components/driver/include -I/home/johann/.esp/esp-idf/components/driver/esp32/include -I/home/johann/.esp/esp-idf/components/esp_ringbuf/include -I/home/johann/.esp/esp-idf/components/efuse/include -I/home/johann/.esp/esp-idf/components/efuse/esp32/include -I/home/johann/.esp/esp-idf/components/espcoredump/include -I/home/johann/.esp/esp-idf/components/esp_timer/include -I/home/johann/.esp/esp-idf/components/soc/soc/esp32/. -I/home/johann/.esp/esp-idf/components/soc/soc/esp32/include -I/home/johann/.esp/esp-idf/components/soc/soc/esp32/../include -I/home/johann/.esp/esp-idf/components/vfs/include -I/home/johann/.esp/esp-idf/components/esp_wifi/include -I/home/johann/.esp/esp-idf/components/esp_wifi/esp32/include -I/home/johann/.esp/esp-idf/components/esp_event/include -I/home/johann/.esp/esp-idf/components/esp_netif/include -I/home/johann/.esp/esp-idf/components/esp_eth/include -I/home/johann/.esp/esp-idf/components/tcpip_adapter/include -I/home/johann/.esp/esp-idf/components/app_trace/include -I/home/johann/.esp/esp-idf/components/mbedtls/port/include -I/home/johann/.esp/esp-idf/components/mbedtls/mbedtls/include -I/home/johann/.esp/esp-idf/components/mbedtls/esp_crt_bundle/include -I/home/johann/.esp/esp-idf/components/bootloader_support/include -I/home/johann/.esp/esp-idf/components/app_update/include -I/home/johann/.esp/esp-idf/components/spi_flash/include -I/home/johann/.esp/esp-idf/components/wpa_supplicant/include -I/home/johann/.esp/esp-idf/components/wpa_supplicant/port/include -I/home/johann/.esp/esp-idf/components/wpa_supplicant/include/esp_supplicant -I/home/johann/.esp/esp-idf/components/nvs_flash/include -I/home/johann/.esp/esp-idf/components/pthread/include -I/home/johann/.esp/esp-idf/components/perfmon/include -I/home/johann/.esp/esp-idf/components/asio/asio/asio/include -I/home/johann/.esp/esp-idf/components/asio/port/include -I/home/johann/.esp/esp-idf/components/cbor/port/include -I/home/johann/.esp/esp-idf/components/coap/port/include -I/home/johann/.esp/esp-idf/components/coap/port/include/coap -I/home/johann/.esp/esp-idf/components/coap/libcoap/include -I/home/johann/.esp/esp-idf/components/coap/libcoap/include/coap2 -I/home/johann/.esp/esp-idf/components/console -I/home/johann/.esp/esp-idf/components/nghttp/port/include -I/home/johann/.esp/esp-idf/components/nghttp/nghttp2/lib/includes -I/home/johann/.esp/esp-idf/components/esp-tls -I/home/johann/.esp/esp-idf/components/esp_adc_cal/include -I/home/johann/.esp/esp-idf/components/esp_gdbstub/include -I/home/johann/.esp/esp-idf/components/esp_hid/include -I/home/johann/.esp/esp-idf/components/tcp_transport/include -I/home/johann/.esp/esp-idf/components/esp_http_client/include -I/home/johann/.esp/esp-idf/components/esp_http_server/include -I/home/johann/.esp/esp-idf/components/esp_https_ota/include -I/home/johann/.esp/esp-idf/components/protobuf-c/protobuf-c -I/home/johann/.esp/esp-idf/components/protocomm/include/common -I/home/johann/.esp/esp-idf/components/protocomm/include/security -I/home/johann/.esp/esp-idf/components/protocomm/include/transports -I/home/johann/.esp/esp-idf/components/mdns/include -I/home/johann/.esp/esp-idf/components/esp_local_ctrl/include -I/home/johann/.esp/esp-idf/components/sdmmc/include -I/home/johann/.esp/esp-idf/components/esp_serial_slave_link/include -I/home/johann/.esp/esp-idf/components/esp_websocket_client/include -I/home/johann/.esp/esp-idf/components/expat/expat/expat/lib -I/home/johann/.esp/esp-idf/components/expat/port/include -I/home/johann/.esp/esp-idf/components/wear_levelling/include -I/home/johann/.esp/esp-idf/components/fatfs/diskio -I/home/johann/.esp/esp-idf/components/fatfs/vfs -I/home/johann/.esp/esp-idf/components/fatfs/src -I/home/johann/.esp/esp-idf/components/freemodbus/common/include -I/home/johann/.esp/esp-idf/components/idf_test/include -I/home/johann/.esp/esp-idf/components/idf_test/include/esp32 -I/home/johann/.esp/esp-idf/components/jsmn/include -I/home/johann/.esp/esp-idf/components/json/cJSON -I/home/johann/.esp/esp-idf/components/libsodium/libsodium/src/libsodium/include -I/home/johann/.esp/esp-idf/components/libsodium/port_include -I/home/johann/.esp/esp-idf/components/mqtt/esp-mqtt/include -I/home/johann/.esp/esp-idf/components/openssl/include -I/home/johann/.esp/esp-idf/components/spiffs/include -I/home/johann/.esp/esp-idf/components/ulp/include -I/home/johann/.esp/esp-idf/components/unity/include -I/home/johann/.esp/esp-idf/components/unity/unity/src -I/home/johann/.esp/esp-idf/components/wifi_provisioning/include 

