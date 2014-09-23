/* Start */

/*--------------------------------------------------------------------------------*/
/*INCLUDES                                                                        */
/*--------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
//#include <comdef.h>
#include <windows.h>

/*--------------------------------------------------------------------------------*/
/* DEFINES                                                                        */
/*--------------------------------------------------------------------------------*/
#define COM_PORT "COM3"

#define OP_LENGTH 4
#define PL_LENGTH 16
#define HIGH 1
#define LOW  0
/*--------------------------------------------------------------------------------*/
/* GLOBALS                                                                        */
/*--------------------------------------------------------------------------------*/
static HANDLE hFile; 


/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/

/**
 * @brief Open the COM (serial) port connection. 
 *
 */
int32_t open_com() 
{
    hFile = CreateFile(COM_PORT,
                       GENERIC_WRITE,          
                       0,                  
                       NULL,               
                       OPEN_EXISTING,      
                       0,                  
                       NULL);

    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        printf("Failed to open COM\n");
        return -1;
    }

    return 0;
}

void close_com() 
{
    CloseHandle(hFile);
}

void t_clock(bool high) 
{
    high ? EscapeCommFunction(hFile, SETRTS) : 
           EscapeCommFunction(hFile, CLRRTS);
            
}   

void t_data(bool high) 
{
    high ? EscapeCommFunction(hFile, SETDTR) :
           EscapeCommFunction(hFile, CLRDTR);
           
}

void send_op(uint8_t op)
{
    bool op_bit;

    for (uint32_t i=0; i < OP_LENGTH; i++) {
        t_clock(HIGH);
        op_bit = (bool)((op >> i) & 0x1);
        t_data(op_bit);
        t_clock(LOW);
    }

    // Sleep P5 (40 ns)
    Sleep(1);
    t_data(LOW);
}
    
    
void send_pl(uint16_t pl)
{
    bool pl_bit;
    for (uint32_t i=0; i < PL_LENGTH; i++) {
        t_clock(HIGH);
        pl_bit = (bool)((pl >> i) & 0x1);
        t_data(pl_bit);
        t_clock(LOW);
    }

    // Sleep P5 (40 ns)
    Sleep(1);
    t_data(LOW);
}


int32_t main () 
{
    
    open_com();

    /* Ensure clock and data lines start low */
    t_clock(LOW);
    t_data(LOW);


    /* Test */
    printf("Starting test...\n");
    send_op(1);
    send_pl(0x1234);
    send_op(2);
    send_pl(0x5678);
    printf("Finished test\n");

    close_com();

    return 0;
}

// vim:set sts=4 sw=4 tw=0 expandtab:
