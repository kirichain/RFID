//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_DISPLAY_H
#define RFID_DISPLAY_H

#include "Arduino.h"
#include "qrcode_espi.h"
#include "GIFDraw.h"
#include "icons/loading_animation.h"
#include "AnimatedGIF.h"
#include "enums.h"
#include "structs.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include "icons/connect-to-device.h"
#include "icons/connect-to-server.h"
#include "icons/co-working.h"
#include "icons/scan-nearby-devices.h"
#include "icons/export.h"
#include "icons/rfid.h"
#include "icons/scan.h"
#include "icons/scan-history.h"
#include "icons/modify-rfid-tag.h"
#include "icons/register_rfid_tag.h"
#include "icons/wifi-setting.h"
#include "icons/user.h"
#include "icons/login.h"
#include "icons/logout.h"
#include "icons/import.h"
#include "icons/import-from-sd-card.h"
#include "icons/import-from-server.h"
#include "icons/import-from-computer.h"
#include "icons/package.h"
#include "icons/sync-data.h"
#include "icons/database-setting.h"
#include "icons/package-details.h"
#include "icons/wifi-connection-successful.h"
#include "icons/wifi-connection-failed.h"
#include "icons/server-connection-successful.h"
#include "icons/server-connection-failed.h"
#include "icons/register_rfid_tag_button.h"
#include "icons/setting_button.h"
#include "icons/incoming-packed-boxes-banner.h"
#include "icons/outgoing-packed-boxes-banner.h"
#include "icons/product-counting-banner.h"
#include "icons/incoming-box.h"
#include "icons/outgoing-box.h"
#include "icons/green_tick_icon.h"
#include "icons/red_x_icon.h"
#include "icons/qr-code-placeholder-banner.h"
#include "icons/blurred_register_rfid_tag_button.h"
#include "icons/blurred_scan_button.h"
#include "icons/mes_package_dialog_banner.h"
#include "icons/start_scanning_button.h"
#include "icons/submit_button.h"
#include "icons/scan_button.h"
#include "icons/text_scan_button.h"
#include "icons/company_logo.h"
#include "icons/clear_button.h"
#include "icons/none_wifi.h"
#include "icons/setting_icon.h"
#include "icons/wifi_setting_ssid_banner.h"
#include "icons/sound_setting_banner.h"
#include "icons/wifi_setting_guide_banner.h"
#include "icons/back_home_button.h"
#include "icons/sound.h"
#include "icons/muted_sound.h"
#include "icons/blurred_back_button.h"
#include "icons/blurred_done_button.h"
#include "icons/back_button.h"
#include "icons/done_button.h"
#include "icons/failure_registration_banner.h"
#include "icons/succcess_registration_banner.h"
#include "icons/success_submit_banner.h"
#include "icons/failure_submit_banner.h"

extern TFT_eSPI tft;

void GIFDraw(GIFDRAW *pDraw);

class Display {
public:
    // Constants
    const byte HEADER_HEIGHT = 36;
    const byte NAV_BAR_HEIGHT = HEADER_HEIGHT;
    static const byte numIcons = 62;

    // Colors
    uint32_t headerColor = 0x3B2D;
    uint32_t backgroundColor = 0x84B2;
    uint16_t screen_selector_border_color = backgroundColor;

    // Booleans
    bool is_background_task_required = false;
    bool is_background_task_completed = false;
    bool is_loading_animation_displayed = false;
    bool is_back_to_home = false;
    bool is_viewport_cleared = true;

    // Integers
    int SCREEN_WIDTH = 320;
    int SCREEN_HEIGHT = 480;
    int iconWidth = 64;
    int iconHeight = 64;
    byte screen_item_count;

    // Strings
    String qr_code_type = "";
    String current_screen_list_items[8] = {""};

    // Arrays
    feature_t current_screen_features[10];
    task_t current_screen_tasks[10];
    task_t current_screen_background_tasks[10];
    screen_item_position screen_items[10];

    // Structs and Enums
    feature_layout_t feature_layout;
    feature_item_type_t current_feature_item_type;
    screen_selector current_screen_selector;

    // Define an array of menu icon names corresponding to the header files
    const char *menu_icon_names[numIcons] = {
            "setting_icon",
            "rfid_icon",
            "package_icon",
            "co-working_icon",
            "database-setting_icon",
            "sync-data_icon",
            "connect-to-device_icon",
            "connect-to-server_icon",
            "scan-nearby-devices_icon",
            "scan_icon",
            "scan-history_icon",
            "modify-rfid-tag_icon",
            "wifi-setting_icon",
            "user_icon",
            "login_icon",
            "logout_icon",
            "import_icon",
            "export_icon",
            "import-from-sd-card_icon",
            "import-from-server_icon",
            "import-from-computer_icon",
            "package-details_icon",
            "register_rfid_tag_icon",
            "wifi_connection_successful_icon",
            "wifi_connection_failed_icon",
            "server_connection_successful_icon",
            "server_connection_failed_icon",
            "register_rfid_tag_button_icon",
            "setting_button_icon",
            "scan_button_icon",
            "incoming_packed_boxes_banner_icon",
            "outgoing_packed_boxes_banner_icon",
            "product_counting_banner_icon",
            "incoming_box_icon",
            "outgoing_box_icon",
            "green_tick_icon",
            "red_x_icon",
            "qr_code_placeholder_banner_icon",
            "blurred_register_rfid_tag_button_icon",
            "blurred_scan_button_icon",
            "mes_package_dialog_banner_icon",
            "start_scanning_button_icon",
            "text_scan_button_icon",
            "clear_button_icon",
            "submit_button_icon",
            "company_logo_icon",
            "none_wifi_icon",
            "wifi_setting_ssid_banner_icon",
            "sound_setting_banner_icon",
            "wifi_setting_guide_banner_icon",
            "back_home_button_icon",
            "sound_icon",
            "muted_sound_icon",
            "done_button_icon",
            "back_button_icon",
            "blurred_done_button_icon",
            "blurred_back_button_icon",
            "failure_registration_banner_icon",
            "success_registration_banner_icon",
            "success_submit_banner_icon",
            "failure_submit_banner_icon"
    };

    // Map menu names to menu icon data arrays
    menu_icon icons[numIcons] = {
            {"co-working_icon",                       co_working_icon},
            {"connect-to-device_icon",                connect_to_device_icon},
            {"connect-to-server_icon",                connect_to_server_icon},
            {"scan-nearby-devices_icon",              scan_nearby_devices_icon},
            {"rfid_icon",                             rfid_icon},
            {"scan_icon",                             scan_icon},
            {"scan-history_icon",                     scan_history_icon},
            {"modify-rfid-tag_icon",                  modify_rfid_tag_icon},
            {"setting_icon",                          setting_icon},
            {"wifi-setting_icon",                     wifi_setting_icon},
            {"user_icon",                             user_icon},
            {"login_icon",                            login_icon},
            {"logout_icon",                           logout_icon},
            {"database-setting_icon",                 database_setting_icon},
            {"import_icon",                           import_icon},
            {"import-from-sd-card_icon",              import_from_sd_card_icon},
            {"import-from-server_icon",               import_from_server_icon},
            {"import-from-computer_icon",             import_from_computer_icon},
            {"export_icon",                           export_icon},
            {"package_icon",                          package_icon},
            {"package-details_icon",                  package_details_icon},
            {"sync-data_icon",                        sync_data_icon},
            {"register_rfid_tag_icon",                register_rfid_tag_icon},
            {"wifi_connection_successful_icon",       wifi_connection_successful_icon},
            {"wifi_connection_failed_icon",           wifi_connection_failed_icon},
            {"server_connection_successful_icon",     server_connection_successful_icon},
            {"server_connection_failed_icon",         server_connection_failed_icon},
            {"register_rfid_tag_button_icon",         register_rfid_tag_button_icon},
            {"setting_button_icon",                   setting_button_icon},
            {"scan_button_icon",                      scan_button_icon},
            {"incoming_packed_boxes_banner_icon",     incoming_packed_boxes_banner_icon},
            {"outgoing_packed_boxes_banner_icon",     outgoing_packed_boxes_banner_icon},
            {"product_counting_banner_icon",          product_counting_banner_icon},
            {"incoming_box_icon",                     incoming_box_icon},
            {"outgoing_box_icon",                     outgoing_box_icon},
            {"green_tick_icon",                       green_tick_icon},
            {"red_x_icon",                            red_x_icon},
            {"qr_code_placeholder_banner_icon",       qr_code_placeholder_banner_icon},
            {"blurred_register_rfid_tag_button_icon", blurred_register_rfid_tag_button_icon},
            {"blurred_scan_button_icon",              blurred_scan_button_icon},
            {"mes_package_dialog_banner_icon",        mes_package_dialog_banner_icon},
            {"start_scanning_button_icon",            start_scanning_button_icon},
            {"text_scan_button_icon",                 text_scan_button_icon},
            {"clear_button_icon",                     clear_button_icon},
            {"submit_button_icon",                    submit_button_icon},
            {"company_logo_icon",                     company_logo_icon},
            {"none_wifi_icon",                        none_wifi_icon},
            {"wifi_setting_ssid_banner_icon",         wifi_setting_ssid_banner_icon},
            {"sound_setting_banner_icon",             sound_setting_banner_icon},
            {"wifi_setting_guide_banner_icon",        wifi_setting_guide_banner_icon},
            {"back_home_button_icon",                 back_home_button_icon},
            {"sound_icon",                            sound_icon},
            {"muted_sound_icon",                      muted_sound_icon},
            {"done_button_icon",                      done_button_icon},
            {"back_button_icon",                      back_button_icon},
            {"blurred_done_button_icon",              blurred_done_button_icon},
            {"blurred_back_button_icon",              blurred_back_button_icon},
            {"failure_registration_banner_icon",      failure_registration_banner_icon},
            {"success_registration_banner_icon",      succcess_registration_banner_icon},
            {"success_submit_banner_icon",            success_submit_banner_icon},
            {"failure_submit_banner_icon",            failure_submit_banner_icon}
    };

    Display();

    void init(feature_layout_t _feature_layout, task_results &_taskResults);

    void draw_layout(feature_layout_t _feature_layout, task_results &_taskResults);

    const menu_icon *get_icon_by_name(const char *icon_name);

    void put_icon(int x, int y, const char *icon_name);

    void render_feature(feature_t _feature, task_results &_taskResults);

    void reset_display_setting();

    void update_screen_item(byte _index, screen_item_position _item_position);

    void clear_screen_items();

    void update_screen_selector(byte _screen_item_index);

    void clear_screen_selector(task_results &_taskResults);

    void set_screen_selector_border_color(feature_t _next_feature);

    void update_rfid_registration_scan_result(task_results &_taskResults);

    void update_rfid_match_check_scan_result(task_results &_taskResults);
};

#endif //RFID_DISPLAY_H
