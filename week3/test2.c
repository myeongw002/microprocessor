#include <mega128.h>
#include <delay.h>
/*
 * test.c
 *
 * Created: 2023-09-20 ���� 3:28:08
 * Author: user
 */


void main(void)
{
    DDRC = 0xFF;
    PORTC = 0x55; //PORTC 0x55 = 0b01010101 0x:16���� 0b:2����
    
    while(1){
        PORTC = PORTC^0xFF;
        
        delay_ms(500);
        }
}
