from PIL import Image
import os
import sys
import random

def generate_random_image(raw_filename, width, height):
    random_data = random.randbytes(width * height)
    with open(raw_filename, 'wb') as file:
            file.write(random_data)

def raw_to_png(raw_filename, png_filename, width, height):
	
    with open(raw_filename, 'rb') as file:
        raw_data = file.read()

    width = int(width)
    height = int(height)

    # Create a new Image object with 'L' mode for grayscale
    print(raw_filename)
    img = Image.frombytes('L', (width, height), raw_data)

    img.save(png_filename, format='PNG')

def convert_to_raw(original_filename, raw_filename):
    img = Image.open(original_filename)

    width, height = img.size

    # Convert the image to grayscale if it's not already
    img = img.convert('L')  # 'L' mode represents grayscale images
    
    raw_data = img.tobytes()

    raw_filename = raw_filename + str(width) + "x" + str(height) + ".raw"

    with open(raw_filename, 'wb') as file:
        file.write(raw_data)

if __name__ == "__main__":
    any_input_path = "./data/any_input/"
    raw_input_path = "./data/raw_input/"
    
    any_output_path = "./data/any_output/"
    raw_output_path = "./data/raw_output/"

    if sys.argv[1] == "raw":
        count = 1
        for filename in os.listdir(any_input_path):
            number_str = ""
            if(count < 10):
                number_str = "0" + str(count)
            else:
                number_str = str(count)

            raw_filename = number_str + "_image_"
            count += 1
            convert_to_raw(any_input_path + filename, raw_input_path + raw_filename)
    elif sys.argv[1] == "png":
        for filename in os.listdir(raw_output_path):
            raw_filename = raw_output_path + filename
            png_filename = any_output_path + filename.replace(".raw", ".png")

            width, height = filename.replace(".raw", "").split("_")[2].split("x")
            raw_to_png(raw_filename, png_filename, width, height)
    elif sys.argv[1] == "random":
        width = sys.argv[2]
        height = sys.argv[3]
        raw_filename = raw_input_path + "_random_%sx%s.raw" % (width, height)

        generate_random_image(raw_filename, int(width), int(height))