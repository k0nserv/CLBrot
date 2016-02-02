#ifndef LOCATION_FILE_H
#define LOCATION_FILE_H
#include <string>
#include <fstream>
#include <sstream>

#include "SettingsDirectory.h"
#include "File.h"
#include "Settings.h"


using std::string;

typedef struct LocationFile {
    char magic[3];//LOC
    double real[2];//Real coordiantes
    double im[2];//Im coordinates
    short int iterations;
    short int escape;
}LocationFile;

class Location {
public:
    static bool loadLocation(string name, LocationFile* _out) {
        std::ifstream in;
        std::stringstream ss;
        SettingsDirectory* st = Settings::getInstance()->getSettingsDirectory();
        LocationFile file;
        bool validMagic = true;

        /* Build the file name and location */
        ss << st->getSavePath() << "\\" << name;

        in.open(ss.str(), std::ios::binary);
        if(in.fail() || _out == NULL)
            return false;

        in.read(reinterpret_cast<char*>(&file), sizeof(file));
        in.close();

        *_out = file;

        validMagic = file.magic[0] == 0x4C && file.magic[1] == 0x4F && file.magic[2] == 0x43;


        return validMagic;
    }

    static bool saveLocation(string name) {
        Settings* settings = Settings::getInstance();
        SettingsDirectory* st = settings->getSettingsDirectory();
        Mandelbrot m = settings->getMandelBrot();
        LocationFile file;
        std::ofstream out;
        std::stringstream ss;
        string fileName;

        file.escape      = (short int)m.escape;
        file.iterations  = (short int)m.iter;

        file.magic[0]   = 0x4C;//L
        file.magic[1]   = 0x4F;//O
        file.magic[2]   = 0x43;//C

        file.real[0]    = (double)m.max.re;
        file.real[1]    = (double)m.min.re;

        file.im[0]      = (double)m.max.im;
        file.im[1]      = (double)m.min.im;

        /* Build the file name and location */
        ss << st->getSavePath() << "\\" << name;
        fileName = ss.str();

        out.open(fileName.c_str(), std::ios::binary | std::ios::out);
        if(out.fail())
            return false;

        out.write(reinterpret_cast<char*>(&file), sizeof(file));

        out.close();

        return true;
    }
};




#endif