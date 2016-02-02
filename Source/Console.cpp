#include "Console.h"



Console::Console() {
	font = NULL;
	input = Input();
	parser = Parser();

	padding.x = 20;
	padding.y = 40;
	baseFontSize = 10;
	rows = vector<FontString>();
	commandHistory = vector<string>();
	visible = false;
	hPos    = 0;
    scroll  = 0;

	/* Background color and opacity */
	background.r = 0;
	background.g = 0;
	background.b = 0;
	opacity = 100;

	init();
}

Console::~Console() {
	delete font;
	//TODO: Destruct stuff
}

/* Public */

/* Setters */

void Console::setVisible(bool b) {
	visible = b;
}

void Console::setFont(Font* f) {
	font = f;	
}

void Console::toggleVisible() {
	visible = !visible;
}

/* Getters */
bool Console::isVisible(){
	return visible;	
}



/* Add a row to the log */
void Console::log(const string& in, const Color& c) {
	FontString fs = FontString(in, c);

	std::vector<string> vs = Utility::split(fs.value, '\n');
	for (int i = 0; i < vs.size(); i++ ) {
		fs.value = vs.at(i);
		rows.push_back(fs);
        scroll++;
	}
}

void Console::draw(uint width, uint height) {
	/* Don't do anything if the console isn't visible or 
	   the font hasen't been created yet */
	if ( !visible || font == NULL) 
		return;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
	glColor4ub(static_cast<GLubyte>(background.r), 
			   static_cast<GLubyte>(background.g), 
			   static_cast<GLubyte>(background.b), 
			   opacity);

	glBegin(GL_QUADS);
	glVertex2i(0,0);
	glVertex2i(0,height/5);
	glVertex2i(width,height/5);
	glVertex2i(width,0);
	glEnd();

	/* Draw the input field */
	std::stringstream ss;
	ss << ">> " << input.getValue();
	font->drawString(padding.x, height/5 - 10, lineLength, ss.str());


	/* Draw previous logs */
	int y = height/5 -  padding.y;
	for( int i = scroll-1; i >= 0; i-- ){
		if (y <= 0)
			break;

		font->drawFontString(padding.x, y, lineLength, rows.at(i));
		y -= static_cast<int>(font->getSize()*1.5);
	}
}

/**
Tunnel input to the input class
*/
void Console::keyDown (WPARAM wParam){
	if(wParam == VK_BACK)
		input.removeLast();
	else if(wParam == VK_RETURN && !input.isEmpty()){
		log(input.getValue());
		parse(input.getValue());
		commandHistory.push_back(input.getValue());
		hPos = commandHistory.size();
		input.clear();
	}
	else if(wParam == VK_UP || wParam == VK_DOWN ) {
		if(commandHistory.empty())
			return;

		hPos = wParam == VK_UP ? hPos-1 : hPos+1;
		if (hPos < 0 ) hPos = 0;
		else if (hPos >= commandHistory.size()) hPos = commandHistory.size() - 1;

		input.setValue(commandHistory.at(hPos));
	}

    else if( wParam == VK_PRIOR) {
        scroll--;
        if(scroll <= 0 ) scroll = 0;
    }
    else if( wParam == VK_NEXT) {
        scroll++;
        if(scroll >= rows.size()) scroll = rows.size();
    }
	else
		input.append(wParam);

}


void Console::resize(uint width, uint height) {
	lineLength = width - 2*padding.x;
	uint fontSize = static_cast<uint>(height/70);
	if ( font != NULL )
		font->setSize(fontSize);


	draw(width, height);
}




/* Private */
void Console::init() {
	Color c;
	c.r = 255;
	c.g = c.b = 0;
	//font->setColor(c);

}


void Console::parse(const string& input) {
	CommandResult result = Parser::parse(input);
	Color color = result.error ? Color(180, 0, 0) : Color(255, 255, 255);
	string value = result.out;

	if(result.out.length() > 0 )
		log(value, color);
}