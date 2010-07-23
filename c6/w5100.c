#ifndef c6LoadedW5100
#define c6LoadedW5100

#include <c6/spi.c>

#ifndef c6W5100Port
# error c6W5100Port needs to be the port where the SPI SS pin for the W5100 is located (example: B)
#endif

#ifndef c6W5100Pin
# error c6W5100Pin needs to be the pin number of the SPI SS pin for the W5100 (example: 2)
#endif

// Read a number of bytes from consecutive registers.
void c6W5100Read(uint16_t startreg, uint8_t *buf, uint16_t size)
{
	while (size-- > 0) {
		uint8_t send[4] = {
			0x0f,                // opcode "read"
			(startreg >> 8),     // high byte
			(startreg & 0xff),   // low byte
			0xc6                 // ignored
		};
		c6SPISendRecvPacket(c6W5100Port, c6W5100Pin, send, sizeof(send));
		*buf = send[3];
		buf++;
		startreg++;
	}
}

// Write a number of bytes into consecutive registers.
void c6W5100Write(uint16_t startreg, uint8_t *value, uint16_t size)
{
	while (size-- > 0) {
		uint8_t send[4] = {
			0xf0,                // opcode "write"
			(startreg >> 8),     // high byte
			(startreg & 0xff),   // low byte
			*(value++)           // value
		};
		c6SPISendRecvPacket(c6W5100Port, c6W5100Pin, send, sizeof(send));
		startreg++;
	}
}

#endif
