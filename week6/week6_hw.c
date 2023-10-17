/* homework*/

#include <mega128.h>
#include <delay.h>

flash char seg_pat[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

unsigned char led =0xFE;
unsigned char count = 0;
int pos;
int sec, min;


    
        
        
void Seg4_out(int min){
    int  N1000, N100, N10, N1;
    int hour;
    
    hour = min / 60; 
    min = min % 60;
    
    N1000 =  hour / 10; // 10 hour's 
    N100 =  hour % 10;  // 1 hours's
    N10 = min /10; // 10 minuite's
    N1 = min % 10; // 1 minuite's

    PORTG = 0b00001000;
    PORTD =((seg_pat[N1] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N1] & 0x70) | (PORTB & 0x0F);
    delay_ms(1);
        
    PORTG = 0b00000100;
    PORTD =((seg_pat[N10] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N10] & 0x70) | (PORTB & 0x0F);
    delay_ms(1);
        
    PORTG = 0b00000010;
    PORTD =((seg_pat[N100] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N100] & 0x70) | (PORTB & 0x0F);
    delay_ms(1);
        
    PORTG = 0b00000001;
    PORTD =((seg_pat[N1000] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N1000] & 0x70) | (PORTB & 0x0F);
    delay_ms(1);
    
        
}


void main(void){
    DDRB = 0xF0; DDRD = 0xF0; DDRG = 0x0F;
    DDRC = 0xFF;
    PORTC = led;
    
    EIMSK = 0b00110000;
    EICRB = 0b00101010;
        
    TIMSK = 0x01;
    TCCR0 = 0x07;
    TCNT0 = 0x00;
    SREG = 0x80;
    
    while (1){
        if (min >= 1439) min = min - 1439; // reset minuite
        Seg4_out(min);
    }  
}

interrupt [EXT_INT4] void external_int4(void){
    if (pos == 0) min = min +1; // 1 minuite
    else if (pos == 1) min = min + 10; // 10 minuite
    else if (pos == 2) min = min + 60; // 1 hour = 60 minuite
    else if (pos == 3) min = min + 600; // 10 hour = 600 minuite   
}


interrupt [EXT_INT5] void external_int5(void){
    pos = (pos +1) % 4; // select posiotion
} 


interrupt [TIM0_OVF] void time_int0(void){
    count  = count + 1 ;
    if (count == 61) { // count 1 second
        count = 0 ;
        
        sec = sec + 1;
        
        if (sec == 60) {
            sec = 0;
            min = min +1; // minuite  + 1 when sec = 60     
        }
            
        PORTC = sec ^ 0xFF; // make reverse
    }
}    
