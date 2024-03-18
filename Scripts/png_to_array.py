from PIL import Image

def rgb_to_16bit(r, g, b):
    tmp = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3) 
    return (tmp >> 8) | ((tmp & 0x00FF) << 8) #inverting result

def png_to_16bit_color_codes(png_path):
    with Image.open(png_path) as img:
        img = img.convert('RGB')
        width, height = img.size
        
        with open(output_file_path, 'w') as file:
            file.write('#include "images.h"\n\n')
            file.write('const uint16_t logo[][128] = {\n')
            for y in range(height):
                file.write("\t{")
                for x in range(width):
                    r, g, b = img.getpixel((x, y))
                    value = rgb_to_16bit(r, g, b)
                    file.write(f'0x{value:04X}')
                    if x != width - 1:
                        file.write(", ")
                    elif y != height - 1:
                        file.write("},\n")
            file.write("}\n};")
                

# Example usage
png_path = '../img/logo.png'
output_file_path = '../Core/Src/images.c'
png_to_16bit_color_codes(png_path)

