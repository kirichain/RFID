//
// Created by Tan on 25-Oct-23.
//

#include "display.h"

TFT_eSPI tft = TFT_eSPI();

Display::Display() {

}

void Display::init(feature_layout_t _feature_layout) {
    // Initialize display
    feature_layout = _feature_layout;
    tft.init();
    tft.setTextFont(2);
    tft.setSwapBytes(true);
    if (_feature_layout == PORTRAIT) {
        tft.setRotation(2);
        // Display resolution
        SCREEN_WIDTH = 320;
        SCREEN_HEIGHT = 480;
    } else {
        tft.setRotation(1);
        // Display resolution
        SCREEN_WIDTH = 480;
        SCREEN_HEIGHT = 320;
    }
    tft.fillScreen(0xFFFF);
    draw_layout(feature_layout);
}

void Display::draw_layout(feature_layout_t _feature_layout) {
    // Clear the screen before drawing the layout
    tft.fillScreen(TFT_BLACK);

    // Set text font and size
    tft.setTextSize(1);
    tft.setTextDatum(TL_DATUM); // Top-Left datum

    switch (_feature_layout) {
        case PORTRAIT:
            // Draw header at the top
            tft.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, headerColor);

            // Set text color for the header
            tft.setTextColor(textColor, headerColor);

            // Draw header at the top
            tft.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, headerColor);

            // Set the color for the header text
            tft.setTextColor(textColor, headerColor);

            // Draw WiFi status aligned to the top-left of the header
            tft.setTextDatum(TL_DATUM); // Align to the top-left
            tft.drawString(wifiStatus, 5, 5);

            // Draw login status aligned to the top-left of the header, below WiFi status
            tft.drawString(loginStatus, 5, 20);

            // Draw date and time aligned to the top-right of the header
            tft.setTextDatum(TR_DATUM); // Align to the top-right
            tft.drawString(dateTime, SCREEN_WIDTH - 5, 5);

            // Draw server status aligned to the top-right of the header, below date and time
            tft.drawString(serverStatus, SCREEN_WIDTH - 5, 20);

            // Draw viewport below the header
            tft.drawRect(0, HEADER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - HEADER_HEIGHT, borderColor);

            // Draw the navigation bar in the bottom
            // Calculate starting y-coordinate for the navigation bar
            navBarStartY = (_feature_layout == PORTRAIT ? SCREEN_HEIGHT : SCREEN_WIDTH) - NAV_BAR_HEIGHT;

            // Draw the navigation bar background
            tft.fillRect(0, navBarStartY, SCREEN_WIDTH, NAV_BAR_HEIGHT, navBarColor);

            // Calculate the center y-coordinate of each icon
            iconCenterY = navBarStartY + NAV_BAR_HEIGHT / 2;

            // Set the size for the icons
            iconSize = 30;

            // Adjust the x-coordinate calculations
            iconSpacing = (SCREEN_WIDTH - (4 * iconSize)) / 5; // Equal spacing between icons

            leftIconX = iconSpacing; // Start with one spacing unit from the left edge
            cancelIconX = leftIconX + iconSize + iconSpacing; // One icon and one spacing unit from the left icon
            okIconX = cancelIconX + iconSize + iconSpacing; // One icon and one spacing unit from the cancel icon
            rightIconX = okIconX + iconSize + iconSpacing; // One icon and one spacing unit from the ok icon

            // Draw Left/Back icon (left arrow)
            tft.setTextColor(TFT_WHITE);
            tft.fillTriangle(leftIconX, iconCenterY, // Top vertex
                             leftIconX + iconSize, iconCenterY - iconSize / 2, // Bottom left vertex
                             leftIconX + iconSize, iconCenterY + iconSize / 2, // Bottom right vertex
                             TFT_BLUE);

            // Draw Cancel icon (character 'x')
            tft.setTextColor(TFT_RED);
            tft.setTextSize(2); // Adjust text size as needed
            tft.drawChar('X', cancelIconX, iconCenterY - iconSize / 2, 2);

            // Draw Select/OK icon (circle with a dot in the center)
            tft.fillCircle(okIconX + iconSize / 2, iconCenterY, iconSize / 2, TFT_BLUE);
            tft.fillCircle(okIconX + iconSize / 2, iconCenterY, iconSize / 4, TFT_WHITE);

            // Draw Right/Next icon (right arrow)
            tft.setTextColor(TFT_WHITE);
            tft.fillTriangle(rightIconX + iconSize, iconCenterY, // Top vertex
                             rightIconX, iconCenterY - iconSize / 2, // Bottom left vertex
                             rightIconX, iconCenterY + iconSize / 2, // Bottom right vertex
                             TFT_BLUE);
            break;

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

byte Display::get_font_height() {
    // The height of the font
    return tft.fontHeight();
}

const menu_icon *Display::get_icon_by_name(const char *icon_name) {
    for (uint16_t i = 0; i < 22; i++) {
        if (strcmp(icons[i].name, icon_name) == 0) {
            return &icons[i];
        }
    }
    return nullptr; // Return nullptr if the icon is not found
}

void Display::put_icon(int x, int y, const char *icon_name) {
    const menu_icon *icon = get_icon_by_name(icon_name);
    if (icon != nullptr) {
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

void Display::render_icons_grid(const byte *iconIndices, byte _numIcons) {
//    if (not isSmallFontUsed) {
//        tft.setFreeFont(&FreeSans9pt7b);
//    }
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
                screen_item_position _item_position = {x, y, iconHeight + textHeight, iconWidth};
                update_screen_item(screen_item_index, _item_position);
                ++screen_item_index;
            }
        }
    }

    screen_item_count = screen_item_index;
    reset_display_setting();
    // Start to set screen selector to the first one item
    update_screen_selector(0);
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
    // Clear the viewport
    tft.fillRect(0, NAV_BAR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - NAV_BAR_HEIGHT - HEADER_HEIGHT, TFT_BLACK);
    // Clear screen items and reset screen selector
    clear_screen_selector();
    clear_screen_items();

    switch (_feature) {
        case BOOT:
            // Code to handle BOOT feature
            break;
        case HOME_HANDHELD_1: {
            // Define which icons to display for the HOME HANDHELD 1 case
            const byte homeHandheld1IconIndices[] = {0, 1, 2, 3, 4, 5};
            // Call the new render_icons_grid function with the specific icons for HOME HANDHELD 1
            render_icons_grid(homeHandheld1IconIndices, 6);
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
        case HOME_HANDHELD_2:
            // Code to handle HOME_HANDHELD feature
            // Put icons and menu texts on the screen
            break;
        case HOME_TERMINAL:
            // Code to handle HOME_TERMINAL feature
            break;
        case SETTING: {
            // Define which icons to display for the SETTING case
            const byte settingIconIndices[] = {12, 13};
            // Call the new render_icons_grid function with the specific icons for SETTING
            render_icons_grid(settingIconIndices, 2);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = SETTING_WIFI;
            current_screen_features[1] = SETTING_USER_INFO;
            break;
        }
        case SETTING_WIFI: {
            // Check if background task is completed, if yes, start rendering, else, set background tasks and return
            if (is_background_task_completed) {
                Serial.println(F("Start listing all available Wi-Fi networks on the screen"));
                byte wifi_network_index = 0;
                // Print all available wifi networks
                tft.setFreeFont(&FreeSans9pt7b);
                tft.setTextColor(TFT_WHITE);

                // Define the starting position
                int x = 20; // Start from left padding
                int y = 50; // Start from below the header bar + padding
                int lineHeight = 30; // Set the line height or row height to 50% more
                int rowPadding = 5; // Set padding between rows
                int headerHeight = 40; // Height of the header bar
                int navbarHeight = 40; // Height of the navbar
                int titlePadding = 20; // Padding between header and titles set to 20
                int topSpace = 10; // Space between the text in the first row and titles
                int leftPadding = 20; // Left padding
                int rightPadding = 20; // Right padding

                // Adjust the column width for Index by 100%
                int colWidthIndex = 60; // Increased width for No (index) column
                int colWidthRSSI = 50; // Width for RSSI column
                // Calculate remaining width for SSID
                int colWidthSSID = tft.width() - colWidthIndex - colWidthRSSI - (leftPadding + rightPadding);

                // Draw titles with increased y coordinate
                y += titlePadding; // Move below the header bar
                tft.setCursor(x, y);
                tft.print(F("Index"));
                tft.setCursor(x + colWidthIndex + rowPadding, y);
                tft.print(F("SSID"));
                tft.setCursor(x + colWidthIndex + colWidthSSID + rowPadding, y);
                tft.print(F("RSSI"));

                // Start drawing the grid below the title with top space
                y += topSpace + lineHeight; // Apply top space and move below the title

                while (wifi_network_index < _taskResults.wifi_networks_count) {
                    const char *ssid = _taskResults.wifi_networks[wifi_network_index].ssid;
                    int rssi = _taskResults.wifi_networks[wifi_network_index].rssi;

                    // Draw only if SSID is not empty
                    if (strlen(ssid) > 0) {
                        // Draw the index (No)
                        tft.setCursor(x, y);
                        tft.print(wifi_network_index + 1);

                        // Draw vertical line after index
                        tft.drawFastVLine(x + colWidthIndex - rowPadding, y - lineHeight - topSpace,
                                          lineHeight + topSpace, TFT_WHITE);

                        // Draw the SSID
                        tft.setCursor(x + colWidthIndex + rowPadding, y);
                        tft.print(ssid);

                        // Draw vertical line after SSID
                        tft.drawFastVLine(x + colWidthIndex + colWidthSSID, y - lineHeight - topSpace,
                                          lineHeight + topSpace, TFT_WHITE);

                        // Draw the RSSI
                        tft.setCursor(x + colWidthIndex + colWidthSSID + rowPadding, y);
                        tft.print(rssi);

                        // Draw horizontal line after row
                        tft.drawFastHLine(leftPadding, y + rowPadding, tft.width() - (leftPadding + rightPadding),
                                          TFT_WHITE);

                        // Move to the next line
                        y += lineHeight + rowPadding; // Move down with row padding and increased row height

                        // Check if we are near the bottom of the screen, if so, break or scroll
                        if (y > tft.height() - navbarHeight - lineHeight - topSpace) {
                            break; // Or implement scrolling
                        }
                    }

                    wifi_network_index++;
                }

                // Set items type on screen
                current_feature_item_type = LIST_ITEM;
                // Reset current screen features
                memset(current_screen_features, NO_FEATURE, 10);
                screen_item_count = 0;
            } else {
                is_background_task_required = true;
                // Reset current screen background tasks
                for (byte i = 0; i < 10; ++i) {
                    current_screen_background_tasks[i] = NO_TASK;
                }
                current_screen_background_tasks[0] = SCAN_WIFI_NETWORKS;
            }
            break;
        }
        case SETTING_USER_INFO: {
            // Define which icons to display for the SETTING_USER_INFO case
            const byte settingUserInfoIconIndices[] = {14, 15};
            // Call the new render_icons_grid function with the specific icons for SETTING_USER_INFO
            render_icons_grid(settingUserInfoIconIndices, 2);
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
            // Define which icons to display for the RFID case
            const byte rfidIconIndices[] = {9, 10, 11};
            // Call the new render_icons_grid function with the specific icons for RFID
            render_icons_grid(rfidIconIndices, 3);
            current_feature_item_type = MENU_ICON;
            // Reset current screen features
            memset(current_screen_features, NO_FEATURE, 10);
            current_screen_features[0] = RFID_SCAN;
            current_screen_features[1] = RFID_SCAN_HISTORY;
            current_screen_features[2] = RFID_MODIFY_TAG_DATA;
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
            // Draw the grid and column titles
            draw_grid();
            draw_titles();
            // Draw each item in the history
            for (byte i = 0; i < 14; i++) {
                draw_history_item(i, history[i]);
            }
            current_feature_item_type = LIST_ITEM;
            break;
        }
        case RFID_SCAN_RESULT: {
            // Part 1: RFID Scan Result Header below the existing navigation bar
            tft.fillRect(0, HEADER_HEIGHT, SCREEN_WIDTH, RFID_SCAN_RESULT_HEADER_HEIGHT,
                         convert_to_565_color(0x2596be));
            tft.setTextColor(TFT_WHITE);
            tft.setFreeFont(&FreeSans18pt7b);
            //tft.setTextSize(2);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("Scan result", SCREEN_WIDTH / 2, HEADER_HEIGHT + RFID_SCAN_RESULT_HEADER_HEIGHT / 2);
            tft.setTextDatum(TL_DATUM); // Resetting datum for subsequent text

            // Divider line below the header
            int divider1Y = HEADER_HEIGHT + RFID_SCAN_RESULT_HEADER_HEIGHT;
            tft.fillRect(0, divider1Y, SCREEN_WIDTH, 2, TFT_WHITE);

            byte leftPadding = 10;
            // Part 2: RFID Scan Result Product Info
            int productInfoStartY = HEADER_HEIGHT + RFID_SCAN_RESULT_HEADER_HEIGHT + 2 + VERTICAL_SPACE;
            // Display product image/product image container
//            tft.drawRect(leftPadding, productInfoStartY, RFID_SCAN_RESULT_PRODUCT_IMG_WIDTH,
//                         RFID_SCAN_RESULT_PRODUCT_IMG_HEIGHT,
//                         TFT_WHITE);
            tft.pushImage(leftPadding, productInfoStartY, RFID_SCAN_RESULT_PRODUCT_IMG_WIDTH,
                          RFID_SCAN_RESULT_PRODUCT_IMG_HEIGHT, backpack);
            // Adjusted text positions in Part 2 with TEXT_SPACING for vertical space between lines and text size 1
            int textXOffset = RFID_SCAN_RESULT_PRODUCT_IMG_WIDTH + 10 + leftPadding;
            int textYOffset =
                    productInfoStartY + VERTICAL_SPACE; // Start the text below the image with some vertical space
            tft.setFreeFont(&FreeSans9pt7b);
            //tft.setTextSize(1); // Set text size to 1 for Part 2
            tft.setCursor(textXOffset, textYOffset);
            tft.print("Product ID: XYZ");
            tft.setCursor(textXOffset, textYOffset + TEXT_SPACING);
            tft.print("Size: M");
            tft.setCursor(textXOffset, textYOffset + 2 * TEXT_SPACING);
            tft.print("Color: Red");

            // Divider line below Part 2
            int divider2Y = productInfoStartY + RFID_SCAN_RESULT_PRODUCT_IMG_HEIGHT + VERTICAL_SPACE;
            tft.fillRect(0, divider2Y, SCREEN_WIDTH, 2, TFT_WHITE);

            // Part 3: RFID Scan Result Tag Info with TEXT_SPACING between lines and text size 2
            int tagInfoStartY = divider2Y + 2 + VERTICAL_SPACE + 20;

            tft.setFreeFont(&FreeSans12pt7b);
            //tft.setTextFont(2);
            //tft.setTextSize(2); // Set text size to 2 for Part 3
            tft.setCursor(leftPadding, tagInfoStartY);
            tft.print("Brand: BrandName");
            tft.setCursor(leftPadding, tagInfoStartY + TEXT_SPACING + 5);
            tft.print("PO code: 12345");
            tft.setCursor(leftPadding, tagInfoStartY + 2 * TEXT_SPACING + 10);
            tft.print("EPC: 0987654321");
            tft.setCursor(leftPadding, tagInfoStartY + 3 * TEXT_SPACING + 15);
            tft.print("Shipped to ID: STID123");

            // Special handling for "Tag status: Associated" with green background
            tft.setCursor(leftPadding, tagInfoStartY + 4 * TEXT_SPACING + 20);
            tft.print("Tag status: ");
//            tft.setTextColor(TFT_GREEN); // Set text color to black with green background
//            tft.print("Associated");
            tft.setTextColor(TFT_RED); // Set text color to black with green background
            tft.print("Unassociated");
            // Reset text size and color for subsequent text
            reset_display_setting();
            current_feature_item_type = LIST_ITEM;
            break;
        }
        case RFID_MODIFY_TAG_DATA:
            current_feature_item_type = LIST_ITEM;
            break;
        case PACKAGE: {
            // Define which icons to display for the PACKAGE case
            const byte packageIconIndices[] = {21};
            // Call the new render_icons_grid function with the specific icons for PACKAGE
            render_icons_grid(packageIconIndices, 1);
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
            render_icons_grid(coworkingIconIndices, 3);
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
            render_icons_grid(databaseIconIndices, 2);
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
            render_icons_grid(dataImportIconIndices, 3);
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

void Display::draw_grid() const {
    // Draw horizontal lines for the grid
    for (int i = 0; i <= SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT; i += ROW_HEIGHT) {
        tft.drawLine(0, HEADER_HEIGHT + i, SCREEN_WIDTH, HEADER_HEIGHT + i, TFT_WHITE);
    }

    // Draw the vertical lines for the grid
    draw_vertical_line();
}

void Display::draw_titles() const {
    tft.setTextColor(TFT_WHITE);
    // Align text to the left
    tft.setTextDatum(TL_DATUM);

    // Draw title for "No." column
    tft.drawString("No.", TEXT_PADDING, HEADER_HEIGHT + TEXT_PADDING);

    // Draw title for "EPC" column
    tft.drawString("EPC", COL_WIDTH_NO + TEXT_PADDING, HEADER_HEIGHT + TEXT_PADDING);

    // Draw title for "Timestamp/Last scan" column
    tft.drawString("Last scan", COL_WIDTH_NO + COL_WIDTH_EPC + TEXT_PADDING, HEADER_HEIGHT + TEXT_PADDING);

    // Draw title for "Count" column
    tft.drawString("Count", COL_WIDTH_NO + COL_WIDTH_EPC + COL_WIDTH_TIMESTAMP + TEXT_PADDING,
                   HEADER_HEIGHT + TEXT_PADDING);
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

void Display::update_status(status_indicators _status) {

}

uint16_t Display::convert_to_565_color(uint32_t hex_color) {
    // Convert the 24-bit RGB color to a 16-bit color.
    uint16_t r = (hex_color >> 16) & 0xFF; // Extract the red component.
    uint16_t g = (hex_color >> 8) & 0xFF;  // Extract the green component.
    uint16_t b = hex_color & 0xFF;         // Extract the blue component.

    // Convert to 5 bits for red, 6 bits for green, and 5 bits for blue.
    uint16_t color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    return color565;
}

void Display::reset_display_setting() {
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
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

    // Define border thickness
    byte border_thickness = 2; // Adjust the thickness of your border here

    // Define the color for the border
    uint16_t border_color = TFT_RED; // Replace with your desired border color

    // Define the color for clearing the border (background color)
    uint16_t background_color = TFT_BLACK; // Replace with your actual background color

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
    // Define border thickness
    byte border_thickness = 2; // Adjust the thickness of your border here

    // Define the color for the border
    uint16_t border_color = TFT_RED; // Replace with your desired border color

    // Define the color for clearing the border (background color)
    uint16_t background_color = TFT_BLACK; // Replace with your actual background color

    // Clear old screen selector border by drawing over it with the background color
    // Clear top border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y - border_thickness,
                 current_screen_selector.current_position.w + (2 * border_thickness),
                 border_thickness, background_color);
    // Clear bottom border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y + current_screen_selector.current_position.h,
                 current_screen_selector.current_position.w + (2 * border_thickness),
                 border_thickness, background_color);
    // Clear left border
    tft.fillRect(current_screen_selector.current_position.x - border_thickness,
                 current_screen_selector.current_position.y - border_thickness,
                 border_thickness,
                 current_screen_selector.current_position.h + (2 * border_thickness), background_color);
    // Clear right border
    tft.fillRect(current_screen_selector.current_position.x + current_screen_selector.current_position.w,
                 current_screen_selector.current_position.y - border_thickness,
                 border_thickness,
                 current_screen_selector.current_position.h + (2 * border_thickness), background_color);
}


