//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_DISPLAY_H
#define RFID_DISPLAY_H

#include "Arduino.h"
#include "enums.h"
#include "structs.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include "icons/backpack.h"
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
#include "icons/setting.h"
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
#include "icons/scan_button.h"
#include "icons/incoming-packed-boxes-banner.h"
#include "icons/outgoing-packed-boxes-banner.h"
#include "icons/product-counting-banner.h"
#include "icons/incoming-box.h"
#include "icons/outgoing-box.h"
#include "icons/green_tick_icon.h"
#include "icons/red_x_icon.h"

class Display {
public:
    feature_layout_t feature_layout;
    status_indicators status;
    screen_selector current_screen_selector;
    screen_item_position screen_items[10];
    byte screen_item_count;
    feature_item_type_t current_feature_item_type;
    feature_t current_screen_features[10];
    task_t current_screen_tasks[10];
    task_t current_screen_background_tasks[10];
    bool is_background_task_required = false;
    bool is_background_task_completed = false;
    int SCREEN_WIDTH = 320;
    int SCREEN_HEIGHT = 480;

    // Item list on screen with screen item type = LIST_ITEM
    String screen_item_list_type_items[40] = {""};
    String screen_item_list_type_quantities[40] = {""};

    const byte HEADER_HEIGHT = 36;
    const byte NAV_BAR_HEIGHT = HEADER_HEIGHT;
    const byte ROW_HEIGHT = 25; // Height of each data row
    const int BORDER_COLOR = TFT_WHITE; // Assuming TFT_WHITE is the color constant for white
    const byte PAGE_INDICATOR_HEIGHT = 25;
    const byte VIEWPORT_HEIGHT = HEADER_HEIGHT - NAV_BAR_HEIGHT;

    // Column widths based on the content
    const byte COL_WIDTH_NO = 40;
    const byte COL_WIDTH_EPC = 110;
    const byte COL_WIDTH_TIMESTAMP = 130;
    const byte COL_WIDTH_COUNT = SCREEN_WIDTH - COL_WIDTH_NO - COL_WIDTH_EPC - COL_WIDTH_TIMESTAMP;
    // Remaining width for the "Timestamp" column
    const byte TEXT_PADDING = 5; // Padding for the text from the left side of the cell

    // RFID scan result layout
    const byte RFID_SCAN_RESULT_HEADER_HEIGHT = 80;
    const byte RFID_SCAN_RESULT_PRODUCT_INFO_HEIGHT = 150;
    const byte RFID_SCAN_RESULT_PRODUCT_IMG_WIDTH = 100;
    const byte RFID_SCAN_RESULT_PRODUCT_IMG_HEIGHT = 100;
    const byte RFID_SCAN_RESULT_TAG_INFO_HEIGHT =
            VIEWPORT_HEIGHT - RFID_SCAN_RESULT_HEADER_HEIGHT - RFID_SCAN_RESULT_PRODUCT_INFO_HEIGHT -
            RFID_SCAN_RESULT_TAG_INFO_HEIGHT;
    const byte VERTICAL_SPACE = 10; // Space between elements
    const byte TEXT_SPACING = 30; // Additional vertical space between lines of text in Part 3

    // Define colors for different UI elements
    uint32_t headerColor = 0x4A49;
    uint32_t navBarColor = 0x4A49;
    //uint32_t backgroundColor = 0x2966;
    uint32_t backgroundColor = TFT_BLACK;
    uint32_t textColor = TFT_WHITE;
    uint32_t borderColor = TFT_WHITE;
    uint16_t screen_selector_border_color = backgroundColor;

    // Placeholder text for various status indicators
    const char *wifiStatus = "WiFi: Connected";
    const char *dateTime = "2023-12-20 14:30";
    const char *serverStatus = "Server: Connected";
    const char *loginStatus = "User: Logged In";

    // Declare variables before the switch statement
    int dateTimeWidth, serverStatusWidth, dateTimeWidthLandscape, serverStatusWidthLandscape;
    int offset = -15; // Add an offset to adjust text position if needed

    // Constants for the grid layout
    byte numColumns = 2; // Number of columns in the grid
    byte numRows = 3; // Number of rows in the grid
//    const byte iconWidth = 96; // Width of the icon
//    const byte iconHeight = 96; // Height of the icon
    int iconWidth = 64; // Width of the icon
    int iconHeight = 64; // Height of the icon
    static const byte numIcons = 37; //  Number of icons
    const byte textHeight = 10; // Height of the text area under the icon

    // Calculate the horizontal and vertical spacing between the icons
    byte hSpacing;
    byte vSpacing;

    // Initialize x and y coordinates of the first icon
    byte menu_icon_x = NAV_BAR_HEIGHT;
    byte menu_icon_y = NAV_BAR_HEIGHT + 30;

    // Calculate starting y-coordinate for the navigation bar
    int navBarStartY;
    int iconCenterY;
    int iconSize;
    int leftIconX;
    int cancelIconX;
    int okIconX;
    int rightIconX;
    int iconSpacing;

    // Additional code for the page indicator
    const char *pageIndicator = "1/2"; // This text represents the current page and the total number of pages
    byte footerHeight = 31; // Height of the footer area for page indicator, adjust as needed
    byte pageIndicatorMargin = 1; // Vertical position for the page indicator

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
            "red_x_icon"
    };

    // Map menu names to menu icon data arrays
    menu_icon icons[numIcons] = {
            {"co-working_icon",                   co_working_icon},
            {"connect-to-device_icon",            connect_to_device_icon},
            {"connect-to-server_icon",            connect_to_server_icon},
            {"scan-nearby-devices_icon",          scan_nearby_devices_icon},
            {"rfid_icon",                         rfid_icon},
            {"scan_icon",                         scan_icon},
            {"scan-history_icon",                 scan_history_icon},
            {"modify-rfid-tag_icon",              modify_rfid_tag_icon},
            {"setting_icon",                      setting_icon},
            {"wifi-setting_icon",                 wifi_setting_icon},
            {"user_icon",                         user_icon},
            {"login_icon",                        login_icon},
            {"logout_icon",                       logout_icon},
            {"database-setting_icon",             database_setting_icon},
            {"import_icon",                       import_icon},
            {"import-from-sd-card_icon",          import_from_sd_card_icon},
            {"import-from-server_icon",           import_from_server_icon},
            {"import-from-computer_icon",         import_from_computer_icon},
            {"export_icon",                       export_icon},
            {"package_icon",                      package_icon},
            {"package-details_icon",              package_details_icon},
            {"sync-data_icon",                    sync_data_icon},
            {"register_rfid_tag_icon",            register_rfid_tag_icon},
            {"wifi_connection_successful_icon",   wifi_connection_successful_icon},
            {"wifi_connection_failed_icon",       wifi_connection_failed_icon},
            {"server_connection_successful_icon", server_connection_successful_icon},
            {"server_connection_failed_icon",     server_connection_failed_icon},
            {"register_rfid_tag_button_icon",     register_rfid_tag_button_icon},
            {"setting_button_icon",               setting_button_icon},
            {"scan_button_icon",                  scan_button_icon},
            {"incoming_packed_boxes_banner_icon", incoming_packed_boxes_banner_icon},
            {"outgoing_packed_boxes_banner_icon", outgoing_packed_boxes_banner_icon},
            {"product_counting_banner_icon",      product_counting_banner_icon},
            {"incoming_box_icon",                 incoming_box_icon},
            {"outgoing_box_icon",                 outgoing_box_icon},
            {"green_tick_icon",                   green_tick_icon},
            {"red_x_icon",                        red_x_icon}

    };

    // Example history array
    rfid_scan_result history[14] = {
            {true, {"EPC123456789", "PID123", "M",  "Red",     "img_url_1",  "STID123", "BrandA", "PO123456", ASSOCIATED},   "2024-01-02 10:00", 5},
            {true, {"EPC987654321", "PID987", "L",  "Blue",    "img_url_2",  "STID987", "BrandB", "PO654321", UNASSOCIATED}, "2024-01-02 09:45", 5},
            {true, {"EPC564738291", "PID564", "S",  "Green",   "img_url_3",  "STID564", "BrandC", "PO564738", ASSOCIATED},   "2024-01-02 09:30", 5},
            {true, {"EPC864213579", "PID864", "XL", "Black",   "img_url_4",  "STID864", "BrandD", "PO864213", UNASSOCIATED}, "2024-01-02 09:15", 5},
            {true, {"EPC975310864", "PID975", "M",  "Yellow",  "img_url_5",  "STID975", "BrandE", "PO975310", ASSOCIATED},   "2024-01-02 09:00", 5},
            {true, {"EPC472859106", "PID472", "L",  "White",   "img_url_6",  "STID472", "BrandF", "PO472859", UNASSOCIATED}, "2024-01-02 08:45", 5},
            {true, {"EPC582947015", "PID582", "S",  "Purple",  "img_url_7",  "STID582", "BrandG", "PO582947", ASSOCIATED},   "2024-01-02 08:30", 5},
            {true, {"EPC693840257", "PID693", "XL", "Orange",  "img_url_8",  "STID693", "BrandH", "PO693840", UNASSOCIATED}, "2024-01-02 08:15", 5},
            {true, {"EPC204857396", "PID204", "M",  "Pink",    "img_url_9",  "STID204", "BrandI", "PO204857", ASSOCIATED},   "2024-01-02 08:00", 5},
            {true, {"EPC918273645", "PID918", "L",  "Grey",    "img_url_10", "STID918", "BrandJ", "PO918273", UNASSOCIATED}, "2024-01-02 07:45", 5},
            {true, {"EPC918273645", "PID918", "S",  "Brown",   "img_url_11", "STID918", "BrandK", "PO918273", ASSOCIATED},   "2024-01-02 07:45", 5},
            {true, {"EPC918273645", "PID918", "XL", "Cyan",    "img_url_12", "STID918", "BrandL", "PO918273", UNASSOCIATED}, "2024-01-02 07:45", 5},
            {true, {"EPC918273645", "PID918", "M",  "Magenta", "img_url_13", "STID918", "BrandM", "PO918273", ASSOCIATED},   "2024-01-02 07:45", 5},
            {true, {"EPC918273645", "PID918", "L",  "Lime",    "img_url_14", "STID918", "BrandN", "PO918273", UNASSOCIATED}, "2024-01-02 07:45", 5},
    };

    Display();

    void init(feature_layout_t _feature_layout);

    void draw_layout(feature_layout_t _feature_layout);

    static byte get_string_width(const char *string);

    static byte get_font_height();

    const menu_icon *get_icon_by_name(const char *icon_name);

    void put_icon(int x, int y, const char *icon_name);

    void put_text(int x, int y, const char *content) const;

    void draw_icon_with_label(int x, int y, byte iconIndex, const char *iconNames[]);

    void render_icons_grid(const byte *iconIndices, byte _numIcons, feature_render_type_t render_type);

    void render_item_list(bool is_new_list_set, bool navigation_direction);

    static byte calculate_columns(byte iconCount);

    static byte calculate_rows(byte iconCount, byte numColumns);

    void render_feature(feature_t _feature, task_results &_taskResults);

    void draw_history_item(byte index, const rfid_scan_result &item) const;

    void draw_vertical_line() const;

    void draw_grid() const;

    void draw_titles() const;

    static void blink_screen(bool &isTaskCompleted);

    void update_status(status_indicators _status);

    static uint16_t convert_to_565_color(uint32_t hex_color);

    void reset_display_setting();

    void update_screen_item(byte _index, screen_item_position _item_position);

    void clear_screen_items();

    void update_screen_selector(byte _screen_item_index);

    void clear_screen_selector() const;

    void set_screen_selector_border_color(feature_t _next_feature);
};

#endif //RFID_DISPLAY_H
