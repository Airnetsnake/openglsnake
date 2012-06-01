#include "windows.h"
#include "windowsx.h"

#include <vector>
#include <iterator>
#include <map>
#include <sstream>

#ifndef WINAPIWRAPPER_H_
#define WINAPIWRAPPER_H_

class winapiwrapper;


typedef bool (*tyCallBack)(int,long,long);
typedef int (winapiwrapper::* tyMessageHandler)(HWND, long, long); //тип tyMessageHandler -- функци€, возвращающа€ long
typedef std::map<long, tyMessageHandler> tyMessageMap;     //тип tyMessageMap -- ассоциативный список —обытий и прив€занных к ним обрабтчиков
typedef tyMessageMap::iterator tyMessageIterator;      //тип tyMessageIterator -- итератор map списка tyMessageMap

class winapiwrapper
{

private:

	HINSTANCE hInstance;
	HWND hWnd;
	HDC hDC; //device context

	tyCallBack vpFunc;
	tyMessageMap m_MsgHandlers;

	int wHeight, wWidth;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	tyMessageIterator GetMessageHandler(long message);						//возвращает элемент ассоциативного контейнера, содержащий ссылку на нужный метод
	void AddMessageHandler(long message, tyMessageHandler lpHandler);

	//событи€

	int onDestroy(HWND hWnd, long param0, long param1);
	int onLeftMouseDown(HWND hWnd, long param0, long param1);
	int onRightMouseDown(HWND hWnd, long param0, long param1);
	int onMidMouseDown(HWND hWnd, long param0, long param1);
	int onKeyDown(HWND hWnd, long param0, long param1);
	int onResize(HWND hWnd, long param0, long param1);
	int onWheel(HWND hWnd, long param0, long param1);
	//

	static winapiwrapper* selfptr;

public:

	winapiwrapper(HINSTANCE hInstance);
	~winapiwrapper();

	BOOL createWindow(LPCSTR lpszClassName, int x, int y, int nWidth, int nHeight);	//окно произвольного класса
	BOOL createWindow(int x, int y, int nWidth, int nHeight);		//окно дефолтного класса
	BOOL createWindow();

	BOOL addWindowClass(const WNDCLASSEX &WndClsEx);	//регистрирует новый класс, в качестве параметра -- ссылка на заполненную струкртуру WNDCLASSEX. ——ылка, чтобы не копировать лишний раз эту структуру.
	BOOL addWindowClass(LPCSTR lpszClassName, UINT uStyle);	//то же, но по параметрам -- им€ класса и стиль
	BOOL addWindowClass(LPCSTR lpszClassName, UINT uStyle, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIconSm); //то же, но по параметрам -- им€ класса, стиль и прочее

	BOOL WshowWindow();

	HDC getHDC();
	HWND getHWND();

	int getWindowHeight() {return wHeight;}
	int getWindowWidth() {return wWidth;}

	void setCallBack (tyCallBack vpF) {vpFunc = vpF;}

	static LRESULT CALLBACK MsgRoute(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void quit() {PostQuitMessage(0);}

};

#endif /* GAMECONTROLLER_H_ */
