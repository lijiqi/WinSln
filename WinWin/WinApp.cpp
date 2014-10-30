#include "WinApp.h"
#include "resource.h"

CWinApp::CWinApp()
{
	m_hWnd = 0;
	m_hIns = 0;
	m_szClassName = new WCHAR[128];
	m_szWinName = new WCHAR[128];
}

CWinApp::~CWinApp()
{
	delete m_szClassName;
	delete m_szWinName;
	m_szClassName = nullptr;
	m_szWinName = nullptr;
}

bool CWinApp::MyInstance(HINSTANCE hIns,int nShowCmd)
{
	m_hIns = hIns;
	m_hWnd = CreateWindow(m_szClassName,m_szWinName,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT ,
		CW_USEDEFAULT ,CW_USEDEFAULT ,nullptr,nullptr,hIns,nullptr);
	ShowWindow(m_hWnd,nShowCmd);
	UpdateWindow(m_hWnd);

	return true;
}


ATOM CWinApp::MyRegisterClass(HINSTANCE hInstance)
{
	LoadString(hInstance,IDS_CLASS_NAME,m_szClassName,wcslen(m_szClassName));
	LoadString(hInstance,IDS_WIN_NAME,m_szWinName,wcslen(m_szWinName));
	WNDCLASSEX wclsx;
	wclsx.cbClsExtra = 0;
	wclsx.cbSize = sizeof(WNDCLASSEX);
	wclsx.cbWndExtra = 0;
	wclsx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wclsx.hCursor = LoadCursor(hInstance,MAKEINTRESOURCE(IDC_CURSOR));
	wclsx.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON));
	wclsx.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON));
	wclsx.hInstance = hInstance;
	wclsx.lpfnWndProc = WinWinProc;
	wclsx.lpszClassName = m_szClassName;
	wclsx.lpszMenuName = nullptr;
	wclsx.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wclsx);
}

bool CWinApp::MessageLoop(MSG &uMsg)
{
	while (GetMessage(&uMsg,nullptr,0,0))
	{
		TranslateMessage(&uMsg);
		DispatchMessage(&uMsg);
	}
	return true;
}
LRESULT CALLBACK CWinApp::WinWinProc(
	__in  HWND hwnd,
	__in  UINT uMsg,
	__in  WPARAM wParam,
	__in  LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_CHAR:
		if (wParam == 0x63)
		{
			CaptureAnImage(hwnd);
		}
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}


int CWinApp::CaptureAnImage(HWND hWnd)
{
	HDC hdcScreen;
	HDC hdcWindow;
	HDC hdcMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;

	// Retrieve the handle to a display device context for the client 
	// area of the window. 
	hdcScreen = GetDC(NULL);
	hdcWindow = GetDC(hWnd);
	
	// Create a compatible DC which is used in a BitBlt from the window DC
	hdcMemDC = CreateCompatibleDC(hdcWindow); 

	if(!hdcMemDC)
	{
		MessageBox(hWnd, L"StretchBlt has failed",L"Failed", MB_OK);
		goto done;
	}
	
	// Get the client area for size calculation
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	DrawText(hdcWindow,L"客户区字画",_countof(L"客户区字画"),&rcClient,DT_LEFT);
	//This is the best stretch mode
	SetStretchBltMode(hdcWindow,HALFTONE);

	//The source DC is the entire screen and the destination DC is the current window (HWND)
	if(!StretchBlt(hdcWindow, 
		0,0, 
		rcClient.right, rcClient.bottom, 
		hdcScreen, 
		0,0,
		GetSystemMetrics (SM_CXSCREEN),
		GetSystemMetrics (SM_CYSCREEN),
		SRCCOPY))
	{
		MessageBox(hWnd, L"StretchBlt has failed",L"Failed", MB_OK);
		goto done;
	}
	
	// Create a compatible bitmap from the Window DC
	hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);

	if(!hbmScreen)
	{
		MessageBox(hWnd, L"CreateCompatibleBitmap Failed",L"Failed", MB_OK);
		goto done;
	}
	
	// Select the compatible bitmap into the compatible memory DC.
	SelectObject(hdcMemDC,hbmScreen);

	// Bit block transfer into our compatible memory DC.
	if(!BitBlt(hdcMemDC,
		0,0,
		rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, 
		hdcWindow, 
		0,0,
		SRCCOPY))
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		goto done;
	}
	
	// Get the BITMAP from the HBITMAP
	GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

	BITMAPFILEHEADER   bmfHeader;    
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);    
	bi.biWidth = bmpScreen.bmWidth;    
	bi.biHeight = bmpScreen.bmHeight;  
	bi.biPlanes = 1;    
	bi.biBitCount = 32;    
	bi.biCompression = BI_RGB;    
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;    
	bi.biYPelsPerMeter = 0;    
	bi.biClrUsed = 0;    
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
	char *lpbitmap = (char *)GlobalLock(hDIB);    

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO *)&bi, DIB_RGB_COLORS);

	// A file is created, this is where we will save the screen capture.
	HANDLE hFile = CreateFile(L"d:\\captureqwsx.bmp",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);   

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB; 

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM   

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);    
	GlobalFree(hDIB);

	//Close the handle for the file that was created
	CloseHandle(hFile);

	//Clean up
done:
	DeleteObject(hbmScreen);
	ReleaseDC(hWnd, hdcMemDC);
	ReleaseDC(NULL,hdcScreen);
	ReleaseDC(hWnd,hdcWindow);

	return 0;
}
