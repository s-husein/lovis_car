#define F_CPU 16000000UL  
#define BAUD 1000000
#define UBBR_VAL (F_CPU/8/BAUD - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

unsigned char data[2] = {0xff, 0xff};
unsigned char ind=0;
void lcd_wr(unsigned char com, bool ins){
	PORTA = com;//storing instruction to port a
	if(ins) PORTB &= ~0x02;//turning RS to 0
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
	for(uint8_t i=0;i <= 1; i++)
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
	TCNT1 = 0;
	ICR1 = 39999;
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << CS11) | (1 << WGM12) | (1 << WGM13);
	OCR1A = 0;

}
void init_timer0(){
	TCCR0 = 0x7d;
	OCR0 = 0xff;
}
void init_timer2(){
	TCCR2 = 0x7d;
	OCR2 = 0xff;
}
void init_uart(){
 	UBRRH = (unsigned char)(UBBR_VAL >> 8);
	UBRRL = (unsigned char)UBBR_VAL;
	UCSRA |= (1 << U2X);
	UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

uint8_t read_uart(){
	while(!(UCSRA & 0x90));
	return UDR;
}

ISR(USART_RXC_vect){
	data[ind] = UDR;
	ind++;
	if (ind >=2) ind=0;
	OCR0 = data[0];
	OCR2 = data[1];
}
int main(void) {
//	init_timer1();
	DDRD |= 0xA0;
	DDRA |= 0XFF;//declaring all pins of port a as output
	DDRB |= 0x0B;//declaring first two pins of port b as output for enable (EN) and register select (RS)
	PORTA = 0X00;// resetting
	PORTB &= ~(0x03);// setting both pins to zero
	init_timer0();
	init_timer1();
	init_timer2();
	init_uart();
	sei();
	init_lcd();
	while(1){
		OCR1A = 2000;
		_delay_us(1213000);
		// OCR1A = 3000;
		// _delay_ms(2000);
		// OCR1A = 4000;
		// _delay_ms(510);
		OCR1A = 3000;
		// _delay_ms(10);
		// OCR1A = 30000;
		_delay_ms(2000);

		mv_cursor(1, 1);
		wr_str(data);
	}


//	mv_cursor(1, 1);
//	wr_str(data);		
//	lcd_wr(ind, 0);

	return 0;
}

