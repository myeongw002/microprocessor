#include <mega128.h>

unsigned char led =0xFE;
unsigned char count = 0;

void main(void){
    DDRC = 0xFF;
    PORTC = led;
    
    TIMSK = 0x01;
    TCCR0 = 0x07;
    TCNT0 = 0x00;
    SREG = 0x80;
    
    while(1);   
}

interrupt [TIM0_OVF] void time_int0(void){
    count  = count + 1 ;
    if (count == 61) {
        count = 0 ;

    
        led = led << 1;
        led = led | 0x01;
        if (led == 0xFF) led = 0xFE;    
        PORTC = led;
    }
}    