#define F_CPU 4000000UL  
#define BAUD 9600
#define UBBR_VAL (F_CPU/16/BAUD - 1)

#include <avr/io.h>
#include<util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

volatile uint8_t s = 0, m = 0, h = 0;
volatile bool s_up = false, min_up=false, hr_up=false;
void lcd_wr(unsigned char com, bool ins){
	PORTA = com;//storing instruction to port a
	if(ins)
		PORTB &= ~0x02;//turning RS to 0
	else 	PORTB |= 0X02;
	PORTB |= 0x01;//enabling
	_delay_us(2);
	PORTB &= ~(0x01);//turning to 0 after a pulse to write the instruction
	_delay_us(1000);
}

void init_lcd(){
	_delay_ms(20);
	lcd_wr(0x38, true);
	lcd_wr(0x01, true);
	lcd_wr(0x02, true);
	lcd_wr(0x0c, true);
	lcd_wr(0x06, true);
	_delay_ms(4);
}
void mv_cursor(uint8_t row, uint8_t col){
	lcd_wr(128+((row-1)*64 + (col-1)), true);
}
void wr_str(const char *str){
	for(uint8_t i=0;str[i] != '\0'; i++)
		lcd_wr(str[i], false);
}
void wr_num(uint8_t num){
	char str[3];
	for(uint8_t i = 10, x = 0; i >= 1; i /= 10, x++){
		str[x] = (int)(num/i) + '0';
		num %= i;
	}
	str[2] = '\0';
	wr_str(str);
}
void init_timer1(){
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	OCR1A = 15300;
	TIMSK |= (1 << OCIE1A);
	sei();
}

void init_uart(){
 	UBRRH = (uint8_t)(UBBR_VAL >> 8);
	UBRRL = (uint8_t)UBBR_VAL;
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

uint8_t read_uart(){
	while(!(UCSRA & 0x90));
	return UDR;
}

ISR(TIMER1_COMPA_vect){
	s_up = true;
	s++;
	if (s >= 60){
		min_up=true;
		s = 0;
		m++;
		if(m >= 60){
			m = 0;
			hr_up=true;
			h++;
			if(h >= 24) h = 0;
		}
	}
	
}

int main(void) {
	uint8_t x = 0;
//	init_timer1();
	DDRA |= 0XFF;//declaring all pins of port a as output
	DDRB |= 0x03;//declaring first two pins of port b as output for enable (EN) and register select (RS)
	PORTA = 0X00;// resetting
	PORTB &= ~(0x03);// setting both pins to zero
	init_uart();
	init_lcd();
	while(1){
		x = read_uart();
		mv_cursor(1,1);
		lcd_wr(x, 0);		
	}
	return 0;
}

