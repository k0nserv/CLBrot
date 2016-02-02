#include "Input.h"

string Input::getValue() {
	return value;
}

void Input::setValue(const string& in) {
	value = in;
}

void Input::clear() {
	value = "";
}

void Input::append(char c) {
	value.push_back(c);
}


void Input::removeLast() {
	if(value.length() == 0)
		return;

	value.pop_back();
}

bool Input::isEmpty() {
	return value.empty();	
}