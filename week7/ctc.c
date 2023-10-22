/* CTC 모드 1초마다 8개 led가 toggle */

#include <mega128.h>


unsigned char led =0xFF;
int count;



void main(void){
    DDRC = 0xFF;
    PORTC = led;
        
    TIMSK = 0b00000010;
    TCCR0 = 0b00001111;
    OCR0 = 255;
    SREG = 0x80;
    
    while (1){
    }  
}

 


interrupt [TIM0_OVF] void time_int0(void){
    count++;
    
    if (count == 61) { // count 1 second
        led = ~led;
        PORTC = led;
        count = 0;         
        
        //I want to go home professor
        //ao yongsu sichi 
    }
}    