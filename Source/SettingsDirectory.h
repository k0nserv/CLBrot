#ifndef SETTINGS_DIRECTORY_H
#define SETTINGS_DIRECTORY_H

#include <iostream>
#include <sstream>
#include <Shlobj.h>

#include "definitions.h"

#define SETTINGS_DIR "ClBrot"


using std::string;


class SettingsDirectory {
public:
	/* Constructor */
	SettingsDirectory(HWND);
	~SettingsDirectory();

	/* Getters */
	string getRootPath();
	string getScreenshotPath();
	string getSavePath();
	uint getScreenshotIndex();

	/* Utility */
	void incrementScreenshotIndex();

	/* static */
	/* Get the screenshot file prefix e.g "screenshot" */
	static string getScreenshotPrefix();

private:
	/*
		Contains the highest number appended after screenshot names
		Example: screenshot-X.png where x is the number
	*/
	uint screenshotIndex;
	string root;
	SettingsDirectory* instance;

	void init();
	

	/* Static functions */
	static bool directoryExists(const string&);
	/* Attempts to create a directory if it doesn't already exsit */
	static bool createDirectory(const string&);
	/* Find the screenshot index */
	static uint findScreenShotIndex(const string&);
};
#endif