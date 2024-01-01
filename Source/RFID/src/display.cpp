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
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextDatum(TL_DATUM); // Top-Left datum

    switch (_feature_layout) {
        case PORTRAIT:
            // Draw header at the top
            tft.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, headerColor);

            // Set text color for the header
            tft.setTextColor(textColor, headerColor);

            // Draw WiFi status at the top-left
            tft.drawString(wifiStatus, 5, 5);

            // Draw login status directly below WiFi status
            tft.drawString(loginStatus, 5, 20);

            // Draw date and time at the top-right
            dateTimeWidth = get_string_width(dateTime);
            tft.drawString(dateTime, SCREEN_WIDTH - dateTimeWidth - 22, 5);

            // Draw server status directly below date and time
            serverStatusWidth = get_string_width(serverStatus);
            tft.drawString(serverStatus, SCREEN_WIDTH - serverStatusWidth - 15, 20);

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

int Display::get_string_width(const char *string) {
    // For fixed-width font
    int charWidth = 6; // Width of a character at text size 1
    int textSize = 1;  // Current text size setting

    // Calculate the width of the string
    // strlen(str) gives the number of characters in the string
    // The width of one character is multiplied by the number of characters and the text size
    int stringWidth = strlen(string) * charWidth * textSize;

    return stringWidth;
}

const menu_icon *Display::get_icon_by_name(const char *icon_name) {
    for (uint16_t i = 0; i < 21; i++) {
        if (strcmp(icons[i].name, icon_name) == 0) {
            return &icons[i];
        }
    }
    return nullptr; // Return nullptr if the icon is not found
}

void Display::put_icon(int x, int y, const char *icon_name) {
    tft.setSwapBytes(true);
    const menu_icon *icon = get_icon_by_name(icon_name);
    if (icon != nullptr) {
        tft.pushImage(x, y, iconWidth, iconHeight, icon->icon_data);
    } else {
        // Handle the error, for example, by printing to the serial port
        Serial.print("Icon not found: ");
        Serial.println(icon_name);
    }
}

void Display::put_text(int x, int y, const char *content) {
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

    // Calculate the new width after modifications
    int textWidth = get_string_width(text.c_str());

    // Set cursor to the new adjusted position to center the modified text
    tft.setCursor(x - (textWidth / 2), y);

    // Set text color and size
    tft.setTextColor(textColor);
    tft.setTextSize(1); // Set text size if needed

    // Print the modified text
    tft.print(text);

}

void Display::draw_icon_with_label(int x, int y, int iconIndex, const char *iconNames[], TFT_eSPI &tft) {
    put_icon(x, y, iconNames[iconIndex]);
    put_text(x + iconWidth / 2, y + iconHeight, iconNames[iconIndex]);
}

int Display::calculate_columns(int iconCount) {
    return (iconCount % 2 == 0) ? 2 : 1;
}

int Display::calculate_rows(int iconCount, int numColumns) {
    // If there is only one column, the number of rows is equal to the number of icons
    if (numColumns == 1) {
        return iconCount;
    } else {
        // If there are two columns, the number of rows is half of the number of icons for even counts
        // or half + 1 for odd counts
        return (iconCount + 1) / numColumns;
    }
}

void Display::render_feature(feature_t _feature) {
    // Clear the viewport
    tft.fillRect(0, NAV_BAR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - NAV_BAR_HEIGHT - HEADER_HEIGHT, TFT_BLACK);

    switch (_feature) {
        case BOOT:
            // Code to handle BOOT feature
            break;
        case HOME_HANDHELD_1:
            // Refactored to use dynamic calculations
            numIcons = 6; // Assuming 6 icons for this feature
            numColumns = calculate_columns(numIcons);
            numRows = calculate_rows(numIcons, numColumns);

            // Calculate horizontal and vertical spacing for the grid
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));
            // Draw the icons in a grid
            for (int row = 0; row < numRows; row++) {
                menu_icon_y = vSpacing + NAV_BAR_HEIGHT + row * (iconHeight + textHeight + vSpacing);
                menu_icon_x = hSpacing;
                for (int col = 0; col < numColumns; col++) {
                    iconIndex = row * numColumns + col;
                    if (iconIndex < numIcons) { // Ensure we don't draw more icons than we have
                        draw_icon_with_label(menu_icon_x, menu_icon_y, iconIndex, menu_icon_names, tft);
                        menu_icon_x += iconWidth + hSpacing; // Move to the next icon's horizontal position
                    }
                }
            }
            break;
        case HOME_HANDHELD_2:
            // Code to handle HOME_HANDHELD feature
            // Put icons and menu texts on the screen
            break;
        case HOME_TERMINAL:
            // Code to handle HOME_TERMINAL feature
            break;
        case SETTING:
            // Refactored to use dynamic calculations
            numIcons = 2; // WiFi settings and User
            numColumns = calculate_columns(numIcons);
            numRows = calculate_rows(numIcons, numColumns);

            // Calculate horizontal and vertical spacing for the grid
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));

            menu_icon_x = hSpacing;
            menu_icon_y = vSpacing + NAV_BAR_HEIGHT;

            // Assuming the icon indexes are 12 for WiFi and 13 for User
            draw_icon_with_label(menu_icon_x, menu_icon_y, 12, menu_icon_names, tft); // WiFi
            menu_icon_x += iconWidth + hSpacing;
            draw_icon_with_label(menu_icon_x, menu_icon_y, 13, menu_icon_names, tft); // User
            break;
        case SETTING_WIFI:
            // Code to handle SETUP_WIFI feature
            break;
        case SETTING_USER_INFO:
            // Calculate the horizontal starting position for the first icon
            menu_icon_x = round((SCREEN_WIDTH - (2 * iconWidth)) / 3);
            // Calculate the vertical starting position for the icons
            menu_icon_y = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - iconHeight - textHeight) / 2) +
                          NAV_BAR_HEIGHT;

            // Draw the Login icon (assumed index 14 in menu_icon_names)
            draw_icon_with_label(menu_icon_x, menu_icon_y, 14, menu_icon_names, tft);

            // Move to the next column to draw the Logout icon
            menu_icon_x += iconWidth + hSpacing;
            draw_icon_with_label(menu_icon_x, menu_icon_y, 15, menu_icon_names, tft);
            break;
        case SETTING_USER_INFO_LOGIN:
            // Code to handle SETUP_USER_INFO_LOGIN feature
            break;
        case SETTING_USER_INFO_LOGOUT:
            // Code to handle SETUP_USER_INFO_LOGOUT feature
            break;
        case RFID:
            // Assuming that the first RFID icon index is 9
            numColumns = calculate_columns(3);
            numRows = calculate_rows(3, numColumns);

            // Calculate horizontal and vertical spacing for the grid
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));

            // Initialize the starting positions for the icons
            menu_icon_x = hSpacing;
            menu_icon_y = vSpacing + NAV_BAR_HEIGHT; // Start after the navigation bar height

            // Draw the RFID icons in a grid
            for (int row = 0; row < numRows; row++) {
                for (int col = 0; col < numColumns; col++) {
                    int iconIndex = row * numColumns + col;
                    if (iconIndex < 3) { // Check to make sure the icon index is within the range of RFID icons
                        draw_icon_with_label(menu_icon_x, menu_icon_y, 9 + iconIndex, menu_icon_names, tft);
                        menu_icon_x += iconWidth + hSpacing;
                    }
                }
                // Reset X to the starting position and increment Y for the next row
                menu_icon_x = hSpacing;
                menu_icon_y += iconHeight + textHeight + vSpacing;
            }
            break;
        case RFID_SCAN:
            // Code to handle RFID_SCAN feature
            break;
        case RFID_SCAN_HISTORY:
            // Code to handle RFID_SCAN_HISTORY feature
            break;
        case RFID_SCAN_RESULT:
            // Code to handle RFID_SCAN_RESULT feature
            break;
        case RFID_MODIFY_TAG_DATA:
            // Code to handle RFID_MODIFY_TAG_DATA feature
            break;
        case PACKAGE:
            numIcons = 1; // Package details
            numColumns = calculate_columns(numIcons);
            numRows = calculate_rows(numIcons, numColumns);

            // Logic similar to other cases with single icon
            // Calculate horizontal and vertical spacing for the grid
            menu_icon_x = round((SCREEN_WIDTH - iconWidth) / 2);
            menu_icon_y = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - iconHeight - textHeight) / 2) +
                          NAV_BAR_HEIGHT;

            // Assuming the icon index for 'Package details' is 20
            draw_icon_with_label(menu_icon_x, menu_icon_y, 20, menu_icon_names, tft);
            break;
        case PACKAGE_DETAILS:
            // Code to handle PACKAGE_DETAILS feature
            break;
        case CO_WORKING:
            // Assuming that the first CO_WORKING icon index is 6
            numColumns = calculate_columns(3);
            numRows = calculate_rows(3, numColumns);

            // Calculate horizontal and vertical spacing for the grid
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));

            // Initialize the starting positions for the icons
            menu_icon_x = hSpacing;
            menu_icon_y = vSpacing + NAV_BAR_HEIGHT; // Start after the navigation bar height

            // Draw the CO_WORKING icons in a grid
            for (int row = 0; row < numRows; row++) {
                for (int col = 0; col < numColumns; col++) {
                    int iconIndex = row * numColumns + col;
                    if (iconIndex < 3) { // Check to make sure the icon index is within the range of CO_WORKING icons
                        draw_icon_with_label(menu_icon_x, menu_icon_y, 6 + iconIndex, menu_icon_names, tft);
                        menu_icon_x += iconWidth + hSpacing;
                    }
                }
                // Reset X to the starting position and increment Y for the next row
                menu_icon_x = hSpacing;
                menu_icon_y += iconHeight + textHeight + vSpacing;
            }
            break;
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
        case DATABASE:
            // Assuming that the first DATABASE icon index is 16
            numColumns = calculate_columns(2); // Even number of icons, so 2 columns
            numRows = calculate_rows(2, numColumns);

            // Calculate horizontal and vertical spacing for the grid
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));

            // Initialize the starting positions for the icons
            menu_icon_x = hSpacing;
            menu_icon_y = vSpacing + NAV_BAR_HEIGHT; // Start after the navigation bar height

            // Draw the DATABASE icons in a grid
            for (int row = 0; row < numRows; row++) {
                for (int col = 0; col < numColumns; col++) {
                    int iconIndex = row * numColumns + col;
                    if (iconIndex < 2) { // Check to make sure the icon index is within the range of DATABASE icons
                        draw_icon_with_label(menu_icon_x, menu_icon_y, 16 + iconIndex, menu_icon_names, tft);
                        menu_icon_x += iconWidth + hSpacing;
                    }
                }
                // Reset X to the starting position and increment Y for the next row
                menu_icon_x = hSpacing;
                menu_icon_y += iconHeight + textHeight + vSpacing;
            }
            break;
        case DATA_IMPORT:
            numIcons = 2; // Import from SD card and Import from Server
            numColumns = calculate_columns(numIcons);
            numRows = calculate_rows(numIcons, numColumns);

            // Logic similar to other cases with grid drawing
            // Calculate horizontal and vertical spacing for the grid
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));

            menu_icon_x = hSpacing;
            menu_icon_y = vSpacing + NAV_BAR_HEIGHT;

            // Assuming the icon index for 'Import from SD card' is 18 and 'Import from Server' is 19
            draw_icon_with_label(menu_icon_x, menu_icon_y, 18, menu_icon_names, tft); // SD Card
            menu_icon_x += iconWidth + hSpacing;
            draw_icon_with_label(menu_icon_x, menu_icon_y, 19, menu_icon_names, tft); // Server
            break;
        case DATA_IMPORT_FROM_SD_CARD:
            // Code to handle DATA_IMPORT_FROM_SD_CARD feature
            break;
        case DATA_IMPORT_FROM_SERVER:
            // Code to handle DATA_IMPORT_FROM_SERVER feature
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

void Display::blink_screen(bool &isTaskCompleted) {
    static int blink_count = 0;
    if (blink_count <= 5) {
        Serial.print(F("Execute time: "));
        Serial.println(blink_count);
        tft.fillScreen(random(0xFFFF));
        delay(500);
        tft.fillScreen(0xFFFF);
        delay(500);
        blink_count++;
    } else {
        isTaskCompleted = true;
    }
}

void Display::update_status(status_indicators _status) {

}