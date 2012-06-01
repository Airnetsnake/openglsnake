#include "windows.h"
#include <stdexcept>
#include <typeinfo>
#include <windows.h>
#include <time.h>

#include "winapiwrapper.h"
#include "openglwrapper.h"
#include "datatype.h"
#include "snake.h"
#include "area.h"
#include "timer.h"

//#define GC_KEYDOWN 1
//#define GC_KEYUP 2
//#define GC_LMOUSE 2

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

typedef LARGE_INTEGER winlarge;

enum TypeLoop {
	FIXED_FPS,
	DYNAMIC_FPS
};

namespace GController
{
	class gameController
	{
	private:
		HINSTANCE hInstance;

		static gameController *selfptr;
		winapiwrapper *window;
		openglwrapper *opengl;
		explosion *expl;		//новое
		snake *playerSnake;
		area *curLevel;

		txyz playerPos;
		txyz fruitPos;
		txyz direction;

		bool arrowPressed;
		bool gamePaused;
		bool fixCamera;
		bool animooted;

		int frames;

		time_t start_tick;
		time_t end_tick;
		double frame_time;

		void spawnFruit(int,int,int); //spawn fruit in range

		Timer loopTimer;
		TypeLoop fpsUpdateType;

	public:
		gameController(HINSTANCE hInst);
		~gameController();
		bool init();
		void go();
		void mainloop();
		static bool callBack(int, long, long);
		void ProcessEvent(int, long, long);
	};
}
#endif /* GAMECONTROLLER_H_ */
