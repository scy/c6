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

// Initialize a W5100. Pass the last 4 bytes of the desired MAC address. The
// first two ones are fixed to 02:c6. These are the steps:
// 1. Reset the chip.
// 2. Assign full 8 KB of RX buffer to socket 0 (raw ethernet).
// 3. Assign full 8 KB of TX buffer to socket 0 (raw ethernet).
// 4. Set socket 0 to raw ethernet mode (also called MACRAW).
// 5. Activate socket 0.
// 6. Write last 4 bytes of MAC.
// 7. Write first 2 (constant) bytes of MAC.
#define c6W5100Init(mac1,mac2,mac3,mac4) \
{ uint8_t send[4] = { (1 << 7), mac2, mac3, mac4 }; \
c6W5100Write(0x0000, send, 1); \
send[0] = 0b00000011; \
c6W5100Write(0x001a, send, 1); \
c6W5100Write(0x001b, send, 1); \
send[0] = 0b00000100; \
c6W5100Write(0x0400, send, 1); \
send[0] = 0x01; \
c6W5100Write(0x0401, send, 1); \
send[0] = mac1; \
c6W5100Write(0x000b, send, 4); \
send[0] = 0x02; send[1] = 0xc6; \
c6W5100Write(0x0009, send, 2); \
}

#endif
