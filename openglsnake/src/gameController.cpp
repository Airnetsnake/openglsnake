#include <iostream>
#include "gameController.h"


#define fixedFPS 60
#define FRAMES_PER_STEP 8 //на отрисовку 1 перемещения тратится 8 кадров

#define X_ 0
#define Y_ 1
#define Z_ 2

int score = 0;

void loadBand(int n);
void waiter(long wticks);

//void newExplosion(float pos[3], float Vinit[3]);
float pos[3];
float Vinit[3];

namespace GController
{
	bool failFlag = false;

	gameController *gameController::selfptr = 0;

	gameController::gameController(HINSTANCE hInst):
				hInstance(hInst),
				window(0),
				opengl(0),
				//expl(0),							//новое
				frames(0),
				arrowPressed(false),
				gamePaused(false),
				fixCamera(true),
				animooted(true),
				start_tick(0),
				end_tick(0),
				frame_time(0),
				fpsUpdateType(FIXED_FPS)
	{

		OutputDebugString("aaa");
		selfptr = this;

		//set start positions
		direction[X_] = 0;	// up/down
		direction[Y_] = 0;	//	depths
		direction[Z_] = 1;	//	left/right

		//initial position
		playerPos[X_] = 0;
		playerPos[Y_] = 0;
		playerPos[Z_] = 0;

		//fruit
		fruitPos[X_] = -1;
		fruitPos[Y_] = -1;
		fruitPos[Z_] = -1;

		srand (time(NULL));
	}

	gameController::~gameController()
	{
		delete window;
		delete opengl;
	}

	bool gameController::init()
	{
		window = new winapiwrapper(hInstance);
		playerSnake = new snake(0,0,1); //new snake moving to the right
		expl = new explosion();											//новое
		spawnFruit(10,0,10);
		playerSnake->add(); //add length

		try
		{
			window->createWindow(300,300,500,500);
			window->WshowWindow();
			window->setCallBack(&gameController::callBack);

			opengl = new openglwrapper(window->getHDC());
			opengl->EnableOpenGL(window->getHWND());
		}
		catch ( std::exception &e )
		{
			std::cerr << "Caught " << e.what( ) << std::endl;
			return false;
		};

		return true;

	}

	void gameController::go()
	{
		MSG Msg;
		BOOL bRet;

		while (1)
		{
			if((bRet = PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) != 0)
			{
				if (Msg.message == WM_QUIT)
					break;
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			else
			{
				//main game loop
				mainloop();
			}
		}

	}

	bool gameController::callBack(int type, long param0, long param1)
	{
		if(!selfptr)
			return false;

		selfptr->ProcessEvent(type, param0, param1);
		return true;
	}

	void gameController::ProcessEvent(int type, long param0, long param1)
	{
		if (type == WM_KEYDOWN)
		{
			switch (param0)
			{

				case(VK_UP):

					if(!arrowPressed)
					{
						direction[X_] = 1;
						direction[Y_] = 0;
						direction[Z_] = 0;

						arrowPressed = true;
					}

				break;

				case(VK_DOWN):

					if(!arrowPressed)
					{
						direction[X_] = -1;
						direction[Y_] = 0;
						direction[Z_] = 0;

						arrowPressed = true;
					}

				break;

				case(VK_LEFT):
					if(!arrowPressed)
					{
						direction[X_] = 0;
						direction[Y_] = 0;
						direction[Z_] = -1;

						arrowPressed = true;
					}
				break;

				case(VK_RIGHT):
					if(!arrowPressed)
					{
						direction[X_] = 0;
						direction[Y_] = 0;
						direction[Z_] = 1;

						arrowPressed = true;
					}

				break;

				case(VK_HOME):
					direction[X_] = 0;
					direction[Y_] = 1;
					direction[Z_] = 0;
				break;

				case(VK_END):
					direction[X_] = 0;
					direction[Y_] = -1;
					direction[Z_] = 0;
				break;

				case(VK_SPACE):
					animooted = !animooted;
				break;

				//A
				case(0x41):
					playerSnake->add();
				break;

				//C
				case(0x43):
					if(!fixCamera) 
					{
						opengl->initCamera();
						fixCamera = true;
					}
					else
						fixCamera = false;
				break;


				case(VK_ESCAPE):
					opengl->DisableOpenGL();
					window->quit();
				break;
			}
		}
	}

	void gameController::mainloop()
	{
		//FPS counter
		loopTimer.start();

		opengl->begindraw();
		//opengl->drawAxis(); //draw axis
		//opengl->drawCube(0,0,0); //draw cube in that location

		//logic moving the snake

		if (frames == FRAMES_PER_STEP)	//if new step started
		{
			//do move
			playerPos[X_] += direction[X_];	//move the snake head
			playerPos[Y_] += direction[Y_];
			playerPos[Z_] += direction[Z_];

			playerSnake->move(direction);	//move the tail

			frames = 0;	
			arrowPressed = false;
		}

			
		if(playerPos[X_] == fruitPos[X_] &&		//if the fruit hit
			playerPos[Y_] == fruitPos[Y_] && 
			playerPos[Z_] == fruitPos[Z_])
		{
			playerSnake->add();	//add body cell
			spawnFruit(10,0,10);
		}

		//drawing the snake here

		if(!fixCamera)	//follow the snake head with camera or set camera to the def points
			opengl->setCameraCellDelta(
									playerPos[X_], playerPos[Y_], playerPos[Z_], 
									direction[X_] * (float)frames/FRAMES_PER_STEP,
									direction[Y_] * (float)frames/FRAMES_PER_STEP,
									direction[Z_] * (float)frames/FRAMES_PER_STEP,
									direction[X_], direction[Y_], direction[Z_]
									);

		opengl->drawCubeCellsDelta(			//snake head
									playerPos[X_], 
									playerPos[Y_], 
									playerPos[Z_],
									direction[X_] * (float)frames/FRAMES_PER_STEP * ((animooted)?1.0f:0),//animate if enabled
									direction[Y_] * (float)frames/FRAMES_PER_STEP * ((animooted)?1.0f:0),
									direction[Z_] * (float)frames/FRAMES_PER_STEP * ((animooted)?1.0f:0)
									);	

		//draw snake body

		txyz prev = { playerPos[X_],  playerPos[Y_],  playerPos[Z_]};
		const snakeCell *next = playerSnake->getHead()->next;


		//making edges
		if (playerPos[X_] > 30)
		{
			playerPos[X_]-=58;
		}
		else if (playerPos[X_] < -30)
		{
			playerPos[X_]+=58;
		}
		else if (playerPos[Y_] > 30)
		{
			playerPos[Y_]-=58;
		}
		else if (playerPos[Y_] < -30)
		{
			playerPos[Y_]+=58;
		}
		else if (playerPos[Z_] > 30)
		{
			playerPos[Z_]-=58;
		}
		else if (playerPos[Z_] < -30)
		{
			playerPos[Z_]+=58;
		};


		//go through the linked list
		while(next)
		{
			prev[X_] -= next->move.x;
			prev[Y_] -= next->move.y;
			prev[Z_] -= next->move.z;

			//draw each body part
			opengl->drawCubeCellsDelta(
					prev[X_],
					prev[Y_],
					prev[Z_],
					next->move.x*(float)frames/FRAMES_PER_STEP * ((animooted)?1.0f:0),	//animate if enabled
					next->move.y*(float)frames/FRAMES_PER_STEP * ((animooted)?1.0f:0),
					next->move.z*(float)frames/FRAMES_PER_STEP * ((animooted)?1.0f:0)
				);

			next = playerSnake->getNext(next);

			//where we can fail
			if 	(playerPos[X_] == prev[X_] &&		
			playerPos[Y_] == prev[Y_] && 
			playerPos[Z_] == prev[Z_])
			{
				if (failFlag == false)
				{
				std::cout << "\nYou failed. Total Score: " << score-1;
				opengl->DisableOpenGL();
				failFlag = true;
				}
				//window->quit();
			}
		}
			
		//draw the fruit
		if(fruitPos[X_] != 100)
		{
			opengl->drawCubeCellsStatic(fruitPos[X_], fruitPos[Y_], fruitPos[Z_]);	
		}

		frames++;
		opengl->enddraw();

		//wait until fixed frame time is reached
		float fps = (float)loopTimer.getTicksRate().QuadPart/loopTimer.getTime().QuadPart;
	
		if(fps > fixedFPS)
		{
			waiter(long(loopTimer.getTicksRate().QuadPart/fixedFPS - loopTimer.getTime().QuadPart));
		}

		loopTimer.reset();

	}

	//support pow
	int power(int val, unsigned int P)
	{
     int r = val;
     while (--P)
         r *= val;
     return r;
	}

	void gameController::spawnFruit(int xr, int yr, int zr)
	{
		//throw fruit randomly
		std::cout << "\rScore " << score;

		int xdel = (xr!=0)?(int)rand()%xr:0;
		int ydel = (yr!=0)?(int)rand()%yr:0;
		int zdel = (zr!=0)?(int)rand()%zr:0;

		/*int xt = -(int)xr/2 + xdel;
		int yt = -(int)yr/2 + ydel;
		int zt = -(int)zr/2 + zdel;*/

		//fruitPos[X_] = -(int)xr/2 + xdel;
		//fruitPos[Y_] = -(int)yr/2 + ydel;
		//fruitPos[Z_] = -(int)zr/2 + zdel;

		fruitPos[X_] = power(-1, (int)rand()) * xdel;
		fruitPos[Y_] = power(-1, (int)rand()) * ydel;
		fruitPos[Z_] = power(-1, (int)rand()) * zdel;

		pos[0] = fruitPos[X_];						//
		pos[1] = fruitPos[Y_];
		pos[2] = fruitPos[Z_];

		Vinit[0]=1;									//новое
		Vinit[1]=1;
		Vinit[2]=1;


		expl->newExplosion(pos, Vinit);			//
		expl -> DrawExplosion(pos);
		score++;
	}

}

//wait wticks ticks
void waiter(long wticks)
{
	if(wticks <= 0)
		return;

	Timer wtimer;
	wtimer.start();

	do{} while( wtimer.getTime().QuadPart < wticks );

}