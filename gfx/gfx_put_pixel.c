#include <stdio.h>
#include <string.h>
#include <gfx/gfx.h>
#include <mm/alloc.h>







void gfx_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    
    if (x > gfx_ctx->width || y > gfx_ctx->height) {
        return;
    }

    gfx_ctx->fb_base[y * gfx_ctx->width + x] = color;
}