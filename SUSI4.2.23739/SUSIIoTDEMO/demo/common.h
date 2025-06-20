#ifndef _COMMON_H_
#define _COMMON_H_

#include "OsDeclarations.h"
#include "SusiIoT.h"
#include "SusiIoTAPI.h"
#include <stdlib.h>

#ifndef _MSC_VER
    #define SUSI_IOT_API
    #define SCANF(fmt, ...) scanf(fmt, ##__VA_ARGS__)
    #define PAUSE() printf("\nPress Enter key to continue..."); fgetc(stdin); fgetc(stdin);  
    #define CLRSCR() system("clear")
#else
    #include <Windows.h>
    #ifndef SUSI_IOT_API
    #define SUSI_IOT_API __stdcall
    #endif
    #define SCANF(format, ...) scanf_s(format, ##__VA_ARGS__)
    #define PAUSE() system("pause")
    #define CLRSCR() system("cls")
#endif

#ifndef _LINUX
#if defined(_WIN64)
#pragma comment(lib,"lib/x64/SusiIoT.lib")
#pragma comment(lib,"lib/x64/jansson.lib")
#else
#pragma comment(lib,"lib/x32/SusiIoT.lib")
#pragma comment(lib,"lib/x32/jansson.lib")
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
    #include "jansson.h"

#ifdef __cplusplus
}
#endif

#endif /* _COMMON_H_ */
