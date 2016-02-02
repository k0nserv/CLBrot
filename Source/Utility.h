#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <fstream>

using std::string;
using std::vector;

class Utility {
public:
	static string trim(const string&);
	static vector<string> split(const string&,const char&);
	static string resolveEnv(const string&);
};
#endif