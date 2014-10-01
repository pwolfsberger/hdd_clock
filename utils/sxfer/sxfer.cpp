/* Start */

/*--------------------------------------------------------------------------------*/
/*INCLUDES                                                                        */
/*--------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
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
static HANDLE hComm; 
static HANDLE hMutex;

static bool tTerminate;
static bool ctsEvent;

/*--------------------------------------------------------------------------------*/
/* PROTOTYPES                                                                     */
/*--------------------------------------------------------------------------------*/
DWORD WINAPI cts_listener(LPVOID);
/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/

/**
 * @brief Open the COM (serial) port connection. 
 *
 */
int32_t open_comm() 
{
    hComm = CreateFile(COM_PORT,
                       GENERIC_WRITE,          
                       0,                  
                       NULL,               
                       OPEN_EXISTING,      
                       0,                  
                       NULL);

    if (hComm == INVALID_HANDLE_VALUE) 
    { 
        printf("Failed to open COM\n");
        return -1;
    }

    // Enable the event monitor on the CTS pin. 
    // This is used to recieve read-back data.
    SetCommMask(hComm, EV_CTS);

    return 0;
}

void close_comm() 
{
    CloseHandle(hComm);
}

void t_clock(bool high) 
{
    high ? EscapeCommFunction(hComm, SETRTS) : 
           EscapeCommFunction(hComm, CLRRTS);
            
}   

void t_data(bool high) 
{
    high ? EscapeCommFunction(hComm, SETDTR) :
           EscapeCommFunction(hComm, CLRDTR);
           
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


int32_t main() 
{
    HANDLE hThread;
    DWORD threadID;
    

    /* Create the serial connection */
    open_comm();
  
    /* Ensure clock and data lines start low */
    t_clock(LOW);
    t_data(LOW);
    
    /* Create a mutex with no initial owner */
    hMutex = CreateMutex(NULL, false, NULL);

    if (hMutex == NULL) {
        printf("CreateMutex error: %d\n", GetLastError());
        return 1;
    }

    tTerminate = false;

    /* Create worker thread */
#if 0
    hThread = CreateThread(NULL,       
                           0,          
                           (LPTHREAD_START_ROUTINE)cts_listener, 
                           NULL,       
                           0,          
                           &threadID); 

    if(hThread == NULL) {
        printf("CreateThread error: %d\n", GetLastError());
        return 1;
    }
#endif
    

    /* Test */
    printf("Starting test...\n");
    //send_op(1);
    //send_pl(0x1234);
    //send_op(2);
    //send_pl(0x5678);
    
    t_data(HIGH);
    DWORD evtMask;
    //WaitCommEvent(hComm, &evtMask, NULL);
    GetCommModemStatus(hComm, &evtMask);
    if (evtMask == MS_CTS_ON) { 
        printf("CTS HIGH\n");
    }
    Sleep(1);
 
    //WaitForSingleObject(hMutex, INFINITE);
    //printf("0000\n");
    //if (ctsEvent) {
    //    printf("CTS HIGH\n");
    //    printf("1\n");
    //    ctsEvent = false;
    //    printf("2\n");
    //}
    //if(!ReleaseMutex(hMutex)){
    //    printf("AHH!\n");
    //}

    //DWORD commErr;
    //COMSTAT commStat;
    //if (!ClearCommError(hComm, &commErr, &commStat)) {
    //    printf("com clear error %d\n", GetLastError());
    //}
    //printf("err: %d\n", commErr);
    
    printf("3\n");
    Sleep(1);
    t_data(LOW);
    printf("4\n");
    GetCommModemStatus(hComm, &evtMask);
    if (evtMask != MS_CTS_ON) { 
        printf("CTS LOW\n");
    }
    Sleep(10);
    printf("5\n");

    WaitForSingleObject(hMutex, INFINITE);
    printf("6\n");
    if (ctsEvent) {
        printf("CTS LOW");
        ctsEvent = false;
    }
    ReleaseMutex(hMutex);

    Sleep(1);
    t_data(LOW);
    Sleep(10);

    WaitForSingleObject(hMutex, INFINITE);
    if (ctsEvent) {
        printf("False CTS event!");
        ctsEvent = false;
    }
    ReleaseMutex(hMutex);
    
    printf("Finished test\n");

    close_comm();
    
    tTerminate = true;
    WaitForSingleObject(hThread, INFINITE);
    //CloseHandle(hThread);
    CloseHandle(hMutex);


    return 0;
}

DWORD WINAPI cts_listener(LPVOID lpParam)
{
    // lpParam not used
    UNREFERENCED_PARAMETER(lpParam);
    
    DWORD dwWaitResult;
    DWORD evtMask;

    while(1) {

        if (tTerminate){
            printf("termintate!\n");
            return 0;
        }

        /* Wait for an event on the CTS pin */
        WaitCommEvent(hComm, &evtMask, NULL); 

        if (evtMask != EV_CTS){
            printf("WTF! %d %d\n", evtMask, EV_CTS);
            continue;
        }

        /* Check exit condition */
        dwWaitResult = WaitForSingleObject(hMutex, INFINITE);
        
        if (dwWaitResult == WAIT_ABANDONED || dwWaitResult == WAIT_FAILED) {
            printf("Mutex error!\n");
            continue;
        }

        ctsEvent = true;

        if (!ReleaseMutex(hMutex)) {
            printf("AHH!\n");
        }
    }
}


// vim:set sts=4 sw=4 tw=0 expandtab:
