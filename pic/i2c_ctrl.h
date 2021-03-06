/*--------------------------------------------------------------------------------*/
// File: i2c_ctrl.h
//
// @brief I2C mater controller for PIC18F header.
// @author Philip Wolfsberger
/*--------------------------------------------------------------------------------*/
#ifndef I2C_CTRL_H
#define I2C_CTRL_H

/*--------------------------------------------------------------------------------*/
/* INCLUDES                                                                       */
/*--------------------------------------------------------------------------------*/
#include <stdint.h>

/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/
void i2c_open(void);
void i2c_write(uint8_t addr, const uint8_t* data, uint8_t length);
void i2c_read(uint8_t addr, uint8_t* data, uint8_t length);
void i2c_close(void);

#endif /* I2C_CTRL_H */
