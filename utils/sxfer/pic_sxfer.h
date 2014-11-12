/*--------------------------------------------------------------------------------*/
// File: pic_sxfer.h
//
// @brief PIC serial transfer function definitions.
// @author Philip Wolfsberger
/*--------------------------------------------------------------------------------*/
#ifndef PIC_SXFER_H
#define PIC_SXFER_H

#include <stdint.h>

/*--------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                           */
/*--------------------------------------------------------------------------------*/
void sxfer_write(uint8_t op, uint16_t data);
uint8_t sxfer_read(uint8_t op);
int32_t sxfer_open();
void sxfer_start();
void sxfer_close(); 



#endif /*PIC_SXFER_H*/
// vim:set sts=4 sw=4 tw=0 expandtab:
