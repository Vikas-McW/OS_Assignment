#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <math.h>
int main(void)
{
    int width, height, channel;

    unsigned char *image_data = stbi_load("my_image.png", &width, &height, &channel, 0);
    int total_pixels = width * height * channel;
    printf("%d", total_pixels);
    // printf("%u", image_data[0]);
    for (int i = 0; i < total_pixels; ++i)
    {
        // Perform image negation by subtracting the pixel value from 255 (for 8-bit images)
        image_data[i] = 255 - image_data[i];
        printf("%d\t", image_data[i]);
    }

    stbi_write_jpg("output_image2.jpg", width, height, channel, image_data, width * channel);

    stbi_image_free(image_data);
}