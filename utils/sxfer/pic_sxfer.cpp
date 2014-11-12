/*--------------------------------------------------------------------------------*/
// File: pic_sxfer.cpp
//
// @brief Serial transfer functions for PIC mircoprocessor programming.
// @author Philip Wolfsberger
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* INCLUDES                                                                       */
/*--------------------------------------------------------------------------------*/
#include "pic_sxfer.h"

#include <stdio.h>
#include <windows.h>

/*--------------------------------------------------------------------------------*/
/* DEFINES                                                                        */
/*--------------------------------------------------------------------------------*/
#define COM_PORT "COM3"

/*--------------------------------------------------------------------------------*/
/* CONSTANTS                                                                      */
/*--------------------------------------------------------------------------------*/
static const uint32_t OP_LENGTH = 4;
static const uint32_t WR_LENGTH = 16;
static const uint32_t RD_LENGTH = 8;
#ifndef INVERT_LOGIC
static const bool HIGH = true;
static const bool LOW  = false;
#else
static const bool HIGH = false;
static const bool LOW  = true;
#endif

/*--------------------------------------------------------------------------------*/
/* GLOBALS                                                                        */
/*--------------------------------------------------------------------------------*/
static HANDLE hComm; 

/*--------------------------------------------------------------------------------*/
/* PROTOTYPES                                                                     */
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/

/*-------------------*/
/* PRIAVTE FUNCTIONS */
/*-------------------*/
static inline void wait()
{
    // Kill cycles
    for (uint32_t i=0; i<10000; i++)
    {
    }
}

static inline void set_clock(bool high) 
{
    high ? EscapeCommFunction(hComm, SETRTS) : 
           EscapeCommFunction(hComm, CLRRTS);
    wait();
}   

static inline void set_data(bool high) 
{
    high ? EscapeCommFunction(hComm, SETDTR) : 
           EscapeCommFunction(hComm, CLRDTR);
    wait();
}

static bool get_data()
{
    DWORD evtMask;
    Sleep(5);
    GetCommModemStatus(hComm, &evtMask);
    return ((evtMask & MS_CTS_ON) == 0) ? LOW : HIGH;
}


/*------------------*/
/* PUBLIC FUNCTIONS */
/*------------------*/
    
void sxfer_write(uint8_t op, uint16_t data)
{
    bool bit;

    for (uint32_t i=0; i < OP_LENGTH; i++) {
        bit = (bool)((op >> i) & 0x1);
        set_clock(HIGH);
        set_data(bit);
        set_clock(LOW);
    }

    set_data(LOW);
    // Wait P5 (40 ns)
    
    for (uint32_t i=0; i < WR_LENGTH; i++) {
        bit = (bool)((data >> i) & 0x1);
        set_clock(HIGH);
        set_data(bit);
        set_clock(LOW);
    }


    set_data(LOW);
    // Wait P5A (40 ns)
}

uint8_t sxfer_read(uint8_t op)
{
    bool bit;
    uint8_t data = 0;

    for (uint32_t i=0; i < OP_LENGTH; i++) {
        bit = (bool)((op >> i) & 0x1);
        set_clock(HIGH);
        set_data(bit);
        set_clock(LOW);
    }
    
    set_data(LOW);
    // Wait P5 (40 ns)
    
    // Burn 8 cycles
    for (uint32_t i=0; i < RD_LENGTH; i++) {
        set_clock(HIGH);
        set_clock(LOW);
    }
    
    //set_data(HIGH);
    
    // Wait P6 (20 ns)
    for (uint32_t i=0; i < RD_LENGTH; i++) {
        set_clock(HIGH);
        set_clock(LOW);
        bit = get_data();
        data |= (bit << i);
    }


    set_data(LOW);
    // Sleep P5A (40 ns)
    
    return data;
}


/**
 * @brief Open the COM (serial) port connection. 
 *
 */
int32_t sxfer_open() 
{
    hComm = CreateFile(COM_PORT,
                       GENERIC_WRITE,          
                       0,                  
                       NULL,               
                       OPEN_EXISTING,      
                       0, //FILE_FLAG_OVERLAPPED,                  
                       NULL);

    if (hComm == INVALID_HANDLE_VALUE) 
    { 
        printf("Failed to open COM\n");
        return -1;
    }

    // Enable the event monitor on the CTS pin. 
    // This is used to recieve read-back data.
    //SetCommMask(hComm, EV_CTS);
   
    /* Ensure clock and data lines start low */
    set_clock(LOW);
    set_data(LOW);

    return 0;
}

void sxfer_start() 
{
    SetCommBreak(hComm);
}

void sxfer_close() 
{
    ClearCommBreak(hComm);
    CloseHandle(hComm);
}


// vim:set sts=4 sw=4 tw=0 expandtab:
