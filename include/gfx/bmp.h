#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stddef.h>







#define BMP_BPP_MONOCHROME      1
#define BMP_BPP_4BIT            4
#define BMP_BPP_8BIT            8
#define BMP_BPP_16BIT           16
#define BMP_BPP_24BIT           24

#define BMP_COMPRESSION_RGB     0
#define BMP_COMPRESSION_RLE8    1
#define BMP_COMPRESSION_RLE4    2






typedef struct {
    uint8_t     signature[2];   // 'BM'
    uint32_t    file_size;      // File size in bytes
    uint32_t    reserved;       // Unused
    uint32_t    data_offset;    // Offset to Raster Data   
} __attribute__((packed)) bmp_header_t;

typedef struct {
    uint32_t    size;               // Size of bmp_info_header_t
    uint32_t    width;              // Bitmap width
    uint32_t    height;             // Bitmap height
    uint16_t    planes;             // Number of planes
    uint16_t    bpp;                // Bites per pixel
    uint32_t    compression;        // Type of compression
    uint32_t    size_of_image;      // Size of image
    uint32_t    x_pixels_per_meter; // Horizontal resolution
    uint32_t    y_pixels_per_meter; // Vertical resolution
    uint32_t    colors_used;        // Number of actual colors used
    uint32_t    important_colors;   // Number of important colors (0 = all)
    // 0x36+ is the color table (only present if BPP <= 8)
} __attribute__((packed)) bmp_info_header_t;







void bmp_display(void *buf);

#endif