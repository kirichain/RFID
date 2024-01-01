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
            tft.drawTriangle(leftIconX, iconCenterY, // Top vertex
                             leftIconX + iconSize, iconCenterY - iconSize / 2, // Bottom left vertex
                             leftIconX + iconSize, iconCenterY + iconSize / 2, // Bottom right vertex
                             TFT_WHITE);

            // Draw Cancel icon (character 'x')
            tft.setTextColor(TFT_RED);
            tft.setTextSize(2); // Adjust text size as needed
            tft.drawChar('X', cancelIconX, iconCenterY - iconSize / 2, 2);

            // Draw Select/OK icon (circle with a dot in the center)
            tft.fillCircle(okIconX + iconSize / 2, iconCenterY, iconSize / 2, TFT_BLUE);
            tft.fillCircle(okIconX + iconSize / 2, iconCenterY, iconSize / 4, TFT_WHITE);

            // Draw Right/Next icon (right arrow)
            tft.setTextColor(TFT_WHITE);
            tft.drawTriangle(rightIconX + iconSize, iconCenterY, // Top vertex
                             rightIconX, iconCenterY - iconSize / 2, // Bottom left vertex
                             rightIconX, iconCenterY + iconSize / 2, // Bottom right vertex
                             TFT_WHITE);
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

const menu_icon* Display::get_icon_by_name(const char* icon_name) {
    for (uint16_t i = 0; i < 21; i++) {
        if (strcmp(icons[i].name, icon_name) == 0) {
            return &icons[i];
        }
    }
    return nullptr; // Return nullptr if the icon is not found
}

void Display::put_icon(int x, int y, const char *icon_name) {
    tft.setSwapBytes(true);
    const menu_icon* icon = get_icon_by_name(icon_name);
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

void Display::render_feature(feature_t _feature) {
    draw_layout(feature_layout);
    switch (_feature) {
        case BOOT:
            // Code to handle BOOT feature
            break;
        case HOME_HANDHELD_1:
            // Put icons and menu texts on the screen
            hSpacing = round((SCREEN_WIDTH - (numColumns * iconWidth)) / (numColumns + 1));
            vSpacing = round((SCREEN_HEIGHT - HEADER_HEIGHT - NAV_BAR_HEIGHT - (numRows * (iconHeight + textHeight)))
                             / (numRows + 1));
            Serial.print("hSpacing: ");
            Serial.println(hSpacing);
            Serial.print("vSpacing: ");
            Serial.println(vSpacing);
            Serial.print("Display size");
            Serial.println(SCREEN_WIDTH);
            Serial.println(SCREEN_HEIGHT);
            tft.fillRect(0, NAV_BAR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - NAV_BAR_HEIGHT - HEADER_HEIGHT, TFT_BLACK);

            // Draw the icons in a 2x3 grid
            for (int row = 0; row < numRows; row++) {
                menu_icon_x = hSpacing;
                for (int col = 0; col < numColumns; col++) {
                    iconIndex = row * numColumns + col;
                    put_icon(menu_icon_x, menu_icon_y, menu_icon_names[iconIndex]);
                    put_text(menu_icon_x + iconWidth / 2, menu_icon_y + iconHeight,
                             menu_icon_names[iconIndex]); // Adjust x-coordinate for centered text
                    menu_icon_x += iconWidth + hSpacing;
                }
                menu_icon_y += iconHeight + textHeight + vSpacing;
            }

            // Center the page indicator horizontally, use `put_text` method to display it
            //put_text(SCREEN_WIDTH / 2, y + pageIndicatorMargin, pageIndicator);
            break;
        case HOME_HANDHELD_2:
            // Code to handle HOME_HANDHELD feature
            // Put icons and menu texts on the screen
            break;
        case HOME_TERMINAL:
            // Code to handle HOME_TERMINAL feature
            break;
        case SETUP:
            // Code to handle SETUP feature
            break;
        case SETUP_WIFI:
            // Code to handle SETUP_WIFI feature
            break;
        case SETUP_USER_INFO:
            // Code to handle SETUP_USER_INFO feature
            break;
        case SETUP_USER_INFO_LOGIN:
            // Code to handle SETUP_USER_INFO_LOGIN feature
            break;
        case SETUP_USER_INFO_LOGOUT:
            // Code to handle SETUP_USER_INFO_LOGOUT feature
            break;
        case RFID:
            // Code to handle RFID feature
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
            // Code to handle PACKAGE feature
            break;
        case PACKAGE_DETAILS:
            // Code to handle PACKAGE_DETAILS feature
            break;
        case CO_WORKING:
            // Code to handle CO_WORKING feature
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
            // Code to handle DATABASE feature
            break;
        case DATA_IMPORT:
            // Code to handle DATA_IMPORT feature
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