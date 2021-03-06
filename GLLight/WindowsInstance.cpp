#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <windowsx.h>
#include <SDKDDKVer.h>
#include <gl\GL.h>
#include "RotateHelper.h"


// Global Variables:
TCHAR szTitle[] = TEXT("GL");					// The title bar text
TCHAR szWindowClass[] = TEXT("GLTitle");			// the main window class name

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void                Init();
void                Paint();


extern float g_fScale;
extern float g_rotAngle;
extern float g_xOffSet;
extern float  g_yOffSet;
extern rthlp::CVector3<float> g_axis;

bool g_bRButtonDown = false;
rthlp::CVector3<float> g_lastPoint;
rthlp::CVector3<float> g_currPoint;
RECT g_prevRect;
DWORD g_dwPrevStyle;
DWORD g_dwPrevExStyle;
bool g_bFullScreen = false;
extern bool g_bHelpInfo;

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int       nCmdShow)
{
	// Initialize global strings
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szWindowClass;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}



HGLRC SetupGLContext(HWND hWnd, HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int nFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nFormat, &pfd);
	HGLRC hGLRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hGLRC);
	SetWindowTextA(hWnd, (char *)glGetString(GL_VERSION));
	Init();
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	wglUseFontBitmaps(hdc, 0, 255, 1000);
	wglMakeCurrent(NULL, NULL);
	return hGLRC;
}


void OnMouseWheel(HWND hWnd, int xPos, int yPos, int zDelta, UINT)
{
	if (zDelta > 0)
	{
		g_fScale += 0.1f;
	}
	else
	{
		g_fScale -= 0.1f;
		if (g_fScale < 0.1f)
		{
			g_fScale = 0.1f;
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
}

void OnLButtonDown(HWND hWnd, bool bDoubleClicked, int xPos, int yPos, UINT keyFlags)
{
	if (bDoubleClicked)
	{
		OutputDebugString(L"double...");
		return;
	}
	RECT rc;
	GetClientRect(hWnd, &rc);
	g_lastPoint.MapFromPoint(xPos, yPos, rc.right - rc.left, rc.bottom - rc.top);
}

void OnMouseMove(HWND hWnd, int x, int y, UINT keyFlags)
{
	static bool bSkip = true;
	bSkip = !bSkip;
	if (bSkip)
	{
		return;
	}
	if (keyFlags == MK_LBUTTON)
	{
		using namespace rthlp;
		RECT rc;
		GetClientRect(hWnd, &rc);
		g_currPoint.MapFromPoint(x, y, rc.right - rc.left, rc.bottom - rc.top);
		g_axis = rthlp::CrossProduct(g_currPoint, g_lastPoint);
		g_rotAngle = -90.0f * (g_currPoint - g_lastPoint).Norm();
		g_lastPoint = g_currPoint;
		InvalidateRect(hWnd, NULL, FALSE);
	}
	else if (g_bRButtonDown) // MK_RBUTTON is never specified
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		g_currPoint.m_data[0] = float(x) / (rc.right - rc.left);
		g_currPoint.m_data[1] = float(y) / (rc.bottom - rc.top);
		g_xOffSet -= 0.1f * (g_lastPoint.m_data[0] - g_currPoint.m_data[0]);
		g_yOffSet += 0.1f * (g_lastPoint.m_data[1] - g_currPoint.m_data[1]);
		g_xOffSet = rthlp::Clamp(g_xOffSet, -1.0f, 1.0f);
		g_yOffSet = rthlp::Clamp(g_yOffSet, -1.0f, 1.0f);
		InvalidateRect(hWnd, NULL, FALSE);
	}
}


void OnRButtonDown(HWND hWnd, bool, int x, int y, UINT)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	g_lastPoint.m_data[0] = float(x) / (rc.right - rc.left);
	g_lastPoint.m_data[1] = float(y) / (rc.bottom - rc.top);
	g_bRButtonDown = true;
	SetCapture(hWnd);
}

void OnRButtonUp(HWND hWnd, int x, int y, UINT)
{
	g_bRButtonDown = false;
	ReleaseCapture();
}

void OnLButtonDblClk(HWND hWnd, bool, int x, int y, UINT)
{
	if (!g_bFullScreen)
	{
		GetWindowRect(hWnd, &g_prevRect);
		g_dwPrevStyle = GetWindowLong(hWnd, GWL_STYLE);
		g_dwPrevExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

		g_bFullScreen = true;
		MONITORINFO mnInfo;
		mnInfo.cbSize = sizeof (MONITORINFO);
		GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mnInfo);

		SetWindowLong(hWnd, GWL_STYLE, g_dwPrevStyle & (~(WS_CAPTION | WS_THICKFRAME)));
		SetWindowLong(hWnd, GWL_EXSTYLE, g_dwPrevExStyle & (~(WS_EX_DLGMODALFRAME |
			WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE)));

		SetWindowPos(hWnd, NULL, mnInfo.rcMonitor.left, mnInfo.rcMonitor.top,
			mnInfo.rcMonitor.right - mnInfo.rcMonitor.left,
			mnInfo.rcMonitor.bottom - mnInfo.rcMonitor.top,
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else
	{
		g_bFullScreen = false;
		SetWindowLong(hWnd, GWL_STYLE, g_dwPrevStyle);
		SetWindowLong(hWnd, GWL_EXSTYLE, g_dwPrevExStyle);
		SetWindowPos(hWnd, NULL, g_prevRect.left, g_prevRect.top, 
			g_prevRect.right - g_prevRect.left, 
			g_prevRect.bottom - g_prevRect.top, 
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}

}


void OnChar(HWND hWnd, TCHAR ch, int)
{
	switch (ch)
	{
	case 'q':
	case 'Q':
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case 'H':
	case 'h':
		if (!g_bHelpInfo)
		{
			g_bHelpInfo = true;
		}
		else
		{
			g_bHelpInfo = false;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hGLRC;
	static HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		hGLRC = SetupGLContext(hWnd, hdc);
		break;
	case WM_PAINT:
		wglMakeCurrent(hdc, hGLRC);
		Paint();
		SwapBuffers(hdc);
		wglMakeCurrent(NULL, NULL);
		ValidateRect(hWnd, NULL);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_SIZE:
		wglMakeCurrent(hdc, hGLRC);
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		wglMakeCurrent(NULL, NULL);
		break;
	case WM_LBUTTONDOWN:
		HANDLE_WM_LBUTTONDOWN(hWnd, wParam, lParam, OnLButtonDown);
		break;
	case WM_MOUSEMOVE:
		HANDLE_WM_MOUSEMOVE(hWnd, wParam, lParam, OnMouseMove);
		break;
	case WM_MOUSEWHEEL:
		HANDLE_WM_MOUSEWHEEL(hWnd, wParam, lParam, OnMouseWheel);
		break;
	case WM_RBUTTONDOWN:
		HANDLE_WM_RBUTTONDOWN(hWnd, wParam, lParam, OnRButtonDown);
		break;
	case WM_RBUTTONUP:
		HANDLE_WM_RBUTTONUP(hWnd, wParam, lParam, OnRButtonUp);
		break;
	case WM_LBUTTONDBLCLK:
		HANDLE_WM_LBUTTONDBLCLK(hWnd, wParam, lParam, OnLButtonDblClk);
		break;
	case WM_CHAR:
		HANDLE_WM_CHAR(hWnd, wParam, lParam, OnChar);
		break;
	case WM_DESTROY:
		wglDeleteContext(hGLRC);
		ReleaseDC(hWnd, hdc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
