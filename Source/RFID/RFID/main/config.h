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
#define base_api_server_url "http://mespk2.pungkookvn.com:8888"
#define tpm_server_url "http://tpm.pungkookvn.com"
#define resized_image_server_url "http://203.113.151.196:8888"
#define header_name_1 "keyCode"
#define header_value_1 "PkerpVN2024*"

// Defined API Endpoints & Queries
#define get_mqtt_config "/RFIotDevice/GetMqttConfig"
#define get_registered_rfid_tag_list "/RFIotDevice/GetCardRegistration"
#define register_new_rfid_tag "/RFIotDevice/PostCardRegistration"
#define get_resized_mes_img "/mpmts/minimizeImage"
#define get_mqtt_config_query "?macAddress="
#define get_registered_rfid_tag_mes_key_query "?mesKey="
#define get_registered_rfid_tag_mes_type_query "type="
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
#define fs_device_data_file "/device_data.txt"

// Defined Navigation Button Pins
#define leftUpNavButtonPinDefinition 4
//27
#define backCancelNavButtonPinDefinition 0
//32
#define menuSelectNavButtonPinDefinition 5
//33
#define rightDownNavButtonPinDefinition 32

// Defined Gun Button Pin
#define gunButtonPinDefinition 12

// Defined Buzzer Pin
#define buzzerPinDefinition 27

// Defined RFID Module Configuration
#define rfid_rx_pin 25
#define rfid_tx_pin 26

#endif //RFID_CONFIG_H
