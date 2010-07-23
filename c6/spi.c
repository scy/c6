#ifndef c6LoadedSPI
#define c6LoadedSPI

#include <c6/delay.c>

uint8_t c6SPISendRecvByte(uint8_t send)
{
	SPDR = send;
	while (!(SPSR && (1 << SPIF))) { }
	return SPDR;
}

void c6SPISendRecvBytes(uint8_t *send, uint8_t size)
{
	for (uint8_t i = 0; i <= size; i++) {
		send[i] = c6SPISendRecvByte(send[i]);
	}
}

// Currently, this is just a macro. If SPI is used from a lot of different
// places, it could be changed to be a function instead, to reduce code size.
#define c6SPISendRecvPacket(port,pin,send,size) \
PORT##port &= ~(1 << pin); \
c6SPISendRecvBytes(send, size); \
PORT##port |= (1 << pin);

// Set the uC to be an SPI master. MISO, MOSI and SCK must reside on the same
// port. SS is not relevant here, it will be used when sending and receiving and
// can well be on a completely different port. Also, multiple SS pins can be
// used to access different slaves.
#define c6SPIMaster(port,miso,mosi,sck) \
DDR##port |= (1 << mosi) | (1 << sck); \
DDR##port &= ~(1 << miso); \
SPCR |= (1 << SPE) | (1 << MSTR);

#endif
