#ifndef DEDCOM_COMMANDPROCESSOR_HPP
#define DEDCOM_COMMANDPROCESSOR_HPP

#include <command.hpp>
#include <string>



namespace hapnmc { namespace dedcom {

	/** 
	 *	Responsible for reacting to commands.
	 *  
	 */
	class CommandProcessor
	{
	private:
		static std::string explorationStatusTxt();
		static std::string eventInfoTxt();
		static std::string encounterStatusTxt();
		static std::string partyInfo();
		static std::string unitInfo(std::string& name);
		static std::string protocolInfo(std::string& actor_name, std::string& action_name);
	public:
		static Command Process(Command c);
	};

}} // hapnmc::dedcom

#endif // DEDCOM_COMMANDPROCESSOR_HPP