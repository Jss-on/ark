#ifndef _PORT_H_
#define _PORT_H_

#include "SusiDeviceAPI.h"
#include <string>

#define POE_ID_BASE				0x00200000

#define POE_ID_INFO_TYPE			0x00000
#define POE_ID_INFO_BASE			(POE_ID_BASE | POE_ID_INFO_TYPE)
#define POE_ID_INFO_AVAILABLE		((SusiId_t)(POE_ID_INFO_BASE + 0))		// only for 1-4 Ports

#define POE_ID_POWER_TYPE			0x10000
#define POE_ID_POWER_BASE			(POE_ID_BASE | POE_ID_POWER_TYPE)
#define POE_ID_POWER_ENABLE    		((SusiId_t)(POE_ID_POWER_BASE + 0))		// only for 1-4 Ports
#define POE_ID_POWER_DISABLE		((SusiId_t)(POE_ID_POWER_BASE + 1))		// only for 1-4 Ports

#define POE_ID_DETECT_TYPE			0x20000
#define POE_ID_DETECT_BASE			(POE_ID_BASE | POE_ID_DETECT_TYPE)
#define POE_ID_DETECT_PORT(x)		((SusiId_t)(POE_ID_DETECT_BASE + x - 1))

#define POE_ID_CLASS_TYPE			0x30000
#define POE_ID_CLASS_BASE			(POE_ID_BASE | POE_ID_CLASS_TYPE)
#define POE_ID_CLASS_PORT(x)		((SusiId_t)(POE_ID_CLASS_BASE + x - 1))

#define POE_ID_CURRENT_TYPE			0x40000
#define POE_ID_CURRENT_BASE			(POE_ID_BASE | POE_ID_CURRENT_TYPE)
#define POE_ID_CURRENT_PORT(x)		((SusiId_t)(POE_ID_CURRENT_BASE + x - 1))

#define POE_ID_VOLTAGE_TYPE			0x50000
#define POE_ID_VOLTAGE_BASE			(POE_ID_BASE | POE_ID_VOLTAGE_TYPE)
#define POE_ID_VOLTAGE_PORT(x)		((SusiId_t)(POE_ID_VOLTAGE_BASE + x - 1))

#define POE_ID_CAP_TYPE				0x60000
#define POE_ID_CAP_BASE				(POE_ID_BASE | POE_ID_CAP_TYPE)
#define POE_ID_CAP_PORT(x)			((SusiId_t)(POE_ID_CAP_BASE + x - 1))

#define POE_ID_PORT_POWER_TYPE		0x70000
#define POE_ID_PORT_POWER_BASE		(POE_ID_BASE | POE_ID_PORT_POWER_TYPE)
#define POE_ID_PORT_POWER_PORT(x)	(POE_ID_PORT_POWER_BASE + x - 1)

#define GROUP_MAX_PORT	4
#define PORT_MAX_NUM	8
#define POWER_ON		1
#define POWER_OFF		0

class Port {
public:
	uint32_t _portNum;
	uint32_t _portPower;

public:
	Port();
	Port(uint32_t portNum);
	~Port();

	static bool isAvailable();
	static bool getCap(uint8_t portNum);

	void enablePort(int enable);
	void getPower();
	uint32_t getDetection();
	uint32_t getClassification();
	double getVoltage();
	double getCurrent();
	uint32_t getPortNum();
	std::string getPowerString();
	std::string getDetectionString();
	std::string getClassString();
};

#endif /* _PORT_H_ */