#ifndef CONSOLE_H
#define CONSOLE_H
#include <vector>
#include <Windows.h>
#include <Wingdi.h>
#include <sstream>

#include <gl/glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Definitions.h"
#include "Font.h"
#include "Input.h"
#include "Parser.h"
#include "Command.h"



//I am lazy
using std::vector;
using std::string;

class Console {
public:
	Console();
	~Console();

	/* Setters */
	void setVisible(bool);
	void setFont(Font*);
	void toggleVisible();

	/* Getters */
	bool isVisible();
	

	void resize(uint,uint);
	void keyDown (WPARAM);
	void log(const string&, const Color& c = Color(255, 255, 255));
	void draw(uint,uint);

private:
	/* Contains rows in console */
	vector<FontString> rows;
	/* Command History */
	vector<string> commandHistory;
	/* Background Color */
	Color background;
	/* Font used */
	Font* font;
	/* Parser used when parsing command line */
	Parser parser;
	/* The input for commands */
	Input input;
	uint opacity;
	/* Keeps track of where we are in the history vector */
	int hPos;
    /* Scroll amount */
    uint scroll;
	/* Used for scaling font size */
	uint baseFontSize;
	/* Max length of a line used for word wrapping */
	uint lineLength;
	/* x and y padding of the console */
	FT_Vector padding;
	bool visible;

	void init();
	void parse(const string&);
};
#endif