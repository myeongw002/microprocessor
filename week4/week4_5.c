#include <mega128.h>
#include <delay.h>

flash char seg_pat[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};


void Seg2_out(int); //function to pisplay four digit number


struct Result {
    int quotient;
    int remainder;
};



struct Result divide(int dividend, int divisor) {
    struct Result result;
    
    result.quotient = dividend / divisor;
    result.remainder = dividend % divisor;
    
    return result;
}    
        
        
void Seg2_out(int num){
    int i, N1000, N100, N10, N1;
    struct Result result; 
    
    result = divide(num, 1000); // divide original number with 1000
    N1000 = result.quotient; // thousands 
    
    result = divide(result.remainder, 100); // divide remainder with 100 
    N100 = result.quotient; // hundreds
    
    result = divide(result.remainder, 10); // divide remainder with 10 
    N10 = result.quotient; // tens
    N1 = result.remainder; //ones
    
    
    for (i=0;i<49;i++){
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
        if(num > 9999) num = 0;  

    }

}
