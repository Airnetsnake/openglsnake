#ifndef OPENGLWRAPPER_H_
#define OPENGLWRAPPER_H_

#include "windows.h"
#include "wingdi.h"

#include <stdexcept>
#include <typeinfo>

#include <gl/gl.h>
#include <gl/glu.h>
#include "math.h"

class openglwrapper {
private:
	HDC hDC;
	HGLRC hRC;
	HWND hWnd;

	bool switched1;
	bool switched2;
	bool isPixelFormatSet;

	float fTriangleTheta;
	float fXSpeed, fYSpeed, fZSpeed;

	static openglwrapper* selfptr;
	bool inited;
	GLfloat fval;
	GLfloat fzoom;

	//opengl verticies

	static GLfloat cube_verticies[];
	static GLfloat cubeEdge;
	static GLfloat cellSize;

	static GLubyte frontIndicies[];
	static GLubyte leftIndicies[];
	static GLubyte rightIndicies[];
	static GLubyte botIndicies[];
	static GLubyte topIndicies[];
	static GLubyte backIndicies[];

	//methods

public:
	openglwrapper(HDC hDC);
	~openglwrapper();

	bool EnableOpenGL(HWND hWnd);
	bool DisableOpenGL();
	void setHDC(HDC h) {hDC = h;};
	void reshape(int, int);

	BOOL SetPixelFormat();
	BOOL SetPixelFormat(DWORD dwFlags, BYTE iPixelType, BYTE cColorBits, BYTE cDepthBits);
	bool CreateContext();

	void drawAxis();
	void initCamera();
	void initSettings();

	void begindraw();
	void enddraw();

	void setXSpeed(float speed){fXSpeed = speed;}
	void setYSpeed(float speed){fYSpeed = speed;}
	void setZSpeed(float speed){fZSpeed = speed;}

	void upZoom(GLfloat step){fzoom+=step/1200;}
	void downZoom(GLfloat step){fzoom-=step/100;}

	void drawCube(GLfloat,GLfloat,GLfloat);
	void drawCubeCells(int x, int y, int z);
	void drawCubeCellsStatic(int x, int y, int z);

	void drawCubeCellsDelta(int x, int y, int z, float dx, float dy, float dz);

	void drawCage(int xs, int ys, int zs);

	void moveSpawn(GLfloat x, GLfloat y, GLfloat z); 

	void setCamera(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat px, GLfloat py, GLfloat pz, int upx, int upy, int upz);
	void setCameraCell(int cx, int cy, int cz, int upx, int upy, int upz);
	void setCameraCellDelta(int cx, int cy, int cz,
							float dx, float dy, float dz,
							int upx, int upy, int upz);

};



#endif /* OPENGLWRAPPER_H_ */
