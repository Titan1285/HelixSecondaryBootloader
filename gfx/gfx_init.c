#include <stdio.h>
#include <mm/alloc.h>
#include <gfx/gfx.h>








gfx_ctx_t *gfx_ctx = NULL;










void gfx_init(void) {
    gfx_ctx = alloc(sizeof(gfx_ctx_t));

    if (!gfx_ctx) {
        printf("Failed to allocate GFX context!\n");
        return;
    }

    gfx_ctx->fb_base = (uint32_t *)RAMFB_BASE;
    gfx_ctx->width = RAMFB_WIDTH;
    gfx_ctx->height = RAMFB_HEIGHT;
    gfx_ctx->bpp = 4;
    gfx_ctx->stride = gfx_ctx->width * gfx_ctx->bpp;
    gfx_ctx->curr_x = 0;
    gfx_ctx->curr_y = 0;
}