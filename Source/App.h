#ifndef APP_H
#define APP_H
#include <tchar.h>
#include <Windows.h>
#include <WindowsX.h>
#include <atlimage.h>
#include <sstream>

#include <gl/glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include "Definitions.h"
#include "Engine.h"
#include "SettingsDirectory.h"
#include "Console.h"
#include "Settings.h"



#define KERNEL_SRC "kernel.c"
#define FULLSCREEN_STYLE  ( WS_EX_TOPMOST | WS_OVERLAPPED | WS_POPUP )


class App {
public:
    App (HINSTANCE, uint, uint);
    ~App ();

	LRESULT msgProc (UINT msg, WPARAM wParam, LPARAM lParam);
    int  run ();

private:
    /* Window related variables and handles */
    HINSTANCE hInstance;
    HWND mainWnd;
    HDC hDC;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC hRC;

	Console* console;
	SettingsDirectory* settingsDirectory;
	Settings* settings;
	Engine* engine;

    
    bool lowIteration;
	bool fullscreen;
	uint width;
    uint height;
	uint previousWidth;
	uint previousHeight;

	/* Inti functions */
	void init();
	void initWindow ();
    void initGL ();

	/* Input Detection and response */    
	void keyDown (WPARAM, short int);
	void charInput(WPARAM);
	void zoomAroundMouse (int, int, int);

	/* Size updates */
	void resize (uint, uint);
    void changeSize (uint, uint);

	/* Utility functions */
    void captureScreen ();

	/* Rendering */
	void render ();
};
#endif