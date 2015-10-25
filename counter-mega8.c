/*
* counter-mega8.c
*
* Created: 05.10.2015 13:41:54
* Author : yakov
*/

#include <avr/io.h>
#include <avr/eeprom.h>

#define INPUT_PIN PINC0

// number segments map in ABCDEFG
int numberSegments[]={0b1111110,0b0110000,0b1101101,0b1111001,0b0110011,0b1011011,0b1011111,0b1110000,0b1111111,0b1111011};

uint8_t counter_start_eeprom EEMEM;
uint8_t counter_address_offset_eeprom EEMEM=0;

void display(int num) {
	
	//1st indicator
	int digit1=(num/10)%10;
	
	if(numberSegments[digit1]&(1<<6))
	PORTD|=1<<PIND2;
	else
	PORTD&=~(1<<PIND2);
	
	if(numberSegments[digit1]&(1<<5))
	PORTD|=1<<PIND3;
	else
	PORTD&=~(1<<PIND3);
	
	if(numberSegments[digit1]&(1<<4))
	PORTB|=1<<PINB4;
	else
	PORTB&=~(1<<PINB4);
	
	if(numberSegments[digit1]&(1<<3))
	PORTB|=1<<PINB5;
	else
	PORTB&=~(1<<PINB5);
	
	if(numberSegments[digit1]&(1<<2))
	PORTB|=1<<PINB0;
	else
	PORTB&=~(1<<PINB0);
	
	if(numberSegments[digit1]&(1<<1))
	PORTD|=1<<PIND1;
	else
	PORTD&=~(1<<PIND1);
	
	if(numberSegments[digit1]&1)
	PORTD|=1<<PIND0;
	else
	PORTD&=~(1<<PIND0);
	
	//2nd indicator
	int digit2=num%10;
	
	if(numberSegments[digit2]&(1<<6))
	PORTD|=1<<PIND6;
	else
	PORTD&=~(1<<PIND6);
	
	if(numberSegments[digit2]&(1<<5))
	PORTD|=1<<PIND7;
	else
	PORTD&=~(1<<PIND7);
	
	if(numberSegments[digit2]&(1<<4))
	PORTB|=1<<PINB2;
	else
	PORTB&=~(1<<PINB2);
	
	if(numberSegments[digit2]&(1<<3))
	PORTB|=1<<PINB1;
	else
	PORTB&=~(1<<PINB1);
	
	if(numberSegments[digit2]&(1<<2))
	PORTB|=1<<PINB3;
	else
	PORTB&=~(1<<PINB3);
	
	if(numberSegments[digit2]&(1<<1))
	PORTD|=1<<PIND5;
	else
	PORTD&=~(1<<PIND5);
	
	if(numberSegments[digit2]&1)
	PORTD|=1<<PIND4;
	else
	PORTD&=~(1<<PIND4);
}

void initSegments() {
	//init 7 segments output
	DDRB=0b00111111;
	DDRD|=0b11111111;
}


void init() {
	initSegments();
}

int main(void)
{
	init();
	
	int counter_address_offset=eeprom_read_byte(&counter_address_offset_eeprom);
	uint8_t* counter_ref=(&counter_start_eeprom)+counter_address_offset;
	int counter=eeprom_read_byte(counter_ref);
	
	int pinState=PINC&(1<<INPUT_PIN);
	display(counter);
	
	while (1)
	{
		int newPinState=PINC&(1<<INPUT_PIN);
		if(pinState && !newPinState) {
			counter=counter%16+1;
			display(counter);
			eeprom_write_byte(counter_ref,counter);
			
		}
		pinState=newPinState;
	}
}

