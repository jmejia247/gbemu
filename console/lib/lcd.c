#include <lcd.h>
#include <ppu.h>
#include <dma.h>

static lcd_context ctx;

static unsigned long colors_default[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

void lcd_init() {
    printf("LCD INIT...\n");
    // zero out initial values for lcd 
    ctx.lcdc = 0x91;
    ctx.scroll_x = 0;
    ctx.scroll_y = 0;
    ctx.ly = 0;
    ctx.ly_compare = 0;
    ctx.bg_palette = 0xFC;
    ctx.obj_palette[0] = 0xFF;
    ctx.obj_palette[1] = 0xFF;
    ctx.win_y = 0;
    ctx.win_x = 0;

    for (int i = 0; i < 4; i++) {
        ctx.bg_colors[i] = colors_default[i];
        ctx.sp1_colors[i] = colors_default[i];
        ctx.sp2_colors[i] = colors_default[i];
    }
}

lcd_context *lcd_get_context() {
    // return address of out ctx variable
    return &ctx; 
}

u8 lcd_read(u16 address) {
    u8 offset = (address - 0xFF40);
    // convert ctx into byte array
    u8 *p = (u8 *)&ctx;

    // return byte array at that offset
    return p[offset];
}

void update_palette(u8 palette_data, u8 pal) {
    u32 *p_colors = ctx.bg_colors;

    switch (pal) {
        case 1:
            p_colors = ctx.sp1_colors;
            break;

        case 2:
            p_colors = ctx.sp2_colors;
            break;
    }

    // for each color we are updating 2 bits with the bottom two bits, so we grab them and then shift over by 2 bits
    p_colors[0] = colors_default[palette_data & 0b11];
    p_colors[1] = colors_default[(palette_data >> 2) & 0b11];
    p_colors[2] = colors_default[(palette_data >> 4) & 0b11];
    p_colors[3] = colors_default[(palette_data >> 6) & 0b11];
}

void lcd_write(u16 address, u8 value) {
    u8 offset = (address - 0xFF40);
    u8 *p = (u8 *)&ctx;
    p[offset] = value;

    // IF our offset it 6, we are at dma
    if (offset == 6) {
        dma_start(value);
    }

    // if we are in the range of the palette
    if (address == 0xFF47) {
        update_palette(value, 0);
    } else if (address == 0xFF48) {
        // 0b11111100 is to remove last two bits because there is no white color for sprites, its transparent, 1 tells update palate this is a different case 
        update_palette(value & 0b11111100, 1);
    } else if (address == 0xFF49) {
        // should this one be case 2?
        update_palette(value & 0b11111100, 2);
    }
}