#include <mega128.h>
#include <delay.h>

typedef unsigned char u_char;

flash u_char seg_pat[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

u_char N1 = 0, N10 = 0, N100 = 0, N1000 = 0;
u_char pos = 0;

void Seg4_out(void);

void main(void) {

    DDRB = 0xF0; DDRD = 0xF0; DDRG = 0x0F;
    EIMSK = 0b00110000;
    EICRB = 0b00001010;
    SREG = 0x80;
    
    while(1) Seg4_out();
}

void Seg4_out(void){

    PORTG = 0b00001000;
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70)  | (PORTB & 0x0F);
    delay_ms(5);
    
    PORTG = 0b00000100;
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70)| (PORTB & 0x0F);
    delay_ms(5);

    PORTG = 0b00000010;
    PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N100] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
    
    PORTG = 0b00000001;
    PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1000] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);    
}


interrupt [EXT_INT4] void external_int4(void){
    if(pos==0) N1 = (N1 +1) % 10;
    else if (pos==1) N10 = (N10 +1) % 10;
    else if (pos==2) N100 = (N100 +1) % 10;
    else N1000 = (N1000 +1) % 10;
}


interrupt [EXT_INT5] void external_int5(void){
    pos = (pos + 1) % 4;
}









