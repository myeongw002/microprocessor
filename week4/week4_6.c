#include <mega128.h>
#include <delay.h>

flash char seg_pat[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};


struct RESULT {
    int quotient;
    int remainder;
};


struct NUMBERS {
    int N1000;
    int N100;
    int N10;
    int N1;
    
};


struct RESULT divide(int dividend, int divisor) {
    struct RESULT result;
    
    result.quotient = dividend / divisor;
    result.remainder = dividend % divisor;
    
    return result;
}  


struct NUMBERS output(int num) {
    struct NUMBERS numbers;
    struct RESULT result = divide(num, 1000);

    numbers.N1000 = result.quotient;
    
    result = divide(result.remainder, 100);
    numbers.N100 = result.quotient;
    
    result = divide(result.remainder, 10);
    numbers.N10 = result.quotient;
    numbers.N1 = result.remainder;
     
    return numbers;
}    
        
        
        
void Seg2_out(int num){
    int i, N1000, N100, N10, N1;
    struct NUMBERS numbers = output(num); 
    
    N1000 = numbers.N1000; 
    N100 = numbers.N100; 
    N10 = numbers.N10; 
    N1 = numbers.N1; 
    
    
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
