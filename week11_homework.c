#include <mega128.h>
#include <delay.h>


flash unsigned char seg_pat[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};


int N1000,N100,N10,N1;   
        
        
void Seg4_out(void){  
         
    
    PORTG = 0b00001000;
    PORTD =((seg_pat[N1] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N1] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
        
    PORTG = 0b00000100;
    PORTD =((seg_pat[N10] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N10] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
        
    PORTG = 0b00000010;
    PORTD =((seg_pat[N100] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N100] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
        
    PORTG = 0b00000001;
    PORTD =((seg_pat[N1000] & 0x0F)<<4) | (PORTD & 0x0F); 
    PORTB =(seg_pat[N1000] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
}




void main(void) {
    int duty_cycle = 0;
    int num = 0;
    DDRB = 0xF0; DDRD = 0xF0; DDRG = 0x0F;   
    
    
    
    TCCR0 = 0b01101010;
    
    
    EIMSK = 0b11110000;
    EICRB = 0b11111111;
    SREG = 0x80;
    OCR0 = 0;
        
    
    while(1){                                                  
        Seg4_out(); 
        
        num  = (N1000*1000) + (N100*100) + (N10*10) + N1;
        duty_cycle = (int)(((num/9999) * 255) + 0.5);   
        OCR0 = duty_cycle;
        
    }
}



interrupt [EXT_INT4] void external_int4(void){
    N1000 = (N1000 + 1) % 10;
}

interrupt [EXT_INT5] void external_int5(void){
     N100 = (N100 + 1) % 10;
}    

interrupt [EXT_INT6] void external_int6(void){
    N10 = (N10 + 1) % 10;
}

interrupt [EXT_INT7] void external_int7(void){
    N1 = (N1 + 1) % 10;
}