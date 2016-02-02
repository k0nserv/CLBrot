#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "Definitions.h"
#include "Settings.h"
#include "LocationFile.h"



#define COMMAND_HELP "cl_help"
#define COMMAND_RESET "cl_reset"
#define COMMAND_SET_ITERATION "cl_set_iterations"
#define COMMAND_SET_ESCAPE_RADIUS "cl_set_escape_radius"
#define COMMAND_SAVE_POSITIONF "cl_save_positionf"
#define COMMAND_SAVE_POSITION "cl_save_position"
#define COMMAND_LOAD_POSITION "cl_load_position"
#define COMMAND_LOAD_POSITIONF "cl_load_positionf"
#define COMMAND_GET_POSITION "cl_get_position"



using std::string;
using std::vector;

struct CommandResult {
	string out;
	bool error;
};

/* Base command class */
class Command {
public:
	virtual CommandResult run(const vector<string>&) = 0;
    virtual string help() {
        return "Unsupported";
    }
};
/* Reset command class */
class ResetCommand : public Command {
public:

	CommandResult run(const vector<string>& args) {
		Settings* settings = Settings::getInstance();
		Mandelbrot m = settings->getMandelBrot();
		CommandResult result;
		result.error = false;
		result.out   = "";
		
		m.max.re = DEFAULT_MAX_RE;
		m.max.im = DEFAULT_MAX_IM;
		m.min.re = DEFAULT_MIN_RE;
		m.min.im = DEFAULT_MIN_IM;

		settings->setMandelbrot(m);
		
		return result;
	}

    string help() {
        return "Resets the position to the default";
    }



};


/* Change the iteration count */
class SetIterationCommand : public Command {
public:

	CommandResult run(const vector<string>& args) {
		Settings* settings = Settings::getInstance();
		Mandelbrot m = settings->getMandelBrot();
		CommandResult result;
		int iterations;
		result.error = false;
		result.out   = "";

		/* Missing argument to command */
		if(args.size() == 0){
			std::stringstream ss;
			result.error = true;
			ss << "Invalid usage. Should be " << COMMAND_SET_ITERATION << " [number].";
			result.out = ss.str();
			
			return result;
		}

		std::istringstream ss(args.at(0));
		ss >> iterations;

		if ( ss.fail() ){
			result.error = true;
			result.out   = "Argument should be a positive number";

			return result;
		}

		if(iterations < 0) {
			result.error = true;
			result.out   = "Argument cannot be negative";

			return result;
		}



		m.iter = iterations;
		settings->setMandelbrot(m);

		return result;
	}

    string help() {
        return "[number] set the number of iterations";
    }
};

/* Get the current position in the mandelbrot */
class GetPositionCommand : public Command {
public:

	CommandResult run(const vector<string>& args) {
		Settings* settings = Settings::getInstance();
		Mandelbrot m = settings->getMandelBrot();
		CommandResult result;
		result.error = false;
		result.out   = "";
		std::stringstream ss;
		
		ss << "Max (" << m.max.re << ", " << m.max.im << ") | Min(" << m.min.re << ", " << m.min.im << ")." << m.xScale;
		result.out = ss.str();

		return result;
		}

    string help() {
        return "The current max and min coordinates";
    }
};

class SetEscapeRadiusCommand : public Command {
public:
	CommandResult run(const vector<string>& args) {
		Settings* settings = Settings::getInstance();
		Mandelbrot m = settings->getMandelBrot();
		CommandResult result;
		int escaperadii;
		result.error = false;
		result.out   = "";

		/* Missing argument to command */
		if(args.size() == 0){
			std::stringstream ss;
			result.error = true;
			ss << "Invalid usage. Should be " << COMMAND_SET_ESCAPE_RADIUS << " [number].";
			result.out = ss.str();
			
			return result;
		}

		std::istringstream ss(args.at(0));
		ss >> escaperadii;

		if ( ss.fail() ){
			result.error = true;
			result.out   = "Argument should be a positive number";

			return result;
		}

		if(escaperadii < 0) {
			result.error = true;
			result.out   = "Argument cannot be negative";

			return result;
		}



		m.escape = escaperadii;
		settings->setMandelbrot(m);

		return result;
	}

    string help() {
        return "[number] Set the escape radius";
    }
};

class SavePositionFCommand : public Command {
public:
    CommandResult run(const vector<string>& args) {
        Settings* settings = Settings::getInstance();
        CommandResult result;
        result.error = false;
        result.out   = "Saved successfully";

        if(args.size() == 0) {
            result.error = true;
            result.out   = "Invalid usage, missing argument";

            return result;
        }

        if (!Location::saveLocation(args.at(0))) {
            result.error = true;
            result.out   = "Unknow file error";
        }

        return result;
    }

    string help() {
        return "[file name] save the current location to file, stored in the save folder";
    }
};

class SavePositionCommand : public Command {
public:
	CommandResult run(const vector<string>& args) {
		Settings* settings = Settings::getInstance();
		Mandelbrot m = settings->getMandelBrot();
		std::stringstream ss;
		CommandResult result;
		uint index;
		result.error = false;
		result.out   = "";

		index = settings->saveCurrentPosition();
		ss << "Position saved at index: " << index;
		result.out = ss.str();

		return result;
	}

    string help() {
        std::stringstream ss;
        ss << "Save the current position, saved positions get a unique index that can be used with " << COMMAND_LOAD_POSITION;
        return ss.str();
    }
};

class LoadPositionCommand : public Command {
public:
	CommandResult run(const vector<string>& args) {
		Settings* settings = Settings::getInstance();
		Mandelbrot m = settings->getMandelBrot();
		CommandResult result;
		int index;
		result.error = true;
		result.out   = "Cannot load: index invalid";

		/* Missing argument to command */
		if(args.size() == 0){
			std::stringstream ss;
			ss << "Invalid usage. Should be " << COMMAND_LOAD_POSITION << " [index].";
			result.out = ss.str();
			
			return result;
		}

		std::istringstream ss(args.at(0));
		ss >> index;

		if ( ss.fail() ){
			result.out   = "Argument should be a positive number";

			return result;
		}

		if(index < 0) {
			result.out   = "Argument cannot be negative";

			return result;
		}

		bool good = settings->loadPosition(index);
		if (good) {
			result.error = false;
			result.out = "Loading position";
		}


		return result;
	}

    string help() {
        std::stringstream ss;
        ss << "[index] Load a previously save position from " << COMMAND_SAVE_POSITION;
        return ss.str();
    }
};


class LoadPositionFCommand : public Command {
public:
    CommandResult run(const vector<string>& args) {
        Settings* settings = Settings::getInstance();
        Mandelbrot m = settings->getMandelBrot();
        SettingsDirectory* st = settings->getSettingsDirectory();
        CommandResult result;
        LocationFile file;
        std::stringstream ss;

        result.error = false;
        result.out   = "Loaded file";

        if(args.size() == 0) {
            result.error = true;
            result.out   = "Invalid usage, missing argument";

            return result;
        }

        if(!Location::loadLocation(args.at(0), &file)) {
            result.error = true;
            result.out   = "Bad input file";

            return result;
        }

        m.max.re = file.real[0];
        m.min.re = file.real[1];
        m.max.im = file.im[0];
        m.min.im = file.im[1];
        m.escape = file.escape;
        m.iter   = file.iterations;

        ss << "Loaded file " << args.at(0) << "\nmax: (" << m.max.re << ", " << m.max.im << ")\nmin ("
                                           << m.min.re << ", " << m.min.im << ")\n iterations: " << m.iter << "\nescape: " << m.escape;

        result.out = ss.str();

        settings->setMandelbrot(m);

        return result;
    }


};


#endif