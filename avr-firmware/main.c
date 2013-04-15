/* Name: main.c
 * Author: Dennis Gentry
 * Copyright 2010, Dennis Gentry
 * License: Creative Commons Attribution
 */

/* 
 * Title: PWM for the Neophyte
 * Hardware: ATtiny13
 */
 
//#define F_CPU 9600000UL // Define software reference clock for delay duration
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <assert.h>


// For hardware PWM, only PB0 or PB1 have timer/counter support
#define PWM_OUT_PIN PB0

#define LED PB2
#define POT PB3  // (aka ADC3)
#define GATE PB1  // The gate of the MOSFETs we want to control
#define TEMP PB4  // (ADC 2) The Thermistor (other leg is grounded)

unsigned char duty_cycle;  // In the 0 - 255 range
#define MIN_DUTY_CYCLE 0
#define MAX_DUTY_CYCLE 255

void mydelay(int ms);
void flash_number(int num);
void flash_number_in_binary(unsigned char num);
unsigned char a_to_d(unsigned char pin);

/* Making variables static for ISRs */

unsigned char temperature;

int main (void)
{
  DDRB |= (1 << LED); // Make LED an output, Will want to blink LED
  DDRB |= (1 << GATE); // Make GATE an output
  DDRB &= ~(1 << TEMP);

  /* Set up to read Pot */
  DDRB &= ~(1<< POT); // Set POT as an input (0-5v)
  ADCSRA |= (1 << ADEN)| // Analog-Digital enable bit
    (1 << ADPS1)| // set prescaler to 8 (clock / 8)
    (1 << ADPS0); // set prescaler to 8 (clock / 8)
 

  /* Set up PWM */
  //DDRB |= (1 << PWM_OUT_PIN); // OC0A on PB0
  // COM0A1 - COM0A0 (Set OC0A on Compare Match, clear OC0A at TOP)
  //TCCR0A |= ((1 << COM0A1) | (1 << COM0A0) 
  //	     | (1 << WGM01) | (1 << WGM00)); // WGM01 - WGM00 (set fast PWM)
  //OCR0A = 0; // initialize Output Compare Register A to 0

  //TCCR0B |= (1 << CS01); // Start timer at Fcpu / 256
 

  flash_number_in_binary(0);
  flash_number_in_binary(TEMP);
  flash_number_in_binary(15);
  flash_number_in_binary(POT);

  while (1) {

    // Sample POT to get desired motor speed
    duty_cycle = a_to_d(POT);

    //----------Show ADCH Byte in Led variable brightness indicator---------
 
    if (duty_cycle > 0) {
      PORTB |= (1 << LED);  // Turn on the LED
      PORTB |= (1 << GATE); // and the MOSFETs
    }
    mydelay(duty_cycle);

    // Sample
    temperature = a_to_d(TEMP); //(100K/4.7K thermistor/resistor
			// ~5v - .2v, ~4.1v = 60C, ~2.5v = 100C)

    if (temperature > ((4.5/5.0)*256)) {
      flash_number_in_binary(temperature/32);
    }

    if (duty_cycle < MAX_DUTY_CYCLE) {
      PORTB &= ~(1 << LED);   // Turn off the LED
      PORTB &= ~(1 << GATE);  // and the MOSFETs
    }
    mydelay(MAX_DUTY_CYCLE - duty_cycle);

  }
  return 0; // not reached
}

unsigned char a_to_d(unsigned char pin) 
{
  unsigned char mux = 0;  //(ADC0/PB5 = 00, ADC1/PB2 = 01, ADC2/PB4 = 10, ADC3/PB3 = 11)

  flash_number_in_binary(pin);
  if (pin == PB2) mux = 1;
  else if (pin == PB4) mux = 2;
  else if (pin == PB3) mux = 3;


  ADMUX |= (1 << ADLAR)| // AD result store in (most significant bit in ADCH)
    (mux & ((1 << MUX1) | (1 << MUX0))); // Choose AD input 

  // ADMUX should be 0b001000mm where mm is the mux value
  assert(ADMUX >= 0b00100000 && ADMUX <= 0b00100011);

  flash_number_in_binary(ADMUX);

  ADCSRA |= (1 << ADEN); // Analog-Digital enable bit
  ADCSRA |= (1 << ADSC); // Discard first conversion

  while (ADCSRA & (1 << ADSC)); // wait until conversion is done
    
  ADCSRA |= (1 << ADSC); // start another conversion
 
  while (ADCSRA & (1 << ADSC)); // wait until conversion is done
 
  ADCSRA &= ~(1<<ADEN); // shut down the ADC

  return ADCH;
}
 
 
/*
 *  Flash LED <num> times, then pause.
 */
void flash_number(int num)
{
  int i;
  for (i = 0; i < 4; i++) {
    _delay_ms(50);
  }

  for (i = 0; i < num; i++) {
    PORTB |= (1 << LED);
    _delay_ms(15);
    PORTB ^= (1 << LED); // Invert led bit and show it
    _delay_ms(45);
  }


  for (i = 0; i < 4; i++) {
    _delay_ms(50);
  }
}



/*
 *  Flash <num> in binary on the LED
 */
void flash_number_in_binary(unsigned char num)
{
  char i, j;  // Really only need 4 bits for each of these -- bit pointers into a byte

  for (i = 0; i < 4; i++) {
    _delay_ms(50);
  }

  // Find first set bit (isn't there a libc function for this?)
  for (i = 7; i >= 0 && !(num & (1 << i)); i--);
  // i either points at the first set bit, or -1

  // Always flash at least bits 3 - 0
  if (i<3) i = 3;


  for (j = i; j >= 0; j--) {
    PORTB |= (1 << LED);  // Flame on
    if (num & (1 << j)) {
      _delay_ms(60);
      PORTB &= ~(1 << LED); // Flame off
    } else {
      _delay_ms(10);
      PORTB &= ~(1 << LED); // Flame off
      _delay_ms(50);
    }

    _delay_ms(40);
  }

  for (i = 0; i < 4; i++) {
    _delay_ms(50);
  }
}




void mydelay(int ms)
{
  int i;

  for (i = 0; i < ms; i++) {
    _delay_us(5);
  }
}

