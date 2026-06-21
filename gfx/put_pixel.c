#include <stdio.h>
#include <string.h>
#include <gfx/gfx.h>
#include <mm/alloc.h>







void gfx_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    // uint32_t width = RAMFB_WIDTH, height = RAMFB_HEIGHT;


    // if (!ctx) {
    //     return;
    // } 

    // if (x > width || y > height) {
    //     return;
    // }

    // ctx->fb_base[y * width + x] = color;
}

void gfx_put_char(uint32_t x, uint32_t y, uint32_t fg, uint32_t bg, uint8_t *data) {
}