#ifndef c6LoadedUART
#define c6LoadedUART

#include <c6/io.c>

// Some uCs do not use an URSEL flag. For portability, define c6URSEL0 to be an
// always ORable expression.
#if defined URSEL0
# define c6URSEL0 (1 << URSEL0)
#elif defined URSEL
# define c6URSEL0 (1 << URSEL)
#else
# define c6URSEL0 (0)
#endif

// A lot of uCs do not have multiple UARTs and therefore do not have an UART
// number in the macros. This is really bad for portability. Therefore, define
// the macros we need.
#ifndef  URSEL0
# define URSEL0 URSEL
#endif
#ifndef  UBRR0L
# define UBRR0L UBRRL
#endif
#ifndef  UBRR0H
# define UBRR0H UBRRH
#endif
#ifndef  UCSR0A
# define UCSR0A UCSRA
#endif
#ifndef  UCSR0B
# define UCSR0B UCSRB
#endif
#ifndef  UCSR0C
# define UCSR0C UCSRC
#endif
#ifndef  UCSZ00
# define UCSZ00 UCSZ0
#endif
#ifndef  UCSZ01
# define UCSZ01 UCSZ1
#endif
#ifndef  UDR0
# define UDR0 UDR
#endif
#ifndef  UDRE0
# define UDRE0 UDRE
#endif
#ifndef  TXEN0
# define TXEN0 TXEN
#endif

// Baud calculation (word, high, low). //FD
#define c6UARTBaud(b) ((F_CPU + b * 8UL) / (b * 16UL) - 1UL)
#define c6UARTBaudH(b) (c6UARTBaud(b) >> 8)
#define c6UARTBaudL(b) (c6UARTBaud(b) & 0xff)

// Set baud rate for a port.
#define c6UARTSetBaud(port,baud) UBRR##port##H = (c6UARTBaudH(baud)); \
                                  UBRR##port##L = (c6UARTBaudL(baud));

// Possible port modes. TODO: Add more.
#define c6UART8N1(port) ((1 << UCSZ##port##0) | (1 << UCSZ##port##1))

// Set a port's options.
#define c6UARTMode(port,mode) UCSR##port##C = c6URSEL##port | (mode);

// Enable TX on a port.
#define c6UARTEnableTX(port) UCSR##port##B |= (1 << TXEN##port);

#ifdef c6UseUART0
void c6UARTSendChar0(char c) {
	// Wait until the UART is ready.
	while (!(UCSR0A & (1 << UDRE0))) { }
	// Write into the data register.
	UDR0 = c;
}

void c6UARTSendBytes0(char *buf, uint8_t size)
{
	while (size-- > 0) {
		// Wait until the UART is ready.
		while (!(UCSR0A & (1 << UDRE0))) { }
		// Write into the data register.
		UDR0 = *(buf++);
	}
}
#endif

// Shortcut to enable 115200 baud 8N1 TX on UART.
#define c6UARTSimple(port) \
	c6UARTSetBaud(port, 115200); \
	c6UARTEnableTX(port); \
	c6UARTMode(port, c6UART8N1(port));

#endif
