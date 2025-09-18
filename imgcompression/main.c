#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t *bitmap;
int width, height;

typedef struct __attribute__((packed))
{
    uint32_t header_size;
    uint32_t image_width;
    uint32_t image_height;
    uint16_t color_planes;
    uint16_t bbp;
    uint32_t compression;
    uint32_t image_size;
} bitmap_info_header_t;

typedef struct __attribute__((packed))
{
    char sig[2];
    uint32_t fsz;
    uint32_t resv;
    uint32_t pixel_data_offset;
    bitmap_info_header_t info;
} bmp_header_t;

void read(char *fp)
{
    FILE *f = fopen(fp, "rb");
    if (!f)
    {
        perror("fopen");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *file_data = malloc(len);
    fread(file_data, 1, len, f);
    fclose(f);

    bmp_header_t *header = (bmp_header_t *)file_data;

    if (header->sig[0] != 'B' || header->sig[1] != 'M')
    {
        fprintf(stderr, "Not a BMP\n");
        exit(1);
    }
    if (header->info.bbp != 24)
    {
        fprintf(stderr, "Only 24-bit BMP supported\n");
        exit(1);
    }

    int w = header->info.image_width;
    int h = header->info.image_height;
    int row_stride = (w * 3 + 3) & ~3;

    uint8_t *pixel_data = malloc(w * h);
    uint8_t *file_pixels = (uint8_t *)file_data + header->pixel_data_offset;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int pixel_index = (y * row_stride) + (x * 3);
            uint8_t b = file_pixels[pixel_index];
            uint8_t g = file_pixels[pixel_index + 1];
            uint8_t r = file_pixels[pixel_index + 2];

            uint8_t brightness = (r + g + b) / 3;
            pixel_data[x + ((h - 1 - y) * w)] = brightness;
        }
    }

    bitmap = pixel_data;
    width = w;
    height = h;

    free(file_data);
}

uint8_t *scale_down(uint8_t *src, int src_w, int src_h, int dst_w, int dst_h)
{
    uint8_t *dst = malloc(dst_w * dst_h);
    float scale_x = (float)src_w / dst_w;
    float scale_y = (float)src_h / dst_h;

    for (int y = 0; y < dst_h; y++)
    {
        for (int x = 0; x < dst_w; x++)
        {
            int start_x = (int)(x * scale_x);
            int end_x = (int)((x + 1) * scale_x);
            int start_y = (int)(y * scale_y);
            int end_y = (int)((y + 1) * scale_y);

            if (end_x >= src_w)
                end_x = src_w - 1;
            if (end_y >= src_h)
                end_y = src_h - 1;

            int sum = 0, count = 0;
            for (int sy = start_y; sy <= end_y; sy++)
            {
                for (int sx = start_x; sx <= end_x; sx++)
                {
                    sum += src[sx + sy * src_w];
                    count++;
                }
            }

            dst[x + y * dst_w] = sum / count;
        }
    }
    return dst;
}

uint8_t *rle_data;
int num_rle = 0;
void rle()
{
    rle_data = malloc(128 * 64 * 2);
    int n = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width;)
        {
            int num = 1;
            int c = bitmap[x + (y * width)];
            // if (x < 10 || x > 54)
            {
                // num = 10;
                // x += 10;
            }
            // if (y < 10 || y > 38)
            {
                // num = width;
                // x += width;
            }
            // else
            {
                while (x < width)
                {
                    int diff = bitmap[x + (y * width)] - bitmap[(x + 1) + (y * width)];
                    diff = diff < 0 ? -diff : diff;
                    ++x;
                    if (diff < 34)
                    {
                        ++num;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            rle_data[n++] = (uint8_t)num;
            rle_data[n++] = (uint8_t)c;
        }
    }
    num_rle = n;
}

int main()
{
    read("img.bmp");
    uint8_t *scaled = scale_down(bitmap, width, height, 64, 48);
    free(bitmap);
    bitmap = scaled;
    width = 64;
    height = 48;

    rle();

    printf("#include<avr/pgmspace.h>\nconst uint8_t img[%d] = \n{\n", num_rle);
    for (int i = 0; i < num_rle; ++i)
    {
        printf("%d,\n", rle_data[i]);
    }
    printf("};\n");
}