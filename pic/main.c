/*--------------------------------------------------------------------------------*/
// File: main.c
//
// @brief Project main.
// @author Philip Wolfsberger
/*--------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------*/
/* INCLUDES                                                                       */
/*--------------------------------------------------------------------------------*/
#include "config.h"

#include "i2c_ctrl.h"
#include "lcd_display.h"

// xc8 Library 
#include <delays.h>
#include <stdint.h>
#include <stdio.h>

/*--------------------------------------------------------------------------------*/
/* DEFINES                                                                        */
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/* GLOBALS                                                                        */
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/* PROTOTYPES                                                                     */
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/

void timer_init(void)
{
    // Set up Interrupts for timer
    INTCONbits.TMR0IF = 0;          // clear roll-over interrupt flag
    INTCON2bits.TMR0IP = 0;         // Timer0 is low priority interrupt
    INTCONbits.TMR0IE = 1;          // enable the Timer0 interrupt.
    // Set up timer itself
    T0CON = 0b01001000;             // Bit timer w/ prescale bypass. ~1 ms 
    TMR0H = 0;                      // clear timer - always write upper byte first
    TMR0L = 0x80;                   // Preload counter. Focs/4 = 250 KHz | 1/Focs * 128 = 0.5 ms timer 
    T0CONbits.TMR0ON = 1;           // start timer
}


void motor_control(void)
{
    //static const uint8_t MOTOR_STATE[] = {1, 1, 3, 2, 2, 6, 4, 4, 5}; 
    static const uint8_t MOTOR_STATE[] = {1, 1, 1, 2, 2, 2, 4, 4, 4}; 

    static uint8_t motor_state_select = 0; 
    static uint8_t motor_started = 0; 
    
    static uint8_t motor_wait_time  = 60; // 60 ticks per step ~= 3.7 rps start rotation  
    static uint8_t motor_wait_cnt   = 0;   

    static const uint16_t TICKS_PER_SECOND = 2000; // Each timer tick (interrupt) is 500 us
    static uint16_t tick_cnt = 0;   

    //static uint8_t acc_factor = 10;   
    //static uint16_t acc_interval = 200; //TICKS_PER_SECOND / acc_factor;   
    //static uint16_t acc_cnt = 0;   

    if (motor_started == 0) 
    {
        LATA = ~MOTOR_STATE[0];
        Delay1KTCYx(200);	  // Delay 1s
        motor_started = 1; // Set motor started flag
    }
    else
    {
        // Advance the motor state at calculated wait
        // intervals to achive a target motor RPS.
        if (motor_wait_cnt >= motor_wait_time)
        {
            motor_state_select++;
            //motor_state_select %= 6;
            motor_state_select %= 9;
            LATA = ~MOTOR_STATE[motor_state_select];
            motor_wait_cnt = 0;
        }
        else
        {
            motor_wait_cnt++;
        }
/*
        if ((acc_cnt >= acc_interval) && (motor_wait_time > 5) && (motor_wait_cnt < motor_wait_time))
        {
            motor_wait_time--;
            acc_cnt = 0;
        } 
        else
        {
            acc_cnt++;
        }

        // Acceleration stages are broken up into 
        // 1 second intervals. 
        if ((tick_cnt >= TICKS_PER_SECOND) && (acc_factor > 1)) 
        {
            
		//LATAbits.LATA3 = ~LATAbits.LATA3;
            tick_cnt = 0;
            acc_cnt = 0;
            acc_factor--;
            acc_interval = TICKS_PER_SECOND / acc_factor;
            //uint8_t buf[10];
            //uint8_t n = sprintf(buf, "cnt=%d %d", motor_wait_time, acc_factor);
            //lcd_write(buf, n);
        }
        else
        {
            tick_cnt++;
        }
*/
        if ((tick_cnt >= (TICKS_PER_SECOND/2)) && (motor_wait_time > 4))
        {
            tick_cnt = 0;
            motor_wait_time--;
        }
        else
        {
            tick_cnt++;
        }

    }
}

void main (void)
{

	//TRISC = 0b11011111; 	// PORTC bit 5 to output (0)
    //TRISA = 0b11111000; 	// PORTA bits 0-2 to output
    TRISA = 0b11110000; 	// PORTA bits 0-3 to output

    i2c_open();
    lcd_open();
    lcd_write("Init...", 7);// Test LCD

    timer_init();           // Setup Timer for motor control
    ei();                   // Enable interrupts    


	while (1)
	{
		//LATCbits.LATC5 = ~LATCbits.LATC5; // toggle LATC

        // TODO menu loop
	    Delay1KTCYx(50);	// Delay 50 x 1000 = 50,000 cycles; 200ms @ 1MHz
        
        
	}

}

/*--------------------------------------------------------------------------------*/
/* INTERRUPTS                                                                     */
/*--------------------------------------------------------------------------------*/
void interrupt isr(void)
{
     // Check for Timer0 Interrupt
    if  (INTCONbits.TMR0IF)
    {
        TMR0L = 0x80;           // Preload counter. Focs/4 = 250 KHz | 1/Focs * 128 = 0.5 ms timer 
        INTCONbits.TMR0IF = 0;  // clear (reset) flag
        motor_control();
	}
}


// vim:set sts=4 sw=4 tw=0 expandtab:

