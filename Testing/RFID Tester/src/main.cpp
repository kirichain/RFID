#include <Arduino.h>

// Use Hardware Serial on pins 16 and 17 for communication with the RFID device
HardwareSerial rfidSerial(2);

#define BAUD_RATE 115200

// Command definitions
const uint8_t CMD_HARDWARE_VERSION[] = {0xBB, 0x00, 0x03, 0x00, 0x01, 0x00, 0x04, 0x7E};
const uint8_t CMD_SOFTWARE_VERSION[] = {0xBB, 0x00, 0x03, 0x00, 0x01, 0x01, 0x05, 0x7E};
const uint8_t SET_TX_POWER[]         = {0xBB, 0x00, 0xB6, 0x00, 0x02, 0x07, 0xD0, 0x8F, 0x7E};
const uint8_t READ_STORAGE_CMD[]     = {0xBB, 0x00, 0x39, 0x00, 0x09, 0x00, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x02, 0x45, 0x7E};

void sendCommand(const uint8_t *command, size_t commandLength) {
    rfidSerial.write(command, commandLength);
}

String readResponse() {
    String response = "";
    while (!rfidSerial.available()) {
        // Wait for response
        delay(10);
    }
    while (rfidSerial.available()) {
        char hexBuffer[4];
        sprintf(hexBuffer, "%02X ", rfidSerial.read());
        response += hexBuffer;
    }
    return response;
}

void setup() {
    // Start the main serial port
    Serial.begin(BAUD_RATE);
    // Start the RFID serial port
    rfidSerial.begin(BAUD_RATE, SERIAL_8N1, 4, 25); // RX, TX

    if (rfidSerial) {
        Serial.println("Serial port is already open.");
    }

    // Send the hardware version command
    Serial.println("Hardware version command sent. Waiting for response...");
    sendCommand(CMD_HARDWARE_VERSION, sizeof(CMD_HARDWARE_VERSION));
    Serial.print("Hardware version response received: ");
    Serial.println(readResponse());

    // Send the software version command
    Serial.println("Software version command sent. Waiting for response...");
    sendCommand(CMD_SOFTWARE_VERSION, sizeof(CMD_SOFTWARE_VERSION));
    Serial.print("Software version response received: ");
    Serial.println(readResponse());

    // Send the set TX power command
    Serial.println("Set TX power command sent. Waiting for response...");
    sendCommand(SET_TX_POWER, sizeof(SET_TX_POWER));
    Serial.print("Set TX power response received: ");
    Serial.println(readResponse());

    // Send the read storage command
    Serial.println("Read storage command sent. Waiting for response...");
    sendCommand(READ_STORAGE_CMD, sizeof(READ_STORAGE_CMD));
    Serial.print("Read storage response received: ");
    Serial.println(readResponse());

    Serial.println("Serial port closed.");
}

void loop() {
    // No repeating actions needed
}