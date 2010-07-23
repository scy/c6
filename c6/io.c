#ifndef c6LoadedIO
#define c6LoadedIO

#include <avr/io.h>
#include <c6/main.c>

#define c6SetOutput(port,pin) \
DDR##port |= (1 << pin);

#endif
