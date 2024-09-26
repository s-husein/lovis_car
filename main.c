#define F_CPU 16000000UL  
#define BAUD 1000000
#define UBBR_VAL (F_CPU/8/BAUD - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>

volatile unsigned char data[3] = {0x00, 0x00, 0x00};
unsigned char ind=0;
uint16_t t = 0, calc_delay = 0;
volatile signed char angle = 0;
short diff = 0;
// void lcd_wr(unsigned char com, bool ins){
// 	PORTA = com;//storing instruction to port a
// 	if(ins) PORTB &= ~0x02;//turning RS to 0
// 	else 	PORTB |= 0X02;
// 	PORTB |= 0x01;//enabling
// 	_delay_us(2);
// 	PORTB &= ~(0x01);//turning to 0 after a pulse to write the instruction
// 	_delay_us(1000);
// }

// void init_lcd(){
// 	_delay_ms(20);
// 	lcd_wr(0x38, true);
// 	lcd_wr(0x01, true);
// 	lcd_wr(0x02, true);
// 	lcd_wr(0x0c, true);
// 	lcd_wr(0x06, true);
// 	_delay_ms(4);
// }
// void mv_cursor(uint8_t row, uint8_t col){
// 	lcd_wr(128+((row-1)*64 + (col-1)), true);
// }
// void wr_str(const char *str){
// 	for(uint8_t i=0;i <= 1; i++)
// 		lcd_wr(str[i], false);
// }
// void wr_num(uint8_t num){
// 	char str[3];
// 	for(uint8_t i = 10, x = 0; i >= 1; i /= 10, x++){
// 		str[x] = (int)(num/i) + '0';
// 		num %= i;
// 	}
// 	str[2] = '\0';
// 	wr_str(str);
// }
void init_timer1(){
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << CS11) | (1 << CS10) | (1 << WGM12) | (1 << WGM13);
	ICR1 = 4999;
	TCNT1 = 0;
	OCR1A = 390;

}

void init_timer0(){
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01) | (1 << CS00);
	TIMSK0 |= (1 << OCIE0A);
	TIFR0 |= (1 << OCF0A);
	OCR0A = 250;
}

void init_timer2(){
	TCCR2A |= 0XF3;
	TCCR2B |= 0X07;
	sei();
	OCR2A = 0xff;
	OCR2B = 0xff;
}
void init_uart(){
 	UBRR0H = (unsigned char)(UBBR_VAL >> 8);
	UBRR0L = (unsigned char)UBBR_VAL;
	UCSR0A |= (1 << U2X0);
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void ang_to_delay(){
	diff = (signed char)data[2] - angle;
	angle = data[2];
	if(diff < 0){
		OCR1A = 250;
	}
	else if (diff > 0){
		OCR1A = 500;
	}
	calc_delay = (uint16_t)round(abs(diff)/0.298);
}

ISR(USART_RX_vect){
	t = 0;
	data[ind] = UDR0;
	ind++;
	if (ind >=3) ind=0;
	OCR2A = 255 - data[0];
	OCR2B = 255 - data[1];
	OCR1A = (data[2] - 97)*(470 - 312)/(157 - 97) + 312;
}

ISR(TIMER0_COMPA_vect){
	if (abs(diff) > 0) {
		t++;
		if (t == calc_delay){
			diff = 0;
			OCR1A = 375;
		}
		
	}
}

int main(void) {
	init_uart();
	// init_timer0();
	init_timer1();
	init_timer2();
	sei();
	DDRD |= (1 << PD3);
	DDRB |= (1 << PB3) | (1 << PB1);
//	init_timer1();
	// DDRD |= 0xA0;
	// DDRA |= 0XFF;//declaring all pins of port a as output
	// DDRB |= 0x0B;//declaring first two pins of port b as output for enable (EN) and register select (RS)
	// PORTA = 0X00;// resetting
	// PORTB &= ~(0x03);// setting both pins to zero
	// init_timer0();
	// init_timer1();
	// init_timer2();
	// init_uart();
	// sei();
	// init_lcd();
	while(1){
		// OCR1A = 250;
		// _delay_ms(2000);
		// OCR1A = 385;
		// _delay_ms(2000);
		// OCR1A = 500;
		// _delay_ms(2000);
		// OCR1A = 375;
		// _delay_ms(2000);
		// OCR1A = 4000;
		// _delay_ms(510);
		// OCR1A = 3000;
		// _delay_ms(10);
		// OCR1A = 30000;
		// _delay_ms(2000);

		// mv_cursor(1, 1);
		// wr_str(data);
	}


//	mv_cursor(1, 1);
//	wr_str(data);		
//	lcd_wr(ind, 0);

	return 0;
}

