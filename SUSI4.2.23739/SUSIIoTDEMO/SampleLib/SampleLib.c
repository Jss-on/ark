#include "SampleLib.h"
#include <stdio.h>
#include <string.h>

#define ARRAYSIZE(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define DATA_MAX 100
SUSI_IOT_EVENT_CALLBACK IoT_callback = NULL; 

typedef enum DATATYPE_T 
{
	INT,
	REAL,
	STRING,
	OBJECT,
} DATATYPE;

typedef struct DataRecord_t
{
	DATATYPE dataType;
	int id;
	union dataValue 
	{
		int intValue;
		double realValue;
		char stringValue[20];
		json_t* objectValue;
	} data;
} DataRecord;

static json_t* jsonCapabilityPackage;
static int count = 0;
static DataRecord dataRecord[DATA_MAX];

static int findIndex(int iotId)
{
	int i = 0;
	for (; i < DATA_MAX; i++)
	{
		if (dataRecord[i].id == iotId)
		{
			return i;
		}
	}
	return -1;
}

static void setDataRecord(DATATYPE type, int id, json_t* data)
{
	dataRecord[count].dataType = type;
	dataRecord[count].id = id;

	switch (type)
	{
		case INT:
		{
			dataRecord[count++].data.intValue = (int)json_integer_value(data);
			break;
		}
		case REAL:
		{
			dataRecord[count++].data.realValue = json_real_value(data);
			break;
		}
		case STRING:
		{
			SCPY(dataRecord[count++].data.stringValue, json_string_value(data));
			break;
		}
		case OBJECT:
		{
			json_object_update(dataRecord[count++].data.objectValue, data);
			break;
		}
		default:
			return;
	}
}

static json_t* createInfo()
{
	json_t* infos[5] = {0};
	
	infos[0] = createIoTArrayElementString("Platform Name", "SAMPLE-123", 0x81000101);
	infos[1] = createIoTArrayElementInt("Platform Revision", 5, 0x81000102);

	setDataRecord(STRING, 0x81000101, json_string("SAMPLE-123"));
	setDataRecord(INT, 0x81000102, json_integer(5));

	return createIoTFuncPackage(infos, ARRAYSIZE(infos), INFO);
}

static json_t* createHWM()
{
	json_t* hwm[5] = {0};
	json_t* voltage[2] = {0};
	json_t* temperature[2] = {0};

	voltage[0] = createIoTArrayElementReal("3.3V", 3.3, 0x81000201);
	insertItemEx(voltage[0], "{s:f, s:f, s:s, s:s}", "max", 15.0, "min", 0.0, "asm", "r", "ui", "Label");
	voltage[1] = createIoTArrayElementReal("V5", 5.0, 0x81000202);
	insertItemEx(voltage[1], "{s:f, s:f, s:s, s:s}", "max", 15.0, "min", 0.0, "asm", "r", "ui", "Label");

	temperature[0] = createIoTArrayElementReal("CPU", 30.2, 0x81000211);
	insertItemEx(temperature[0], "{s:f, s:f, s:s, s:s}", "max", 100.0, "min", 0.0, "asm", "r", "ui", "Label");
	temperature[1] = createIoTArrayElementReal("SYS", 28.7, 0x81000212);
	insertItemEx(temperature[1], "{s:f, s:f, s:s, s:s}", "max", 100.0, "min", 0.0, "asm", "r", "ui", "Label");
	 
	hwm[0] = createIoTIPSOPackage(voltage, ARRAYSIZE(voltage), VOLTAGE);
	hwm[1] = createIoTIPSOPackage(temperature, ARRAYSIZE(temperature), TEMPERATURE);

	setDataRecord(REAL, 0x81000201, json_real(3.3));
	setDataRecord(REAL, 0x81000202, json_real(5.0));
	setDataRecord(REAL, 0x81000211, json_real(30.2));
	setDataRecord(REAL, 0x81000212, json_real(28.7));

	return createIoTFuncPackage(hwm, ARRAYSIZE(hwm), HWM);
}

static json_t* createGPIO()
{
	json_t* gpios[5] = {0};
	json_t* gpio0[2] = {0};
	json_t* gpio1[2] = {0};
	json_t* gpio2[2] = {0};

	gpio0[0] = createIoTArrayElementInt("Dir", 1, 0x81000301);
	insertItemEx(gpio0[0], "{s:i, s:i, s:s, s:s}", "max", 1, "min", 1, "asm", "r", "ui", "Label");
	gpio0[1] = createIoTArrayElementInt("Level", 1, 0x81000302);
	insertItemEx(gpio0[1], "{s:i, s:i, s:s, s:s}", "max", 1, "min", 0, "asm", "r", "ui", "Label");
	
	gpio1[0] = createIoTArrayElementInt("Dir", 0, 0x81000311);
	insertItemEx(gpio1[0], "{s:i, s:i, s:s, s:s}", "max", 0, "min", 0, "asm", "r", "ui", "Label");
	gpio1[1] = createIoTArrayElementInt("Level", 1, 0x81000312);	
	insertItemEx(gpio1[1], "{s:i, s:i, s:s, s:s}", "max", 1, "min", 0, "asm", "rw", "ui", "TextBox");
	
	gpio2[0] = createIoTArrayElementInt("Dir", 0, 0x81000321);
	insertItemEx(gpio2[0], "{s:i, s:i, s:s, s:s}", "max", 1, "min", 0, "asm", "rw", "ui", "TextBox");
	gpio2[1] = createIoTArrayElementInt("Level", 0, 0x81000322);
	insertItemEx(gpio2[1], "{s:i, s:i, s:s, s:s}", "max", 1, "min", 0, "asm", "rw", "ui", "TextBox");
	
	gpios[0] = createIoTIPSOPackageEx(gpio0, ARRAYSIZE(gpio0), "GPIO1", "N/A", 0x81000300);
	gpios[1] = createIoTIPSOPackageEx(gpio1, ARRAYSIZE(gpio1), "GPIO2", "N/A", 0x81000310);
	gpios[2] = createIoTIPSOPackageEx(gpio2, ARRAYSIZE(gpio2), "GPIO3", "N/A", 0x81000320);

	setDataRecord(INT, 0x81000301, json_integer(1));
	setDataRecord(INT, 0x81000302, json_integer(1));
	setDataRecord(INT, 0x81000311, json_integer(0));
	setDataRecord(INT, 0x81000312, json_integer(1));
	setDataRecord(INT, 0x81000321, json_integer(0));
	setDataRecord(INT, 0x81000322, json_integer(0));

	return createIoTFuncPackage(gpios, ARRAYSIZE(gpios), GPIO);
}

static json_t* createBKL()
{
	json_t* bkls[5] = {0};
	json_t* bkl1[3] = {0};

	bkl1[0] = createIoTArrayElementInt("Brightness", 65 ,0x81000401);
	insertItemEx(bkl1[0], "{s:i, s:i, s:s, s:s}", "max", 255, "min", 0, "asm", "rw", "ui", "TextBox");
	bkl1[1] = createIoTArrayElementInt("Backlight", 1, 0x81000402);
	insertItemEx(bkl1[1], "{s:i, s:i, s:s, s:s}", "max", 1, "min", 0, "asm", "rw", "ui", "TextBox");
	bkl1[2] = createIoTArrayElementInt("Frequency", 12345, 0x81000403);
	insertItemEx(bkl1[2], "{s:i, s:i, s:s, s:s}", "max", 65535, "min", 0, "asm", "rw", "ui", "TextBox");
	
	bkls[0] = createIoTIPSOPackageEx(bkl1, ARRAYSIZE(bkl1), "Backlight 1", "N/A", 0x81000400);

	setDataRecord(INT, 0x81000401, json_integer(65));
	setDataRecord(INT, 0x81000402, json_integer(1));
	setDataRecord(INT, 0x81000403, json_integer(123));

	return createIoTFuncPackage(bkls, ARRAYSIZE(bkls), BKL);
}

static json_t* createOEM()
{
	json_t* ipsoPacks[5] = {0};
	json_t* arrayElems[5] = {0};

	arrayElems[0] = createIoTArrayElementInt("OEM item 0", 100, 0x81000F01);
	insertItemInt(arrayElems[0], "int", 1);
	insertItemString(arrayElems[0], "asm", "rw");

	arrayElems[1] = createIoTArrayElementReal("OEM item 1", 100.5, 0x81000F02);
	insertItemReal(arrayElems[1], "real", 1.0);

	arrayElems[2] = createIoTArrayElementString("OEM item 2", "hello", 0x81000F03);
	insertItemString(arrayElems[2], "string", "oem");

	arrayElems[3] = createIoTArrayElementInt("OEM item 3", 100, 0x81000F04);
	insertItemJsonObject(arrayElems[3], "object", json_integer(1));

	arrayElems[4] = createIoTArrayElementInt("OEM item 4", 100, 0x81000F05);
	insertItemEx(arrayElems[4], "{s:i, s:f, s:s, s:o}", "int", -100, "real", -100.0, "string", "oem", "object", arrayElems[0]);
	deleteItem(arrayElems[4], "int");

	ipsoPacks[0] = createIoTIPSOPackageEx(arrayElems, ARRAYSIZE(arrayElems), "OEM IPSO 0", "", 0x81000F10);

	setDataRecord(INT, 0x81000F01, json_integer(100));
	setDataRecord(REAL, 0x81000F02, json_real(100.5));
	setDataRecord(STRING, 0x81000F03, json_string("hello"));
	setDataRecord(INT, 0x81000F04, json_integer(100));
	setDataRecord(INT, 0x81000F05, json_integer(100));

	return createIoTFuncPackageEx(ipsoPacks, ARRAYSIZE(ipsoPacks), "OEM Function 0", 0x81000F00);
}

SusiIoTStatus_t SUSI_IOT_API SusiIoTInitialize(void)
{
	json_t* funcPacks[5] = {0};
	funcPacks[0] = createInfo();
	funcPacks[1] = createHWM();
	funcPacks[2] = createGPIO();
	funcPacks[3] = createBKL();
	funcPacks[4] = createOEM();

	jsonCapabilityPackage = createIoTJsonCapability(funcPacks, 5);

	return SUSIIOT_STATUS_SUCCESS;
}

SusiIoTStatus_t SUSI_IOT_API SusiIoTUninitialize(void)
{
	freeJsonObject(&jsonCapabilityPackage);
	return SUSIIOT_STATUS_SUCCESS;
}

SusiIoTStatus_t SUSI_IOT_API SusiIoTGetMDCapability(json_t *capability)
{
	if (json_object_update(capability, jsonCapabilityPackage))
	{
		printf("SampleLib SusiIoTGetMDCapability failed\n");
		return SUSIIOT_STATUS_ERROR;
	}

	return SUSIIOT_STATUS_SUCCESS;
}

SusiIoTStatus_t SUSI_IOT_API SusiIoTGetMDValue(SusiIoTId_t id, json_t *jValue)
{
	int index = findIndex(id);

	if (index == -1)
	{
		printf("SampleLib SusiIoTGetMDValue index failed\n");
		return SUSIIOT_STATUS_UNSUPPORTED;
	}

	switch (dataRecord[index].dataType)
	{
		case INT:
		{
			json_integer_set(jValue, dataRecord[index].data.intValue);
			break;
		}
		case REAL:
		{
			json_real_set(jValue, dataRecord[index].data.realValue);
			break;
		}
		case STRING:
		{
			json_string_set(jValue, dataRecord[index].data.stringValue);
			break;
		}
		case OBJECT:
		{
			json_object_update(jValue, dataRecord[index].data.objectValue);
			break;
		}
		default:
		{
			printf("SampleLib SusiIoTGetMDValue dataType failed\n");
			return SUSIIOT_STATUS_ERROR;
		}
	}
	
	return SUSIIOT_STATUS_SUCCESS;
}

SusiIoTStatus_t SUSI_IOT_API SusiIoTSetMDValue(SusiIoTId_t id, json_t *jValue)
{
	int index = findIndex(id);

	if (index == -1)
	{
		printf("SampleLib SusiIoTGetMDValue index failed\n");
		return SUSIIOT_STATUS_UNSUPPORTED;
	}

	if (json_is_integer(jValue))
	{
		dataRecord[index].data.intValue = (int)json_integer_value(jValue);
	}
	else if (json_is_real(jValue))
	{
		dataRecord[index].data.realValue = json_real_value(jValue);
	}
	else if (json_is_string(jValue))
	{
		SCPY(dataRecord[index].data.stringValue, json_string_value(jValue));
	}
	else if (json_is_object(jValue))
	{
		json_object_update(dataRecord[index].data.objectValue, jValue);
	}
	else
	{
		printf("SampleLib SusiIoTSetMDValue object type failed\n");
		return SUSIIOT_STATUS_ERROR;
	}

	if(id == 0x81000F01 && (int)json_integer_value(jValue) > 100)
	{
		json_t* temp = json_pack("{s:s, s:o, s:i, s:s}", "n", "OEM item 0", "v", jValue, "id", id, "msg", "Warning");
		if(IoT_callback)
			IoT_callback(0x81000F01, json_dumps(temp, JSON_PRESERVE_ORDER|JSON_REAL_PRECISION(5)));
	}

	return SUSIIOT_STATUS_SUCCESS;
}

SusiIoTStatus_t SUSI_IOT_API SusiIoTSetMDEventHandler(SUSI_IOT_EVENT_CALLBACK eventCallbackFun)
{
	IoT_callback = eventCallbackFun;
	return SUSIIOT_STATUS_SUCCESS;
}

uint8_t SUSI_IOT_API SusiIoTGetModuleID()
{
	return 0x81;
}
