//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_CONFIG_H
#define RFID_CONFIG_H

// Define Server Information
#define web_socket_server_url "xyz.com:80"
#define web_socket_secure_server_url "xyz.com:443"
#define base_server_url "pungkook.com"
#define base_api_server_url "api.pungkook.com"
#define ota_server_url "ota.pungkook.com"

// Define FS Information
#define fs_device_data_file "device_data.txt"
#define fs_message_queue_file "message_queue.txt"
#define fs_user_data_file "user_data.txt"
#define fs_warehouse_file "warehouse_data.txt"
#define fs_package_file "package_data.txt"

// Define SD Card Information
#define sd_card_csv_import_file "import.csv"
#define sd_card_import_file "import.txt"
#define sd_card_csv_export_file "export.csv"
#define sd_card_export_file "export.txt"

// Define Display Configuration
//#define display_size 3.5
//#define display_size 7
//#define display_size 2.8

// Define Navigation Buttons
#define menuSelectNavButton 4
#define leftUpNavButton 5
#define rightDownNavButton 6

#endif //RFID_CONFIG_H
