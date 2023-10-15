#include <mega128.h>
#include <delay.h>
flash char seg_pat[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void Seg2_out(int); //function to pisplay wo digit number


void Seg2_out(int num){
    int i, N10, N1;
    
    N10 = num / 10;
    N1 = num % 10;
    
    for (i=0;i<49;i++){
        PORTG = 0b00001000;
        PORTD =((seg_pat[N1] & 0x0F)<<4) | (PORTD & 0x0F); 
        PORTB =(seg_pat[N1] & 0x70) | (PORTB & 0x0F);
        delay_ms(5);
        
        PORTG = 0b00000100;
        PORTD =((seg_pat[N10] & 0x0F)<<4) | (PORTD & 0x0F); 
        PORTB =(seg_pat[N10] & 0x70) | (PORTB & 0x0F);
        delay_ms(5);
   }     
}



void main(void) {
    int num = 0;    
    
    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    PORTB = 0x00;
    PORTD = 0x00;
    
    while (1){
        Seg2_out(num);
        num++;
        if(num > 99) num = 0;  

    }

}
