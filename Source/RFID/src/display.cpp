//
// Created by Tan on 25-Oct-23.
//

#include "display.h"

AnimatedGIF gif;
TFT_eSPI tft = TFT_eSPI();
QRcode_eSPI qrcode(&tft);

// This section is used for display GIF---------------------------------------------------
#ifdef USE_DMA
uint16_t usTemp[2][BUFFER_SIZE]; // Definition for DMA buffer
#else
uint16_t usTemp[1][BUFFER_SIZE]; // Definition for non-DMA buffer
#endif
bool dmaBuf = 0; // Definition of dmaBuf
// End section----------------------------------------------------------------------------

Display::Display() {

}

void Display::init(feature_layout_t _feature_layout) {
    // Initialize display
    feature_layout = _feature_layout;
    tft.init();
    qrcode.init();
    tft.setTextFont(2);
    tft.setSwapBytes(true);
    if (_feature_layout == PORTRAIT) {
        tft.setRotation(0);
        // Display resolution
        SCREEN_WIDTH = 320;
        SCREEN_HEIGHT = 480;
    } else {
        tft.setRotation(1);
        // Display resolution
        SCREEN_WIDTH = 480;
        SCREEN_HEIGHT = 320;
    }
    tft.fillScreen(backgroundColor);
    draw_layout(feature_layout);
    gif.begin(BIG_ENDIAN_PIXELS);
}

void Display::draw_layout(feature_layout_t _feature_layout) {
    // Clear the screen before drawing the layout
    tft.fillScreen(backgroundColor);

    // Set text font and size
    tft.setTextSize(1);
    tft.setTextDatum(TL_DATUM); // Top-Left datum

    switch (_feature_layout) {
        case PORTRAIT: {
            // Draw header at the top
            tft.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, headerColor);

            // Set text color for the header
            tft.setTextColor(textColor, headerColor);

            // Wifi and server status icons
            tft.pushImage(10, 11, 19, 14, company_logo_icon);
            tft.pushImage(39, 10, 16, 16, server_connection_successful_icon);
            tft.drawString("Server not connected", 58, 10);
            tft.pushImage(294, 10, 16, 16, wifi_connection_successful_icon);

            // Draw date and time aligned to the top-right of the header
            tft.setTextDatum(TR_DATUM); // Align to the top-right
            //tft.drawString(dateTime, SCREEN_WIDTH - 5, 10);

            break;
        }
        case LANDSCAPE:
            // Draw header at the top
            tft.fillRect(0, 0, SCREEN_HEIGHT, HEADER_HEIGHT, headerColor);

            // Set text color for the header
            tft.setTextColor(textColor, headerColor);

            // Draw WiFi status at the top-left
            tft.drawString(wifiStatus, 5, 5);

            // Draw login status directly below WiFi status
            tft.drawString(loginStatus, 5, 20);

            // Draw date and time at the top-right
            dateTimeWidthLandscape = get_string_width(dateTime);
            tft.drawString(dateTime, SCREEN_HEIGHT - dateTimeWidthLandscape - 5, 5);

            // Draw server status directly below date and time
            serverStatusWidthLandscape = get_string_width(serverStatus);
            tft.drawString(serverStatus, SCREEN_HEIGHT - serverStatusWidthLandscape - 5, 20);

            // Draw viewport below the header
            tft.drawRect(0, HEADER_HEIGHT, SCREEN_HEIGHT, SCREEN_WIDTH - HEADER_HEIGHT, borderColor);
            break;
    }
}

byte Display::get_string_width(const char *string) {
    // The width of the string is now calculated based on the actual font
    byte stringWidth = tft.textWidth(string);
    return stringWidth;
}

const menu_icon *Display::get_icon_by_name(const char *icon_name) {
    for (uint16_t i = 0; i < numIcons; ++i) {
        if (strcmp(icons[i].name, icon_name) == 0) {
//            Serial.println(F("Got icon name: "));
//            Serial.println(String(icon_name));
//            Serial.println(icons[i].name);
//            Serial.println(i);
            return &icons[i];
        }
    }
    return nullptr; // Return nullptr if the icon is not found
}

void Display::put_icon(int x, int y, const char *icon_name) {
    const menu_icon *icon = get_icon_by_name(icon_name);
    if (icon != nullptr) {
//        Serial.println("Icon to be put: ");
//        Serial.println(String(icon_name));
        tft.pushImage(x, y, iconWidth, iconHeight, icon->icon_data);
    } else {
        // Handle the error, for example, by printing to the serial port
        Serial.print("Icon not found: ");
        Serial.println(icon_name);
    }
}

void Display::put_text(int x, int y, const char *content) const {
    // Create a mutable copy of the content
    String text(content);

    // Capitalize the first letter
    if (text.length() > 0) {
        text[0] = toupper(text[0]);
    }

    // Remove "_icon" from the string
    text.replace("_icon", "");

    // Replace remaining underscores with spaces
    text.replace("_", " ");

    // Replace hyphen with space
    text.replace("-", " ");

    tft.setTextSize(1); // Set text size if needed

    // Calculate the new width after modifications
    byte textWidth = get_string_width(text.c_str());

    // Set cursor to the new adjusted position to center the modified text
    tft.setCursor(x - (textWidth / 2), y);

    // Set text color
    tft.setTextColor(textColor);

    // Print the modified text
    tft.print(text);
}

void Display::draw_icon_with_label(int x, int y, byte _iconIndex, const char *iconNames[]) {
    put_icon(x, y, iconNames[_iconIndex]);
    put_text(x + iconWidth / 2, y + iconHeight + 20, iconNames[_iconIndex]);
}

// This method renders menu icons as grid or list
void Display::render_icons_grid(const byte *iconIndices, byte _numIcons, feature_render_type_t render_type) {
//    if (not isSmallFontUsed) {
//        tft.setFreeFont(&FreeSans9pt7b);
//    }
    switch (render_type) {
        case GRID: {
            tft.setFreeFont(&FreeSans9pt7b);
            numColumns = calculate_columns(_numIcons);
            numRows = calculate_rows(_numIcons, numColumns);
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));
//    vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + get_font_height())))
//                     / (numRows + 1));

            byte screen_item_index = 0;
            for (byte row = 0; row < numRows; ++row) {
                for (byte col = 0; col < numColumns; ++col) {
                    byte index = row * numColumns + col;
                    if (index < _numIcons) {
                        int x = hSpacing + col * (iconWidth + hSpacing);
                        //int y = vSpacing + NAV_BAR_HEIGHT + row * (iconHeight + get_font_height() + vSpacing);
                        int y = vSpacing + NAV_BAR_HEIGHT + row * (iconHeight + textHeight + vSpacing);
                        draw_icon_with_label(x, y, iconIndices[index], menu_icon_names);
                        // Update accordingly screen item
                        screen_item_position _item_position = {x, y, iconWidth, iconHeight};
                        update_screen_item(screen_item_index, _item_position);
                        ++screen_item_index;
                    }
                }
            }

            screen_item_count = screen_item_index;
            reset_display_setting();
            // Start to set screen selector to the first one item
            update_screen_selector(0);
            break;
        }
        case LIST: {
            //tft.drawRect(16, 52, 288, 304, TFT_WHITE);
            //tft.fillRect(16, 52, 288, 304, 0x528B);
            tft.setTextColor(TFT_WHITE);
            const int rectWidth = 288; // Width of the rectangle
            const int rectHeight = iconHeight + textHeight + 30; // Total height including padding
            const int rectX = (SCREEN_WIDTH - rectWidth) / 2; // Center the rectangle on the screen

            byte screen_item_index = 0;
            for (byte i = 0; i < _numIcons; ++i) {
                int rectY;
                // Adding gap = 5 between rects
                if (i == 0) rectY = 175 + i * rectHeight;
                else rectY = 175 + i * (rectHeight + 5);

                // Draw the rectangle for the icon-text pair
                tft.fillRect(rectX, rectY, rectWidth, rectHeight, 0x528B);

                // Calculate positions for the icon and text
                int iconX = rectX + 16; // 15 pixels padding from the left edge of the rectangle
                int iconY = rectY + 16; // 15 pixels padding from the top edge of the rectangle
                int textX = iconX + iconWidth + 15; // Text starts after the icon and padding
                int textY =
                        iconY + (iconHeight / 2) - (textHeight / 2); // Center text vertically with respect to the icon

                // Get the icon name using the index
                const char *icon_name = menu_icon_names[iconIndices[i]];
                String text(icon_name);

                // Remove "_icon" from the string
                text.replace("_icon", "");
                // Replace remaining underscores with spaces
                text.replace("_", " ");
                // Replace hyphen with space
                text.replace("-", " ");
                // Capitalize the first letter
                if (text.length() > 0) {
                    text[0] = toupper(text[0]);
                }

                // Draw the icon
                put_icon(iconX, iconY, icon_name);
                // Draw the text next to the icon
                tft.setFreeFont(&FreeSans12pt7b);
                tft.drawString(text, textX, textY);
                // Draw the rectangle for decoration to the right side
                tft.fillTriangle(270, rectY + (rectHeight / 2), 260, (rectY + (rectHeight / 2)) - 5, 260,
                                 (rectY + (rectHeight / 2)) + 5, TFT_WHITE);
                // Update accordingly screen item
                screen_item_position _item_position = {rectX, rectY, rectWidth, rectHeight};
                update_screen_item(screen_item_index, _item_position);
                ++screen_item_index;
            }

            screen_item_count = screen_item_index;
            reset_display_setting();
            // Start to set screen selector to the first one item
            update_screen_selector(0);
            break;
        }
    }
}

// This method renders list of items
void Display::render_item_list(bool is_new_list_set, bool navigation_direction, bool is_page_displayed, int item_count,
                               uint16_t item_background_color) {
    const int x_index = 22;
    const int x_item = 44;
    const int x_item_qty = 235;

    int y_all = 150;

    static byte current_item_index = 0;
    static byte current_page = 1;

    if (is_new_list_set) {
        current_item_index = 0;
        current_page = 1;

        Serial.println(F("Set item index in the list to 0. Page to 1"));
    } else {
        // Navigation direction: Determine which nav button is pressed and if current index is in the beginning/ending,
        // we know which part of list should be shown (previous/next items)
        // true (1) is down direction, false (0) is up direction
        if (navigation_direction) {
            if (current_item_index > 7) {
                current_item_index -= 8;
                --current_page;
            }
        } else {
            if (current_item_index <= 31) {
                current_item_index += 8;
                ++current_page;
            }
        }
        Serial.print(F("Current item index in the list: "));
        Serial.println(current_item_index);
    }

    // Clear current list items array
    for (byte i = 0; i < 8; ++i) {
        current_screen_list_items[i] = "";
    }

    // Just render 8 items per called time, as long as item content is not blank
    // Neednt update screen item position again, just re-render content
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextDatum(TL_DATUM);
    Serial.print(F("Items in this page are: "));
    // For updating screen selector and screen items
    byte screen_item_index = 0;
    screen_item_position _item_position;
    for (byte i = 0; i < 8; ++i) {
        byte item_index = current_item_index + i;
        if (item_index < 40) {
            if (screen_item_list_type_items[item_index] != "") {
                // Render items on the screen and quantities if this param is available
                //tft.fillRect(x_index, y_all - 10, 276, 36, 0x528B);
                tft.fillRect(x_index, y_all - 10, 276, 36, item_background_color);
                tft.setTextColor(0xFCC0);
                tft.drawString(String(item_index + 1), x_index, y_all);
                tft.setTextColor(TFT_WHITE);
                tft.drawString(screen_item_list_type_items[item_index], x_item, y_all);
                if (screen_item_list_type_quantities[item_index] != "") {
                    tft.drawString(screen_item_list_type_quantities[item_index], x_item_qty, y_all);
                }
                // Update accordingly current screen list items array
                current_screen_list_items[i] = screen_item_list_type_items[item_index];
                Serial.println(screen_item_list_type_items[item_index]);

                // Update accordingly screen item
                _item_position = {x_index, y_all - 10, 276, 36};
                update_screen_item(screen_item_index, _item_position);
                ++screen_item_index;
            }
            y_all += 38;
        }
    }

    screen_item_count = screen_item_index;
    screen_selector_border_color = 0x4208;
    // Start to set screen selector to the first one item
    update_screen_selector(0);
    current_feature_item_type = LIST_ITEM;

    // Page indicator on the bottom left
    if (is_page_displayed) {
        tft.setTextDatum(BL_DATUM);
        tft.fillRect(22, 443, 276, 27, 0x4208);
        tft.drawString("Page " + String(current_page) + "/5", 22, 468);
        // Item count on the bottom right
        tft.setTextDatum(BR_DATUM);
        tft.drawString("Item count: " + String(item_count), 300, 468);
    }

    // Reset display setting
    reset_display_setting();

    // Start to set screen selector to the first one item
    //update_screen_selector(0);
}

byte Display::calculate_columns(byte iconCount) {
    return (iconCount % 2 == 0) ? 2 : 1;
}

byte Display::calculate_rows(byte iconCount, byte _numColumns) {
    // If there is only one column, the number of rows is equal to the number of icons
    if (_numColumns == 1) {
        return iconCount;
    } else {
        // If there are two columns, the number of rows is half of the number of icons for even counts
        // or half + 1 for odd counts
        return (iconCount + 1) / _numColumns;
    }
}

void Display::render_feature(feature_t _feature, task_results &_taskResults) {
    static bool _is_viewport_cleared = true;
    // Clear the viewport
    //backgroundColor = 0x441C;
    if (_is_viewport_cleared) {
        Serial.println("Viewport is cleared now");
        tft.fillRect(0, NAV_BAR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - HEADER_HEIGHT, backgroundColor);

        // Clear screen items and reset screen selector
        clear_screen_selector();
        clear_screen_items();
    }

    switch (_feature) {
        case BOOT:
            // Code to handle BOOT feature
            break;
        case LOADING: {
            tft.setSwapBytes(false);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextColor(TFT_WHITE, 0x84B2);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("Loading...", SCREEN_WIDTH / 2, 270);
            for (int i = 0; i < 2; i++) {
                if (gif.open((uint8_t *) loading_animation, sizeof(loading_animation), GIFDraw)) {
                    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(),
                                  gif.getCanvasHeight());
                    tft.startWrite();  // The TFT chip select is locked low
                    while (gif.playFrame(true, NULL)) {
                        yield();
                    }
                    gif.close();
                    tft.endWrite();  // Release TFT chip select for other SPI devices
                }
            }
            tft.setSwapBytes(true);
            reset_display_setting();
            //draw_layout(feature_layout);
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            break;
        }
        case QR_CODE_SCANNING: {
            // Wait for mobile app to scan the QR code and select MES package, then back to home
//            if ((is_background_task_completed) and
//                ((_taskResults.selected_mes_package != "") or (_taskResults.selected_mes_package_group != ""))) {
//                // Reset current screen tasks
//                memset(current_screen_tasks, NO_TASK, 10);
//                current_screen_tasks[0] = NO_TASK;
//                // Reset current screen features
//                memset(current_screen_features, NO_FEATURE, 10);
//                // Reset display settings
//                reset_display_setting();
//                // Back to home
//                Serial.println(F("MES package has been selected. Back to home"));
//                is_back_to_home = true;
//            } else {
                Serial.println(F("This is the first time QR_CODE_SCANNING feature is rendered"));

                //is_background_task_required = true;
                //is_back_to_home = true;
                //is_loading_animation_displayed = false;
                // Reset current screen background tasks
                for (byte i = 0; i < 10; ++i) {
                    current_screen_background_tasks[i] = NO_TASK;
                }

                //current_screen_background_tasks[0] = HANDLE_MQTT_MESSAGE;
                //Put QR code placeholder-------------------------------
                iconWidth = 230;
                iconHeight = 268;
                put_icon(45, 76, menu_icon_names[37]);
                // Generate and display QR code on the screen, content is mac address
                // mes-package/mes-package-group/shipment-plan
                String qr_code_type = R"({"qrCodeType":")";
                String type = "";
                switch (_taskResults.currentScreenItemIndex) {
                    case 0:
                        type = "MES-PACKAGE";
                        break;
                    case 1:
                        type = "MES-PACKAGE-GROUP";
                        break;
                    case 2:
                        type = "SHIPMENT-PLAN";
                        break;
                }
                String mac_addr = R"(","macAddress":")";
                Serial.println(
                        "Generated QR code string: " + qr_code_type + type + mac_addr + _taskResults.mac_address +
                        "\"}");
                qrcode.create(qr_code_type + type + mac_addr + _taskResults.mac_address + "\"}");
//            }
            break;
        }
        case HOME_HANDHELD_1: {
            // Define which icons to display for the HOME HANDHELD 1 case
            const byte homeHandheld1IconIndices[] = {0, 1, 2, 3, 4, 5};
            // Call the new render_icons_grid function with the specific icons for HOME HANDHELD 1
            render_icons_grid(homeHandheld1IconIndices, 6, GRID);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = SETTING;
            current_screen_features[1] = RFID;
            current_screen_features[2] = PACKAGE;
            current_screen_features[3] = CO_WORKING;
            current_screen_features[4] = DATABASE;
            current_screen_features[5] = DATA_SYNC;
            break;
        }
        case HOME_HANDHELD_2: {
            // Define which icons to display for the HOME HANDHELD 1 case
            const byte homeHandheld1IconIndices[] = {28, 27, 9, 10, 0};

            // Put elements by hand-------------------------------------------------
            byte screen_item_index = 0;
            screen_item_position _item_position;

            // Put current counted packed boxes-------------------------------------
            iconWidth = 300;
            iconHeight = 131;
            put_icon(10, 46, menu_icon_names[32]);
            tft.setFreeFont(&FreeSansBold9pt7b);
//            tft.setTextColor(TFT_WHITE);
//            tft.drawString("Product Counting", 100, 51);
//            tft.setFreeFont(&FreeSans9pt7b);
//            tft.setTextColor(0x6B4D);
//            tft.drawString("(Factory line)", 100, 75);
//            tft.drawString("IoT mode: ", 100, 100);
            tft.setTextColor(TFT_WHITE);
            tft.setTextFont(2);
            tft.setTextSize(1);
            if (_taskResults.selected_mes_package != "") {
                tft.fillRect(58, 10, 150, 20, headerColor);
                tft.drawString("Server connected", 58, 10);
                tft.setTextColor(0x573F);
                tft.drawString("Connected", 185, 85);
                tft.drawString(String(_taskResults.mes_target), 185, 105);
                tft.drawString("Op Name: " + _taskResults.mes_operation_name, 20, 131);
                tft.drawString(_taskResults.selected_mes_package, 20, 153);
                tft.fillRect(10, 173, 300, 4, 0x2E3B);
                tft.pushImage(20, 56, 70, 70, _taskResults.mes_img_buffer);
            } else {
                tft.drawString("Not connected", 185, 85);
                tft.drawString("[ Target ]", 185, 105);
                tft.drawString("[ Op Name ]", 20, 131);
                tft.drawString("[ MES Package ]", 20, 153);
                tft.fillRect(10, 173, 300, 4, 0x95B7);
            }
            // Update accordingly screen item
            _item_position = {10, 46, iconWidth, iconHeight};
            update_screen_item(screen_item_index, _item_position);
            ++screen_item_index;

            iconWidth = 300;
            iconHeight = 102;
            // Put Incoming packed boxes banner--------------------------------------
            put_icon(10, 191, menu_icon_names[30]);
            tft.drawString("[ Package groups: ]", 20, 273);
            // Update accordingly screen item
            _item_position = {10, 191, iconWidth, iconHeight};
            update_screen_item(screen_item_index, _item_position);
            ++screen_item_index;

            // Put Outgoing packed boxes banner------------------------------------
            put_icon(10, 307, menu_icon_names[31]);
            tft.drawString("[ Shipment Plan: ]", 20, 390);
            // Update accordingly screen item
            _item_position = {10, 307, iconWidth, iconHeight};
            update_screen_item(screen_item_index, _item_position);
            ++screen_item_index;

            iconWidth = 144;
            iconHeight = 45;
            if ((_taskResults.selected_mes_package != "") or (_taskResults.selected_mes_package_group != "")) {
                Serial.println("MES Package or MES Package Group is selected");
                // Put Register button on the bottom left------------------------------------
                put_icon(10, 425, menu_icon_names[27]);
                // Update accordingly screen item
                _item_position = {10, 425, 144, 45};
                update_screen_item(screen_item_index, _item_position);
                ++screen_item_index;

                // Put Scan button------------------------------------W
                put_icon(166, 425, menu_icon_names[29]);
                // Update accordingly screen item
                _item_position = {166, 425, 144, 45};
                update_screen_item(screen_item_index, _item_position);
                ++screen_item_index;
            } else {
                Serial.println("MES Package or MES Package Group is not selected");
                // Put Register button on the bottom left------------------------------------
                put_icon(10, 425, menu_icon_names[38]);
                // Put Scan button------------------------------------
                put_icon(166, 425, menu_icon_names[39]);
            }

            // Put Setting button on the bottom right------------------------------------
//            iconWidth = 290;
//            iconHeight = 40;
//            put_icon(15, 425, menu_icon_names[28]);
//            // Update accordingly screen item
//            _item_position = {15, 425, 290, 40};
//            update_screen_item(screen_item_index, _item_position);
//            ++screen_item_index;

            screen_selector_border_color = backgroundColor;
            screen_item_count = screen_item_index;
            reset_display_setting();
            // Start to set screen selector to the first one item
            update_screen_selector(0);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = QR_CODE_SCANNING;
            current_screen_features[1] = QR_CODE_SCANNING;
            current_screen_features[2] = QR_CODE_SCANNING;
            if ((_taskResults.selected_mes_package != "") or (_taskResults.selected_mes_package_group != "")) {
                current_screen_features[3] = RFID_REGISTER_TAG;
                current_screen_features[4] = RFID_SCAN_DETAILS_REVIEW;
            }
            break;
        }
        case HOME_TERMINAL:
            // Code to handle HOME_TERMINAL feature
            break;
        case SETTING: {
            tft.setFreeFont(&FreeSansBold12pt7b);
            tft.setTextColor(TFT_WHITE);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("SETTING", SCREEN_WIDTH / 2, 60);
            tft.setTextDatum(TL_DATUM);

            // User info pane
            tft.fillRect(16, 81, 288, 79, TFT_WHITE);
            // Draw the user info avatar
            tft.fillRect(32, 96, 45, 45, TFT_BLUE);
            // Draw package infomartion
            tft.setFreeFont(&FreeSansBold9pt7b);
            tft.setTextColor(TFT_BLACK);
            tft.drawString("Nguyen Van A", 86, 96);
            tft.setTextFont(2);
            tft.drawString("User ID: 12345678", 86, 126);

            iconWidth = 36;
            iconHeight = 36;
            // Define which icons to display for the Setting case
            const byte settingIconIndices[] = {12};
            // Call the new render_icons_grid function with the specific icons for Setting
            render_icons_grid(settingIconIndices, 1, LIST);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = SETTING_WIFI;
            //current_screen_features[1] = RFID_SCAN_HISTORY;
            // Reset display settings
            reset_display_setting();
            break;
        }
        case SETTING_WIFI: {
            // Check if background task is completed, if yes, start rendering, else, set background tasks and return
            if (is_background_task_completed) {
                Serial.println(F("Start listing all available Wi-Fi networks on the screen"));
                byte wifi_network_index = 0;
                byte wifi_network_count = 0;
                // Print all available wifi networks
                tft.setTextColor(TFT_WHITE);
                tft.setTextDatum(MC_DATUM);
                tft.setFreeFont(&FreeSansBold12pt7b);
                tft.drawString("WIFI", SCREEN_WIDTH / 2, 60);
                tft.setTextDatum(TL_DATUM);

                tft.fillRect(10, 81, 300, 389, 0x4208);
                tft.fillRect(22, 95, 276, 40, 0x5333);
                tft.setTextColor(TFT_WHITE);
                tft.setFreeFont(&FreeSans9pt7b);
                tft.setTextDatum(TL_DATUM);
                tft.drawString("#", 22, 107);
                tft.drawString("SSID", 44, 107);
                tft.drawString("RSSI", 235, 107);

                while (wifi_network_index < _taskResults.wifi_networks_count) {
                    const char *ssid = _taskResults.wifi_networks[wifi_network_index].ssid;
                    int rssi = _taskResults.wifi_networks[wifi_network_index].rssi;

                    // Draw only if SSID is not empty
                    if (strlen(ssid) > 0) {
                        screen_item_list_type_items[wifi_network_index] = String(ssid);
                        screen_item_list_type_quantities[wifi_network_index] = String(rssi);
                        ++wifi_network_count;
                    }
                    wifi_network_index++;
                }

                // Render list
                render_item_list(true, false, false, 8, 0x528B);

                // Reset current screen tasks
                memset(current_screen_tasks, NO_TASK, 10);
                current_screen_tasks[0] = NO_TASK;
                // Reset current screen features
                memset(current_screen_features, NO_FEATURE, 10);
                current_screen_features[0] = HOME_HANDHELD_2;
                // Reset display settings
                reset_display_setting();
            } else {
//                tft.setFreeFont(&FreeSans9pt7b);
//                tft.setTextDatum(MC_DATUM);
                //tft.drawString("Please wait, scanning networks", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                is_background_task_required = true;
                is_loading_animation_displayed = true;
                // Reset current screen background tasks
                for (byte i = 0; i < 10; ++i) {
                    current_screen_background_tasks[i] = NO_TASK;
                }
                current_screen_background_tasks[0] = SCAN_WIFI_NETWORKS;
                //current_screen_background_tasks[1] = READ_RFID_TAG;
            }
            break;
        }
        case SETTING_USER_INFO: {
            // Define which icons to display for the SETTING_USER_INFO case
            const byte settingUserInfoIconIndices[] = {14, 15};
            // Call the new render_icons_grid function with the specific icons for SETTING_USER_INFO
            render_icons_grid(settingUserInfoIconIndices, 2, GRID);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = SETTING_USER_INFO_LOGIN;
            current_screen_features[1] = SETTING_USER_INFO_LOGOUT;
            break;
        }
        case SETTING_USER_INFO_LOGIN:
            // Code to handle SETUP_USER_INFO_LOGIN feature
            break;
        case SETTING_USER_INFO_LOGOUT:
            // Code to handle SETUP_USER_INFO_LOGOUT feature
            break;
        case RFID: {
            iconWidth = 36;
            iconHeight = 36;
            // Define which icons to display for the RFID case
            const byte rfidIconIndices[] = {9, 10, 11, 22};
            // Call the new render_icons_grid function with the specific icons for RFID
            render_icons_grid(rfidIconIndices, 4, LIST);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            //current_screen_features[0] = RFID_SCAN;
            current_screen_features[0] = RFID_MES_PACKAGES_LIST;
            current_screen_features[1] = RFID_SCAN_HISTORY;
            //current_screen_features[2] = RFID_MODIFY_TAG_DATA;
            current_screen_features[2] = RFID_SCAN_RESULT;
            //current_screen_features[3] = RFID_REGISTER_TAG;
            current_screen_features[3] = RFID_SCAN_DETAILS_REVIEW;
            iconWidth = 64;
            iconHeight = 64;
            break;
        }
        case RFID_SCAN: {
            byte button_radius = 60;
            byte text_padding = 20;
            // Draw a blue circle button
            tft.fillCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, button_radius, TFT_BLUE);
            // Draw the text below the button
            tft.setFreeFont(&FreeSans12pt7b);
            tft.setTextColor(TFT_WHITE);
            // Assuming your library provides a way to set text alignment
            tft.setTextDatum(MC_DATUM); // MC_DATUM typically means Middle-Center datum point
            // Draw the button text centered below the circle button
            tft.drawString("Press select to start scanning", SCREEN_WIDTH / 2,
                           SCREEN_HEIGHT / 2 + button_radius + text_padding);

            // When the user presses the select button, change the circle to green
            // and update the text (this part should be uncommented when implementing the interaction)
            //
            // tft.fillCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BUTTON_RADIUS, TFT_GREEN);
            // tft.setCursor(SCREEN_WIDTH / 2 - TEXT_OFFSET, SCREEN_HEIGHT / 2 + BUTTON_RADIUS + TEXT_PADDING);
            // tft.print("Press cancel to stop scanning");
            reset_display_setting();
            current_feature_item_type = LIST_ITEM;
            // Reset current screen tasks
            memset(current_screen_tasks, NO_TASK, 10);
            current_screen_tasks[0] = READ_RFID_TAG;
            break;
        }
        case RFID_SCAN_HISTORY: {
//            // Draw the grid and column titles
//            draw_grid();
//            draw_titles();
//            // Draw each item in the history
//            for (byte i = 0; i < 14; i++) {
//                draw_history_item(i, history[i]);
//            }
//            //current_feature_item_type = LIST_ITEM;
            break;
        }
        case RFID_SCAN_DETAILS_REVIEW: {
            tft.setFreeFont(&FreeSansBold9pt7b);
            tft.setTextColor(0x12AC);
            tft.setTextDatum(TC_DATUM);
            tft.drawString("REVIEW SCANNING DETAILS", SCREEN_WIDTH / 2, 50);
            tft.setTextDatum(TL_DATUM);
            tft.fillRect(10, 81, 300, 389, 0x1B2E);
            //tft.fillRect(22, 93, 275, 32, 0x8430);
            tft.fillRect(22, 93, 275, 54, 0x7556);
            //tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextFont(2);
            tft.setTextSize(1);
            tft.setTextColor(0x09E8);
            //tft.drawString("Package groups: " + _taskResults.selected_list_items[2], 32, 101);
            //tft.drawString("MES Packages:", 32, 101);
            tft.drawString(_taskResults.selected_mes_package, 25, 111);
            // Draw the product image
            tft.pushImage(22, 156, 70, 70, _taskResults.mes_img_buffer);
            // Draw package information
            tft.setTextFont(2);
            tft.setTextColor(TFT_WHITE);
            tft.drawString("AO No: ", 102, 156);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.ao_no, 170, 156); // Assuming that ao_no is a String
            tft.setTextColor(TFT_WHITE);
            tft.setTextFont(2);
            tft.drawString("ADQty: ", 102, 178);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.target_qty, 170, 178); // Assuming that target_qty is a String
            tft.setTextColor(TFT_WHITE);
            tft.setTextFont(2);
            tft.drawString("Delivery date: ", 102, 200);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.delivery_date, 190, 200); // Assuming that delivery_data is a String
            tft.setTextColor(TFT_WHITE);
            tft.setTextFont(2);
            tft.drawString("Destination: ", 102, 222);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.destination, 190, 222); // Assuming that destination is a String
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans9pt7b);
            // More package information
            tft.setTextFont(2);
            tft.setTextSize(1);
            tft.drawString("StyleName: ", 22, 247);
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.style_text, 130, 247); // Assuming that style_text is a String
            tft.setTextColor(TFT_WHITE);
            tft.drawString("BuyerStyleName: ", 22, 272);
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.buyer_style_text, 130, 272); // Assuming that buyer_style_text is a String
            tft.setTextColor(TFT_WHITE);
            tft.drawString("StyleColor: ", 22, 297); // Adjusted y position after removal of StyleSize
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.style_color, 130, 297); // Assuming that style_color is a String
            tft.setTextColor(TFT_WHITE);
            tft.drawString("LineNo: ", 22, 322); // Adjusted y position after removal of StyleSize
            tft.setTextColor(0x573F);
            tft.drawString(_taskResults.line_name, 130, 322); // Assuming that line_name is a String
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans9pt7b);
            // Draw the start scanning button
            tft.fillRect(22, 418, 275, 40, 0x0A6A);
            tft.setTextDatum(MC_DATUM);
            tft.setTextColor(TFT_WHITE);
            tft.drawString("START SCANNING", SCREEN_WIDTH / 2, 437);

            // Update accordingly screen item
            screen_item_position _item_position = {22, 418, 275, 40};
            update_screen_item(0, _item_position);
            screen_selector_border_color = backgroundColor;
            screen_item_count = 1;
            // Start to set screen selector to the first one item
            update_screen_selector(0);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            //current_screen_features[0] = RFID_SCAN_RESULT;
            current_screen_features[0] = RFID_SCAN_RESULT;
            // Reset display settings
            reset_display_setting();
            break;
        }
        case RFID_SCAN_RESULT: {
            // Check if background task is completed, if yes, start rendering, else, set background tasks and return
            if (is_background_task_completed) {
                // Update the "Submitted/target" field with the latest count
                String submittedTargetStr =
                        String(_taskResults.current_scanned_rfid_tag_count) + "/" + String(_taskResults.mes_target);
                tft.setFreeFont(&FreeSans9pt7b);
                tft.setTextColor(0x1ACC);
                tft.fillRect(22, 214, 275, 37, 0x7556); // Clear the previous area
                tft.drawString("Submitted/target", 37, 225);
                tft.setTextColor(0x09E8); // Color for the count
                tft.setFreeFont(&FreeSansBold9pt7b);
                tft.drawString(submittedTargetStr, 198, 225);

                // Update the "Quantity" field with the latest count
                String quantityStr = String(_taskResults.current_scanned_rfid_tag_count);
                tft.fillRect(22, 302, 276, 50, TFT_WHITE); // Clear the previous area
                tft.setFreeFont(&FreeSans9pt7b);
                tft.setTextColor(0x5ACB);
                tft.drawString("Quantity", 37, 319);
                tft.setFreeFont(&FreeSansBold12pt7b);
                tft.setTextColor(0x350F);
                tft.drawString(quantityStr, 248, 319);
                // Do nothing, lets user choose to Submit scan results to server or Clear and scan again
            } else {
                // Check if this is the first time this feature is rendered
                if (_taskResults.currentFeature == RFID_SCAN_DETAILS_REVIEW) {
                    Serial.println(F("This is the first time RFID_SCAN_RESULT feature is rendered"));

                    // Reset RFID scan results
                    _taskResults.current_scanned_rfid_tag_count = 0;

                    // Check provided arguments which are selected items from previous lists
                    Serial.println(F("Selected items from previous lists: "));
                    for (byte i = 0; i < 10; ++i) {
                        if (_taskResults.selected_list_items[i] != "")
                            Serial.println(_taskResults.selected_list_items[i]);
                    }

                    tft.setFreeFont(&FreeSansBold12pt7b);
                    tft.setTextColor(0x12AC);
                    tft.drawString("SCAN RESULT", 75, 50);
                    tft.fillRect(10, 81, 300, 389, 0x1B2E);
                    // Draw the product image
                    tft.pushImage(22, 93, 70, 70, _taskResults.mes_img_buffer);
                    // Draw package information
                    tft.setTextFont(2);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawString("BuyerPO:", 102, 93);
                    tft.setFreeFont(&FreeSans9pt7b);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawString(_taskResults.buyer_po, 170, 93); // Assuming that buyer_po is a String
                    tft.setTextFont(2);
                    tft.drawString("ADQty: ", 102, 115);
                    tft.setFreeFont(&FreeSans9pt7b);
                    tft.drawString(_taskResults.target_qty, 170, 115); // Assuming that target_qty is a String
                    tft.setTextFont(2);
                    tft.drawString("Delivery date: ", 102, 137);
                    tft.setFreeFont(&FreeSans9pt7b);
                    tft.drawString(_taskResults.delivery_date, 190, 137); // Assuming that delivery_data is a String
                    tft.setTextFont(2);
                    tft.drawString("Destination: ", 102, 159);
                    tft.setFreeFont(&FreeSans9pt7b);
                    tft.drawString(_taskResults.destination, 190, 159); // Assuming that destination is a String
                    tft.setFreeFont(&FreeSans9pt7b);
                    // Submitted status
                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.setTextColor(0x5D56);
                    tft.drawString("Submitted status", 22, 187);
                    tft.setFreeFont(&FreeSans9pt7b);
                    tft.setTextColor(0x1ACC);
                    tft.fillRect(22, 214, 275, 37, 0x7556);
                    tft.drawString("Submitted/target", 37, 225);
                    tft.setTextColor(0x09E8);
                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.drawString(
                            String(_taskResults.current_scanned_rfid_tag_count) + "/" + String(_taskResults.mes_target),
                            198, 225);
                    // Current scan status
                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.setTextColor(0x5D56);
                    tft.drawString("Current scan status", 22, 275);
                    tft.setFreeFont(&FreeSans9pt7b);
                    //tft.drawRect(22, 335, 275, 50, 0x8430);
                    tft.fillRect(22, 302, 276, 50, TFT_WHITE);
                    tft.setTextColor(0x5ACB);
                    tft.drawString("Quantity", 37, 319);
                    tft.setFreeFont(&FreeSansBold12pt7b);
                    tft.setTextColor(0x350F);
                    tft.drawString(String(_taskResults.current_scanned_rfid_tag_count), 248, 319);
                    tft.setFreeFont(&FreeSans9pt7b);

                    byte screen_item_index = 0;
                    screen_item_position _item_position;
                    // Draw the clear button
                    tft.fillRect(22, 420, 130, 40, 0x0A6A);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawString("SCAN", 65, 433);
                    // Update accordingly screen item
                    _item_position = {22, 420, 130, 40};
                    update_screen_item(screen_item_index, _item_position);
                    ++screen_item_index;
                    // Draw the submit button
                    tft.fillRect(168, 420, 130, 40, 0x0A6A);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawString("SUBMIT", 200, 433);
                    // Update accordingly screen item
                    _item_position = {168, 420, 130, 40};
                    update_screen_item(screen_item_index, _item_position);
                    ++screen_item_index;
                    screen_item_count = screen_item_index;

                    // Start to set screen selector to the first one item
                    update_screen_selector(0);
                    // Reset current screen tasks
                    memset(current_screen_tasks, NO_TASK, 10);
                    current_screen_tasks[0] = READ_RFID_TAG;
                    current_feature_item_type = TASK_ITEM;
                    // Reset current screen features
                    memset(current_screen_features, NO_FEATURE, 10);
                    // Reset display settings
                    reset_display_setting();

                    _is_viewport_cleared = false;
                } else {
                    is_background_task_required = true;
                    // Reset current screen background tasks
                    for (byte i = 0; i < 10; ++i) {
                        current_screen_background_tasks[i] = NO_TASK;
                    }
                    switch (_taskResults.currentScreenItemIndex) {
                        case 0:
                            // Start scanning task
                            Serial.println(F("Start background scanning task"));
                            current_screen_background_tasks[0] = READ_RFID_TAG;
                            break;
                        case 1:
                            // Start submitting task
                            Serial.println(F("Start background submitting task"));
                            current_screen_background_tasks[0] = NO_TASK;
                            _is_viewport_cleared = true;
                            is_back_to_home = true;
                            break;
                    }
                }
            }
            break;
        }
        case RFID_MODIFY_TAG_DATA:
            current_feature_item_type = LIST_ITEM;
            break;
        case RFID_REGISTER_TAG: {
            // Check if background task is completed, if yes, start rendering, else, set background tasks and return
            if (is_background_task_completed) {
                // Just display the recently scanned result and increase total scans
                // Assuming numScanned is an integer variable holding the number of scanned RFID tags
                String displayStr = String(_taskResults.current_scanned_rfid_tag_count) + "/200";
                // Set the font for the number display
                tft.setFreeFont(&FreeSansBold12pt7b);
                tft.setTextColor(0x350F);
                // Calculate the width of the text to be removed
                int width = tft.textWidth(displayStr);
                // Clear the area where the "0/200" is displayed
                tft.fillRect(218, 307, width, 50, TFT_WHITE);
                // Draw the new string with the updated count
                tft.drawString(displayStr, 218, 321);

                //_is_viewport_cleared = true;
                // Do nothing, lets user choose to Submit scan results to server or Clear and scan again
            } else {
                // Check if this is the first time this feature is rendered
                if (_taskResults.currentScreenItemIndex == 3) {
                    Serial.println(F("This is the first time RFID_REGISTER_TAG feature is rendered"));

                    // Reset RFID scan results
                    _taskResults.current_scanned_rfid_tag_count = 0;

                    tft.setFreeFont(&FreeSansBold12pt7b);
                    tft.setTextColor(0x12AC);
                    tft.drawString("RFID REGISTRATION", 38, 50);
                    tft.fillRect(10, 81, 300, 184, 0x1B2E);
                    tft.setFreeFont(&FreeSansBold12pt7b);
                    iconWidth = 280;
                    iconHeight = 41;
                    put_icon(20, 101, menu_icon_names[40]);
                    //tft.drawString("Package groups: " + _taskResults.selected_list_items[2], 32, 101);
                    tft.setTextFont(2);
                    tft.setTextSize(1);
                    tft.setTextColor(TFT_WHITE);
                    //tft.drawString(_taskResults.selected_list_items[3], 32, 138);
                    tft.drawString(_taskResults.selected_mes_package, 20, 141);
                    // Draw the product image
                    tft.pushImage(20, 175, 70, 70, _taskResults.mes_img_buffer);
                    // Draw package infomartion
                    tft.setTextFont(2);
                    tft.drawString("AO No: ", 100, 175);
                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.drawString(_taskResults.ao_no, 180, 175);
                    tft.setTextFont(2);
                    tft.drawString("ADQty: ", 100, 200);
                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.drawString(_taskResults.target_qty, 180, 200);
                    tft.setTextFont(2);
                    tft.drawString("Delivery date: ", 100, 225);
                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.drawString(_taskResults.delivery_date, 200, 225);
                    tft.setTextFont(2);
                    tft.drawString("Destination: ", 100, 250);
                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.drawString(_taskResults.destination, 200, 250);

                    tft.setFreeFont(&FreeSansBold9pt7b);
                    tft.setTextColor(0x4228);
                    tft.drawString("Total registered RFID tags:", 10, 280);
                    tft.fillRect(10, 307, 300, 50, TFT_WHITE);
                    tft.setTextColor(0x5ACB);
                    tft.drawString("Pass", 30, 325);
                    tft.setFreeFont(&FreeSansBold12pt7b);
                    tft.setTextColor(0x350F);
                    tft.drawString("0/200", 218, 321);

                    iconWidth = 140;
                    iconHeight = 45;
                    put_icon(10, 425, menu_icon_names[41]);
                    put_icon(170, 425, menu_icon_names[42]);
                    // Update accordingly screen item
                    screen_item_position _item_position = {10, 425, iconWidth, iconHeight};
                    update_screen_item(0, _item_position);
                    _item_position = {170, 425, iconWidth, iconHeight};
                    update_screen_item(1, _item_position);
                    screen_selector_border_color = backgroundColor;
                    screen_item_count = 2;
                    // Start to set screen selector to the first one item
                    update_screen_selector(0);

                    //current_feature_item_type = MENU_ICON;
                    // Reset current screen tasks
                    memset(current_screen_tasks, NO_TASK, 10);
                    current_screen_tasks[0] = READ_RFID_TAG;
                    current_screen_tasks[1] = REGISTER_RFID_TAG;
                    current_feature_item_type = TASK_ITEM;
                    // Reset current screen features
                    memset(current_screen_features, NO_FEATURE, 10);
                    // Reset display settings
                    reset_display_setting();

                    _is_viewport_cleared = false;
                } else {
                    is_background_task_required = true;
                    // Reset current screen background tasks
                    for (byte i = 0; i < 10; ++i) {
                        current_screen_background_tasks[i] = NO_TASK;
                    }
                    switch (_taskResults.currentScreenItemIndex) {
                        case 0:
                            // Start scanning task
                            Serial.println(F("Start background scanning task"));
                            current_screen_background_tasks[0] = READ_RFID_TAG;
                            break;
                        case 1:
                            // Start submitting task
                            Serial.println(F("Start background submitting task"));
                            current_screen_background_tasks[0] = REGISTER_RFID_TAG;
                            _is_viewport_cleared = true;
                            is_back_to_home = true;
                            break;
                    }
                    //is_loading_animation_displayed = false;
                }
            }
            break;
        }
        case RFID_FACTORY_SELECT: {
            tft.setFreeFont(&FreeSansBold12pt7b);
            tft.setTextColor(TFT_WHITE);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("PK2 FACTORY", SCREEN_WIDTH / 2, 60);
            tft.fillRect(10, 81, 300, 389, 0x4208);
            tft.fillRect(22, 95, 276, 40, 0x5333);
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextDatum(TL_DATUM);
            tft.drawString("#", 22, 107);
            tft.drawString("Factory", 44, 107);

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_items[i] = "";
            }

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_quantities[i] = "";
            }

            // Clear selected items of screens which have list
            for (byte i = 0; i < 10; ++i) {
                _taskResults.selected_list_items[i] = "";
            }

            screen_item_list_type_items[0] = "[P2A1]-V-2A",
                    screen_item_list_type_items[1] = "[P2B1]-V-2B";
            screen_item_list_type_items[2] = "[P2C1]-V-2C";
            screen_item_list_type_items[3] = "[P2D1]-V-2D";
            screen_item_list_type_items[4] = "[P2E1]-V-2E";
            screen_item_list_type_items[5] = "[PJA1]-V-J1";

            // Render list
            render_item_list(true, false, true, 40, 0x528B);

            // Reset current screen tasks
            memset(current_screen_tasks, NO_TASK, 10);
            current_screen_tasks[0] = NO_TASK;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = RFID_WEEK_SELECT;
            // Reset display settings
            reset_display_setting();
            break;
        }
        case RFID_WEEK_SELECT: {
            tft.setFreeFont(&FreeSansBold12pt7b);
            tft.setTextColor(TFT_WHITE);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("WEEK SELECTING", SCREEN_WIDTH / 2, 60);
            tft.fillRect(10, 81, 300, 389, 0x4208);
            tft.fillRect(22, 95, 276, 40, 0x5333);
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextDatum(TL_DATUM);
            tft.drawString("#", 22, 107);
            tft.drawString("Week", 44, 107);

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_items[i] = "Week - " + String(i + 1);
            }

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_quantities[i] = "";
            }

            // Render list
            render_item_list(true, false, true, 40, 0x528B);

            // Reset current screen tasks
            memset(current_screen_tasks, NO_TASK, 10);
            current_screen_tasks[0] = NO_TASK;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = RFID_PACKAGE_GROUPS_LIST;
            // Reset display settings
            reset_display_setting();
            break;
        }
        case RFID_MES_PACKAGES_LIST: {
            tft.setFreeFont(&FreeSansBold12pt7b);
            tft.setTextColor(TFT_WHITE);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("MES PACKAGES LIST", SCREEN_WIDTH / 2, 60);
            tft.fillRect(10, 81, 300, 389, 0x4208);
            tft.fillRect(22, 95, 276, 40, 0x5333);
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextDatum(TL_DATUM);
            tft.drawString("#", 22, 107);
            tft.drawString("MES Packages", 44, 107);

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_items[i] = "0770_5_0927MDI003001_01_01";
            }

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_quantities[i] = "";
            }

            // Render list
            render_item_list(true, false, true, 40, 0x528B);

            // Reset current screen tasks
            memset(current_screen_tasks, NO_TASK, 10);
            current_screen_tasks[0] = NO_TASK;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = RFID_SCAN_DETAILS_REVIEW;
            // Reset display settings
            reset_display_setting();
            break;
        }
        case RFID_PACKAGE_GROUPS_LIST: {
            tft.setFreeFont(&FreeSansBold12pt7b);
            tft.setTextColor(TFT_WHITE);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("PACKAGE GROUPS", SCREEN_WIDTH / 2, 60);
            tft.fillRect(10, 81, 300, 389, 0x4208);
            tft.fillRect(22, 95, 276, 40, 0x5333);
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.setTextDatum(TL_DATUM);
            tft.drawString("#", 22, 107);
            tft.drawString("Package group", 44, 107);
            tft.drawString("Lot Qty", 235, 107);

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_items[i] = "P2C1-2312-0000074" + String(i);
            }

            for (byte i = 0; i < 40; ++i) {
                screen_item_list_type_quantities[i] = "0/" + String(i);
            }

            // Render list
            render_item_list(true, false, true, 40, 0x528B);

            // Reset current screen tasks
            memset(current_screen_tasks, NO_TASK, 10);
            current_screen_tasks[0] = NO_TASK;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = RFID_MES_PACKAGES_LIST;
            // Reset display settings
            reset_display_setting();
            break;
        }
        case RFID_BUYER_PO_LIST: {
            tft.setFreeFont(&FreeSansBold12pt7b);
            tft.setTextColor(TFT_WHITE);
            tft.drawString("Buyer PO list", 95, 50);
            tft.fillRect(10, 81, 300, 389, TFT_WHITE);
            tft.fillRect(22, 93, 276, 40, headerColor);
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans9pt7b);
            tft.drawString("#", 32, 103);
            tft.drawString("Buyer PO", 50, 103);
            tft.drawString("Lot Quantity", 190, 103);
            // Draw list of items to be displayed
            int x_index = 32;
            int x_mes_package_group = 50;
            int x_qty = 190;
            int y_index = 143;
            int y_mes_package_group = 143;
            int y_qty = 143;
            tft.setTextColor(TFT_BLACK);
            for (byte i = 0; i < 8; ++i) {
                tft.setTextColor(TFT_RED);
                tft.drawString(String(i), x_index, y_index);
                tft.setTextColor(TFT_BLACK);
                tft.drawString(String(random(12345456)) + "AD", x_mes_package_group, y_mes_package_group);
                tft.setTextColor(TFT_RED);
                tft.drawString(String(i + random(12345)) + "/" + String(random(12343)), x_qty, y_qty);
                tft.setTextColor(TFT_BLACK);
//                tft.drawLine(x_mes_package_group, y_mes_package_group, x_mes_package_group, y_mes_package_group,
//                             TFT_BLACK);
                y_index += 40;
                y_mes_package_group += 40;
                y_qty += 40;
            }
            tft.setTextColor(TFT_BLUE);
            // Page indicator on the bottom left
            tft.setTextDatum(BL_DATUM);
            tft.drawString("Page 1/15", 10, 470);
            // Item count on the bottom right
            tft.setTextDatum(BR_DATUM);
            tft.drawString("Item count: 12345", 308, 470);
            // Reset display settings
            reset_display_setting();
            break;
        }
        case PACKAGE: {
            // Define which icons to display for the PACKAGE case
            const byte packageIconIndices[] = {21};
            // Call the new render_icons_grid function with the specific icons for PACKAGE
            render_icons_grid(packageIconIndices, 1, GRID);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = PACKAGE_DETAILS;
            break;
        }
        case PACKAGE_DETAILS:
            // Code to handle PACKAGE_DETAILS feature
            break;
        case CO_WORKING: {
            // Define which icons to display for the CO WORKING case
            const byte coworkingIconIndices[] = {6, 7, 8};
            // Call the new render_icons_grid function with the specific icons for CO WORKING
            render_icons_grid(coworkingIconIndices, 3, GRID);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = CO_WORKING_SCAN_NEARBY_DEVICE;
            current_screen_features[1] = CO_WORKING_CONNECT_TO_DEVICE;
            current_screen_features[2] = CO_WORKING_CONNECT_TO_SERVER;
            break;
        }
        case CO_WORKING_SCAN_NEARBY_DEVICE:
            // Code to handle CO_WORKING_SCAN_NEARBY_DEVICE feature
            break;
        case CO_WORKING_CONNECT_TO_DEVICE:
            // Code to handle CO_WORKING_CONNECT_TO_DEVICE feature
            break;
        case CO_WORKING_CONNECT_TO_SERVER:
            // Code to handle CO_WORKING_CONNECT_TO_SERVER feature
            break;
        case CO_WORKING_RUNNING:
            // Code to handle CO_WORKING_RUNNING feature
            break;
        case DATABASE: {
            // Define which icons to display for the DATABASE case
            const byte databaseIconIndices[] = {16, 17,};
            // Call the new render_icons_grid function with the specific icons for DATABASE
            render_icons_grid(databaseIconIndices, 2, GRID);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = DATA_IMPORT;
            current_screen_features[1] = DATA_EXPORT;
            break;
        }
        case DATA_IMPORT: {
            // Define which icons to display for the DATA IMPORT case
            const byte dataImportIconIndices[] = {18, 19, 20};
            // Call the new render_icons_grid function with the specific icons for DATA IMPORT
            render_icons_grid(dataImportIconIndices, 3, GRID);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = DATA_IMPORT_FROM_SD_CARD;
            current_screen_features[1] = DATA_IMPORT_FROM_SERVER;
            current_screen_features[2] = DATA_IMPORT_FROM_COMPUTER;
            break;
        }
        case DATA_IMPORT_FROM_SD_CARD:
            // Code to handle DATA_IMPORT_FROM_SD_CARD feature
            break;
        case DATA_IMPORT_FROM_SERVER:
            // Code to handle DATA_IMPORT_FROM_SERVER feature
            break;
        case DATA_IMPORT_FROM_COMPUTER:
            // Code to handle DATA_IMPORT_FROM_COMPUTER feature
            break;
        case DATA_EXPORT:
            // Code to handle DATA_EXPORT feature
            break;
        case DATA_EXPORT_TO_SERVER:
            // Code to handle DATA_EXPORT_TO_SERVER feature
            break;
        case DATA_EXPORT_TO_SD_CARD:
            // Code to handle DATA_EXPORT_TO_SD_CARD feature
            break;
        case DATA_SYNC:
            // Code to handle DATA_SYNC feature
            break;
        case DATA_SYNC_TO_SERVER:
            // Code to handle DATA_SYNC_TO_SERVER feature
            break;
        case DATA_SYNC_TO_DEVICE:
            // Code to handle DATA_SYNC_TO_DEVICE feature
            break;
        default:
            // Code to handle unknown feature
            break;
    }
}

void Display::draw_history_item(byte index, const rfid_scan_result &item) const {
    // Calculate the y position for the current row, taking into account the header bar
    int y_position = HEADER_HEIGHT + ROW_HEIGHT + (index * ROW_HEIGHT);

    tft.setTextColor(TFT_WHITE);
    // Align text to the left
    tft.setTextDatum(TL_DATUM);

    // Draw the index (No.)
    tft.drawString(String(index + 1), TEXT_PADDING, y_position);

    // Draw the EPC
    tft.drawString(item.scan_data.epc, COL_WIDTH_NO + TEXT_PADDING, y_position);

    // Draw the Timestamp/ Last scan
    tft.drawString(item.timestamp, COL_WIDTH_NO + COL_WIDTH_EPC + TEXT_PADDING, y_position);

    // Draw the Count
    tft.drawString(String(item.scan_count), COL_WIDTH_NO + COL_WIDTH_EPC + COL_WIDTH_TIMESTAMP + TEXT_PADDING,
                   y_position);
}

void Display::draw_vertical_line() const {
    // Calculate the height at which the vertical lines should stop
    int vertical_line_stop = SCREEN_HEIGHT - NAV_BAR_HEIGHT - PAGE_INDICATOR_HEIGHT;

    // Draw vertical lines between columns
    tft.drawLine(COL_WIDTH_NO, HEADER_HEIGHT, COL_WIDTH_NO, vertical_line_stop, TFT_WHITE);
    tft.drawLine(COL_WIDTH_NO + COL_WIDTH_EPC, HEADER_HEIGHT, COL_WIDTH_NO + COL_WIDTH_EPC, vertical_line_stop,
                 TFT_WHITE);
    tft.drawLine(COL_WIDTH_NO + COL_WIDTH_EPC + COL_WIDTH_TIMESTAMP, HEADER_HEIGHT,
                 COL_WIDTH_NO + COL_WIDTH_EPC + COL_WIDTH_TIMESTAMP, vertical_line_stop, TFT_WHITE);
    // No need to draw the last line as it would be the edge of the screen
}

void Display::blink_screen(bool &isTaskCompleted) {
    static int blink_count = 0;
    if (blink_count <= 10) {
        Serial.print(F("Execute time: "));
        Serial.println(blink_count);
        tft.fillScreen(random(TFT_WHITE));
        delay(500);
        tft.fillScreen(TFT_BLACK);
        delay(500);
        blink_count++;
    } else {
        isTaskCompleted = true;
    }
}

void Display::reset_display_setting() {
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TL_DATUM);
    iconWidth = 64;
    iconHeight = 64;
}

void Display::update_screen_item(byte _index, screen_item_position _item_position) {
    screen_items[_index].x = _item_position.x;
    screen_items[_index].y = _item_position.y;
    screen_items[_index].w = _item_position.w;
    screen_items[_index].h = _item_position.h;
    Serial.println(F("Updated screen item"));
}

void Display::clear_screen_items() {
    memset(screen_items, 0, sizeof(screen_items));
    Serial.println(F("Cleared screen items array"));
}

void Display::update_screen_selector(byte _screen_item_index) {
    current_screen_selector.old_position = current_screen_selector.current_position;
    current_screen_selector.current_position = screen_items[_screen_item_index];
    current_screen_selector.screen_item_index = _screen_item_index;
    Serial.println(F("Updated screen selector. Re render now"));
//    Serial.print(F("Current position w: "));
//    Serial.println(current_screen_selector.current_position.w);
//    Serial.print(F("Current position h: "));
//    Serial.println(current_screen_selector.current_position.h);
    // Define border thickness
    byte border_thickness = 2; // Adjust the thickness of your border here

    // Define the color for the selector border
    uint16_t border_color = 0xEF51;

    // Draw new screen selector border
    // Draw top border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y - border_thickness,
                 current_screen_selector.current_position.w + (2 * border_thickness),
                 border_thickness, border_color);
    // Draw bottom border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y + current_screen_selector.current_position.h,
                 current_screen_selector.current_position.w + (2 * border_thickness),
                 border_thickness, border_color);
    // Draw left border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y - border_thickness,
                 border_thickness,
                 current_screen_selector.current_position.h + (2 * border_thickness), border_color);
    // Draw right border
    tft.fillRect(current_screen_selector.current_position.x + current_screen_selector.current_position.w,
                 current_screen_selector.current_position.y - border_thickness,
                 border_thickness,
                 current_screen_selector.current_position.h + (2 * border_thickness), border_color);
}

void Display::clear_screen_selector() const {
    //static uint16_t border_color = backgroundColor;
    // Define border thickness
    byte border_thickness = 2; // Adjust the thickness of your border here

    // Define the color for clearing the border if it changes
    //if (_border_color != border_color) border_color = _border_color;

    // Clear old screen selector border by drawing over it with the background color
    // Clear top border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y - border_thickness,
                 current_screen_selector.current_position.w + (2 * border_thickness),
                 border_thickness, screen_selector_border_color);
    // Clear bottom border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y + current_screen_selector.current_position.h,
                 current_screen_selector.current_position.w + (2 * border_thickness),
                 border_thickness, screen_selector_border_color);
    // Clear left border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y - border_thickness,
                 border_thickness,
                 current_screen_selector.current_position.h + (2 * border_thickness), screen_selector_border_color);
    // Clear right border
    tft.fillRect(current_screen_selector.current_position.x + current_screen_selector.current_position.w,
                 current_screen_selector.current_position.y - border_thickness,
                 border_thickness,
                 current_screen_selector.current_position.h + (2 * border_thickness), screen_selector_border_color);
}

void Display::set_screen_selector_border_color(feature_t _next_feature) {
    // Set screen selector border color accordingly to next feature
    switch (_next_feature) {
        //Serial.println(F("Set screen selector border color accordingly to next feature"));
//        case RFID_FACTORY_SELECT:
//        case RFID_PACKAGE_GROUPS_LIST:
//        case RFID_MES_PACKAGES_LIST:
//            screen_selector_border_color = 0x4208;
//            break;
        default:
            screen_selector_border_color = backgroundColor;
            break;
    }
}

void GIFDraw(GIFDRAW *pDraw) {
    uint8_t *s;
    uint16_t *d, *usPalette;
    int x, y, iWidth, iCount, centerX, centerY;

    // Calculate centered x and y coordinates
    centerX = (DISPLAY_WIDTH - pDraw->iWidth) / 2;
    centerY = (DISPLAY_HEIGHT - pDraw->iHeight) / 2;

    // Set drawing start point (top-left corner of the image) to the centered coordinates
    pDraw->iX = centerX;
    pDraw->iY = centerY;

    // Continue with the original code but now with the centered position
    iWidth = pDraw->iWidth;
    if (iWidth + pDraw->iX > DISPLAY_WIDTH)
        iWidth = DISPLAY_WIDTH - pDraw->iX;
    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line

    // Ensure that the drawing does not go outside the display bounds
    if (y >= DISPLAY_HEIGHT || pDraw->iX >= DISPLAY_WIDTH || iWidth < 1)
        return;

    // Old image disposal
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
        for (x = 0; x < iWidth; x++) {
            if (s[x] == pDraw->ucTransparent)
                s[x] = pDraw->ucBackground;
        }
        pDraw->ucHasTransparency = 0;
    }

    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
        uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
        pEnd = s + iWidth;
        x = 0;
        iCount = 0; // count non-transparent pixels
        while (x < iWidth) {
            c = ucTransparent - 1;
            d = &usTemp[0][0];
            while (c != ucTransparent && s < pEnd && iCount < BUFFER_SIZE) {
                c = *s++;
                if (c == ucTransparent) // done, stop
                {
                    s--; // back up to treat it like transparent
                } else // opaque
                {
                    *d++ = usPalette[c];
                    iCount++;
                }
            } // while looking for opaque pixels
            if (iCount) // any opaque pixels?
            {
                // DMA would degrtade performance here due to short line segments
                tft.setAddrWindow(pDraw->iX + x, y, iCount, 1);
                tft.pushPixels(usTemp, iCount);
                x += iCount;
                iCount = 0;
            }
            // no, look for a run of transparent pixels
            c = ucTransparent;
            while (c == ucTransparent && s < pEnd) {
                c = *s++;
                if (c == ucTransparent)
                    x++;
                else
                    s--;
            }
        }
    } else {
        s = pDraw->pPixels;

        // Unroll the first pass to boost DMA performance
        // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
        if (iWidth <= BUFFER_SIZE)
            for (iCount = 0; iCount < iWidth; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];
        else
            for (iCount = 0; iCount < BUFFER_SIZE; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA // 71.6 fps (ST7796 84.5 fps)
        tft.dmaWait();
    tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
    tft.pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
    dmaBuf = !dmaBuf;
#else // 57.0 fps
        tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
        tft.pushPixels(&usTemp[0][0], iCount);
#endif

        iWidth -= iCount;
        // Loop if pixel buffer smaller than width
        while (iWidth > 0) {
            // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
            if (iWidth <= BUFFER_SIZE)
                for (iCount = 0; iCount < iWidth; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];
            else
                for (iCount = 0; iCount < BUFFER_SIZE; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA
            tft.dmaWait();
      tft.pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
      dmaBuf = !dmaBuf;
#else
            tft.pushPixels(&usTemp[0][0], iCount);
#endif
            iWidth -= iCount;
        }
    }
} /* GIFDraw() */