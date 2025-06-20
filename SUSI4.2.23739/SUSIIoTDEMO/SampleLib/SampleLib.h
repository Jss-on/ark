#ifndef _SAMPLELIB_H_
#define _SAMPLELIB_H_

#include "OsDeclarations.h"
#include "SusiIoT.h"
#include "IoTJsonLibrary.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
    #ifndef SUSI_IOT_API
    #define SUSI_IOT_API __stdcall
    #endif
    #define SCPY(fmt, input) strcpy_s(fmt, 20, input)
#else
    #define SUSI_API
	#define SCPY(fmt, input) strcpy(fmt, input)
#endif

#ifndef _LINUX
#if defined(_WIN64)
#pragma comment(lib,"../lib/x64/IoTJsonLibrary.lib")
#pragma comment(lib,"../lib/x64/jansson.lib")
#else
#pragma comment(lib,"../lib/Win32/IoTJsonLibrary.lib")
#pragma comment(lib,"../lib/Win32/jansson.lib")
#endif
#endif

typedef void (SUSI_IOT_API *SUSI_IOT_EVENT_CALLBACK)(SusiIoTId_t id, char *jsonstr);

#ifdef __cplusplus
extern "C" {
#endif

SusiIoTStatus_t SUSI_IOT_API SusiIoTInitialize(void);
SusiIoTStatus_t SUSI_IOT_API SusiIoTUninitialize(void);

SusiIoTStatus_t SUSI_IOT_API SusiIoTGetMDCapability(json_t *capability);
SusiIoTStatus_t SUSI_IOT_API SusiIoTGetMDValue(SusiIoTId_t id, json_t *jValue);
SusiIoTStatus_t SUSI_IOT_API SusiIoTSetMDValue(SusiIoTId_t id, json_t *jValue);

SusiIoTStatus_t SUSI_IOT_API SusiIoTSetMDEventHandler(SUSI_IOT_EVENT_CALLBACK eventCallbackFun);

uint8_t SUSI_IOT_API SusiIoTGetModuleID();

#ifdef __cplusplus
}
#endif

#endif /* _SAMPLELIB_H_ */

