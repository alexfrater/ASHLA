import os
import re
prefix = 'bottle'
def find_highest_cap_number(folder_path, prefix):
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

    return highest_number if highest_number != -1 else None

# Specify the path to your folder
folder_path = 'data'
highest_cap_number = find_highest_cap_number(folder_path, prefix)
print(f"The highest number following 'cap' in the .txt files is: {highest_cap_number}")

