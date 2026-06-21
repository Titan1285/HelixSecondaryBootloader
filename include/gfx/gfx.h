#ifndef GFX_H
#define GFX_H

#include <stdint.h>






#define RAMFB_BASE      0x7FFF0000

#define RAMFB_WIDTH     1024
#define RAMFB_HEIGHT    768
#define RAMFB_BPP       4
#define RAMFB_STRIDE    (RAMFB_WIDTH * RAMFB_BPP)



typedef struct {
    uint32_t    *fb_base;
    uint32_t    width;
    uint32_t    height;
    uint32_t    bpp;
    uint32_t    stride;
    uint32_t    curr_x;
    uint32_t    curr_y;
} gfx_ctx_t;















void gfx_init(void);
void gfx_put_pixel(uint32_t x, uint32_t y, uint32_t color);

#endif