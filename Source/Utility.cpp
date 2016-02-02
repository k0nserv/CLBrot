#include "Utility.h"

string Utility::trim(const string& in){
	string out = in;
    if(in.length() == 0 )
        return in;

	while(out.at(0) == ' ' || out.at(0) == '\t')
		out = out.substr(1, out.length());

	while( out.at(out.length()-1) == ' ' || out.at(out.length()-1) == '\t')
		out = out.substr(0, out.length()-1);


	return out;
}


string Utility::resolveEnv(const string& var) {
	char* result;
	result = getenv(var.c_str());

	return string(result);
}


vector<string> Utility::split(const string& in, const char& delimiter) {
	vector<string> result = vector<string>();
	string value = trim(in);
	size_t first;
	
	if(value.length() == 0)
		return result;


	first = value.find_first_of(delimiter);

	do {
		string arg = value.substr(0, first);
		result.push_back(arg);

		if (first == string::npos){
			value = "";
			break;
		}

		value = value.substr(first+1, value.length());
		value = trim(value);

		first = value.find_first_of(delimiter);
	} while(first != string::npos);

	if (value.length() != 0)
		result.push_back(value);


	return result;
}