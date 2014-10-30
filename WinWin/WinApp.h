/************************************************************************/
/* 应用程序声明头文件                                                   */
/************************************************************************/
#ifndef _WINAPP_H_
#define _WINAPP_H_

#include <Windows.h>
class CWinApp
{
public:
	CWinApp();
	~CWinApp();

	ATOM MyRegisterClass(HINSTANCE hInstance);
	bool MyInstance(HINSTANCE hIns,int nShowCmd);
	bool MessageLoop(MSG &uMsg);
	static int CaptureAnImage(HWND hWnd);
private:
	static LRESULT CALLBACK WinWinProc(
		__in  HWND hwnd,
		__in  UINT uMsg,
		__in  WPARAM wParam,
		__in  LPARAM lParam
		);

private:
	HWND m_hWnd;
	HINSTANCE m_hIns;
	WCHAR *m_szClassName;
	WCHAR *m_szWinName;
};


#endif /*_WINAPP_H_*/