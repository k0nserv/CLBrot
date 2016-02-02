#include "Engine.h"


Engine::Engine(const string& source) {
	settings = Settings::getInstance();
	settings->addWatcher(this);
	Mandelbrot m = settings->getMandelBrot();
	char* src = const_cast<char*>(source.c_str());
	ocl = new OCLWrapper(src);
	colors = new Color[m.width * m.height];

	init();
}

Engine::~Engine() {
	ocl->freeMemory();

	delete ocl;
}


Color* Engine::getPixels() {
	if(!ocl->isDone())
		runKernel();

	return colors;
}

char* Engine::getBuildResult() {
	return buildResult;
}

void Engine::zoomAroundPoint(int dz, double xC, double yC) {
    double change, xDeltaN, xDeltaP, yDeltaN, yDeltaP;
	/* Retive max and min coordiantes */
	Complex max, min;
	Mandelbrot result = settings->getMandelBrot();
	settings->getMaxAndMin(max, min);


	if (dz == 0 )
		return;//Prevent dividing by zero

	//change = 1 - (dz / 100.0)*settings->getZoomFactor();
	change = 1 - settings->getZoomFactor()*( (dz >= 0 )? 1 : -1 );

	xDeltaN = min.re - xC;
	xDeltaP = max.re - xC;
    yDeltaN = min.im - yC;
	yDeltaP = max.im - yC;

    Complex newMax = max;
    Complex newMin = min;
	newMax.re = xDeltaP * change + xC;
    newMax.im = yDeltaP * change + yC;
    newMin.re = xDeltaN * change + xC;
    newMin.im = yDeltaN * change + yC;

	result.max = newMax;
	result.min = newMin;

	settings->setMandelbrot(result);
}


void Engine::init() {
	/* Build the opencl kernel */
	ocl->build(KERNEL_ENTRY);
	buildResult = ocl->buildLog();

	buildMap();
}

/* Run the kernel on the device */
void Engine::runKernel() {
	Mandelbrot m = settings->getMandelBrot();
	uint width, height;
	width = m.width;
	height= m.height;
	

    ocl->freeMemory();
    ocl->addArgument(&m, sizeof (Mandelbrot));
	ocl->addArgument(map, sizeof (Color) * settings->getNumberOfColors());
    ocl->addReturn(width * height * sizeof (Color));
    ocl->execute(width*height, 16);
    delete[] colors;
    colors = new Color[width * height];
    ocl->readMemory(width * height * sizeof (Color), 0, colors);
}


/* Calculate the color lookup table */
void Engine::buildMap () {	
	Mandelbrot m = settings->getMandelBrot();
	map = &(settings->g->map[0]);

	/*
	map = new Color[m.numberOfColors];
    for (uint i = 0; i < m.numberOfColors; i++) {
        Color c;
        c.r = c.g = c.b = 0;

        if (i <= 255)
            c.b = i;

        else if (i > 255 && i <= 512) {
            c.b = 255;
            c.r = i;
            c.g = i;
        }

        else if (i >= 512 && i <= 569) {
            c.r = 255;
            c.g = 767 - i;
            c.b = 767 - static_cast<cl_ushort> (i * 1.34);
        }

        else if (i >= 569 && i <= 767) {
            c.r = 255;
            c.g = 767 - i;
        }

        this->map[i] = c;
    }*/
}

void Engine::dataChanged() {
	runKernel();
}