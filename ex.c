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
    // printf("%d", total_pixels);
    int j;
    for (j = 0; j < 10; j++)
    {
        printf("%d\n", image_data[j]);
    }
    unsigned char d[total_pixels];
    for (int i = 0; i < total_pixels; ++i)
    {

        d[i] = 255 - image_data[i];
    }
    printf("after negotion\n");
    for (j = 0; j < 10; j++)
    {
        printf("%d\n", d[j]);
    }

    stbi_write_jpg("output_image2.jpg", width, height, channel, d, width * channel);

    stbi_image_free(image_data);
}