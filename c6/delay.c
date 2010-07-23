#ifndef c6LoadedDelay
#define c6LoadedDelay

#include <c6/main.c>
#include <util/delay.h>

#define c6DelayUS(us) _delay_us(us);
#define c6DelayMS(ms) _delay_ms(ms);

#endif
