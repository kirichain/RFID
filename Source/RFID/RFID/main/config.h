//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_CONFIG_H
#define RFID_CONFIG_H

// Defined Wi-Fi Configuration
#define device_hostname "RFID-001"
#define default_wifi_ssid_1 "SFS OFFICE"
#define default_wifi_password_1 "sfs#office!@"
#define default_wifi_ssid_2 "ERP LTD"
#define default_wifi_password_2 "erp@@2020"
#define default_wifi_ssid_3 "kiri"
#define default_wifi_password_3 "101conchodom"

// Defined Server Information
#define web_socket_server_url "xyz.com:80"
#define web_socket_secure_server_url "xyz.com:443"
#define base_server_url "pungkook.com"
#define base_api_server_url "http://mespk2.pungkookvn.com:8888"
#define ota_server_url "ota.pungkook.com"
#define tpm_server_url "http://tpm.pungkookvn.com"

// Defined API Endpoints & Queries
#define get_mqtt_config "/RFIotDevice/GetMqttConfig"
#define get_rfid_tag_info "/RFIotDevice/GetCardRegistration"
#define register_new_rfid_tag "/RFIotDevice/PostCardRegistration"
#define get_mqtt_config_query "?macAddress="
#define get_rfid_tag_info_query "?mesKey="
#define get_resized_mes_img "/mpmts/minimizeImage"
#define get_resized_mes_img_query "?width=70&height=70&url="

// Defined MQTT Configuration
#define	mqtt_tcp_server "125.234.135.55"
#define	mqtt_auto_reconnect_delay 5000
#define	mqtt_username "pk2"
#define	mqtt_password "abc123"
#define	mqtt_port 1883
#define	mqtt_wsport 1884
#define mqtt_lwt_topic "rfid/status/"
#define mqtt_mes_selection_topic "rfid/mes/"

// Defined FS Information
#define fs_device_data_file "device_data.txt"
#define fs_message_queue_file "message_queue.txt"
#define fs_user_data_file "user_data.txt"
#define fs_warehouse_file "warehouse_data.txt"
#define fs_package_file "package_data.txt"

// Defined SD Card Information
#define sd_card_csv_import_file "import.csv"
#define sd_card_import_file "import.txt"
#define sd_card_csv_export_file "export.csv"
#define sd_card_export_file "export.txt"

// Defined Display Configuration
//#define display_size 3.5
//#define display_size 7
//#define display_size 2.8

// Defined Navigation Button Pins
#define leftUpNavButtonPinDefinition 4
#define backCancelNavButtonPinDefinition 27
#define menuSelectNavButtonPinDefinition 32
#define rightDownNavButtonPinDefinition 33

// Defined Buzzer Pin
#define buzzerPinDefinition 15

// Defined RFID Module Configuration
#define rfid_rx_pin 25
#define rfid_tx_pin 26

#endif //RFID_CONFIG_H
