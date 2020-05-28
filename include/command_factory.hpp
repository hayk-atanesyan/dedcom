#ifndef DEDCOM_COMMANDFACTORY_HPP
#define DEDCOM_COMMANDFACTORY_HPP

#include <command.hpp>
#include <command_structures.hpp>
#include <unordered_map>
#include <string>

namespace hapnmc { namespace dedcom {

	struct ComAlias {
		std::string txt;
		std::string short_txt;

		std::string description;
	};

	typedef std::unordered_map<Command::Type, ComAlias> comalias_map;

	/**
	* Responsible for turning user input into a viable command 
	* and constructing commands from other inputs as well.
	*/
	class CommandFactory
	{
	private:
		CommandFactory(){}

		static comalias_map aliases;

		static void FillContext(Command& c, const std::string& params);
	public:
		static void Init();

		static void AddAlias(Command::Type t, std::string&& txt, std::string&& short_txt, std::string&& description = "");

		/** Constructs a viable commans from user input */
		static Command Construct(const std::string& alias, const std::string& params);

		static void Help(const std::string& command_name);
	};

}} // hapnmc::dedcom

#endif // DEDCOM_COMMANDFACTORY_HPP
