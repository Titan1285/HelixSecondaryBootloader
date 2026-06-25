#include <stdio.h>
#include <gfx/gfx.h>











void gfx_draw_rect(uint32_t pitch, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t bpp, uint32_t color) {
    uint32_t bytes_per_pixel = bpp / 8;
    
    
    if (w > gfx_ctx->width || h > gfx_ctx->height) {
        return;
    }

    for (uint32_t row = y; row < y + h; row++) {
        for (uint32_t col = x; col < x + w; col++) {
            uint32_t offset = (row * pitch) + (col * bytes_per_pixel);
            uint32_t *ptr = (uint32_t *)gfx_ctx->fb_base + offset;

            switch(bpp) {
                case 16:
                    *((uint16_t *)ptr) = (uint16_t)color;
                    break;

                case 24:
                    *((uint16_t *)ptr) = color;
                    break;

                case 32:
                    ptr[0] = (color >> 16) & 0xFF;  // R
                    ptr[1] = (color >> 8) & 0xFF;   // G
                    ptr[2] = color & 0xFF;          // B
                    break;
            }
        }
    }
}