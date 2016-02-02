#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <CL/opencl.h>
#include <string>

#define USE_DOUBLE

typedef unsigned int uint;
typedef unsigned short int uint_8;

typedef struct Color{
	cl_uchar r;
	cl_uchar g;
	cl_uchar b;
	Color() : r(255), g(255), b(255) {}
	Color(cl_uchar red, cl_uchar green, cl_uchar blue) : r(red), g(green), b(blue) {}
}Color;

typedef struct HSV{
	int h;
	double s;
	double v;
}HSV;



typedef struct Complex {
#ifdef USE_DOUBLE
	cl_double re;
	cl_double im;
#else
	cl_float re;
	cl_float im;
#endif
} Complex;

typedef struct Position {
	Complex max;
	Complex min;
}Position;



typedef struct Mandelbrot{
	Complex min;
	Complex max;
	cl_ushort height;
	cl_ushort width;
#ifdef USE_DOUBLE
	cl_double xScale;
	cl_double yScale;
#else
	cl_float xScale;
	cl_float yScale;
#endif
	cl_ushort iter;
	cl_ushort escape;
	cl_uint   numberOfColors;
}Mandelbrot;

#endif
