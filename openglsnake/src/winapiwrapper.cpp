#include "winapiwrapper.h"
#include <iostream>
#include <stdexcept>

winapiwrapper *winapiwrapper::selfptr = 0;

winapiwrapper::winapiwrapper(HINSTANCE hInstance):
				hWnd(0),
				hDC(0),
				vpFunc(0),
				wHeight(0),
				wWidth(0)
{
	selfptr = this;
	this->hInstance = hInstance;

	addWindowClass("mywrapperdefclass", CS_HREDRAW | CS_VREDRAW);

	AddMessageHandler(WM_DESTROY,&winapiwrapper::onDestroy);

	AddMessageHandler(WM_LBUTTONDOWN,&winapiwrapper::onLeftMouseDown);
	AddMessageHandler(WM_RBUTTONDOWN,&winapiwrapper::onRightMouseDown);
	AddMessageHandler(WM_MBUTTONDOWN,&winapiwrapper::onMidMouseDown);
	AddMessageHandler(WM_KEYDOWN,&winapiwrapper::onKeyDown);
	AddMessageHandler(WM_SIZE,&winapiwrapper::onResize);
	AddMessageHandler(WM_MOUSEWHEEL,&winapiwrapper::onWheel);

}

winapiwrapper::~winapiwrapper()
{
		//destruction
}

BOOL winapiwrapper::addWindowClass(const WNDCLASSEX &WndClsEx)
{
	if(!RegisterClassEx(&WndClsEx))
	{
		throw std::runtime_error(std::string("winapiwrapper::addWindowClass(): Cannot add window class via RegisterClassEx()"));
		return false;
	}
	else
	{
		return true;
	}
}

BOOL winapiwrapper::addWindowClass(LPCTSTR lpszClassName, UINT uStyle) //возвращает структуру WNDCLASSEX дл€ создани€ окна, второй параметр -- ссылка кал бэк процедуру внутри класса
{

	WNDCLASSEX WndClsEx;

	WndClsEx.cbSize        = sizeof(WNDCLASSEX);
	WndClsEx.style         = uStyle;
	WndClsEx.lpfnWndProc   = winapiwrapper::MsgRoute;
	WndClsEx.cbClsExtra    = 0;
	WndClsEx.cbWndExtra    = 0;
	WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = lpszClassName;
	WndClsEx.hInstance     = hInstance;
	WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&WndClsEx))
	{
		throw  std::runtime_error(std::string("winapiwrapper::addWindowClass(): Cannot add window class via RegisterClassEx()"));
		return false;
	}
	else
		return true;

}

BOOL winapiwrapper::addWindowClass(LPCSTR lpszClassName, UINT uStyle, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIconSm)
{
	WNDCLASSEX WndClsEx;

	WndClsEx.cbSize        = sizeof(WNDCLASSEX);
	WndClsEx.style         = uStyle;
	WndClsEx.lpfnWndProc   = winapiwrapper::MsgRoute;
	WndClsEx.cbClsExtra    = 0;
	WndClsEx.cbWndExtra    = 0;
	WndClsEx.hIcon         = hIcon;
	WndClsEx.hCursor       = hCursor;
	WndClsEx.hbrBackground = hbrBackground;
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = lpszClassName;
	WndClsEx.hInstance     = hInstance;
	WndClsEx.hIconSm       = hIconSm;

	if(!RegisterClassEx(&WndClsEx))
	{
		throw std::runtime_error(std::string("winapiwrapper::addWindowClass(): Cannot add window class via RegisterClassEx()"));
		return false;
	}
	else
		return true;
}

BOOL winapiwrapper::createWindow(LPCSTR lpszClassName, int x, int y, int nWidth, int nHeight)
{

	hWnd = ::CreateWindow(lpszClassName,
			  "",
			  WS_OVERLAPPEDWINDOW,
			  x,
			  y,
			  nWidth,
			  nHeight,
			  NULL,
			  NULL,
			  hInstance, NULL);
			//this);			//передаем в винду указатель на текущий объект, чтобы потом его же получить

	if(!hWnd)
	{
		throw std::runtime_error(std::string("winapiwrapper::createWindow(): Cannot create window via ::CreateWindow()"));
		return FALSE;
	}

	wWidth = nWidth;
	wHeight = nHeight;

	return TRUE;

}

BOOL winapiwrapper::createWindow(int x, int y, int nWidth, int nHeight)
{

	return(createWindow("mywrapperdefclass",x,y,nWidth,nHeight));

}

BOOL winapiwrapper::createWindow()
{

	return(createWindow("mywrapperdefclass",0,0,800,600));

}

BOOL winapiwrapper::WshowWindow()
{
	if(::ShowWindow(hWnd,SW_NORMAL))
	{
		throw std::runtime_error(std::string("winapiwrapper::showWindow(): Cannot show window via ::ShowWindow()"));
		return FALSE;
	}
	if(!::UpdateWindow(hWnd))
	{
		throw std::runtime_error(std::string("winapiwrapper::showWindow(): Cannot update window via ::UpdateWindow()"));
		return FALSE;
	}

	return TRUE;
}

tyMessageIterator winapiwrapper::GetMessageHandler(long message)
{
	return m_MsgHandlers.find(message);			//ищем в контейнере событие

}

void winapiwrapper::AddMessageHandler(long message, tyMessageHandler lpHandler)
{
	m_MsgHandlers[message] = lpHandler;			//добавл€ем событие и метод, перезаписыва€ старые
}

LRESULT CALLBACK winapiwrapper::MsgRoute(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*winapiwrapper* pWnd;		//указатель на текущий объект
	LPCREATESTRUCT strc = (LPCREATESTRUCT)lParam;		//структура параметров

	if (uMsg == WM_NCCREATE)	//при создании окна
	{

		SetWindowLong(hwnd, GWL_USERDATA, (long)(strc->lpCreateParams));		//сохран€ем указатель на текущий объект в пам€ти винапи
	}

	pWnd = (winapiwrapper *)GetWindowLongPtr(hwnd, GWL_USERDATA);		//получаем этот указатель*/

	if (selfptr)
		return selfptr->WndProc(hwnd, uMsg, wParam, lParam);	//передаем управление нашей функции обработки
	else
		return DefWindowProc(hwnd, uMsg, wParam, lParam);	//передаем управление виндовой функции обработки
}

LRESULT CALLBACK winapiwrapper::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	tyMessageIterator itHandler = GetMessageHandler(Msg);	//получаем итератор контейнера мэп с указателем на нужный метод, обрабатывающий событие

	if(itHandler != m_MsgHandlers.end())						// если он найден (итератор не равен нулевому в списке)
	{
		tyMessageHandler handl = itHandler->second;				//получаем указатель на этот метод
		return (this->*handl)(hWnd, wParam, lParam);			//вызываем его с полученными от винды параметрами
	}
	else
		return DefWindowProc(hWnd, Msg, wParam, lParam);		//если не найден, то обрабатываем виндой
}


HDC winapiwrapper::getHDC() {
	if(hWnd == 0)
	{
		throw std::runtime_error(std::string("winapiwrapper::getHDC(): no window created yet"));
		return 0;
	}
	else
	{
		hDC = GetDC(hWnd);
		return hDC;
	}
}

HWND winapiwrapper::getHWND() {
	if(hWnd == 0)
	{
		throw std::runtime_error(std::string("winapiwrapper::getHWND(): no window created yet"));
		return 0;
	}
	else
	{
		return hWnd;
	}
}


int  winapiwrapper::onDestroy(HWND hWnd, long param0, long param1)
{
	//MessageBox(NULL, "destroying window", "exiting", MB_OK);
	 PostQuitMessage(0);
	  return 0;
}

int  winapiwrapper::onLeftMouseDown(HWND hWnd, long param0, long param1)
{
	//xPos = GET_X_LPARAM(param1);
	//yPos = GET_Y_LPARAM(param1);

	//std::stringstream msgtext;
	//msgtext<<"left mouse click at x"<<xPos<<" y"<<yPos;

	//MessageBox(NULL, msgtext.str().c_str(), "mouse", MB_OK);

	if(vpFunc)
	{
		vpFunc(WM_LBUTTONDOWN,param0, param1);
	}

	return 0;
}

int  winapiwrapper::onRightMouseDown(HWND hWnd, long param0, long param1)
{

	if(vpFunc)
	{
		vpFunc(WM_RBUTTONDOWN,param0, param1);
	}

	  return 0;
}

int  winapiwrapper::onMidMouseDown(HWND hWnd, long param0, long param1)
{
	MessageBox(NULL, "middle mouse down", "mouse", MB_OK);
	  return 0;
}

int winapiwrapper::onKeyDown(HWND hWnd, long param0, long param1)
{
	if(vpFunc)
	{
		vpFunc(WM_KEYDOWN,param0, param1);
	}
	  return 0;
}

int  winapiwrapper::onResize(HWND hWnd, long param0, long param1)
{
	if(vpFunc)
	{
		vpFunc(WM_SIZE, param0, param1);
		wWidth = LOWORD(param1);
		wHeight = HIWORD(param1);
	}
	  return 0;
}

int  winapiwrapper::onWheel(HWND hWnd, long param0, long param1)
{
	if(vpFunc)
	{
		vpFunc(WM_MOUSEWHEEL, param0, param1);
	}
	  return 0;
}


