#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <u8g2_port.h>

#define OLED_PIN_SCL                    GET_PIN(D,14)       // PD14
#define OLED_PIN_SDA                    GET_PIN(C,8)        // PC8
#define OLED_PIN_RST                    GET_PIN(D,12)       // pd12
#define OLED_PIN_CS                     GET_PIN(D,11)       // PD11
#define OLED_PIN_DC                     GET_PIN(D,13)       // pd13

void Lcd_12864_Init_(u8g2_t * u8g2)
{
    
//    unsigned int l1 = OLED_PIN_SCL;
//    unsigned int l2 = OLED_PIN_SDA;
//    unsigned int l3 = OLED_PIN_RST;
//    unsigned int l4 = OLED_PIN_CS;
//    unsigned int l5 = OLED_PIN_DC;
    // Initialization
    u8g2_Setup_st7567_jlx12864_f( u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_rt_gpio_and_delay);
    
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_SPI_CLOCK, OLED_PIN_SCL);
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_SPI_DATA, OLED_PIN_SDA); 
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_RESET, OLED_PIN_RST); 
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_CS, OLED_PIN_CS);
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_DC, OLED_PIN_DC);
    
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);
    
    // Draw Graphics
    /* full buffer example, setup procedure ends in _f */
//    u8g2_ClearBuffer(u8g2);
//    u8g2_SetFont(u8g2, u8g2_font_baby_tf);
//    u8g2_DrawStr(u8g2, 1, 18, "U8g2 on RT-Thread");
//    u8g2_SendBuffer(u8g2);

//    u8g2_SetFont(u8g2, u8g2_font_unifont_t_symbols);
//    u8g2_DrawGlyph(u8g2, 112, 56, 0x2603 );
//    u8g2_SendBuffer(u8g2);
}

