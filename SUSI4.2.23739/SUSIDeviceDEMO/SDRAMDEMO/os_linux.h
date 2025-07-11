#ifndef _OS_LINUX_H_
#define _OS_LINUX_H_

#if defined(_LINUX) && !defined(_KERNEL)

#include <stdlib.h>
#define CLRSCR "clear"

#include <stdio.h>
#define SCANF_IN(format, pvalue)	scanf(format, pvalue)
#define scanf_s(format, pvalue)	    scanf(format, pvalue)

#include <unistd.h>
#define SLEEP_USEC(ms)				usleep(1000 * (unsigned long)ms)



#endif /* Linux */

#endif /* _OS_LINUX_H_ */
