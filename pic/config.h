/* 
 * File:   config.h
 * Author: Philip
 *
 * Created on November 19, 2014, 8:52 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

/** C O N F I G U R A T I O N   B I T S ******************************/

#pragma config OSC = INTIO67, FCMEN = OFF, IESO = OFF                       // CONFIG1H
#pragma config PWRT = OFF, BOREN = SBORDIS, BORV = 43                        // CONFIG2L
#pragma config WDT = OFF, WDTPS = 32768                                    // CONFIG2H
#pragma config MCLRE = OFF, LPT1OSC = OFF, PBADEN = ON, CCP2MX = PORTC       // CONFIG3H
#pragma config STVREN = ON, LVP = OFF, XINST = OFF			                // CONFIG4L
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF                   // CONFIG5L
#pragma config CPB = OFF, CPD = OFF                                         // CONFIG5H
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF                // CONFIG6L
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF                            // CONFIG6H
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF           // CONFIG7L
#pragma config EBTRB = OFF                                                  // CONFIG7H



#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */


