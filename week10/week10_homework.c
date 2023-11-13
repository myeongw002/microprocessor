#include <mega128.h>
#include <delay.h>
#include <math.h>
#include <stdlib.h>

flash unsigned char seg_pat[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

int count, angle, dp_mode;
int acc_x, acc_y, acc_z;   
int offset_x, offset_z, offset_y;        
        

int AD_disp(int acc_x, int acc_y, int acc_z, int mode){
    float facc_x, facc_y, facc_z, fangle;
    int iacc_x, iacc_y, iacc_z, iangle, negative, ival, buf;
    unsigned char N100,N10,N1;
    
    
    facc_x = (float)acc_x * 5.0 / 1023.0; 
    facc_x = facc_x / 0.8 ; // calculate acceleration of x 
    iacc_x = (int)(facc_x * 100.0 + 0.5); //round up 
    
    facc_y = (float)acc_y * 5.0 / 1023.0; 
    facc_y = facc_y / 0.8 ; // calculate acceleration of y 
    iacc_y = (int)(facc_y * 100.0 + 0.5); //round up 
    
    facc_z = (float)acc_z * 5.0 / 1023.0; 
    facc_z = facc_z / 0.8 ; // calculate acceleration of z
    iacc_z = (int)(facc_z * 100.0 + 0.5); //round up
    
    
    fangle = atan(facc_x/facc_z) * 180 / 3.141592; // calculate anngle of x,z 
    iangle = (int)(fangle + 0.5); //round up
        
    if (mode == 0) ival = iacc_x; // display acc_x
    if (mode == 1) ival = iacc_y; // display acc_x
    else if (mode == 2) ival = iacc_z; // display acc_z
    else if (mode == 3) ival = iangle; // display angle
    
    if (ival < 0) negative = 1;
    else negative = 0; 
    ival = abs(ival);
    
    if (mode == 3) {
        N10 = ival / 10;
        N1 = ival % 10;
        }
    else {          
        N100 = ival / 100;
        buf = ival %  100;
        N10 = buf / 10;
        N1 = buf % 10;
        }
                     
    
    PORTG = 0b00000001;
    if (mode != 3){ 
        if (negative == 1) PORTB = 0x40; // display '-' if acceleration < 0 
        else PORTB = 0x00;
        PORTD = 0x00; 
        }
    else {
        PORTD = 0x00;
        PORTB = 0x00;
        }             
    delay_ms(1);
    
    
    PORTG = 0b00001000;
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70) | (PORTB & 0x0F);
    delay_ms(1);
    
    PORTG = 0b00000100;
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70) | (PORTB & 0x0F);
    delay_ms(1);
    
    
    PORTG = 0b00000010;
    if (mode == 3){ //angle mode
        if (negative == 1) PORTB = 0x40; // display '-' if angle < 0 
        else PORTB = 0x00;
        PORTD = 0x00; 
        delay_ms(1);
    }
     
    else {
        PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F); 
        PORTB = (seg_pat[N100] & 0x70) | (PORTB & 0x0F);   
        PORTB = PORTB | 0x80;
    }
    delay_ms(1);  
    
    return fangle ; 
       
}


void main(void) { 
    int i;
         
    DDRB = 0xF0; DDRD = 0xF0; DDRG = 0x0F;   
    DDRC = 0xFF;
    PORTC = 0xFF;
    ADCSRA = 0x87; 

    offset_x = 0; 
    offset_y = 0;
    offset_z = 0;
    

    for (i = 0; i < 10; i++){
        ADMUX = 0x03; // X(ADC3) single input
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_x = ADCW;
        offset_x = offset_x + acc_x;      
    }
    offset_x = offset_x / 10; // offset_x = average of measured acc_x    


    for (i = 0; i < 10;i++){
        ADMUX = 0x04; // Y (ADC4) single input
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_y = ADCW;
        offset_y = offset_y + acc_y; 
    }
    offset_y = offset_y / 10; // offset_y = average of measured acc_y
    
    
    for (i = 0; i < 10; i++){
        ADMUX = 0x05; // Z (ADC5) single input select
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_z = ADCW ;
        offset_z = offset_z + acc_z;
    }
    offset_z = (offset_z / 10) - 162 ; // offset_z = average of measured acc_z, -162 makes z acceleration 1   

    // set registers after calculating offsets
    TIMSK = 0x01;
    TCCR0 = 0x07;
    TCNT0 = 0x00;
    
    EIMSK = 0b11110000;
    EICRB = 0b10101010;
    SREG = 0x80;
    

    dp_mode = 0;

    while(1){ 
        angle = AD_disp(acc_x, acc_y, acc_z, dp_mode); 
        if (abs(angle) >= 45) PORTC = 0x00; // turn led on 
        else PORTC = 0xFF; // turn led off
    
    }
}


interrupt [TIM0_OVF] void time_int0(void){
    count  = count + 1 ; 
    
    if (count >= 15) {
        count = 0 ;
        ADMUX = 0x03; // X(ADC3) single input
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_x = ADCW - offset_x;                      
        
        ADMUX = 0x04; // Y (ADC4) single input
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_y = ADCW - offset_y;                           
                            
        
        ADMUX = 0x05; // Z (ADC5) single input select
        ADCSRA = ADCSRA | 0x40; // ADSC = 1 convert start
        while((ADCSRA & 0x10) == 0); // ADIF=1
        acc_z = ADCW - offset_z;
    }
} 


interrupt [EXT_INT4] void external_int4(void){
    dp_mode = 0;
}

interrupt [EXT_INT5] void external_int5(void){
    dp_mode = 1;
}    

interrupt [EXT_INT6] void external_int6(void){
    dp_mode = 2;
}

interrupt [EXT_INT7] void external_int7(void){
    dp_mode = 3;
}
















