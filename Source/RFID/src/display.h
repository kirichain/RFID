//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_DISPLAY_H
#define RFID_DISPLAY_H

#include "Arduino.h"
#include "enums.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include "structs.h"
#include "icons/connect-to-device.h"
#include "icons/connect-to-server.h"
#include "icons/co-working.h"
#include "icons/scan-nearby-devices.h"
#include "icons/export.h"
#include "icons/rfid.h"
#include "icons/scan.h"
#include "icons/scan-history.h"
#include "icons/modify-rfid-tag.h"
#include "icons/setting.h"
#include "icons/wifi-setting.h"
#include "icons/user.h"
#include "icons/login.h"
#include "icons/logout.h"
#include "icons/import.h"
#include "icons/import-from-sd-card.h"
#include "icons/import-from-server.h"
#include "icons/package.h"
#include "icons/sync-data.h"
#include "icons/database-setting.h"
#include "icons/package-details.h"

class Display {
public:
    feature_layout_t feature_layout;
    status_indicators status;

    int SCREEN_WIDTH, SCREEN_HEIGHT;

    const int HEADER_HEIGHT = 40;
    const int NAV_BAR_HEIGHT = HEADER_HEIGHT;

    // Define colors for different UI elements
    uint32_t headerColor = TFT_DARKGREY;
    uint32_t navBarColor = TFT_DARKGREY;
    uint32_t textColor = TFT_WHITE;
    uint32_t borderColor = TFT_WHITE;

    // Placeholder text for various status indicators
    const char *wifiStatus = "WiFi: Connected";
    const char *dateTime = "2023-12-20 14:30";
    const char *serverStatus = "Server: Connected";
    const char *loginStatus = "User: Logged In";

    // Declare variables before the switch statement
    int dateTimeWidth, serverStatusWidth, dateTimeWidthLandscape, serverStatusWidthLandscape;
    int offset = -15; // Add an offset to adjust text position if needed

    // Constants for the grid layout
    int numColumns = 2; // Number of columns in the grid
    int numRows = 3; // Number of rows in the grid
    const int iconWidth = 96; // Width of the icon
    const int iconHeight = 96; // Height of the icon
    const int textHeight = 10; // Height of the text area under the icon

    // Calculate the horizontal and vertical spacing between the icons
    int hSpacing;
    int vSpacing;

    // Initialize x and y coordinates of the first icon
    int menu_icon_x = NAV_BAR_HEIGHT;
    int menu_icon_y = NAV_BAR_HEIGHT + 15;

    // Calculate starting y-coordinate for the navigation bar
    int navBarStartY;
    int iconCenterY;
    int iconSize;
    int leftIconX;
    int cancelIconX;
    int okIconX;
    int rightIconX;
    int iconSpacing;
    int iconIndex;

    // Define an array of menu icon names corresponding to the header files
    const char *menu_icon_names[21] = {
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
            "package-details_icon"
    };

    // Map menu names to menu icon data arrays
    menu_icon icons[21] = {
            {"co-working_icon",          co_working_icon},
            {"connect-to-device_icon",   connect_to_device_icon},
            {"connect-to-server_icon",   connect_to_server_icon},
            {"scan-nearby-devices_icon", scan_nearby_devices_icon},
            {"rfid_icon",                rfid_icon},
            {"scan_icon",                scan_icon},
            {"scan-history_icon",        scan_history_icon},
            {"modify-rfid-tag_icon",     modify_rfid_tag_icon},
            {"setting_icon",             setting_icon},
            {"wifi-setting_icon",        wifi_setting_icon},
            {"user_icon",                user_icon},
            {"login_icon",               login_icon},
            {"logout_icon",              logout_icon},
            {"database-setting_icon",    database_setting_icon},
            {"import_icon",              import_icon},
            {"import-from-sd-card_icon", import_from_sd_card_icon},
            {"import-from-server_icon",  import_from_server_icon},
            {"export_icon",              export_icon},
            {"package_icon",             package_icon},
            {"package-details_icon",     package_details_icon},
            {"sync-data_icon",           sync_data_icon},
    };

    // Additional code for the page indicator
    const char *pageIndicator = "1/2"; // This text represents the current page and the total number of pages
    int footerHeight = 30; // Height of the footer area for page indicator, adjust as needed
    int pageIndicatorMargin = 1; // Vertical position for the page indicator

    Display();

    void init(feature_layout_t _feature_layout);

    void draw_layout(feature_layout_t _feature_layout);

    int get_string_width(const char *string);

    const menu_icon *get_icon_by_name(const char *icon_name);

    void put_icon(int x, int y, const char *icon_name);

    void put_text(int x, int y, const char *content);

    void draw_icon_with_label(int x, int y, int iconIndex, const char *iconNames[], TFT_eSPI &tft);

    int calculate_columns(int iconCount);

    int calculate_rows(int iconCount, int numColumns);

    void render_feature(feature_t _feature);

    void blink_screen(bool &isTaskCompleted);

    void update_status(status_indicators _status);
};

#endif //RFID_DISPLAY_H
