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

/*--------------------------------------------------------------------------------*/
/* DEFINES                                                                        */
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/* GLOBALS                                                                        */
/*--------------------------------------------------------------------------------*/
uint8_t motor_ctrl = 0; // Motor control bits

/*--------------------------------------------------------------------------------*/
/* FUNCTIONS                                                                      */
/*--------------------------------------------------------------------------------*/



void interrupt isr(void)
{
     // Check for Timer0 Interrupt
    if  (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;          // clear (reset) flag
		
        LATA = motor_ctrl;

        /* Implelemt motor_ctrl as a 3-bit roll register */
        motor_ctrl <<= 1;
        motor_ctrl = (motor_ctrl % 8) + (motor_ctrl / 8);
    }

}

void timer_init(void)
{
    // Set up Interrupts for timer
    INTCONbits.TMR0IF = 0;          // clear roll-over interrupt flag
    INTCON2bits.TMR0IP = 0;         // Timer0 is low priority interrupt
    INTCONbits.TMR0IE = 1;          // enable the Timer0 interrupt.
    // Set up timer itself
    T0CON = 0b01001000;             // Bit timer w/ prescale bypass. ~1 ms 
    TMR0H = 0;                      // clear timer - always write upper byte first
    TMR0L = 0;
    T0CONbits.TMR0ON = 1;           // start timer
}

void main (void)
{

	//TRISC = 0b11011111; 	// PORTC bit 5 to output (0)
	TRISA = 0b11111000; 	// PORTA bits 0-2 to output

    i2c_open();
    lcd_open();
    lcd_write("Init...", 7);// Test LCD

    timer_init();           // Setup Timer for motor control
    motor_ctrl = 1;
    ei();                   // Enable interrupts    


	while (1)
	{
		//LATCbits.LATC5 = ~LATCbits.LATC5; // toggle LATC

        // TODO menu loop
		Delay1KTCYx(50);	// Delay 50 x 1000 = 50,000 cycles; 200ms @ 1MHz
        
	}

}




// vim:set sts=4 sw=4 tw=0 expandtab:

