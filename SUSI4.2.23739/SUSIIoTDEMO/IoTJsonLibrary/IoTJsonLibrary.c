#include "IoTJsonLibrary.h"

#define JSON_OBJECT_EMPTY  0xFFFFF8FE
#define JSON_UPDATE_FAIL   0xFFFFF8FD
#define JSON_COPY_FAIL     0xFFFFF8FC
#define JSON_KEY_NOT_FOUND 0xFFFFF8FB

static int error_code = 0x00000000;

json_t* SUSI_IOT_API createIoTArrayElementInt(const char* name, const int value, const int id)
{
	json_t* ret = json_pack("{s:s, s:i, s:i}", "n", name, "v", value, "id", id);

	if(json_object_size(ret) < 1)
	{
		error_code = JSON_OBJECT_EMPTY;
		freeJsonObject(&ret);
	}

	return ret;
}

json_t* SUSI_IOT_API createIoTArrayElementReal(const char* name, const double value, const int id)
{
	json_t* ret = json_pack("{s:s, s:f, s:i}", "n", name, "v", value, "id", id);

	if(json_object_size(ret) < 1)
	{
		error_code = JSON_OBJECT_EMPTY;
		freeJsonObject(&ret);
	}

	return ret;
}

json_t* SUSI_IOT_API createIoTArrayElementString(const char* name, const char* value, const int id)
{
	json_t* ret = json_pack("{s:s, s:s, s:i}", "n", name, "sv", value, "id", id);

	if(json_object_size(ret) < 1)
	{
		error_code = JSON_OBJECT_EMPTY;
		freeJsonObject(&ret);
	}

	return ret;
}

static json_t* SUSI_IOT_API createIoTGeneralIPSOPackage(json_t* jsonArrayObjects[], int size, const char *bn, const char *bu, const int bt, const int ver, const int id)
{
	int i = 0;
	json_t* package = NULL;
	json_t* e = json_array();

	for (; i < size; i++)
	{
		if (json_is_object(jsonArrayObjects[i]))
		{
			json_array_append_new(e, jsonArrayObjects[i]);
		}
	}

	if (json_array_size(e) < 1)
	{
		error_code = JSON_OBJECT_EMPTY;
		freeJsonObject(&e);
	}

	package = json_pack("{s:o, s:s, s:s, s:i, s:i, s:i}",
		"e", e,
		"bn", bn,
		"bu", bu,
		"bt", bt,
		"ver", ver,
		"id", id);

	if(json_object_size(package) < 1)
	{
		error_code = JSON_OBJECT_EMPTY;
		freeJsonObject(&package);
	}

	return package;
}

json_t* SUSI_IOT_API createIoTIPSOPackage(json_t* jsonArrayObjects[], int size, IPSO_T type)
{
	switch (type)
	{
		case TEMPERATURE:
		{
			return createIoTGeneralIPSOPackage(jsonArrayObjects, size, "Temperature", "Cel", 1276020076, 1, ID_HWM_TEMPERATURE);
		}
		case VOLTAGE:
		{
			return createIoTGeneralIPSOPackage(jsonArrayObjects, size, "Voltage", "V", 1276020076, 1, ID_HWM_VOLTAGE);
		}
		case FAN:
		{
			return createIoTGeneralIPSOPackage(jsonArrayObjects, size, "Fan Speed", "RPM", 1276020076, 1, ID_HWM_FAN);
		}
		case CURRENT:
		{
			return createIoTGeneralIPSOPackage(jsonArrayObjects, size, "Current", "A", 1276020076, 1, ID_HWM_CURRENT);
		}
		case CASEOPEN:
		{
			return createIoTGeneralIPSOPackage(jsonArrayObjects, size, "Case Open", "N/A", 1276020076, 1, ID_HWM_CASEOPEN);
		}
		default:
			return NULL;
	}
}

json_t* SUSI_IOT_API createIoTIPSOPackageEx(json_t* jsonArrayObjects[], int size, const char* bn, const char* bu, const int id)
{
	return createIoTGeneralIPSOPackage(jsonArrayObjects, size, bn, bu, 1276020076, 1, id);
}

static json_t* SUSI_IOT_API createIoTGeneralFuncPackage(json_t* jsonIPSOObjects[], int size, const int id, const char* bn)
{
	int i = 0;
	json_t* ret = json_object();
	json_t* package = json_pack("{s:i, s:s}", "id", id, "bn", bn);

	for (; i < size; i++)
	{
		if (json_is_object(jsonIPSOObjects[i]))
		{
			const char *str;
			json_unpack(jsonIPSOObjects[i], "{s:s}", "bn", &str);
			json_object_set_new(package, str, jsonIPSOObjects[i]);
		}
	}

	json_object_set_new(ret, bn, package);

	if(json_object_size(ret) < 1)
	{
		error_code = JSON_OBJECT_EMPTY;
		freeJsonObject(&ret);
	}

	return ret;
}

json_t* SUSI_IOT_API createIoTFuncPackage(json_t* jsonIPSOObjects[], int size, FUNC_T type)
{
	switch (type)
	{
		case INFO:
		{
			json_t* package = createIoTGeneralIPSOPackage(jsonIPSOObjects, size, "Platform Information", "N/A", 1276020076, 1, ID_INFO);
			json_t* ret = json_object();
			json_object_set_new(ret, "Platform Information", package);
			return ret;
		}
		case HWM:
		{
			return createIoTGeneralFuncPackage(jsonIPSOObjects, size, ID_HWM, "Hardware Monitor");
		}
		case GPIO:
		{
			return createIoTGeneralFuncPackage(jsonIPSOObjects, size, ID_GPIO, "GPIO");
		}
		case BKL:
		{
			return createIoTGeneralFuncPackage(jsonIPSOObjects, size, ID_BACKLIGHT, "Backlight");
		}
		default:
			return NULL;
	}

	return createIoTGeneralFuncPackage(jsonIPSOObjects, size, ID_HWM, "Hardware Monitor");
}

json_t* SUSI_IOT_API createIoTFuncPackageEx(json_t* jsonIPSOObjects[], int size, const char* bn, const int id)
{
	return createIoTGeneralFuncPackage(jsonIPSOObjects, size, id, bn);
}

json_t* SUSI_IOT_API createIoTJsonCapability(json_t* jsonObjects[], int size)
{
	int i = 0;
	json_t* ret = json_object();

	for (; i < size; i++)
	{
		if (json_is_object(jsonObjects[i]))
		{
			json_object_update_missing(ret, jsonObjects[i]);
		}
	}

	return ret;
}

void SUSI_IOT_API freeJsonObject(json_t **jsonObject)
{
	json_object_clear(*jsonObject);
	json_decref(*jsonObject);
	*jsonObject = NULL;
}

int SUSI_IOT_API insertItemInt(json_t* object, const char* key, const int value)
{
	return json_object_set_new(object, key, json_integer(value));
}

int SUSI_IOT_API insertItemReal(json_t* object, const char* key, const double value)
{
	return json_object_set_new(object, key, json_real(value));
}

int SUSI_IOT_API insertItemString(json_t* object, const char* key, const char* value)
{
	return json_object_set_new(object, key, json_string(value));
}

int SUSI_IOT_API insertItemJsonObject(json_t* object, const char* key, json_t* other)
{
	return json_object_set_new(object, key, other);
}

int SUSI_IOT_API insertItemEx(json_t* object, const char *fmt, ...)
{
	json_t *value;
	va_list ap;

	va_start(ap, fmt);
	value = json_vpack_ex(NULL, 0, fmt, ap);
	va_end(ap);

	return json_object_update_missing(object, value);
}

int SUSI_IOT_API deleteItem(json_t* object, const char* key)
{
	return json_object_del(object, key);
}

int SUSI_IOT_API appendJsonObject(json_t* object, json_t* other)
{
	int ret = json_object_update_missing(object, other);

	if (ret != 0)
	{
		error_code = JSON_UPDATE_FAIL;
	}

	return ret;
}

int SUSI_IOT_API getLastErrorStatus()
{
	return error_code;
}
