#include <msp430.h> 

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           //off watchdog timer

    P2DIR &= ~BIT2;                     //set P2.2 as input
    P2REN |= BIT2;                      //on suspender resistor for P2.2
    P2OUT |= BIT2;                      //set suspender resistor (high logic level)

    P1DIR |= BIT0;                      //set P1.0 as output
    P8DIR |= BIT1;                      //set P8.1 as output

    P1OUT &= ~BIT0;                     //off P1.0
    P8OUT |= BIT1;                      //on P8.1
    while(1)
    {
        if (!(P2IN & BIT2))             //button S2 is pressed (low logic level)
        {
            __delay_cycles(20000);      //delay for eliminating contact chatter
            if (!(P2IN & BIT2))         //button S2 is still pressed
            {

                P1OUT ^= BIT0;          //inverse P1.0
                P8OUT ^= BIT1;          //inverse P8.1

                while (!(P2IN & BIT2)); //button S2 is still pressed
                __delay_cycles(20000);  //delay for nothing
            }
        }
    }
}
