#include <mega128.h>

unsigned char led =0xFE;

void main(void){
    DDRC = 0xFF;
    PORTC = led;
    
    TIMSK = 0b00000010;
    TCCR0 = 0b00001111;
    OCR0 = 155;
    SREG = 0x80;
    
    while(1);   
}

interrupt [TIM0_OVF] void time_int0(void){
    led = led << 1;
    led = led | 0x01;
    if (led == 0xFF) led = 0xFE;    
    PORTC = led;
}    