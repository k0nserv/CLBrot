#include <CL/cl.h>
#include <iostream>
#include <Windows.h>
#include <tchar.h>

#include "OCLWrapper.h"
#include "App.h"
#include "definitions.h"
#include "Settings.h"




int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,int nCmdShow){
	App* app = new App(hInstance, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	return app->run();
}
