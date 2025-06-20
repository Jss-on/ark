#include "SmartBattery.h"
#include "os_winnt.h"
#include "os_linux.h"
#include "SusiDeviceAPI.h"
#include <string>

void printTitle()
{
	printf("******************************************************************\n"
		"**                     Smart Battery DEMO                       **\n"
		"******************************************************************\n");
}

bool SmartBattery::isAvailable()
{
	uint32_t status, tmp;
	status = SusiDeviceGetValue(SBS_ID_BASE, &tmp);
	return status == SUSI_STATUS_SUCCESS ? true : false;
}

void SmartBattery::GetBatteryInfo()
{
	uint32_t data = 0 ,charge, errorcode;
	short current = 0;
	uint32_t status = 0;
	std::string Unit = "";

	status = SusiDeviceGetValue(SBS_ID_unit, &data);
	Unit = (data > 0 ? "0 mWh" : " mAh");

	status = SusiDeviceGetValue(SBS_ID_Current, &data);
	current = data;

	status = SusiDeviceGetValue(SBS_ID_BatteryStatus, &data);
	charge = (data & 0x40) >> 6;
	if (current == 0)
		printf("\nBattery Status:\t\t\t AC Power \n");
	else
		printf("\nBattery Status:\t\t\t %s \n", charge>0? "Discharging" : "Charging");

	status = SusiDeviceGetValue(SBS_ID_ManufacturerName1, &data);
	printf("Manufacturer Name:\t\t %c%c%c%c", data >> 24, data >> 16, data >> 8, data);
	
	status = SusiDeviceGetValue(SBS_ID_ManufacturerName2, &data);
	printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);
	
	status = SusiDeviceGetValue(SBS_ID_ManufacturerName3, &data);
	printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);
	
	status = SusiDeviceGetValue(SBS_ID_ManufacturerName4, &data);
	printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);
	
	status = SusiDeviceGetValue(SBS_ID_ManufacturerName5, &data);
	printf("%c%c%c%c \n", data >> 24, data >> 16, data >> 8, data);

	status = SusiDeviceGetValue(SBS_ID_ManufacturerDate, &data);
	printf("Manufacture Date:\t\t %d/%d/%d \n", (data >> 9) + 1980, (data >> 5) & 0xF, data & 0x01F);

	status = SusiDeviceGetValue(SBS_ID_SerialNumber, &data);
	printf("Serial Number:\t\t\t 0x%04X \n", data);

	status = SusiDeviceGetValue(SBS_ID_DesignVoltage, &data);
	printf("Design Voltage:\t\t\t %d mV \n", data);

	status = SusiDeviceGetValue(SBS_ID_DesignCapacity, &data);
	printf("Design Capacity:\t\t %d %s \n", data, Unit.c_str());

	status = SusiDeviceGetValue(SBS_ID_Voltage, &data);
	printf("Voltage:\t\t\t %d mV \n", data);

	printf("Current:\t\t\t %d mA \n", current);

	status = SusiDeviceGetValue(SBS_ID_RelativeStateOfCharge, &data);
	printf("Relative State Of Charge:\t %d %%\n", data);

	status = SusiDeviceGetValue(SBS_ID_AbsoluteStateOfCharge, &data);
	printf("Absolute State Of Charge:\t %d %%\n", data);

	status = SusiDeviceGetValue(SBS_ID_SoH, &data);
	printf("SoH:\t\t\t\t %d %%\n", data);

	status = SusiDeviceGetValue(SBS_ID_FullChargeCapacity, &data);
	printf("Full Charge Capacity:\t\t %d %s \n", data, Unit.c_str());

	status = SusiDeviceGetValue(SBS_ID_RemainingCapacity, &data);
	printf("Remaining Capacity:\t\t %d %s \n", data, Unit.c_str());

	status = SusiDeviceGetValue(SBS_ID_AverageTimeToEmpty, &data);
	if (data == 65535 || current == 0)
		printf("Average Time To Empty:\t\t --\n");
	else
		printf("Average Time To Empty:\t\t %d h %d m\n", data/60, data%60);

	status = SusiDeviceGetValue(SBS_ID_AverageTimeToFull, &data);
	if (data == 65535 || current == 0)
		printf("Average Time To Full:\t\t --\n");
	else
		printf("Average Time To Full:\t\t %d h %d m\n", data / 60, data % 60);

	status = SusiDeviceGetValue(SBS_ID_CycleCount, &data);
	printf("Cycle Count:\t\t\t %d cycles \n", data);

	status = SusiDeviceGetValue(SBS_ID_Temperature, &data);
	printf("Temperature:\t\t\t %.2f Celsius \n", (data * 0.10) - 273.15);

	status = SusiDeviceGetValue(SBS_ID_BatteryStatus, &data);
	errorcode = data & 0x0000000F;
	printf("Error Codes:\t\t\t 0x0%X \n", errorcode);

	printf("\nPress ENTER to continue. ");
	SmartBattery::wait_enter_key();
	if (status != 0)
	{
	}
}

void SmartBattery::wait_enter_key()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int main()
{
	uint32_t op = 0;
	if (SmartBattery::isAvailable() == false)
	{
		printf("Device is not Available.\n");
		printf("Exit the Program...\n");
		return -1;
	}

	while (1)
	{
#if defined(WIN32) && !defined(WINCE) || defined(_WIN64)
		system(CLRSCR);
#endif

		printTitle();
		printf("\n0) Terminate this program "
			"\n1) Battery Information ");

		printf("\n\nEnter your choice:\t");
		if (SCANF_IN("%d", &op) <= 0)
			op = -1;

		SmartBattery::wait_enter_key();

		if (op == 0)
		{
			printf("Exit the program...\n");
			break;
		}

		if (op > 1 )
		{
			op = -1;
			printf("Unknown choice!\n");
			printf("Press ENTER to continue. ");
			SmartBattery::wait_enter_key();
			continue;
		}

		SmartBattery::GetBatteryInfo();
	}
	return 0;
}

