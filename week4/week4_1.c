#include<mega128.h>



void main(void) {
    
    unsigned char sw1 ,sw2;
    
    DDRC = 0xFF;
    DDRE = 0x00;
    
    while (1){
        sw1 = PINE & 0b00010000;
        sw2 = PINE & 0b00100000;
        
        if (sw1 == 0 && sw2 == 0){ //if sw1 and sw2 are on
            PORTC = 0xFF;
        }
        
        else if (sw1 != 0 || sw2 != 0){ //if sw1 or sw2 are off 
            PORTC = 0x00;
        }
 }
}
