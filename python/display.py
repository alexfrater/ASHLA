import matplotlib.pyplot as plt
import numpy as np
from scipy.ndimage import gaussian_filter,uniform_filter
# import cv2



# Step 1: Load the data from 'image.txt'
def load_data_from_file(file_path):
    with open(file_path, 'r') as file:
        # Read lines, strip leading and trailing whitespace, and remove brackets
        lines = [line.strip().strip('[]') for line in file.readlines()]
        # Split each line on commas and convert to integers
        data = [list(map(int, line.split(','))) for line in lines if line]
    return np.array(data)

# Adjust the path if necessary
file_path = 'image.txt'
image = load_data_from_file(file_path)



array = np.array(image)

# Clip values above 1000 to 1000
array = np.clip(array, None, 2000)

# Optionally, normalize the clipped data to the range [0, 1] to enhance contrast
# Note: This step is useful if your data varies widely in scale
normalized_array = (array - array.min()) / (array.max() - array.min())
# flipped_array = np.flipud(array)

array = np.flipud(normalized_array)
# array = gaussian_filter(array, sigma=0.5)
# array = uniform_filter(array, size=2)

# Display the array as an image
plt.imshow(array, cmap='gray', interpolation='nearest')
plt.colorbar()  # Optional: adds a color bar to indicate the scale
plt.show()


