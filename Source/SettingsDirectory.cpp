#include "SettingsDirectory.h"

SettingsDirectory::SettingsDirectory(HWND hwnd) {
	TCHAR result[MAX_PATH];
	std::stringstream ss;

	SHGetFolderPath(hwnd, CSIDL_APPDATA, NULL, 0, result );
	ss << result << "\\" << SETTINGS_DIR; 
	root = ss.str();
	
	init();
}

SettingsDirectory::~SettingsDirectory(){
}


void SettingsDirectory::init(){
	createDirectory(getRootPath());
	createDirectory(getScreenshotPath());
    createDirectory(getSavePath());
	screenshotIndex = findScreenShotIndex(getScreenshotPath());
}

string SettingsDirectory::getRootPath() {
	return root;
}

string SettingsDirectory::getScreenshotPath(){
	std::stringstream ss;

	ss << getRootPath() << "\\screenshots";
	return ss.str();
}

string SettingsDirectory::getSavePath() {
	std::stringstream ss;

	ss << getRootPath() << "\\save";
	return ss.str();
}

uint SettingsDirectory::getScreenshotIndex (){
	return screenshotIndex;
}

void SettingsDirectory::incrementScreenshotIndex(){
	screenshotIndex++;
}


/* Static functions */
/*
Check if a given directory already exists, source from
http://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
*/
 bool SettingsDirectory::directoryExists(const string& in) {
	DWORD ftyp = GetFileAttributesA(in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

 bool SettingsDirectory::createDirectory(const string& path){
	 if (directoryExists(path))
		 return true;//Directory already exists, sit back and relax
	 
	 return CreateDirectory(path.c_str(), NULL);
}

 uint SettingsDirectory::findScreenShotIndex(const string& path){
	 //default result to 0
	 uint result = 0;
	 WIN32_FIND_DATA fileData;
	 HANDLE hFind = INVALID_HANDLE_VALUE;
	 string screenshotPrefix = getScreenshotPrefix();
	 string pathCopy = path;

	pathCopy.append("\\*");
	hFind = FindFirstFile(pathCopy.c_str(),&fileData);

	 if (hFind == INVALID_HANDLE_VALUE)
		 return 0;//There are no screenshots at all

	 do {
		 if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			 string name = fileData.cFileName;
			 if (name.substr(0,screenshotPrefix.length()) == screenshotPrefix )
				 result++;

		}
	}
	 while (FindNextFile(hFind, &fileData) != 0);

	 return result;
}

 string SettingsDirectory::getScreenshotPrefix(){
	return "screenshot";	
}

