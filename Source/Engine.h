#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "OCLWrapper.h"
#include "WatcherCallback.h"
#include "Definitions.h"
#include "Settings.h"
#include "Gradient.h"

#define KERNEL_ENTRY "kernel_entry"

using std::string;

class Engine : public WatcherCallback{
public:
	Engine(const string&);
	~Engine();

	/* Getters */
	Color* getPixels();
	char* getBuildResult();

	/* Utility */
	void zoomAroundPoint(int, double, double);
	void dataChanged();


private:
	/* OCL wrapper to extend easier management of the opencl framework */
	OCLWrapper* ocl;
	/* This array is filled out by the kernel with the pixel data the render resulted in. */
	Color* colors;
	/* Contains the color map used for color lookup, passed as an argument to the kernel. */
	Color* map;
	/* Settings class */
	Settings* settings;
	
	char* buildResult;
	/* Number of colors used in map */
    uint numberOfColors;

	void init();
	void buildMap();
	void runKernel();

	
};


#endif