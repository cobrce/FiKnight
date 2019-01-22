#ifndef FIKNIGHT_PORTS_H_
#define FIKNIGHT_PORTS_H_

#include <avr/io.h>
#ifdef PORTA
#define PORTA_ PORTA
#else
#define PORTA_ 0
#endif

#ifdef PORTB
#define PORTB_ PORTB
#else
#define PORTB_ 0
#endif

#ifdef PORTC
#define PORTC_ PORTC
#else
#define PORTC_ 0
#endif

#ifdef PORTD
#define PORTD_ PORTD
#else
#define PORTD_ 0
#endif

#ifdef PINA
#define PINA_ PINA
#else
#define PINA_ 0
#endif

#ifdef PINB
#define PINB_ PINB
#else
#define PINB_ 0
#endif

#ifdef PINC
#define PINC_ PINC
#else
#define PINC_ 0
#endif

#ifdef PIND
#define PIND_ PIND
#else
#define PIND_ 0
#endif

#ifdef DDRA
#define DDRA_ DDRA
#else
#define DDRA_ 0
#endif

#ifdef DDRB
#define DDRB_ DDRB
#else
#define DDRB_ 0
#endif

#ifdef DDRD
#define DDRD_ DDRD
#else
#define DDRD_ 0
#endif

#ifdef DDRC
#define DDRC_ DDRC
#else
#define DDRC_ 0
#endif

#endif