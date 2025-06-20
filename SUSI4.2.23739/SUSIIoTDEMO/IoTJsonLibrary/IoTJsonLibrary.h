#ifndef _IOT_JSON_LIBRARY_H_
#define _IOT_JSON_LIBRARY_H_

#include <jansson.h>

#ifndef NULL
#define NULL 0
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
    #ifndef SUSI_IOT_API
    #define SUSI_IOT_API __stdcall
    #endif
#else
    #define SUSI_IOT_API
#endif

#define ID_INFO            0x00010000
#define ID_HWM             0x00020000
#define ID_HWM_TEMPERATURE 0x00020100
#define ID_HWM_VOLTAGE     0x00020200
#define ID_HWM_FAN         0x00020300
#define ID_HWM_CURRENT     0x00020400
#define ID_HWM_CASEOPEN    0x00020500
#define ID_GPIO            0x00040000
#define ID_BACKLIGHT       0x00050000

typedef enum
{
	TEMPERATURE,
	VOLTAGE,
	FAN,
	CURRENT,
	CASEOPEN,
} IPSO_T;

typedef enum
{
	INFO,
	HWM,
	GPIO,
	BKL,
} FUNC_T;

/* Create JSON Array Element */
json_t* SUSI_IOT_API createIoTArrayElementInt(const char* n, const int v, const int id);
json_t* SUSI_IOT_API createIoTArrayElementReal(const char* n, const double v, const int id);
json_t* SUSI_IOT_API createIoTArrayElementString(const char* n, const char* sv, const int id);

/* Create JSON IPSO Package */
json_t* SUSI_IOT_API createIoTIPSOPackage(json_t* jsonArrayElements[], int size, IPSO_T type);
json_t* SUSI_IOT_API createIoTIPSOPackageEx(json_t* jsonArrayElements[], int size, const char* bn, const char* bu, const int id);

/* Create JSON Function Package */
json_t* SUSI_IOT_API createIoTFuncPackage(json_t* jsonIPSOObjects[], int size, FUNC_T type);
json_t* SUSI_IOT_API createIoTFuncPackageEx(json_t* jsonIPSOObjects[], int size, const char* bn, const int id);

/* Create JSON Package */
json_t* SUSI_IOT_API createIoTJsonCapability(json_t* jsonObjects[], int size);

/* General Function */
void SUSI_IOT_API freeJsonObject(json_t **jsonObject);

int SUSI_IOT_API insertItemInt(json_t* object, const char* key, const int value);       // ("key", 5)
int SUSI_IOT_API insertItemReal(json_t* object, const char* key, const double value);   // ("key", 5.2)
int SUSI_IOT_API insertItemString(json_t* object, const char* key, const char* value);  // ("key", "5")
int SUSI_IOT_API insertItemJsonObject(json_t* object, const char* key, json_t* other);  // ("key", json_integer(5))
int SUSI_IOT_API insertItemEx(json_t* object, const char *fmt, ...);                    // ("{s:i, s:s}", "key1", 5, "key2", "5")

int SUSI_IOT_API deleteItem(json_t* object, const char* key);

int SUSI_IOT_API appendJsonObject(json_t* object, json_t* other);

int SUSI_IOT_API getLastErrorStatus();

#endif /* _IOT_JSON_LIBRARY_H_ */