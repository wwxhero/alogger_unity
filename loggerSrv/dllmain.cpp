// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "loggerSrv_i.h"
#include "dllmain.h"

const TCHAR CloggerSrvModule::c_logSrc[] = _T("LoggerSrv");

CloggerSrvModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved);
}
