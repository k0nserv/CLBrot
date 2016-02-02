/*
The class contained in this documented provies a wrapper for the Freetype library.
Portions of this software are copyright © 2012 The FreeType
Project (www.freetype.org).  All rights reserved.
*/
#ifndef FONT_H
#define FONT_H
#include <string>
#include <Windows.h>
#include <vector>

#include <gl/glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Definitions.h"
#include "Utility.h"

#define FALLBACK_FONT "arial.ttf"

using std::string;


typedef struct fontCache{
	GLuint textureID;
	uint width;
	uint height;
	uint left;
	uint top;
	FT_Vector advance;
	char ch;
}fontCache;

struct FontString {
	string value;
	Color color;
	FontString( string v, Color c = Color(255, 255, 255) ) : value(v), color(c) {}
	FontString() : value("") {}
};





class Font {
public:
	Font(const string&,uint);

	/* Setters */
	void setSize(uint);
	void setColor(const Color&);

	/* Getters */
	uint getSize();
	Color getColor();


	FT_Vector drawCharAt(uint, uint, char);
	void drawString(uint, uint, uint, const string&);
	void drawFontString(uint, uint, uint, const FontString&);

private:
	FT_Library library;
	FT_Face face;

	fontCache* cachedGlyphs;

	string font;
	string fontDirectory;
	Color color;
	uint size;

	void init();
	void buildGlyphCache();

	/* 
		Gets the first power of 2 counting from a,
		take from http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/ 
	*/
	inline int next_p2 (int a ) {
		int rval=1;
		// rval<<=1 Is A Prettier Way Of Writing rval*=2;
		while(rval<a) rval<<=1;
		return rval;
		}
};


#endif