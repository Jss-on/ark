#include "PIC.h"
#include "os_winnt.h"
#include "os_linux.h"
#include <thread>
#include <chrono>
#if defined(_LINUX) && !defined(_KERNEL)
#include "conio_linux.h"
#endif /* Linux */

static bool Returnmean;

#if defined(_LINUX) && !defined(_KERNEL)
void isKBhit()
{
	char c = '\0';

	if (kbhit())
	{
		c = getch();
	}

	switch(tolower(c))
	{
		case 'r':
		{
			Returnmean = true;
			break;
		}
	}
}
#else
void isKBhit()
{
	char c = '\0';

	if (_kbhit())
	{
		c = _getch();
	}

	switch(tolower(c))
	{
		case 'r':
		{
			Returnmean = true;
			break;
		}
	}
}
#endif /* Linux */
void Title()
{
	printf("******************************************************************\n"
	       "**                           PIC DEMO                           **\n"
	       "******************************************************************\n");
}

void page_set_up_status()
{
	uint32_t status = 0;
	uint32_t data = 0;
	uint32_t Max = 0;
	uint32_t Min = 0;

	status = SusiDeviceGetValue(PIC_ID_GET_BAT_LOW_SWITCH, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_BAT_LOW_SWITCH, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_BAT_LOW_SWITCH, &Max);
	printf("1)  Low battery voltage set up:\t %d\t(%d: Disable, %d: Enable) \n", data, Min, Max);

	status = SusiDeviceGetValue(PIC_ID_GET_BAT_TYPE, &data);
	printf("2)  Battery type setting: \t %d\t(0: Auto, 1: 12V, 2: 24V) \n", data);

	status = SusiDeviceGetValue(PIC_ID_GET_BL_12V, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_BL_12V, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_BL_12V, &Max);
	printf("3)  12V low battery voltage:\t %.1f\t(%.1f ~ %.1f V) \n", (double)data / 10, (double)Min / 10, (double)Max / 10);

	status = SusiDeviceGetValue(PIC_ID_GET_BL_24V, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_BL_24V, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_BL_24V, &Max);
	printf("4)  24V low battery voltage:\t %.1f\t(%.1f ~ %.1f V) \n", (double)data / 10, (double)Min / 10, (double)Max / 10);

	status = SusiDeviceGetValue(PIC_ID_GET_BL_DELAY_OFF, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_BL_DELAY_OFF, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_BL_DELAY_OFF, &Max);
	printf("5)  Low battery detection timer: %d\t(%d ~ %d sec) \n", data, Min, Max);

	status = SusiDeviceGetValue(PIC_ID_GET_BL_HARD_OFF, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_BL_HARD_OFF, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_BL_HARD_OFF, &Max);
	printf("6)  Low battery Hard off timer:\t %d\t(%d ~ %d sec) \n", data, Min, Max);

	status = SusiDeviceGetValue(PIC_ID_GET_IGN_DELAY, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_IGN_DELAY, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_IGN_DELAY, &Max);
	printf("7)  Ignition delay on timer:\t %d\t(%d ~ %d sec) \n", data, Min, Max);

	status = SusiDeviceGetValue(PIC_ID_GET_DELAY_OFF, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_DELAY_OFF, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_DELAY_OFF, &Max);
	printf("8)  Ignition delay off timer:\t %d\t(%d ~ %d sec) \n", data, Min, Max);

	status = SusiDeviceGetValue(PIC_ID_GET_HARD_OFF, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_HARD_OFF, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_HARD_OFF, &Max);
	printf("9)  Ignition hard off timer:\t %d\t(%d ~ %d sec) \n", data, Min, Max);

	status = SusiDeviceGetValue(PIC_ID_GET_PWR_RETRIES, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_PWR_RETRIES, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_PWR_RETRIES, &Max);
	printf("10) Power off Retry times:\t %d\t(%d ~ %d times) \n", data, Min, Max);

	status = SusiDeviceGetValue(PIC_ID_GET_PWR_INTERVAL, &data);
	status = SusiDeviceGetValue(PIC_ID_MIN_PWR_INTERVAL, &Min);
	status = SusiDeviceGetValue(PIC_ID_MAX_PWR_INTERVAL, &Max);
	printf("11) Power off Retry timer:\t %d\t(%d ~ %d sec) \n", data, Min, Max);

	printf("12) Reset default setting \n");

	printf("13) Return to menu \n");

	printf("\nEnter choice:");
	if (status != 0)
	{
	}

}
void page_set_up_set(int option)
{
	uint32_t status = 0;
	char s[100];

	printf("\nEnter setting value:");
	int temp = SCANF_IN("%s", s);
	if (temp != 0)
	{
	}
	PIC::wait_enter_key();
	switch(option)
	{
	case 1:
		status = SusiDeviceSetValue(PIC_ID_SET_BAT_LOW_SWITCH, atoi(s));
		break;
	case 2:
		status = SusiDeviceSetValue(PIC_ID_SET_BAT_TYPE, atoi(s));
		break;
	case 3:
		status = SusiDeviceSetValue(PIC_ID_SET_BL_12V, int(atof(s)*10));
		break;
	case 4:
		status = SusiDeviceSetValue(PIC_ID_SET_BL_24V, int(atof(s)*10));
		break;
	case 5:
		status = SusiDeviceSetValue(PIC_ID_SET_BL_DELAY_OFF, atoi(s));
		break;
	case 6:
		status = SusiDeviceSetValue(PIC_ID_SET_BL_HARD_OFF, atoi(s));
		break;
	case 7:
		status = SusiDeviceSetValue(PIC_ID_SET_IGN_DELAY, atoi(s));
		break;
	case 8:
		status = SusiDeviceSetValue(PIC_ID_SET_DELAY_OFF, atoi(s));
		break;
	case 9:
		status = SusiDeviceSetValue(PIC_ID_SET_HARD_OFF, atoi(s));
		break;
	case 10:
		status = SusiDeviceSetValue(PIC_ID_SET_PWR_RETRIES, atoi(s));
		break;
	case 11:
		status = SusiDeviceSetValue(PIC_ID_SET_PWR_INTERVAL, atoi(s));
		break;
	}
	if (status != 0)
	{
	}
	
}
void page_set_up_default()
{
	uint32_t data = 0;
	uint32_t status = 0;

	printf("Setting...");

	status = SusiDeviceGetValue(PIC_ID_DEF_BAT_LOW_SWITCH, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_BAT_LOW_SWITCH, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_BAT_TYPE, &data);
	status = SusiDeviceSetValue(PIC_ID_SET_BAT_TYPE, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_BL_12V, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_BL_12V, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_BL_24V, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_BL_24V, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_BL_DELAY_OFF, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_BL_DELAY_OFF, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_BL_HARD_OFF, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_BL_HARD_OFF, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_IGN_DELAY, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_IGN_DELAY, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_DELAY_OFF, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_DELAY_OFF, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_HARD_OFF, &data);
    status = SusiDeviceSetValue(PIC_ID_SET_HARD_OFF, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_PWR_RETRIES, &data);
	status = SusiDeviceSetValue(PIC_ID_SET_PWR_RETRIES, data);

	SLEEP_USEC(500);

	status = SusiDeviceGetValue(PIC_ID_DEF_PWR_INTERVAL, &data);
	status = SusiDeviceSetValue(PIC_ID_SET_PWR_INTERVAL, data);

	SLEEP_USEC(500);

	if (status != 0)
	{
	}

}
bool PIC::isAvailable()
{
	uint32_t tmp;
    SusiDeviceGetValue(PIC_ID_INFO_AVAILABLE, &tmp);
	return tmp > 0 ? true : false;
}

void PIC::page_status()
{
	uint32_t data = 0;
	uint32_t status = 0;

	int ret = system(CLRSCR);
	if (ret != 0)
	{
	}
	Title();
	printf("Property\n\n");

	status = SusiDeviceGetValue(PIC_ID_FW_VER, &data);
	printf("FW Version:\t V.%d \n", data & 0xFF);

	status = SusiDeviceGetValue(PIC_ID_SWITCH2_PWR_SELECT, &data);
	if (data == 0)
		printf("V/P mode:\t Vehicle mode \n");
	else
		printf("V/P mode:\t PC mode \n");

	status = SusiDeviceGetValue(PIC_ID_FW_BAT_TYPE, &data);
	switch (data)
    {
        case 0:
            printf("Battery type:\t Err \n");
            break;
        case 1:
            printf("Battery type:\t 12V \n");
            break;
        case 2:
            printf("Battery type:\t 24V \n");
            break;
    }

	status = SusiDeviceGetValue(PIC_ID_SWITCH1_CFG_SELECT, &data);
	if (data == 0)
        printf("SW1_4 status:\t SW mode\n");
	else
        printf("SW1_4 status:\t HW mode\n");

	status = SusiDeviceGetValue(PIC_ID_GET_BL_12V, &data);
	printf("12V low battery voltage:\t %.1fV\n", (double) data / 10);

	status = SusiDeviceGetValue(PIC_ID_GET_BL_24V, &data);
	printf("24V low battery voltage:\t %.1fV\n", (double) data / 10);

	status = SusiDeviceGetValue(PIC_ID_GET_BL_DELAY_OFF, &data);
	printf("Low battery detection timer:\t %d sec \n", data);

	status = SusiDeviceGetValue(PIC_ID_GET_BL_HARD_OFF, &data);
	printf("Low battery Hard off:\t\t %d sec \n", data);

	status = SusiDeviceGetValue(PIC_ID_GET_IGN_DELAY, &data);
	printf("Ignition delay on timer:\t %d \n", data);

	status = SusiDeviceGetValue(PIC_ID_GET_DELAY_OFF, &data);
	printf("Ignition delay off timer:\t %d sec \n", data);

	status = SusiDeviceGetValue(PIC_ID_GET_HARD_OFF, &data);
	printf("Ignition hard off timer:\t %d sec \n", data);

	status = SusiDeviceGetValue(PIC_ID_GET_PWR_RETRIES, &data);
	printf("Retry time press power button for power off:\t %d times \n", data);

	status = SusiDeviceGetValue(PIC_ID_GET_PWR_INTERVAL, &data);
	printf("Power button for power off timer:\t\t %d sec \n", data);

	printf("\nPress ENTER to continue.");

	PIC::wait_enter_key();
	if (status != 0)
	{
	}
}

void PIC::page_set_up()
{
	int32_t op;

	while(1)
	{
		int ret = system(CLRSCR);
		if (ret != 0)
		{
		}

		Title();
		printf("Settings\n\n");

		page_set_up_status();

		int temp = SCANF_IN("%d", &op);
		if (temp != 0)
		{
		}
		PIC::wait_enter_key();

		if(op > 0 && op <= 11)
			page_set_up_set(op);
		else if (op == 12)
			page_set_up_default();
		else if (op == 13)
			break;
		else
			printf("\nUnknown choice!");
	}
	
}

void PIC::page_real_time_status()
{
	uint32_t data = 0;
	uint32_t status = 0;

	while(1)
	{
		int ret = system(CLRSCR);
		if (ret != 0)
		{
		}

		Title();
		printf("Real-time status\n\n");

		status = SusiDeviceGetValue(PIC_ID_FW_V48_STATUS, &data);
		if (data == 0)
            printf("48V output:\t off \n");
        else
            printf("48V output:\t on \n");

		status = SusiDeviceGetValue(PIC_ID_FW_IGN_LEVEL, &data);
		if (data == 0)
			printf("Current IGN status:\t off \n");
		else
			printf("Current IGN status:\t on \n");		

		status = SusiDeviceGetValue(PIC_ID_FW_BAT_VOLT, &data);
		printf("Current power input:\t %.1f \n", (double) data * 0.1);

		status = SusiDeviceGetValue(PIC_ID_TIMER_TMR_HARD_OFF, &data);
		printf("Current hard off timer:\t\t %d sec \n", data);

		status = SusiDeviceGetValue(PIC_ID_TIMER_TMR_DELAY_OFF, &data);
		printf("Current Ignition off timer:\t %d sec \n", data);

		status = SusiDeviceGetValue(PIC_ID_TIMER_TMR_BL_DELAY_OFF, &data);
		printf("Current low battery detection timer:\t %d sec \n", data);

		status = SusiDeviceGetValue(PIC_ID_TIMER_PWR_OFF_RETRIES, &data);
		printf("Current retry times Press power button for power off:\t %d times\n", data);

		status = SusiDeviceGetValue(PIC_ID_TIMER_PWR_OFF_INTERVAL, &data);
		printf("Current retry timer Press power button for power off:\t %d sec \n", data);		

		printf("\nPress [r] return to menu\n");

		int time = 0;
		while (time < 10)
		{
			isKBhit();
			SLEEP_USEC(100);
			time++;
		}
		if(Returnmean)
		{
			Returnmean = false;
			break;
		}
		if (status != 0)
		{
		}
	}
}

void PIC::wait_enter_key()
{
    int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

