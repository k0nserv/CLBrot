#ifndef INPUT_H
#define INPUT_H
#include <string>

using std::string;

class Input {
public:
	
	string getValue();
	void clear();
	void append(char);
	void removeLast();
	bool isEmpty();
	void setValue(const string&);

private:
	string value;
};

#endif
