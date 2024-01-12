//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_CONFIG_H
#define RFID_CONFIG_H

// Defined Server Information
#define web_socket_server_url "xyz.com:80"
#define web_socket_secure_server_url "xyz.com:443"
#define base_server_url "pungkook.com"
#define base_api_server_url "api.pungkook.com"
#define ota_server_url "ota.pungkook.com"

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
#define backCancelNavButtonPinDefinition 0
#define menuSelectNavButtonPinDefinition 5
#define rightDownNavButtonPinDefinition 35

// Defined Buzzer Pin
#define buzzerPinDefinition 15

// Defined WiFi Configuration
#define device_hostname "RFID-001-"
#define default_wifi_ssid_1 "SFS OFFICE"
#define default_wifi_password_1 "sfs#office!@"
#define default_wifi_ssid_2 "ERP LTD"
#define default_wifi_password_2 "erp@@2020"
#define default_wifi_ssid_3 "HaiLong"
#define default_wifi_password_3 "99999999"

// Defined RFID Configuration
#define rfid_rx_pin 25
#define rfid_tx_pin 26

#endif //RFID_CONFIG_H
