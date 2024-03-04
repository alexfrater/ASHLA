import serial
import os
import re

obj_name = 'cap'
def find_highest_data_number(folder_path, prefix):
    # Regex pattern to match files starting with 'cap' followed by numbers
    pattern = re.compile(rf'^{re.escape(prefix)}(\d+)\.txt$')

    highest_number = -1
    for filename in os.listdir(folder_path):
        match = pattern.match(filename)
        if match:
            # Extract number part and convert to integer
            number = int(match.group(1))
            if number > highest_number:
                highest_number = number

    return highest_number if highest_number != -1 else 1

# Parameters
serial_port = 'COM5'  # Change this to your serial port name
baud_rate = 115200  # Match this to your Arduino's baud rate
output_file_path = 'output.txt'
ser = serial.Serial(serial_port, baud_rate)

# Open serial port
max_vert_angle = 1000
min_vert_angle = 720
increment = 2
hor_multiplier = 1
vert_distance = max_vert_angle-min_vert_angle+1

image = [[0]*(60*hor_multiplier+1) for _ in range(int(vert_distance/increment))]
pos = None

# print(image)
pos = -1
start = False
# while(pos<60)

def getpos():
    line = ser.readline().decode('utf-8').strip()
    if 'POS' in line:
        # Extract the part of the string after 'POS'
        pos_value_str = line.split('POS')[-1].strip()
        try:
            # Attempt to convert the following value to an integer
            pos_value = int(pos_value_str)
            # print("POS Value:", pos_value)
            return int(pos_value_str)
        except ValueError:
            # Handle the case where conversion fails
            print("Could not convert POS value to an integer:", pos_value_str)
            return None

ser.write(b'S')
print('S')


pos_value_str = getpos()

while True:
    pos_value = getpos()

    if pos_value!=None:
        print(pos_value)
        if pos_value == min_vert_angle+increment:
            break
print('Started SCAN')
while(pos_value<max_vert_angle):
    line = ser.readline().decode('utf-8').strip()

    if 'POS' in line:
        # Extract the part of the string after 'POS'
        pos_value_str = line.split('POS')[-1].strip()
        try:
            # Attempt to convert the following value to an integer
            pos_value = int(pos_value_str)
            print("POS Value:", pos_value)
        except ValueError:
            # Handle the case where conversion fails
            print("Could not convert POS value to an integer:", pos_value_str)
    elif(pos_value!=None):
        print('here')
        if "Angle" in line:
            parts = line.split(":")
            if len(parts) == 2:
                angle, value = parts
                angle = int(angle.split()[1])  # Extract angle number
                value = int(value.strip())  # Extract value
                if 1 <= angle <= (60*hor_multiplier):
                    print('angle',angle)
                    print('pos',pos_value)

                    image[int((pos_value-min_vert_angle)/increment)][angle]=value
                    print(f"Stored value {value} at angle {angle}")

folder_name = 'data'
folder_path = os.path.join(os.getcwd(), folder_name)
# The name of the file you want to save
next_number = find_highest_data_number(folder_path, obj_name) + 1
file_name = f"{obj_name}{next_number}.txt"
# The path to the folder


# Check if the folder exists, if not, create it
if not os.path.exists(folder_path):
    os.makedirs(folder_path)

# The full path to your file
file_path = os.path.join(folder_path, file_name)
with open(file_path, 'w') as file:
    for element in image:
        file.write(f"{element}\n")

import matplotlib.pyplot as plt
import numpy as np

# Example 2D matrix
# Replace 'matrix' with your actual 2D list from the serial data
# matrix = [[1, 2, 3, 4, 5],
#           [5, 4, 3, 2, 1],
#           [2, 3, 4, 5, 6],
#           [6, 5, 4, 3, 2],
#           [3, 4, 5, 6, 7]]

# Convert the matrix to a NumPy array for better handling by Matplotlib
array = np.array(image)

# Clip values above 1000 to 1000
clipped_array = np.clip(array, None, 1000)

# Optionally, normalize the clipped data to the range [0, 1] to enhance contrast
# Note: This step is useful if your data varies widely in scale
#normalized_array = (clipped_array - clipped_array.min()) / (clipped_array.max() - clipped_array.min())
# flipped_array = np.flipud(array)

flipped_array = np.flipud(clipped_array)

# Display the array as an image
plt.imshow(flipped_array, cmap='gray', interpolation='nearest')
plt.colorbar()  # Optional: adds a color bar to indicate the scale
plt.show()

# # Open or create the output file
# with open(output_file_path, 'w') as file:
#     while True:
#         try:
#             # Read a line from the serial port and decode it to UTF-8
#             line = ser.readline().decode('utf-8').strip()
#
#             # Check if the line contains valid data (customize as needed)
#             # This example assumes the data is formatted as "Angle: X Value: Y"
#             if line.startswith("Angle:"):
#                 # Write the line to the file, each entry on a new line
#                 file.write(line + '\n')
#                 file.flush()  # Ensure data is written to the file
#         except KeyboardInterrupt:
#             # Exit the loop if a keyboard interrupt is detected (Ctrl+C)
#             print("Exiting...")
#             break
#         except Exception as e:
#             # Handle other exceptions
#             print(f"Error: {e}")
#             continue

# Close the serial connection


ser.close()

# import serial
#
# # Parameters
# serial_port = '/dev/cu.usbmodem1101'  # Change this to your serial port name
# baud_rate = 115200  # Match this to your Arduino's baud rate
# output_file_path = 'output.txt'
# ser = serial.Serial(serial_port, baud_rate)
#
# # Open serial port
# max_vert_angle = 60
# min_vert_angle = 16
# vert_distance = max_vert_angle-min_vert_angle+1
#
# image = [[0]*61 for _ in range(vert_distance)]
# pos = None
#
# # print(image)
# pos = -1
# start = False
# while(pos<60)
#     line = ser.readline().decode('utf-8').strip()
#
#     if 'POS' in line:
#         # Extract the part of the string after 'POS'
#         pos_value_str = line.split('POS')[-1].strip()
#         try:
#             # Attempt to convert the following value to an integer
#             pos_value = int(pos_value_str)
#             print("POS Value:", pos_value)
#         except ValueError:
#             # Handle the case where conversion fails
#             print("Could not convert POS value to an integer:", pos_value_str)
#     elif(pos_value!=None):
#         print('here')
#         if "Angle" in line:
#             parts = line.split(":")
#             if len(parts) == 2:
#                 angle, value = parts
#                 angle = int(angle.split()[1])  # Extract angle number
#                 value = int(value.strip())  # Extract value
#                 if 1 <= angle <= vert_distance:
#                     print('angle',angle)
#                     print('pos',pos_value)
#
#                     image[pos_value-min_vert_angle][angle]=value
#                     print(f"Stored value {value} at angle {angle}")
#         # image[pos].append(line)
#
# with open('image.txt', 'w') as file:
#     for element in image:
#         file.write(f"{element}\n")
#
# import matplotlib.pyplot as plt
# import numpy as np
#
# # Example 2D matrix
# # Replace 'matrix' with your actual 2D list from the serial data
# # matrix = [[1, 2, 3, 4, 5],
# #           [5, 4, 3, 2, 1],
# #           [2, 3, 4, 5, 6],
# #           [6, 5, 4, 3, 2],
# #           [3, 4, 5, 6, 7]]
#
# # Convert the matrix to a NumPy array for better handling by Matplotlib
# array = np.array(image)
#
# # Display the array as an image
# plt.imshow(array, cmap='gray', interpolation='nearest')
# plt.colorbar()  # Optional: adds a color bar to indicate the scale
# plt.show()
#
# # # Open or create the output file
# # with open(output_file_path, 'w') as file:
# #     while True:
# #         try:
# #             # Read a line from the serial port and decode it to UTF-8
# #             line = ser.readline().decode('utf-8').strip()
# #
# #             # Check if the line contains valid data (customize as needed)
# #             # This example assumes the data is formatted as "Angle: X Value: Y"
# #             if line.startswith("Angle:"):
# #                 # Write the line to the file, each entry on a new line
# #                 file.write(line + '\n')
# #                 file.flush()  # Ensure data is written to the file
# #         except KeyboardInterrupt:
# #             # Exit the loop if a keyboard interrupt is detected (Ctrl+C)
# #             print("Exiting...")
# #             break
# #         except Exception as e:
# #             # Handle other exceptions
# #             print(f"Error: {e}")
# #             continue
#
# # Close the serial connection
# ser.close()
