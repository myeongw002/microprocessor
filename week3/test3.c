#include <mega128.h>
#include <delay.h>
/*
 * test.c
 *
 * Created: 2023-09-20 ¿ÀÈÄ 3:28:08
 * Author: user
 */


void main(void)
{
    int i;
    DDRC = 0xFF;

    while(1){
        PORTC = 0xFE; 
        delay_ms(500);
        for (i=0;i<7;i++){
            PORTC = PORTC<<1 | 0x01;
            delay_ms(500);
            }
        for (i=0;i<8;i++){
            PORTC = PORTC>>1| 0x80;
            delay_ms(500);
            }
        PORTC = 0x00;
        for (i=0;i<8;i++){
            PORTC = PORTC^0xFF; //xor operation
            delay_ms(500);
            }        
    }
}
