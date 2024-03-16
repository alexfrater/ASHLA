import serial
import time
# Replace '/dev/cu.HC-05-DevB' with the serial port to which your HC-05 is connected.
serial_port = 'COM9'
baud_rate = 9600  # Match the baud rate set on your HC-05 module, default is often 9600.
try:
    # Establishing the connection
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    print(f"Connected to {serial_port} at {baud_rate} baud.")
    # # Sending data
    # send_data = "Hello HC-05!"
    # ser.write(send_data.encode('utf-8'))
    # print(f"Sent data: {send_data}")
    #
    # # Give the HC-05 some time to respond
    # time.sleep(1)
    # Receiving data
    incoming_data = ser.readline().decode('utf-8').rstrip()
    if incoming_data:
        print(f"Received data: {incoming_data}")
    else:
        print("No data received.")
except serial.SerialException as e:
    print(f"Failed to connect to {serial_port}: {e}")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial connection closed.")
 
