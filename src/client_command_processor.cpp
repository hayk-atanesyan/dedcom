#include <client_command_processor.hpp>
#include <exception>
#include <command_structures.hpp>
#include <iostream>

namespace hapnmc { namespace dedcom {

	Command ClientCommandProcessor::Process(Command c)
	{
		Command response;

		switch(c.GetType()) {
			case Command::Type::Text :
				{
					std::cout << c.GetContextBuffer() << std::endl;
				}
				break;
			case Command::Type::UintReport :
				{
					CSUInt cs;
					cs.Read(c);

					if(cs.GetNumber() == 0) std::cout << "Received report: failure." << std::endl;
					else if(cs.GetNumber() == 1) std::cout << "Received report: success." << std::endl;
					else std::cout << "Received report: " << cs.GetNumber() << "." << std::endl;
				}
				break;
			default:
				throw std::runtime_error("Unrecognised command type in ClientCommandProcessor::Process.");
				break;
		}

		return response;
	}

}} // hapnmc::dedcom