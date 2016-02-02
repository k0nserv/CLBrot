#define USE_DOUBLE

#ifdef USE_DOUBLE
	#pragma OPENCL EXTENSION cl_khr_fp64: enable
#endif


/* Data structures */
typedef struct Color{
	uchar r;
	uchar g;
	uchar b;
}Color;

typedef struct Complex {
#ifdef USE_DOUBLE
	double re;
	double im;
#else
	float re;
	float im;
#endif
} Complex;

typedef struct Mandelbrot{
	Complex min;
	Complex max;
	ushort height;
	ushort width;
#ifdef USE_DOUBLE
	double xScale;
	double yScale;
#else
	float xScale;
	float yScale;
#endif
	ushort iter;
	ushort escape;
	uint   numberOfColors;
}Mandelbrot;

/* Complex functions */
double cDot(const Complex z) {
	/* (x*x) + (y*y) */
	return (z.re * z.re) + (z.im * z.im);
}

double cModulus(const Complex z) {
	return sqrt(cDot(z));
}

Complex cSquare(const Complex z) {
	/*
	* x + iy
	* x^2 + 2iy - y^2
	*/
	Complex result;
	result.re = z.re * z.re - z.im*z.im;
	result.im = 2 * z.im * z.re;

	return result;
}

Complex cAdd(const Complex a, const Complex b) {
	Complex result;
	result.re = a.re + b.re;
	result.im = a.im + b.im;

	return result;
}

/* Utility functions */
Complex pixelsToCoordinates(const uint x, const uint y, Complex min, double xS, double yS) {
	Complex p;

	p.re = min.re + x * xS;
	p.im = min.im + y * yS;

	return p;
}

double smoothColor(const double iterations, const Complex z, const double escape) {
	return iterations + 1 - log(log(cModulus(z)))/log(escape);
}



__kernel void kernel_entry(__constant const Mandelbrot* m, __constant  const Color* map, __global Color* pixels) {
	uint x, y, index, escape;
	Complex z, c;

	z.re = 0;
	z.im = 0;
	escape = m->escape * m->escape;

	index = get_global_id(0);
	/* Retrive current x and y */
	x = index % m->width;
	y = index / m->width;

	//Calculate the scaled coordinate
	c = pixelsToCoordinates(x, y, m->min, m->xScale, m->yScale);

	int iterations = 0;
	while(iterations < m->iter) {
		z = cAdd(cSquare(z), c);
		double radii = cDot(z);
		if ( radii >= escape )
			break;

		iterations++;
	}

	Color color;
	color.r = color.g = color.b = 0;

	double smooth = smoothColor(iterations, z, escape);
	uint i = (int)(smooth/m->iter * m->numberOfColors);

	if(i > m->numberOfColors) i = m->numberOfColors;
	if(i < 0 ) i = 0;

	if(iterations != m->iter)
		color = map[i];

	pixels[index] = color;
}