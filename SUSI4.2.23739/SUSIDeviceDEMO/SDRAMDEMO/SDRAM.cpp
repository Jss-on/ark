#include "SPD.h"
#include "os_winnt.h"
#include "os_linux.h"
#include "SusiDeviceAPI.h"

bool isAdvantech = false;
void printTitle()
{
	printf("******************************************************************\n"
		"**                         SDRAM DEMO                           **\n"
		"******************************************************************\n");
}

std::string SPD::MemoryType(uint8_t dev)
{
	typedef struct _MemoryTypeInfo {
		uint8_t index;
		std::string Name;
	} MemoryTypeInfo;
	
	const MemoryTypeInfo MemoryTypeInfoTable[] = {
		{ 0x00,"Reserved" },
		{ 0x01,"Standard FPM DRAM" },
		{ 0x02,"EDO" },
		{ 0x03,"Pipelined Nibble" },
		{ 0x04,"SDRAM" },
		{ 0x05,"ROM" },
		{ 0x06,"DDR SGRAM" },
		{ 0x07,"DDR SDRAM" },
		{ 0x08,"DDR2 SDRAM" },
		{ 0x09,"DDR2 SDRAM FB-DIMM" },
		{ 0x0A,"DDR2 SDRAM FB-DIMM PROBE" },
		{ 0x0B,"DDR3 SDRAM" },
		{ 0x0C,"DDR4 SDRAM" },
		{ 0x12,"DDR5 SDRAM" }
	};

	for (int i =0; i < 14 ; i++)
	{
		if (MemoryTypeInfoTable[i].index == dev)
		{
			return MemoryTypeInfoTable[i].Name;
		}
	}
	return "Unknown";
}

std::string SPD::DDR3ModuleType(uint8_t dev)
{
	typedef struct _ModuleTypeInfo {
		uint8_t index;
		std::string Name;
	} ModuleTypeInfo;

	const ModuleTypeInfo ModuleTypeInfoTable[] = {
		{ 0x00,"Undefined" },
		{ 0x01,"RDIMM" },
		{ 0x02,"UDIMM" },
		{ 0x03,"SO-DIMM" },
		{ 0x04,"Micro-DIMM" },
		{ 0x05,"Mini-RDIMM" },
		{ 0x06,"Mini-UDIMM" },
		{ 0x07,"Mini-CDIMM" },
		{ 0x08,"72b-SO-UDIMM" },
		{ 0x09,"72b-SO-RDIMM" },
		{ 0x0A,"72b-SO-CDIMM" },
		{ 0x0B,"LRDIMM" }
	};

	for (int i = 0; i < 12; i++)
	{
		if (ModuleTypeInfoTable[i].index == dev)
		{
			return ModuleTypeInfoTable[i].Name;
		}
	}
	return "Unknown";
}

std::string SPD::DDR4ModuleType(uint8_t dev)
{
	typedef struct _ModuleTypeInfo {
		uint8_t index;
		std::string Name;
	} ModuleTypeInfo;

	const ModuleTypeInfo ModuleTypeInfoTable[] = {
		{ 0x00,"Undefined" },
		{ 0x01,"RDIMM" },
		{ 0x02,"UDIMM" },
		{ 0x03,"SO-DIMM" },
		{ 0x04,"LRDIMM" },
		{ 0x05,"Mini-RDIMM" },
		{ 0x06,"Mini-UDIMM" },
		{ 0x07,"Reserved" },
		{ 0x08,"72b-SO-RDIMM" },
		{ 0x09,"72b-SO-UDIMM" },
		{ 0x0A,"Reserved" },
		{ 0x0B,"Reserved" },
		{ 0x0C,"16b-SO-DIMM" },
		{ 0x0D,"32b-SO-DIMM" },
		{ 0x0E,"Reserved" },
		{ 0x0F,"Reserved" }
	};

	for (int i = 0; i < 16; i++)
	{
		if (ModuleTypeInfoTable[i].index == dev)
		{
			return ModuleTypeInfoTable[i].Name;
		}
	}
	return "Unknown";
}

std::string SPD::DDR5ModuleType(uint8_t dev)
{
	typedef struct _ModuleTypeInfo {
		uint8_t index;
		std::string Name;
	} ModuleTypeInfo;

	const ModuleTypeInfo ModuleTypeInfoTable[] = {
		{ 0x00,"Undefined" },
		{ 0x01,"RDIMM" },
		{ 0x02,"UDIMM" },
		{ 0x03,"SO-DIMM" },
		{ 0x04,"LRDIMM" },
		{ 0x05,"Undefined" },
		{ 0x06,"Undefined" },
		{ 0x07,"MRDIMM" },
		{ 0x08,"Undefined" },
		{ 0x09,"Undefined" },
		{ 0x0A,"DDIMM" },
		{ 0x0B,"Solder down" },
		{ 0x0C,"Reserved" },
		{ 0x0D,"Reserved" },
		{ 0x0E,"Reserved" },
		{ 0x0F,"Reserved" }
	};

	for (int i = 0; i < 16; i++)
	{
		if (ModuleTypeInfoTable[i].index == dev)
		{
			return ModuleTypeInfoTable[i].Name;
		}
	}
	return "Unknown";
}

bool SPD::isAvailable()
{
	uint32_t tmp;
	SusiDeviceGetValue(SPD_ID_DRAM_QTY, &tmp);
	return tmp > 0 ? true : false;
}

void SPD::GetDRAMQTY(uint32_t *qty)
{
	uint32_t status = 0;
	status = SusiDeviceGetValue(SPD_ID_DRAM_QTY, qty);
	if (status != 0)
	{
	}
}

void SPD::DRAM_Info(uint8_t socket)
{
	uint32_t data = 0;
	//double temperature = 0.0;
	uint32_t DRAMTYPE = 0;
	uint32_t status = 0;
	std::string temp;


	status = SusiDeviceGetValue(SPD_ID_DRAM_PARTNUMBER1(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
	{
		printf("Part Number:\t\t %c%c%c%c", data >> 24, data >> 16, data >> 8, data);
		
		if (data == 1397838381)
		{
			isAdvantech = true;
		}
		else
			isAdvantech = false;

		SusiDeviceGetValue(SPD_ID_DRAM_PARTNUMBER2(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_PARTNUMBER3(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_PARTNUMBER4(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_PARTNUMBER5(socket), &data);
		printf("%c%c%c%c \n", data >> 24, data >> 16, data >> 8, data);
	}
	else if (status == SUSI_STATUS_READ_ERROR) 
	{
		printf("Part Number:\t\t ---- \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_TYPE(socket), &DRAMTYPE);
	if (status == SUSI_STATUS_SUCCESS)
	{
		temp = MemoryType(DRAMTYPE);
		printf("Memory Type:\t\t %s \n", temp.c_str());
	}
	else if (status == SUSI_STATUS_READ_ERROR) 
	{
		printf("Memory Type:\t\t ----  \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_SPEED(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
		printf("Memory Speed:\t\t %d (MT/s)\n", data);
	else if (status == SUSI_STATUS_READ_ERROR)
	{
		printf("Memory Speed:\t\t ----  \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_MODULETYPE(socket), &data);
	if (DRAMTYPE == 12)
	{
		temp = DDR4ModuleType(data);
		printf("Module Type:\t\t %s \n", temp.c_str());
	}
	else if (DRAMTYPE == 11)
	{
		temp = DDR3ModuleType(data);
		printf("Module Type:\t\t %s \n", temp.c_str());
	}
	else if (DRAMTYPE == 0x12) 
	{
		if (status == SUSI_STATUS_READ_ERROR)
		{
			printf("Module Type:\t\t ----  \n");
		}
		else
		{
			temp = DDR5ModuleType(data);
			printf("Module Type:\t\t %s \n", temp.c_str());
		}
	}
	else
	{
		
		printf("Module Type:\t\t Unknown \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_SIZE(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
		printf("Module Size:\t\t %d GB\n", data);
	else if (status == SUSI_STATUS_READ_ERROR)
	{
		printf("Module Size:\t\t ---- \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_WEEKYEAR(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
		printf("Week-Year:\t\t %02x-%02x \n", data >> 8, data & 0xFF);
	else if (status == SUSI_STATUS_READ_ERROR) 
	{
		printf("Week-Year:\t\t ---- \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_RANK(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
		printf("Rank:\t\t\t %d \n", data);
	else if (status == SUSI_STATUS_READ_ERROR)
	{
		printf("Rank:\t\t\t ---- \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_BANK(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
		printf("Bank:\t\t\t %d \n", data);
	else if (status == SUSI_STATUS_READ_ERROR) 
	{
		printf("Bank:\t\t ---- \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_VOLTAGE(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
		printf("Voltage:\t\t %.2f V\n", data / 1000.0);
	else if (status == SUSI_STATUS_READ_ERROR)
	{
		printf("Voltage:\t\t ---- \n");
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_TEMPERATURE(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
	{
		
		printf("Temperature:\t\t %.2f Celsius \n", (data / 100.0) - 273.15);
	}
	else if (status == SUSI_STATUS_READ_ERROR)
	{
		printf("Temperature:\t\t ---- \n");
	}
	else
	{
		printf("Temperature:\t\t No sensor to detect \n");
	}

	if (isAdvantech)
	{
		status = SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA1(socket), &data);
		if (status == SUSI_STATUS_SUCCESS)
		{
			printf("Specific Data:\t\t %c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA2(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA3(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA4(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA5(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA6(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

		SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA7(socket), &data);
		printf("%c%c%c%c", data >> 24, data >> 16, data >> 8, data);

			SusiDeviceGetValue(SPD_ID_DRAM_SPECIFICDATA8(socket), &data);
			printf("%c%c%c%c \n", data >> 24, data >> 16, data >> 8, data);
		}
		else if (status == SUSI_STATUS_READ_ERROR)
		{
			printf("Specific Data:\t\t  ---- \n");
		}
	}

	status = SusiDeviceGetValue(SPD_ID_DRAM_MANUFACTURE(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
	{
		temp = SPD::GetManufacture(data);
		printf("Module Manufacture:\t %s \n", temp.c_str());
	}
	else if (status == SUSI_STATUS_READ_ERROR)
	{
		printf("Module Manufacture:\t ---- \n");
	}


	status = SusiDeviceGetValue(SPD_ID_DRAM_DRAMIC(socket), &data);
	if (status == SUSI_STATUS_SUCCESS)
	{
		temp = SPD::GetManufacture(data);
		printf("DRAM Manufacture:\t %s \n", temp.c_str());
	}
	else if (status == SUSI_STATUS_READ_ERROR)
	{
		printf("DRAM Manufacture:\t ---- \n");
	}


	
	if (isAdvantech)
	{
		status = SusiDeviceGetValue(SPD_ID_DRAM_WRITEPROTECTION(socket), &data);
		if (status == SUSI_STATUS_SUCCESS)
		{
			if ((data & 0xFFFF) == 0)
			{
				printf("Write Protection:\t Disable \n");
			}
			else if ((data & 0xFFFF) == 0x5750)
			{
				printf("Write Protection:\t Enable \n");
			}
			else
			{
				printf("Write Protection:\t Unknown \n");
			}
		}
		else if (status == SUSI_STATUS_READ_ERROR)
		{
			printf("Write Protection:\t ---- \n");
		}
	}

	printf("\nPress ENTER to continue. ");
	SPD::wait_enter_key();
	if (status != 0)
	{
	}
}

void SPD::wait_enter_key()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int main()
{
	uint32_t op = 0;
	uint32_t socket = 0;

	if (SPD::isAvailable() == false)
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

		SPD::GetDRAMQTY(&socket);
		if (socket == 0)
		{
			printf("Not SDRAM to detect.\nsocket: %d \n", socket);
			break;
		}
		else
		{
			printTitle();
			printf("\n"
				"Select Socket (1-%u) or 0 to exit.", socket);

			printf("\nEnter your choice:\t");
			if (SCANF_IN("%d", &op) <= 0)
				op = -1;

			SPD::wait_enter_key();

			if (op == 0)
			{
				printf("Exit the program...\n");
				break;
			}

			if (op < 1 || op > socket)
			{
				op = -1;
				printf("Unknown choice!\n");
				printf("Press ENTER to continue. ");
				SPD::wait_enter_key();
				continue;
			}
			SPD::DRAM_Info(op - 1);
		}
	}
	return 0;
}