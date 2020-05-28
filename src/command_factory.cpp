
#include <command_factory.hpp>
#include <exception>
#include <iostream>

namespace hapnmc { namespace dedcom {

	comalias_map CommandFactory::aliases;

	void CommandFactory::AddAlias(Command::Type t, std::string&& txt, std::string&& short_txt, std::string&& description)
	{
		ComAlias ca;
		ca.txt = txt;
		ca.short_txt = short_txt;
		ca.description = description;
		CommandFactory::aliases[t] = ca;
	}

	void CommandFactory::Init() 
	{
		CommandFactory::AddAlias(Command::Type::Status, "status", "stat",
			"requests current encounter status or the exploration status if there is no encounter");
		CommandFactory::AddAlias(Command::Type::ServerStatus, "server_status", "servstat");

		CommandFactory::AddAlias(Command::Type::ExplorationStatus, "exploration_status", "expstat",
			"requests information about current and adjacent locations");
		CommandFactory::AddAlias(Command::Type::ExploreOption, "explore_option", "explore",
			"requests exploration of one of the adjacent locations");

		CommandFactory::AddAlias(Command::Type::EventInfo, "event_info", "evinfo",
			"requests information about an event which requires an operator decision");
		CommandFactory::AddAlias(Command::Type::EventResolve, "event_resolve", "evres",
			"usage: evres [number] | commands drones to perform the specified preprogrammed action");

		CommandFactory::AddAlias(Command::Type::EncounterStatus, "encounter_status", "encstat",
			"requests compiled overall information about the encounter");

		CommandFactory::AddAlias(Command::Type::PartyStatus, "party_status", "partystat",
			"requests compiled overall information about drones under operator's command");
		CommandFactory::AddAlias(Command::Type::UnitInfo, "unit_info", "unitinfo",
			"usage: unitinfo [identification] | requests detailed information about a drone or one of the targets in an encounter");
		CommandFactory::AddAlias(Command::Type::ProtocolInfo, "protocol_info", "protinfo",
			"usage: protinfo [identification] [protocol_name] | requests detailed information about one of the preprogrammed protocols from a drone");

		CommandFactory::AddAlias(Command::Type::AlterProtocol, "alter_protocol", "protocol",
			"usage: protinfo [identification] [protocol_name] | overrides drone's decisionmaking systems locking it into performing the specified protocol");
		CommandFactory::AddAlias(Command::Type::CommitProtocol, "commit_protocol", "commit",
			"confirms all protocol alterations and allows drones to act in encounter");
	}

	void CommandFactory::FillContext(Command& c, const std::string& params)
	{
		switch(c.GetType()) {
			case Command::Type::Status:
			case Command::Type::ServerStatus:
			case Command::Type::ExplorationStatus:
			case Command::Type::EncounterStatus:
			case Command::Type::PartyStatus:
			case Command::Type::CommitProtocol:
			case Command::Type::EncounterTurnReport:
			case Command::Type::EventInfo:
				{
					CSNoparam cs;
					cs.FillContextFromString(c, params);
				}
				break;
			case Command::Type::ExploreOption:
			case Command::Type::EventResolve:
				{
					CSUInt cs;
					cs.FillContextFromString(c, params);
				}
				break;
			case Command::Type::AlterProtocol:
			case Command::Type::UnitInfo:
			case Command::Type::ProtocolInfo:
				{
					CSText cs;
					cs.FillContextFromString(c, params);
				}
				break;
			default:
				throw std::invalid_argument("Command type unrecognised in CommandFactory::FillContext");
				break;
		}	
	}

	Command CommandFactory::Construct(const std::string& alias, const std::string& params)
	{
		for(auto pair : CommandFactory::aliases) {
			if(pair.second.txt.compare(alias)==0 || pair.second.short_txt.compare(alias)==0) {
				Command c(pair.first);

				try {
					CommandFactory::FillContext(c, params);
				} catch (...) { // failed to create command for whatever reason, maybe bad args or bad name
					throw std::invalid_argument("Failed to fill command context - possibly bad params."); 
				}

				return c;
			}
		}

		throw std::invalid_argument("Failed to construct command - alias not found."); 
	}

	void CommandFactory::Help(const std::string& command_name)
	{
		if(command_name.empty()) {
			std::cout << "List of available commands (type help [command] for more info):" << std::endl;
			for(auto pair : CommandFactory::aliases) {
				std::cout << pair.second.short_txt << " (" << pair.second.txt << ")" << std::endl;
			}
			return;
		}

		for(auto pair : CommandFactory::aliases) {
			if(pair.second.txt.compare(command_name)==0 || pair.second.short_txt.compare(command_name)==0) {
				std::cout << pair.second.description << std::endl;
				return;
			}
		}

		std::cout << "Command " + command_name + " not found." << std::endl;
	}


}} // hapnmc::dedcom