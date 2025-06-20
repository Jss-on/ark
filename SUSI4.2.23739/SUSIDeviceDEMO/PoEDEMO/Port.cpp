#include "Port.h"
#include "os_winnt.h"
#include "os_linux.h"

std::string detectStatus[] = {
	"Unknown", "PD Error", "PD Error", "PD Error", "Detected Good", "PD Error", "Detect Open",
	"PD Error",
};

std::string classStatus[] = {
	"Class Unknown", "Class 1", "Class 2", "Class 3", "Class 4", "Error", "Class 0", "OverCurrent",
};

Port::Port()
: _portNum(0)
{
}

Port::Port(uint32_t portNum)
: _portNum(portNum), _portPower(0)
{
	getPower();
}

Port::~Port()
{
}

bool Port::isAvailable()
{
	uint32_t tmp = 0;
	SusiDeviceGetValue(POE_ID_INFO_AVAILABLE, &tmp);
	return tmp > 0 ? true : false;
}

bool Port::getCap(uint8_t portNum)
{
	uint32_t tmp = 0;
	uint32_t status = SusiDeviceGetValue(POE_ID_CAP_PORT(portNum), &tmp);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("getCap Failed. (0x%08X)", status);
		return false;
	}
	return tmp > 0 ? true : false;
}

void Port::enablePort(int enable)
{
	uint32_t status = SusiDeviceSetValue(POE_ID_PORT_POWER_PORT(_portNum), enable);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("Enable Port Failed. (0x%08X)", status);
	}
	_portPower = (uint32_t)enable;
}

void Port::getPower()
{
	uint32_t status = SusiDeviceGetValue(POE_ID_PORT_POWER_PORT(_portNum), &_portPower);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("Get Power Failed. (0x%08X)", status);
	}
}

uint32_t Port::getDetection()
{
	uint32_t data = 0;
	uint32_t status = SusiDeviceGetValue(POE_ID_DETECT_PORT(_portNum), &data);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("PORT%d - Get Detection Failed. (0x%08X)\n", _portNum, status);
	}
	return data;
}

uint32_t Port::getClassification()
{
	uint32_t data = 0;
	uint32_t status = SusiDeviceGetValue(POE_ID_CLASS_PORT(_portNum), &data);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("PORT%d - Get Class Failed. (0x%08X)\n", _portNum, status);
	}
	return data;
}

double Port::getVoltage()
{
	uint32_t data = 0;
	uint32_t status = SusiDeviceGetValue(POE_ID_VOLTAGE_PORT(_portNum), &data);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("PORT%d - Get Voltage Failed. (0x%08X)\n", _portNum, status);
	}
	return (double)data / 1000;
}

double Port::getCurrent()
{
	uint32_t data = 0;
	uint32_t status = SusiDeviceGetValue(POE_ID_CURRENT_PORT(_portNum), &data);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("PORT%d - Get Current Failed. (0x%08X)\n", _portNum, status);
	}
	return (double)data / 1000;
}

uint32_t Port::getPortNum()
{
	return _portNum;
}

std::string Port::getPowerString()
{
	return _portPower == 0 ? "Off" : "On";
}

std::string Port::getDetectionString()
{
	uint32_t detect = getDetection();
	return detectStatus[detect];
}

std::string Port::getClassString()
{
	uint32_t classification = getClassification();
	return classStatus[classification];
}