// ChartControl.cpp: реализация CChartControlApp и регистрация библиотеки DLL.

#include "pch.h"
#include "framework.h"
#include "ChartControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CChartControlApp theApp;

const GUID CDECL _tlid = {0xb8c8cf12,0xb964,0x479f,{0x86,0xb6,0xdb,0x2a,0xad,0x99,0x1b,0xd5}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CChartControlApp::InitInstance — инициализация библиотеки DLL

BOOL CChartControlApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Добавьте здесь свой код инициализации модуля.
	}

	return bInit;
}



// CChartControlApp::ExitInstance — завершение библиотеки DLL

int CChartControlApp::ExitInstance()
{
	// TODO: Добавьте здесь свой код завершения работы модуля.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - добавляет записи в системный реестр

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - удаляет записи из системного реестра

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
