/*--------------------------------------------------------------------------------*/
// File: lcd_display.c
//
// @brief NHD LCD display driver.
// @author Philip Wolfsberger
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* INCLUDES                                                                       */
/*--------------------------------------------------------------------------------*/
#include "i2c_ctrl.h"
#include "lcd_display.h"

// xc8 Library 
#include <stdint.h>
#include <delays.h>
/*--------------------------------------------------------------------------------*/
/* DEFINES                                                                        */
/*--------------------------------------------------------------------------------*/
#define LCD_I2C_ADDR          0x50
#define LCD_CHAR_WIDTH        16
#define LCD_CHAR_HEIGTH       2

#define LCD_CMD_PREFIX        0xFE
/* LCD COMMANDS */
#define LCD_DISPLAY_ON        0x41
#define LCD_DISPLAY_OFF       0x42
#define LCD_CURSOR_SET        0x45 // + pos
#define LCD_CURSOR_HOME       0x46
#define LCD_CURSOR_ULINE_ON   0x47
#define LCD_CURSOR_ULINE_OFF  0x48
#define LCD_CURSOR_LEFT       0x49
#define LCD_CURSOR_RIGHT      0x4A
#define LCD_CURSOR_BLINK_ON   0x4B
#define LCD_CURSOR_BLINK_OFF  0x4C
#define LCD_CLEAR_SCREEN      0x51

/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/

/**
 * @brief Write LCD commands with parameters
 */
static void lcd_wcmdp(uint8_t cmd,          // LCD Command Code
                      const uint8_t* param, // Command parameters 
                      uint8_t length)       // parameter length
{

    /* Longest parameter length is 9 */
    if (length > 9)
    {
        // Error
        return;
    }

    uint8_t wdata[11];
    
    wdata[0] = LCD_CMD_PREFIX;
    wdata[1] = cmd;
    
    for (uint8_t i=0; i < length; i++) 
    {
        wdata[2+i] = param[i];
    }

    i2c_write(LCD_I2C_ADDR, &wdata, length+2);
    
    // Most commands take atleast 100 us to complete
    Delay1TCYx(25);
}

/**
 * @brief Write LCD commands without parameters
 */
static void lcd_wcmd(uint8_t cmd)
{
    lcd_wcmdp(cmd, 0, 0);
}

/**
 * @brief Write an ASCII character to the display
 */
static void lcd_wchar(uint8_t c)
{
    if ((c >= 0x20) && (c <= 0x7F)) {
        i2c_write(LCD_I2C_ADDR, &c, 1);
        Delay1TCYx(25);
    }
}

    
/**
 * @brief Initialize the display
 */   
void lcd_open(void)
{
    lcd_wcmd(LCD_DISPLAY_ON);
    lcd_wcmd(LCD_CURSOR_HOME);
    lcd_wcmd(LCD_CURSOR_ULINE_ON);
    lcd_wcmd(LCD_CURSOR_BLINK_ON);
}
    
/**
 * @brief Write a text string to the display.
 *
 * TODO: Push previous text string down one line first
 * TODO: Scroll text when length is longer the display width
 */
void lcd_write(const uint8_t* text, uint8_t length)
{
    /* For now check that the text string lenght is less than
     * or equal to the display width. If not, truncate the text. 
     */
    // TODO Implement scrolling text for strings that are too long.
    if (length > LCD_CHAR_WIDTH) {
        length = LCD_CHAR_WIDTH;
    }

    // XXX
    lcd_wcmd(LCD_CLEAR_SCREEN);
    Delay10TCYx(38);
    
    for (uint8_t i=0; i < length; i++) 
    {
        lcd_wchar(text[i]);
    }

}

/**
 * @brief Clear the display and turn it off.
 */
void lcd_close(void)
{
    lcd_wcmd(LCD_CLEAR_SCREEN);
    Delay10TCYx(38);
    lcd_wcmd(LCD_DISPLAY_OFF);
}


// vim:set sts=4 sw=4 tw=0 expandtab:
