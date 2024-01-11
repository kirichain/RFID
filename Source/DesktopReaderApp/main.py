import serial
import time

# Set up serial connection parameters
SERIAL_PORT = 'COM6'  # Replace 'COMx' with your actual COM port number
BAUD_RATE = 115200
TIMEOUT = 1  # Set to None for blocking read or a number for timeout in seconds

# Initialize serial port
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Define the commands as per the protocol
CMD_HARDWARE_VERSION = b'\xBB\x00\x03\x00\x01\x00\x04\x7E'
CMD_SOFTWARE_VERSION = b'\xBB\x00\x03\x00\x01\x01\x05\x7E'
SET_TX_POWER = b'\xBB\x00\xB6\x00\x02\x07\xD0\x8F\x7E'
READ_STORAGE_CMD = b'\xBB\x00\x39\x00\x09\x00\x00\xFF\xFF\x03\x00\x00\x00\x02\x45\x7E'

# Function to send a command to the device
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Give the device time to respond

# Function to read a response from the device
def read_response():
    # Read response from device until the delimiter 0x7E
    response = ser.read_until(b'\x7E')
    # Convert the response to a space-separated hex string
    return ' '.join(format(x, '02X') for x in response)

# Function to send the set transmitting power command
def send_set_tx_power_command():
    send_command(SET_TX_POWER)
    print("Set TX power command sent. Waiting for response...")
    response = read_response()
    print("Set TX power response received:", response)

# Function to send the read storage command
def send_read_storage_command():
    send_command(READ_STORAGE_CMD)
    print("Read storage command sent. Waiting for response...")
    response = read_response()
    print("Read storage response received:", response)

try:
    # Check if the serial port is already open
    if not ser.is_open:
        ser.open()
        print("Serial port opened successfully.")
    else:
        print("Serial port is already open.")

    # Send the command for hardware version and read the response
    send_command(CMD_HARDWARE_VERSION)
    print("Hardware version command sent. Waiting for response...")
    response_hw = read_response()
    print("Hardware version response received:", response_hw)

    # Send the command for software version and read the response
    send_command(CMD_SOFTWARE_VERSION)
    print("Software version command sent. Waiting for response...")
    response_sw = read_response()
    print("Software version response received:", response_sw)

    # Send the command for setting the transmitting power and read the response
    send_set_tx_power_command()

    # Send the command for reading from storage and read the response
    send_read_storage_command()

except serial.SerialException as e:
    print("Error with the serial port:", e)

finally:
    # Close the serial port
    if ser.is_open:
        ser.close()
        print("Serial port closed.")