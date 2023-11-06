#include <mega128.h>
#include <delay.h>
#include <math.h>
#include <stdlib.h>

flash unsigned char seg_pat[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};


   
        
        
void Seg4_out(int num){
    int N1000, N100, N10, N1, buf;
 
    N1000 = num / 1000;
    buf = N1000 /% 1000;
    N100 = buf / 100;
    buf = N100 %  100;
    N10 = buf / 10;
    N1 = buf % 10;
    
    PORTG = 0b00001000;
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
    
    PORTG = 0b00000100;
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
    
    PORTG = 0b00000010;
    PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N100] & 0x70) | (PORTB & 0x0F);
    if (mode == 2) PORTB = PORTB | 0x80;
    delay_ms(5);

    PORTG = 0b00000001;
    PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1000] & 0x70) | (PORTB & 0x0F);   
    
}


int AD_disp(int acc_x,int acc_z, int mode){
    float facc_x, facc_z, fangle;
    int iacc_x, iacc_z, iangle, negative, ival, buf;
    unsigned char N100,N10,N1;
    
    
    facc_x = (float)acc_x * 5.0 / 1023.0; 
    facc_x = facc_x / 0.8 ; // calculate acceleration of x 
    iacc_x = (int)(facc_x * 100.0 + 0.5); //round up 
    
    
    facc_z = (float)acc_z * 5.0 / 1023.0; 
    facc_z = facc_z / 0.8 ; // calculate acceleration of z
    iacc_z = (int)(facc_z * 100.0 + 0.5); //round up
    
    
    fangle = atan(facc_x/facc_z) * 180 / 3.141592; // calculate anngle of x,z 
    iangle = (int)(fangle * 10.0 + 0.5); //round up
    
    if (iangle < 0) negative = 1; //negative flag 
    else if (iangle > 0) negative = 0;
    iangle = abs(iangle); //absolute value of angle       
    
    if (mode == 0) ival = iacc_x; // display acc_x
    else if (mode == 1) ival = iacc_z; // display acc_z
    else if (mode == 2) ival = iangle; // display angle
    
       
    N100 = ival / 100;
    buf = ival %  100;
    N10 = buf / 10;
    N1 = buf % 10;
    
    
    if (negative ==1 && mode == 2){ // display '-' if angle < 0
        PORTG = 0b00000001; 
        PORTB = 0x40;
        PORTD = 0x00; 
        delay_ms(5);
    }  
    else PORTG = 0b00000000;             
    
    
    PORTG = 0b00001000;
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70) | (PORTB & 0x0F);
    delay_ms(5);
    
    PORTG = 0b00000100;
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70) | (PORTB & 0x0F);
    if (mode == 2) PORTB = PORTB | 0x80;
    delay_ms(5);

    PORTG = 0b00000010;
    PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N100] & 0x70) | (PORTB & 0x0F);   
    if (mode == 0 || mode ==1) PORTB = PORTB | 0x80;
    delay_ms(5);   
    
    return fangle ; 
       
}


void main(void) {
    int acc_x, acc_y, acc_z,angle; 
    int offset_mode, dp_mode, offset_x, offset_z;
    
    DDRB = 0xF0; DDRD = 0xF0; DDRG = 0x0F;   
    DDRC = 0xFF;
    PORTC = 0xFF;
    ADCSRA = 0x87; 
    
    offset_x = 0;
    offset_z = 0;
    
    for (int i = 0; i < 10; i++){
        ADMUX = 0x03; // X(ADC3) single input
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_x = ADCW;
        offset_x = offset_x + acc_x;
        offset_x = offset_x / 10; // offset_x = average of measured acc_x
    }

    Seg4_out(offset_x); //display offset_x for 5 sec
    delay_ms(5000);

    for (int i = 0; i < 10; i++){
        ADMUX = 0x05; // Z (ADC5) single input select
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_z = ADCW ;
        offset_z = offset_z + acc_z;
        offset_z = offset_z / 10; // offset_z = average of measured acc_z
    }

    Seg4_out(offset_z); //display offset_z for 5 sec
    delay_ms(5000);

    while(1){
    ADMUX = 0x03; // X(ADC3) single input
    ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
    while((ADCSRA & 0x10) == 0); // ADIF=1
    acc_x = ADCW - offset_x;                      
    
    ADMUX = 0x04; // Y (ADC4) single input
    ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
    while((ADCSRA & 0x10) == 0); // ADIF=1
    acc_y = ADCW;                           
                        
    
    ADMUX = 0x05; // Z (ADC5) single input select
    ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
    while((ADCSRA & 0x10) == 0); // ADIF=1
    acc_z = ADCW - offset_z;  
    
    dp_mode = 2; //select display mode 0: acc_x, 1: acc_z, 2: angle
    
    angle = AD_disp(acc_x, acc_z, dp_mode); 
    if (abs(angle) >= 45) PORTC = 0x00; // turn led on 
    else PORTC = 0xFF; // turn led off      
    
    }
}
