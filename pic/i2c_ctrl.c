/*--------------------------------------------------------------------------------*/
// File: i2c_ctrl.c
//
// @brief I2C mater controller for PIC18F.
// @author Philip Wolfsberger
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* INCLUDES                                                                       */
/*--------------------------------------------------------------------------------*/
#include "i2c_ctrl.h"

// xc8 Library 
#include <i2c.h>

/*--------------------------------------------------------------------------------*/
/* DEFINES                                                                        */
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/
void i2c_open(void)
{
    OpenI2C(MASTER, SLEW_OFF);   
    SSPADD = 1; // SCL = 100 KHz
}

void i2c_write(uint8_t addr, const uint8_t* data, uint8_t length)
{
    IdleI2C();                         
    StartI2C();                        
    IdleI2C();                         
    WriteI2C(addr & 0xfe); 
    IdleI2C();          

    for (uint8_t i=0; i < length; i++) 
    {
        WriteI2C(data[i]);    
        IdleI2C();
    }

    StopI2C();  
}

void i2c_read(uint8_t addr, uint8_t* data, uint8_t length)
{
    IdleI2C();                         
    StartI2C();                        
    IdleI2C();                         
    WriteI2C(addr | 0x1); 
    IdleI2C();          

    for (uint8_t i=0; i < length; i++) 
    {
        data[i] = ReadI2C();    
        
        if (i == (length-1))
        {
            NotAckI2C();  
        }
        else
        {
            AckI2C();  
        }
    }

    StopI2C();  
}

void i2c_close(void)
{
    CloseI2C();
}


// vim:set sts=4 sw=4 tw=0 expandtab:
