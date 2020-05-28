#ifndef DEDCOM_CLCOMMANDPROCESSOR_HPP
#define DEDCOM_CLCOMMANDPROCESSOR_HPP

#include <command.hpp>



namespace hapnmc { namespace dedcom {

	/** 
	 *	Responsible for reacting to reports and commands.
	 *  
	 */
	class ClientCommandProcessor
	{
	private:

	public:
		static Command Process(Command c);
	};

}} // hapnmc::dedcom

#endif // DEDCOM_CLCOMMANDPROCESSOR_HPP