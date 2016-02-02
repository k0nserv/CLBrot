#include "Font.h"


Font::Font(const string& font, uint size) {
	fontDirectory = Utility::resolveEnv("WINDIR").append("\\Fonts\\");
	string dirCopy = fontDirectory;

	this->font = dirCopy.append(font);
	this->size = size;
	


	/* Initial color is white */
	color.r = color.b = color.g = 255;

	init();
}


void Font::init() {
	/*
		Disregard all the errors.
		TODO: Maybe add som error checking
	*/
	//Setup library
	FT_Init_FreeType( &library );
	//Load font
	int result = FT_New_Face( library,
                 font.c_str(),
                 0,
                 &face );
	
	/* Missing font or other error
	use fallback */
	if(result != 0) {
		font = fontDirectory.append(FALLBACK_FONT);
		FT_New_Face( library,
			font.c_str(),
			0,
			&face );
	}

	setSize(size);
	buildGlyphCache();
}


void Font::setSize(uint size) {
	this->size = size;
	FT_Set_Pixel_Sizes(face,   /* handle to face object */
					   0,      /* same as height           */
					   size );   /* pixel height          */
}

void Font::setColor(const Color& c) {
	color = c;
}


uint Font::getSize() {
	return size;	
}

Color Font::getColor() {
	return color;	
}

void Font::buildGlyphCache() {
	glEnable(GL_TEXTURE_2D);

	GLuint* textureIDs = new GLuint[94];
	glGenTextures(94, &textureIDs[0]);
	cachedGlyphs = new fontCache[94];

	for(int i = 32; i < 126; i++){	
		FT_GlyphSlot slot = face->glyph;
		FT_Bitmap bitmap;
		uint height, width;
		fontCache current;

		int error = FT_Load_Char(face, static_cast<char>(i), FT_LOAD_RENDER);

		
		glBindTexture(GL_TEXTURE_2D, textureIDs[i-32]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		bitmap = slot->bitmap;
		width  = next_p2(bitmap.width);
		height = next_p2(bitmap.rows);

		// Allocate Memory For The Texture Data.
		GLubyte* expanded_data = new GLubyte[ 2 * width * height];

		// Here We Fill In The Data For The Expanded Bitmap.
		// Notice That We Are Using A Two Channel Bitmap (One For
		// Channel Luminosity And One For Alpha), But We Assign
		// Both Luminosity And Alpha To The		Value That We
		// Find In The FreeType Bitmap.
		// We Use The ?: Operator To Say That Value Which We Use
		// Will Be 0 If We Are In The Padding Zone, And Whatever
		// Is The FreeType Bitmap Otherwise.
		//This code is taken from Nehe's freetype tutorial 
		//http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
		for(uint j=0; j <height;j++) {
			for(uint i=0; i < width; i++) {
				expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] =
					(i>=bitmap.width || j>=bitmap.rows) ?
					0 : bitmap.buffer[i + bitmap.width*j];
			}
		}





		/* Build the texture */
		//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

		current.width = width;
		current.height = height;
		current.advance = slot->advance;
		current.left = slot->bitmap_left;
		current.top  = slot->bitmap_top;
		current.textureID = textureIDs[i-32];
		current.ch = static_cast<char>(i);

		cachedGlyphs[i-32] =  current;

		delete[] expanded_data;
	}
	delete[] textureIDs;

	glDisable(GL_TEXTURE_2D);
}


/* 
	Draw a charAt x and y
*/
FT_Vector Font::drawCharAt(uint x, uint y, char c) {
	FT_GlyphSlot slot = face->glyph;
	FT_Bitmap bitmap;
	uint width, height;
	fontCache current;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	
	
	glEnable (GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Set the blend function

	/* load the cached texture */
	current = cachedGlyphs[static_cast<int>(c)-32];
	x += current.left;
	y -= current.top;
	width = current.width;
	height = current.height;

	glBindTexture(GL_TEXTURE_2D, current.textureID);
	glColor3ub(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);	glVertex2i(x, y + height); 
	glTexCoord2f(0.0, 0.0);	glVertex2i(x, y); 
	glTexCoord2f(1.0, 0.0);	glVertex2f(x + width ,y);
	glTexCoord2f(1.0, 1.0);	glVertex2f(x+ width, y + height);
	glEnd();

	glDisable (GL_TEXTURE_2D);

	FT_Vector advance = slot->advance;

	return current.advance;
}


void Font::drawString(uint x, uint y, uint length, const  string& s) {
	uint currentX = x;
	uint currentY = y;

	for ( int i = 0; i < s.length(); i++ ){
		if ( currentX > length )
			;//TODO fix new lines

		FT_Vector advance = drawCharAt(currentX, currentY, s.at(i));
		currentX += advance.x >> 6;
		}
	}


void Font::drawFontString(uint x, uint y, uint length, const FontString& fs) {
	Color oldColor = getColor();
	
	setColor(fs.color);
	drawString(x, y, length, fs.value);
	setColor(oldColor);
}





