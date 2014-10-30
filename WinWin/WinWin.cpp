#include <Windows.h>
#include "WinApp.h"

CWinApp App;
CWinApp * AfxGetApp()
{
	return &App;
}

int WINAPI WinMain( __in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine, 
	__in int nShowCmd )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	CWinApp * pApp = AfxGetApp();
	pApp->MyRegisterClass(hInstance);
	pApp->MyInstance(hInstance,nShowCmd);
	MSG uMsg;
	pApp->MessageLoop(uMsg);
	return 0;
}