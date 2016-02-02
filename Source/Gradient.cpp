#include "Gradient.h"



Gradient::Gradient(Color start,Color stop, bool HSVMode) {
	stops.push_back(start);
	stops.push_back(stop);
	this->HSVMode = HSVMode;

	map = calculate();
}

void Gradient::addColorStop(Color c){
	stops.push_back(c);
	map = calculate();
}

uint Gradient::numberOfColors() {
	return size;
}

vector<Color> Gradient::calculate(){
	vector<Color> result = vector<Color>();

	Color previous = stops.at(0);
	for(int i = 1; i <stops.size(); i++) {
		if(HSVMode)
			calculateGradientHSV(previous, stops.at(i), &result);
		else
			calculateGradientRGB(previous, stops.at(i), &result);
		
		previous = stops.at(i);
	}

	size = result.size();

	return result;
}

void Gradient::calculateGradientRGB(const Color& start, const Color& stop, vector<Color>* res) {
	double dr, dg, db, 
		   sr, sg, sb,
		   r, g, b, highest;
	dr = stop.r - start.r;// dr = abs(dr);
	dg = stop.g - start.g;// dg = abs(dg),
	db = stop.b - start.b;// db = abs(db);
	r  = start.r;
	g  = start.g;
	b  = start.b;

	highest = abs(dr);//Default
	if (abs(dg) > highest ) 
		highest = abs(dg);
	if(abs(db) > highest)
		highest = abs(db);

	sr = dr/highest;
	sg = dg/highest;
	sb = db/highest;

	for(int i = 0; i < highest; i++) {
		Color c;
		c.r = r;
		c.g = g;
		c.b = b;

		res->push_back(c);
		r += sr;
		g += sg;
		b += sb;
	}
}

void Gradient::calculateGradientHSV(const Color& start, const Color& stop, vector<Color>*res) {
	double dh, ds, dv,
		   sh, ss, sv,
		   s,  h,  v, hieghest;
	HSV startHSV, stopHSV;
	startHSV = RGBtoHSV(start);
	stopHSV	 = RGBtoHSV(stop);

	dh = stopHSV.h - startHSV.h;
	ds = stopHSV.s - startHSV.s;
	dv = stopHSV.v - startHSV.v;
	h  = startHSV.h;
	s  = startHSV.s;
	v  = startHSV.v;

	sh = dh/360;
	ss = ds/100;
	sv = dv/100;

	for(;;) {
		if ( s > 1.0 || s < 0.0 ||  v > 1.0 || v < 0.0)
			break;
		HSV c;
		c.h = h;
		c.s = s;
		c.v = v;

		res->push_back(HSVtoRGB(c));
		s += sh;
		h += ss;
		v += sv;
	}
}


HSV Gradient::RGBtoHSV(Color c){
	/* Change color space */
	double r = c.r/255;
	double g = c.g/255;
	double b = c.b/255;

	double maxC = b;
	if (maxC < g) maxC = g;
	if (maxC < r) maxC = r;
	double minC = b;
	if (minC > g) minC = g;
	if (minC > r) minC = r;

	double delta = maxC - minC;

	double V = maxC;
	double S = 0;
	double H = 0;

	if (delta == 0)
	{
		H = 0;
		S = 0;
	}
	else
	{
		S = delta / maxC;
		double dR = 60*(maxC - r)/delta + 180;
		double dG = 60*(maxC - g)/delta + 180;
		double dB = 60*(maxC - b)/delta + 180;
		if (r == maxC)
			H = dB - dG;
		else if (g == maxC)
			H = 120 + dR - dB;
		else
			H = 240 + dG - dR;
	}

	if (H<0)
		H+=360;
	if (H>=360)
		H-=360;
	HSV ret;
	ret.h = (int)H;
	ret.s = S;
	ret.v = V;

	return ret;
}


Color Gradient::HSVtoRGB(HSV a){
	double r,g,b;
	double s = a.s;
	double v = a.v;
	double h = a.h;
	double c = v*s;
	h /= 60; // sector 0 to 5
	int i = (int)h;
	double f = h - i; // factorial part of h
	double p = v * (1 - a.s);
	double q = v * (1 -s * f);
	double t = v * (1 - s * (1 - f));

	switch(i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;

	case 1:
		r = q;
		g = v;
		b = p;
		break;

	case 2:
		r = p;
		g = v;
		b = t;
		break;

	case 3:
		r = p;
		g = q;
		b = v;
		break;

	case 4:
		r = t;
		g = p;
		b = v;
		break;

	default: // case 5:
		r = v;
		g = p;
		b = q;
	}
	Color ret;
	ret.r = r * 255;
	ret.g = g * 255;
	ret.b = b * 255;

	return ret;
}


