#include "Settings.h"

Settings* Settings::instance = NULL;


Settings* Settings::getInstance() {
	if(instance == NULL)
		instance = new Settings();

	return instance;
}

Settings::Settings() {
    settingsDirectory = NULL;
	zoomFactor = DEFAULT_ZOOM_FACTOR;
	g = new Gradient(Color(0, 6, 90), Color(85, 164, 223));
    g->addColorStop(Color(255, 255, 255));
	g->addColorStop(Color(255, 163, 15));
    g->addColorStop(Color(15, 211, 255));
    g->addColorStop(Color(219, 255, 15));
    g->addColorStop(Color(255, 15, 15));
    /*g->addColorStop(Color(71, 255, 15));
    g->addColorStop(Color(239, 15, 255));
    g->addColorStop(Color(167, 0, 179));
    g->addColorStop(Color(179, 0, 137));
    */

	/* Setup a default mandelbrot */
	Complex max, min;
	max.re	= DEFAULT_MAX_RE;
	max.im  = DEFAULT_MAX_IM;
	min.re  = DEFAULT_MIN_RE;
	min.im  = DEFAULT_MIN_IM;

	Mandelbrot m;
	m.escape = DEFAULT_ESCAPE_RADIUS;
	m.iter   = DEFAULT_ITERATION_COUNT;
	m.max    = max;
	m.min    = min;
	m.width  = DEFAULT_WIDTH;
	m.height = DEFAULT_HEIGHT;
	m.numberOfColors = DEFAULT_NUMBER_OF_COLORS;
	m.numberOfColors = g->numberOfColors();

	setMandelbrot(m);
	calculateCoordinateScale();
}


Settings::~Settings() {
	delete instance;
	delete[] g;
}

void Settings::setMandelbrot(const Mandelbrot& mandelbrot) {
	this->mandelbrot = mandelbrot;
	calculateCoordinateScale();
}

void Settings::setSettingsDirectory(SettingsDirectory* st) {
    if(settingsDirectory == NULL)//Only allowed to set once
        settingsDirectory = st;
}

void Settings::setNumberOfColors(uint numberOfColors) {
	this->mandelbrot.numberOfColors = numberOfColors;
}


Mandelbrot Settings::getMandelBrot() {
	return mandelbrot;
}

SettingsDirectory* Settings::getSettingsDirectory() {
    return settingsDirectory;
}


void Settings::getMaxAndMin(Complex& max, Complex& min) {
	max = mandelbrot.max;
	min = mandelbrot.min;
}

double Settings::getZoomFactor() {
	return zoomFactor;
}

uint Settings::getNumberOfColors() {
	return mandelbrot.numberOfColors;
}

void Settings::calculateCoordinateScale() {
	double xs, ys;
	Complex max, min;
	uint w,h;
	
	max = mandelbrot.max;
	min = mandelbrot.min;
	w   = mandelbrot.width;
	h   = mandelbrot.height;

	xs = ( max.re - min.re ) / static_cast<double>(w);
	ys = ( max.im - min.im ) / static_cast<double>(h);
	
	mandelbrot.xScale = xs;
	mandelbrot.yScale = ys;

	fire();
}

/* Calcualte a pixel coordinate to the corresponding complex coordinate */
void Settings::pixelsToCoordinates(uint x, uint y, double& xC, double& yC) {
	Complex max,min;
	double xs, ys;
	
	max = mandelbrot.max;
	min = mandelbrot.min;
	xs  = mandelbrot.xScale;
	ys  = mandelbrot.yScale;

	xC  = min.re + x * xs;
	yC  = min.im + y * ys;
}



uint Settings::saveCurrentPosition() {
	Position p;
	p.max = mandelbrot.max;
	p.min = mandelbrot.min;

	savedPositions.push_back(p);

	/* Return the position of the save */
	return savedPositions.size()-1;
}


bool Settings::loadPosition(uint index) {
	if( index < 0 || index >= savedPositions.size() )
		return false;//Invalid index

	Position p = savedPositions.at(index);
	Mandelbrot m = mandelbrot;
	m.max = p.max;
	m.min = p.min;

	/* Done this way instead of modifing the internal object
	   for consitency in firing event through setMandeblort()
	*/
	setMandelbrot(m);

	return true;
}