#pragma once

#include <common.h>

typedef struct {
    u8 lcdc; // control register
    u8 lcds; // status register
    u8 scroll_y; // scroll register FF42  
    u8 scroll_x; // FFFF43 specify the top left coordinates of the visible pixel area
    u8 ly; // FF44 lcd y coordinate current horizontal line
    u8 ly_compare; // compares values of the LYC and LY registers (FF45 and FF44 respectively) if identical, stat register is set and STAT interupt is requested
    u8 dma; // 
    u8 bg_palette; // background pallette 
    u8 obj_palette[2]; // sprite pallette 
    u8 win_y;
    u8 win_x;

    u32 bg_colors[4]; // pallette colors 
    u32 sp1_colors[4]; 
    u32 sp2_colors[4];
} lcd_context;

typedef enum {
    MODE_HBLANK,
    MODE_VBLANK,
    MODE_OAM,
    MODE_XFER
} lcd_mode; // this enumeration will help us know what mode we are in

lcd_context *lcd_get_context();


// each one of these lines below deals with a specific bit from the control register, which is 8 bits, u8 lcdc -> 00000000. Each one holds a value that tells us something
// bit 0 is bg enable, etc
#define LCDC_BGW_ENABLE (BIT(lcd_get_context()->lcdc, 0))
#define LCDC_OBJ_ENABLE (BIT(lcd_get_context()->lcdc, 1))
#define LCDC_OBJ_HEIGHT (BIT(lcd_get_context()->lcdc, 2) ? 16 : 8)
#define LCDC_BG_MAP_AREA (BIT(lcd_get_context()->lcdc, 3) ? 0x9C00 : 0x9800)
#define LCDC_BGW_DATA_AREA (BIT(lcd_get_context()->lcdc, 4) ? 0x8000 : 0x8800)
#define LCDC_WIN_ENABLE (BIT(lcd_get_context()->lcdc, 5))
#define LCDC_WIN_MAP_AREA (BIT(lcd_get_context()->lcdc, 6) ? 0x9C00 : 0x9800)
#define LCDC_LCD_ENABLE (BIT(lcd_get_context()->lcdc, 7))

// bits 0 and 1 for the lcds status register
#define LCDS_MODE ((lcd_mode)(lcd_get_context()->lcds & 0b11))
#define LCDS_MODE_SET(mode) { lcd_get_context()->lcds &= ~0b11; lcd_get_context()->lcds |= mode; }

//bit 2 of the status register
#define LCDS_LYC (BIT(lcd_get_context()->lcds, 2))
#define LCDS_LYC_SET(b) (BIT_SET(lcd_get_context()->lcds, 2, b))

// bits 3-6 of the status register
typedef enum {
    SS_HBLANK = (1 << 3),
    SS_VBLANK = (1 << 4),
    SS_OAM = (1 << 5),
    SS_LYC = (1 << 6),
} stat_src;

#define LCDS_STAT_INT(src) (lcd_get_context()->lcds & src)


void lcd_init();

u8 lcd_read(u16 address);
void lcd_write(u16 address, u8 value);
