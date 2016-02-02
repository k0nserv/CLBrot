#include "Parser.h"

map<string, Command*> buildMap() {
	map<string, Command*> map;
	map[COMMAND_HELP]  = dynamic_cast<Command*>(new HelpCommand());
	map[COMMAND_RESET] = dynamic_cast<Command*>(new ResetCommand());
	map[COMMAND_SET_ITERATION] = dynamic_cast<Command*>(new SetIterationCommand());
	map[COMMAND_SET_ESCAPE_RADIUS] = dynamic_cast<Command*>(new SetEscapeRadiusCommand());
	map[COMMAND_GET_POSITION]  = dynamic_cast<Command*>(new GetPositionCommand());
    map[COMMAND_SAVE_POSITIONF] = dynamic_cast<Command*>(new SavePositionFCommand());
	map[COMMAND_SAVE_POSITION] = dynamic_cast<Command*>(new SavePositionCommand());
	map[COMMAND_LOAD_POSITION] = dynamic_cast<Command*>(new LoadPositionCommand());
    map[COMMAND_LOAD_POSITIONF] = dynamic_cast<Command*>(new LoadPositionFCommand());

	return map;
}

map<string, Command*> Parser::commands = buildMap();

CommandResult Parser::parse(const string& in){
	CommandResult result;
	map<string, Command*>::iterator it;
	string command;
	vector<string> args = vector<string>();
	size_t firstSpace;

	result.error = true;
	result.out   = "Invalid input, must be a valid command";

	if ( in.empty() )
		return result;

	command = Utility::trim(in);
	
	firstSpace = command.find_first_of(" \t");

	command = command.substr(0, firstSpace);
	
	it = commands.find(command);
	
	if (it == commands.end() ){
		result.out = "Not a recognized command, try ";
        result.out.append(COMMAND_HELP);
		return result;
	}

	/* Find arguments passed to command */
	if (firstSpace != string::npos ) {
		string arguments = in.substr(firstSpace, string::npos);

		if ( arguments.length() == 0)
			return it->second->run(args);
		args = Utility::split(arguments, ' ');
	}


	return it->second->run(args);
}



