//====================================================================================
// Revision History :
//---------------------------------------------------------------
// 2013.01.21 by Kevin.Ong
//	 First version
// 
//---------------------------------------------------------------
// Copyright (c) Advantech Co., Ltd. All Rights Reserved
//====================================================================================
#ifndef _DLL_VERSION_H_
#define _DLL_VERSION_H_

#if !defined(SVN_REVISION)
#include "../../SourceCode/Include/svnversion.h"
#endif

// Dll version
#define DLL_VER_MAJOR			4
#define DLL_VER_MINOR			2
#define DLL_VER_BUILD			SVN_REVISION
#define DLL_VER_FIX				0

#define DLL_VER_U32				(DLL_VER_MAJOR << 24 | DLL_VER_MINOR << 16 | DLL_VER_BUILD)

#define CREATE_XVER(maj,min,build,fix) 		maj ##, ## min ##, ## build ##, ## fix

#endif /* _DLL_VERSION_H_ */
