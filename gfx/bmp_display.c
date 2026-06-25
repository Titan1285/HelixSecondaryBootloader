#include <stdio.h>
#include <string.h>
#include <mm/alloc.h>
#include <gfx/gfx.h>
#include <gfx/bmp.h>













void bmp_display(void *buf) {
    bmp_header_t *hdr = NULL;
    bmp_info_header_t *info_hdr = NULL;
    uint32_t w = 0, h = 0;
    uint32_t row_stride = 0;
    uint8_t *pixel_data = NULL;





    if (!buf) {
        return;
    }

    hdr = (bmp_header_t *)buf;
    info_hdr = (bmp_info_header_t *)(buf + sizeof(bmp_header_t));    

    printf("BMP information:\n");
    printf("Image signature:    %c%c\n", hdr->signature[0], hdr->signature[1]);
    printf("Image size:         %lu\n", hdr->file_size);
    printf("Image demensions:   %lux%lu\n", info_hdr->width, info_hdr->height);
    printf("Image BPP:          %lu\n", info_hdr->bpp);

    w = info_hdr->width;
    h = info_hdr->height;
    row_stride = (w * 3 + 3) & ~3;

    pixel_data = (uint8_t *)(buf + hdr->data_offset);

    // TODO: Render this in seperate buffer, then display as needed. Also shrink use of partition size for faster image loading
    // due to the fact it's PIO reading
    for (uint32_t y = 0; y < h; y++) {
        uint8_t *row = pixel_data + ((h - 1 - y) * row_stride);

        for (uint32_t x = 0; x < w; x++) {
            uint32_t idx = x * 3;
            uint8_t b = row[idx];
            uint8_t g = row[idx + 1];
            uint8_t r = row[idx + 2];

            uint32_t color = (r << 16) | (g << 8) | b;

            // TODO: Change this to be properly used...?
            uint32_t draw_x = gfx_ctx->curr_x + x;
            uint32_t draw_y = gfx_ctx->curr_y + y;

            gfx_put_pixel(draw_x, draw_y, color);
        }
    }
}