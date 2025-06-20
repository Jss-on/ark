#include "common.h"

int clr_screen(void)
{
    return system(CLRSCR);
}

void waitKey()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}