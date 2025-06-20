#ifndef _OS_WINDOWS_H_
#define _OS_WINDOWS_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(WIN32) && !defined(WINCE) || defined(_WIN64)

#if defined(_WIN64)
//#pragma comment(lib,"lib/x64/Susi4.lib")
#pragma comment(lib,"lib/x64/SusiDevice.lib")
#else
//#pragma comment(lib,"lib/Win32/Susi4.lib")
#pragma comment(lib,"lib/Win32/SusiDevice.lib")
#endif

#include <stdlib.h>
#define CLRSCR "cls"

#include <stdio.h>
#define SCANF_IN(format, pvalue)	scanf_s(format, pvalue)

#include <windows.h>
#define SLEEP_USEC(ms)	Sleep((DWORD)ms)

#include <conio.h>

#endif /* WIN32 or _WIN64 */

#endif /* _OS_WINDOWS_H_ */
