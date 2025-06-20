#include "os_linux.h"
#include "PIC.h"
#include "SusiDeviceAPI.h"
#include "conio_linux.h"

void printTitle()
{
	printf("******************************************************************\n"
	       "**                           PIC DEMO                           **\n"
	       "******************************************************************\n");
}

void printMenu()
{
	printf("\n"
		   "0) Terminate this program\n"
		   "1) Property\n"
		   "2) Settings\n"
		   "3) Real-time status\n\n"
	       "Enter your choice:");
}

int main()
{
	int32_t op;
	
	if (PIC::isAvailable() == false)
	{
		printf("Device is not Available.\n");
		printf("Exit the Program...\n");
		return -1;
	}
	
	while (1)
	{
		int ret = system(CLRSCR);
		if (ret != 0)
		{
			std::exit(EXIT_SUCCESS);
		}
		printTitle();
		printMenu();
		
		if (SCANF_IN("%d", &op) <= 0)
			op = -1;

		PIC::wait_enter_key();

		if (op == 0)
		{
			printf("Exit the program...\n");
			break;
		}

		switch (op)
		{
		case 1:
			PIC::page_status();
			break;
		case 2:
			PIC::page_set_up();
			break;
		case 3:
			PIC::page_real_time_status();
			break;
		}
		if (op < 1 || op > PICDEMO_PAGE_MAX)
		{
			op = -1;
			printf("Unknown choice!\n");
			printf("Press ENTER to continue. ");
			PIC::wait_enter_key();
			continue;
		}
	}
	return 0;
}
