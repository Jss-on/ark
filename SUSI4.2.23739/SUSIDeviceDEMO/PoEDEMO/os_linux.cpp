#include "os_linux.h"
#include "Port.h"
#include "SusiDeviceAPI.h"
#include "conio_linux.h"

int numPorts;
Port ports[PORT_MAX_NUM];

void printTitle()
{
	printf("******************************************************************\n"
	       "*                            PoE DEMO                            *\n"
	       "******************************************************************\n");
}

void printMenu()
{
	printf("\n\n"
	       "Press [C] to Control Power\n"
	       "Press [E] to Exit Program...\n");
}

void powerControl()
{
	char num = '\0', power = '\0';
	int numToInt = 0;

	while (numToInt < 1 || numToInt > numPorts)
	{
		printf("\r Choice the Port Number [1-%d] or [B] to back: ", numPorts);
		num = getch();
		numToInt = (num - '0');

		if (tolower(num) == 'b')
			return;
	}

	printf("\n");

	while (tolower(power) != 'n' && tolower(power) != 'f')
	{
		printf("\r Power ON/OFF [N/F] or [B] to back: ");
		power = getch();
		
		if (tolower(power) == 'b')
			return;

		int toEnable = (tolower(power) == 'n')? POWER_ON: POWER_OFF;
		ports[numToInt - 1].enablePort(toEnable);
	}
}

void isKBhit()
{
	char c = '\0';

	if (kbhit())
	{
		c = getch();
	}

	switch(tolower(c))
	{
		case 'c':
		{
			powerControl();
			break;
		}
		case 'e':
		{
			std::exit(EXIT_SUCCESS);
			break;
		}
		default:
			break;
	}
}

int main()
{
	if (Port::isAvailable() == false)
	{
		printf("Device is not Available.\n");
		printf("Exit the Program...\n");
		return -1;
	}

	numPorts = 0;
	for (uint8_t i = 0; i < PORT_MAX_NUM; i++)
	{	
		if (Port::getCap(i+1))
		{
			ports[i] = Port(i + 1);
			numPorts++;
		}
		else
		{
			break;
		}
	}

	while(1)
	{
		int ret = system(CLRSCR);
		if (ret != 0)
		{
			std::exit(EXIT_SUCCESS);
		}

		printTitle();

		for (int g = 0; g < (numPorts / GROUP_MAX_PORT); g++)
		{
			int base = g * GROUP_MAX_PORT;

			printf("\n%-10s", "Port:");
			for (int i = 0; i < GROUP_MAX_PORT; i++)
			{
				printf("Port%-11d", ports[base + i].getPortNum());
			}

			printf("\n%-10s", "Power:");
			for (int i = 0; i < GROUP_MAX_PORT; i++)
			{
				ports[base + i].getPower();
				printf("%-15s", ports[base + i].getPowerString().c_str());
			}

			printf("\n%-10s", "Detect:");
			for (int i = 0; i < GROUP_MAX_PORT; i++)
			{
				printf("%-15s", ports[base + i].getDetectionString().c_str());
			}

			printf("\n%-10s", "Class:");
			for (int i = 0; i < GROUP_MAX_PORT; i++)
			{
				printf("%-15s", ports[base + i].getClassString().c_str());
			}

			printf("\n%-10s", "Voltage:");
			for (int i = 0; i < GROUP_MAX_PORT; i++)
			{
				printf("%-6.2lf", ports[base + i].getVoltage());
				printf("%-9s", "V");
			}

			printf("\n%-10s", "Current:");
			for (int i = 0; i < GROUP_MAX_PORT; i++)
			{
				printf("%-6.2lf", ports[base + i].getCurrent());
				printf("%-9s", "mA");
			}

			printf("\n");
		}	

		printMenu();

		int time = 0;
		while (time < 10)
		{
			isKBhit();
			SLEEP_USEC(100);
			time++;
		}
	}

	return 0;
}
