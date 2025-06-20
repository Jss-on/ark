#include <string>
#include "SAB2000.h"
#include "SusiDeviceAPI.h"

#if defined(_LINUX) && !defined(_KERNEL)
#include "os_linux.h"
#else
#include "os_winnt.h"
#endif

#ifndef SUSI_API
#define SUSI_API __stdcall
#endif

#define TEMP_ID_MAX         7
#define FAN_ID_MAX          10

#define SUSI_KELVINS_OFFSET						2731

#define SUSI_ENCODE_CELCIUS(Celsius)			(((Celsius) * 10) + SUSI_KELVINS_OFFSET)
#define SUSI_DECODE_CELCIUS(Celsius)			(((Celsius) - SUSI_KELVINS_OFFSET) / 10)

#define NELEMS(_array)	(uint32_t)(sizeof(_array) / sizeof(_array[0]))


int clr_screen(void);
void waitKey();

