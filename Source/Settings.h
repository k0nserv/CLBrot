#ifndef SETTINGS_H
#define SETTINGS_H
#include <vector>

#include "Definitions.h"
#include "Watcher.h"
#include "SettingsDirectory.h"
#include "Gradient.h"



#define DEFAULT_ESCAPE_RADIUS 2
#define DEFAULT_ITERATION_COUNT 50
#define DEFAULT_MAX_RE 1.0
#define DEFAULT_MAX_IM 1.0
#define DEFAULT_MIN_RE -2.1
#define DEFAULT_MIN_IM -1.0
#define DEFAULT_WIDTH  1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_ZOOM_FACTOR 0.05
#define DEFAULT_NUMBER_OF_COLORS 767

class Settings : public Watcher{
public:
	static Settings* getInstance();
	Gradient* g;

	void setMandelbrot(const Mandelbrot&);
	void setNumberOfColors(uint);

    void setSettingsDirectory(SettingsDirectory*);
    SettingsDirectory* getSettingsDirectory();

	Mandelbrot getMandelBrot();
	void getMaxAndMin(Complex&, Complex&);
	double getZoomFactor();
	uint getNumberOfColors();

	/* Utility */
	void calculateCoordinateScale();
	void pixelsToCoordinates(uint, uint, double&, double&);
	uint saveCurrentPosition();
	bool loadPosition(uint);

private:
	static Settings* instance;

	Settings();
	~Settings();
	Mandelbrot mandelbrot;
    SettingsDirectory* settingsDirectory;
	std::vector<Position> savedPositions;
	double zoomFactor;


};
#endif