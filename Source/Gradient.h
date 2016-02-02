#ifndef GRADIENT_H
#define GRADIENT_H
#include "definitions.h"
#include <vector>

using std::vector;




class Gradient{
public:
	Gradient(Color,Color, bool HSVMode=false);

	void addColorStop(Color);
	uint numberOfColors();
	vector<Color> map;


private:
	vector<Color> stops;
	
	/* Use HSV colors instead */
	bool HSVMode;
	uint size;
	void calculateGradientHSV(const Color&, const Color&, vector<Color>*);
	void calculateGradientRGB(const Color&, const Color&, vector<Color>*);
	vector<Color> calculate();
	
	HSV RGBtoHSV(Color);
	Color HSVtoRGB(HSV);
};
#endif