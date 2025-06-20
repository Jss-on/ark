#include "common.h"

struct tmpthermalctrl {
	std::string name;
	uint32_t ID;	
	uint32_t choice; 
};

static struct tmpthermalctrl page_tbl[] = {
	{"Board  1",	SAB2000_ID_HWM_TEMP_ALERT_BT1,		0},
	{"Board  2",	SAB2000_ID_HWM_TEMP_ALERT_BT2,		0},
	{"Board  3",	SAB2000_ID_HWM_TEMP_ALERT_BT3,		0},
	{"Board  4",	SAB2000_ID_HWM_TEMP_ALERT_BT4,		0},
};

static uint8_t options2pages = NELEMS(page_tbl);

static std::string tmpNames[] =
{
	"CPU1",                 // 0
	"CPU2",                 // 1
	"M/B",                  // 2
	"Board Temperature 1",  // 3
	"Board Temperature 2",  // 4
	"Board Temperature 3",  // 5
	"Board Temperature 4",  // 6
};



static std::string fanNames[] =
{
	"FAN0",                 // 0
	"FAN1",                 // 1
	"FAN2",                 // 2
	"OnBoard FAN1",         // 3
	"OnBoard FAN2",         // 4
	"OnBoard FAN3",         // 5
	"OnBoard FAN4",         // 6
	"OnBoard FAN5",         // 7
	"OnBoard FAN6",         // 8
	"OnBoard FAN7",         // 9
};

void title()
{
	printf("******************************************************************\n"
		"**                           SAB2000 DEMO                       **\n"
		"******************************************************************\n");

	
}

bool isAvailable()
{
	uint32_t tmp;
    SusiDeviceGetValue(SAB2000_ID_DEVICE_AVAILABLE, &tmp);
	return tmp > 0 ? true : false;
}

void GetStatusStr(uint32_t Value)
{
	switch (Value & 0x07)
    {
   		case 0:
      		printf("Dark\n");
			break;
			
		case 1:
			printf("Green\n");
			break;

		case 2:
			printf("Red\n");
			break;

		case 5:
			printf("Green Blink\n");
			break;

		case 6:
			printf("Red Blink\n");
			break;
		
		default:
			printf("Unknown\n");
			break;
	}
}

void printInformation()
{
	uint32_t u32tmp;
	
	printf("[Information]\n");
	if (SusiDeviceGetValue(SAB2000_ID_FW_VER, &u32tmp) == 0)
	{
		printf("Firmware Version: %c%x.%x \n", (char)(u32tmp >> 16) , (u32tmp >> 8)  & 0xFF, u32tmp & 0xFF);
	}	
	
	if (SusiDeviceGetValue(SAB2000_ID_KERNEL_VER, &u32tmp) == 0)
	{
		printf("Kernel Version: %c%x.%x\n", (char)(u32tmp >> 16) , (u32tmp >> 8)  & 0xFF, u32tmp & 0xFF);
	}		

	if (SusiDeviceGetValue(SAB2000_ID_EC_TYPE, &u32tmp) == 0)
	{
		printf("Chip Type: %c%.2x.%u\n",(char)(u32tmp >> 16) , (u32tmp >> 8)  & 0xFF, u32tmp & 0xFF);
	}
	printf("\n");		
}

int printfAlertInfo()
{
	uint32_t u32tmp;
	uint8_t i;
	
	SLEEP_USEC(200);
 
	if (SusiDeviceGetValue(SAB2000_ID_CTRL_ALERT, &u32tmp) == 0)
	{
		printf("%-10s : ", "Alert");

		if(u32tmp == 0)
			printf("Disable\n");
		else
			printf("Enable\n");
	}

	for(i = 0 ; i < options2pages; i++)
	{
		if (page_tbl[i].choice == 1)
		{
			printf("%-10s : %-10s\n", "Source", page_tbl[i].name.c_str());

			if (SusiDeviceGetValue(page_tbl[i].ID, &u32tmp) == 0)
				printf("%-10s : %u\n", "Tempature", SUSI_DECODE_CELCIUS(u32tmp));
	
		}

	}

	SLEEP_USEC(500);
	printf("\n");

	if (SusiDeviceGetValue(SAB2000_ID_LED_POWER, &u32tmp) == 0)
	{
		printf("%-10s : ", "Power LED");
		GetStatusStr(u32tmp);
	}

	if (SusiDeviceGetValue(SAB2000_ID_LED_TEMP, &u32tmp) == 0)
	{
		printf("%-10s : ","Temp  LED");
		GetStatusStr(u32tmp);
	}

	if (SusiDeviceGetValue(SAB2000_ID_LED_FAN, &u32tmp) == 0)
	{
		printf("%-10s : ","Fan   LED");
		GetStatusStr(u32tmp);
	}

	return 0;
}

int SetAlertEnable()
{
	uint32_t u32tmp = 0;

	printf("Set Alert (Enable/Disable)\n\n");
	printf("0)Disable\n");
	printf("1)Enable\n");
	printf("Press number to continue : ");

	if (SCANF_IN("%d", &u32tmp) <= 0)			
		return -1;

	if (SusiDeviceSetValue(SAB2000_ID_CTRL_ALERT, u32tmp) == 0)
	{
	  if(u32tmp == 0)
	  	printf("Set Alert Disable\n");
	  else
	  	printf("Set Alert Enable\n");
	}
	
	return 0;
}

int SetAlertControl()
{
	uint8_t i;
	uint32_t temp, value; 

	printf("Source Tempuature List \n");

	for(i = 0 ; i < options2pages; i++)
		printf("%d) %-20s \n", i, page_tbl[i].name.c_str());
	
	printf("Press Soure Tempuature :");

	if(SCANF_IN("%d", &temp) <= 0)
		return -1;

	if(temp >=0 && temp < options2pages)
	{
		for(i = 0 ; i < options2pages; i++)
		{
			if (i == temp)
				page_tbl[i].choice = 1;
			else
				page_tbl[i].choice = 0;
		}	
	}
	else
	{
		printf("Unknown choice!\n");
		return -1;
	}
	
	printf("Press Temperature  : ");
	
	if(SCANF_IN("%d", &value) <= 0)
		return -1;

	if(value >= 0 && value <= 128)
	{
		for(i = 0 ; i < options2pages; i++)
		{
			if (page_tbl[i].choice == 1)
			{
				if (SusiDeviceSetValue(page_tbl[i].ID, SUSI_ENCODE_CELCIUS(value)) != 0)
				{
					printf("Unknown choice!\n");
					return -1;
				}
			}
			
		}
		
	}

	return 0;
}

int printAlert()
{
	int number;
	
	printf("[Status]\n");
	printfAlertInfo();
	printf("\n");
	
	printf("0) Exit\n");
	printf("1) Set Alert (Enable/Disable)\n");
	printf("2) Set Thermal Alert Control\n");
	printf("3) Refresh Status\n\n");

	printf("Press number to continue : ");
 	
	if (SCANF_IN("%d", &number) <= 0)			
		return -1;

	switch(number)
	{
		case 0:
			return -1;
			break;

		case 1:
			while(true)
			{	
				clr_screen();
				title();

				if(SetAlertEnable() == 0)
					break;
				else
					goto EXIT;
			}
			break;

		case 2:
			while(true)
			{	
				clr_screen();
				title(); 

				if(SetAlertControl() == 0)
					break;
				else
					goto EXIT;
			}
			break;

		case 3:
			while(true)
			{	
				clr_screen();
				title(); 

				if(printAlert()== -1)			
				  return -1;							
			}
			break;
EXIT:
		default:
			printf("Unknown choice!\n");
			break;
	}

return 0;	
}

void printData()
{
	uint32_t u32tmp;
	
	printf("[Temperature]\n");
	for (int i = 3; i < TEMP_ID_MAX; i++)
	{
		if (SusiDeviceGetValue(SAB2000_ID_HWM_TEMP_BASE + i, &u32tmp) == 0)
		{
			printf("%-20s: %7.1f C\n", tmpNames[i].c_str(), (u32tmp - SUSI_KELVINS_OFFSET) / 10.0);
		}
	}		
	printf("\n");

	printf("[Fan Speed]\n");
	for (int i = 3; i < FAN_ID_MAX; i++)
	{
		if (SusiDeviceGetValue(SAB2000_ID_HWM_FAN_BASE + i, &u32tmp) == 0)
		{
			printf("%-20s: %5d RPM\n", fanNames[i].c_str(), u32tmp);
		}
	}
}

int main()
{
	int number;

	if (isAvailable() == false)
	{
		printf("Device is not Available.\n");
		printf("Exit the Program...\n");
		return -1;
	}
  	
	while(true)
	{
		clr_screen();
		title();

		printInformation();
		printf("\n");
		printf("[Menu]\n");
		printf("0) Exit \n");
		printf("1) HWM\n");
		printf("2) Alert\n\n");
		printf("Press ENTER number to continue : ");
		
		if (SCANF_IN("%d", &number) <= 0)			
			return -1;

		switch(number)
		{
			case 0:
				return -1;
				break;	

			case 1:
				while (true)
				{
					clr_screen();
					title();
					printData();
					printf("Press ENTER to continue or Ctrl + C to exit");
					waitKey();
				}
				break;

			case 2:
				while (true)
				{
					clr_screen();
					title();

					if(printAlert() == -1)
						break;
				}		
				break;

			default:
				printf("Unknown choice!\n");
				break;
		}
	}	

	return 0;
}
