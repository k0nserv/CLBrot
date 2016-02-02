#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Command.h"
#include "Utility.h"

using std::string;
using std::map;
using std::vector;


typedef std::pair<std::string, Command*> strCmdPair;


class Parser {
public:
	static CommandResult parse(const string&);
    inline static map<string, Command*> getCommands() {
        return commands;
    }

private:
	static map<string, Command*> commands;
};


class HelpCommand : public Command {
	CommandResult run(const vector<string>& args) {
		CommandResult result;
        std::stringstream ss;
        std::map<string, Command*> commands = Parser::getCommands();
        std::map<string, Command*>::iterator it;

        
        for(it=commands.begin(); it != commands.end(); it++)
            ss << it->first << "  " << it->second->help() << "\n";
        

        result.error = true;
        result.out   = ss.str();

		return result;
	}

    string help() {
        return "Print the help menu";
    }
};

#endif