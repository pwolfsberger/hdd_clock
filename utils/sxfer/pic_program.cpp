/*--------------------------------------------------------------------------------*/
// File: pic_program.cpp
//
// @brief PIC mircoprocessor programming sequence
// @author Philip Wolfsberger
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* INCLUDES                                                                       */
/*--------------------------------------------------------------------------------*/
#include "pic_sxfer.h"

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

/* 
 * PIC18F2520 Programming Sequence
 * 1. Bulk erase
 * 2. Program memory
 * 3. Program IDs
 * 4. Program EEPROM
 * 5. Verify Program
 * 6. Verify IDs
 * 7. Verify EEPROM
 * 8. Program configuration bits
 * 9. Verify configurations bits
 */

/*--------------------------------------------------------------------------------*/
/* CONSTANTS                                                                      */
/*--------------------------------------------------------------------------------*/
/* OP codes */
static const uint8_t OP_CORE_INSTRUCT       = 0x0;
static const uint8_t OP_SHFT_TABLAT         = 0x2;
static const uint8_t OP_TBL_RD              = 0x8;
static const uint8_t OP_TBL_RD_POST_INC     = 0x9;
static const uint8_t OP_TBL_RD_POST_DEC     = 0xA;
static const uint8_t OP_TBL_RD_PRE_INC      = 0xB;
static const uint8_t OP_TBL_WR              = 0xC;
static const uint8_t OP_TBL_WR_POST_INC2    = 0xD;
static const uint8_t OP_TBL_WR_STRT_PGM_PI2 = 0xE;
static const uint8_t OP_TBL_WR_STRT_PGM     = 0xF;



/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/
void set_table_pointer(uint32_t addr)
{
    uint16_t data;

    //data = 0x9EA6; // EECON1, EEPGD clear
    //sxfer_write(OP_CORE_INSTRUCT, data);
    //data = 0x8CA6; // EECON1, CFGS set
    //sxfer_write(OP_CORE_INSTRUCT, data);
    //data = 0x94A6; // EECON1, WREN clear
    //sxfer_write(OP_CORE_INSTRUCT, data);
    //data = 0x82A6; // EECON1, RD set
    //sxfer_write(OP_CORE_INSTRUCT, data);

    
    // MOVLW
    data = (addr >> 16) & 0xFF;
    data |= 0x0E << 8;
    sxfer_write(OP_CORE_INSTRUCT, data);
    // MOVWF TBLPTRU
    data = 0x6EF8;
    sxfer_write(OP_CORE_INSTRUCT, data);
    // MOVLW
    data = (addr >> 8) & 0xFF;
    data |= 0x0E << 8;
    sxfer_write(OP_CORE_INSTRUCT, data);
    // MOVWF TBLPTRH
    data = 0x6EF7;
    sxfer_write(OP_CORE_INSTRUCT, data);
    // MOVLW
    data = addr & 0xFF;
    data |= 0x0E << 8;
    sxfer_write(OP_CORE_INSTRUCT, data);
    // MOVWF TBLPTRL
    data = 0x6EF6;
    sxfer_write(OP_CORE_INSTRUCT, data);
}


uint16_t read_device_id()
{
    /* 3FFFFE & 3FFFFF */
    set_table_pointer(0x3FFFFE);
   
    // XXX What is going on here???

    // Read DEVID1
    //uint8_t rdata = sxfer_read(OP_TBL_RD_POST_DEC);
    uint8_t rdata = sxfer_read(OP_TBL_RD);
    int16_t dev_id = ((uint16_t)rdata) & 0xFF;
    sxfer_write(OP_CORE_INSTRUCT, OP_TBL_RD_POST_INC);
    sxfer_write(OP_CORE_INSTRUCT, 0);
    sxfer_write(OP_CORE_INSTRUCT, 0);
    
    // Read DEVID2
    rdata = sxfer_read(OP_TBL_RD);
    dev_id |= (((uint16_t)rdata) << 8) & 0xFF00;
    return dev_id;

}



int main(int argc, char *argv[])
{

#if 0
    /* Check file name */
    if (argc < 2) {
        printf("Usage: pic_program <program binary>\n");
        return 0;
    }
    
    uint8_t* pfile = argv[1];

    ifstream file;
    file.open(pfile, ios::in|ios::binary);

    if (!file.is_open()) {
        printf("Error: failed to open file %s\n", pfile);
        return -1;
    }

#endif

    /* Open serial comm */
    if (sxfer_open() != 0) {
        printf("Error: failed to open sxfer!\n");
    }

    //Sleep(20);
    system("pause");
    sxfer_start();

    printf("Device ID = 0x%0X\n", read_device_id());

    sxfer_close();
    return 0;

}


// vim:set sts=4 sw=4 tw=0 expandtab:
