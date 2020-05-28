#include <command_processor.hpp>
#include <exception>
#include <command_structures.hpp>
#include <dedcom_engine.hpp>
#include <iostream>
#include <exception>
#include <string>

namespace hapnmc { namespace dedcom {

	std::string CommandProcessor::explorationStatusTxt()
	{
		ExplorationEngine& expl_eng = DedcomEngine::Instance().GetExplorationEngine();
		ExplorationOption& curr_option = expl_eng.GetCurrentOption();
		auto expl_options = expl_eng.GetOptions();

		if(expl_options.size() < 3) {
			std::runtime_error("Exploration options are not generated.");
		}

		return 
			curr_option.name + " | Depth: " + std::to_string(expl_eng.GetDepth())
			+ ", Threats: " + (curr_option.HasEncounter() ? "Detected" : "None")
			+ (curr_option.pchoice ? ", An event must be resolved" : "")
			+ "\r\nExploration options: "
			+ "1. " + expl_options[0].name
			+ " , 2. " + expl_options[1].name
			+ " , 3. " + expl_options[2].name
		;
	}

	std::string CommandProcessor::eventInfoTxt()
	{
		ExplorationEngine& expl_eng = DedcomEngine::Instance().GetExplorationEngine();
		ExplorationOption& curr_option = expl_eng.GetCurrentOption();

		if(!curr_option.pchoice) return "No event active at location.";

		const std::vector<ChoiceOption>& ch_options = curr_option.pchoice.GetOptions();

		std::string s =
			curr_option.pchoice.GetTxt()
			+ "\r\n"
		;

		s.append("<< ");
		int i=1;
		for(ChoiceOption co : ch_options)
		{
			s.append(std::to_string(i++));
			s.append(". ");
			s.append(co.GetTxt());
			s.append(" ");
		}
		s.append(" >>");

		return s;
	}

	std::string CommandProcessor::encounterStatusTxt()
	{
		std::string s;

		s += "Dronecount: " + std::to_string( DedcomEngine::Instance()._party.GetActorsList().size() ) + ", "
		+ "Targetcount: " + std::to_string( DedcomEngine::Instance()._enc_eng.GetParty().GetActorsList().size() ) + "\r\n"
		;

		for(Actor& actr : DedcomEngine::Instance()._party.GetActorsList()) {
			s += actr.GetName() + ": " + actr.GetShortReport() + "; ";
		}

		s += "\r\n -=- vs -=- \r\n";

		if(DedcomEngine::Instance()._enc_eng.GetParty().GetActorsList().size() < 5) {
			for(Actor& actr : DedcomEngine::Instance()._enc_eng.GetParty().GetActorsList()) {
				s += actr.GetName() + ": " + actr.GetShortReport() + "; ";
			}
		} else {
			s += DedcomEngine::Instance()._enc_eng.GetParty().HeadcountTxt();
		}

		return s;
	}

	std::string CommandProcessor::partyInfo()
	{
		std::string s;

		for(Actor& actr : DedcomEngine::Instance()._party.GetActorsList()) {
			s += actr.GetName() + ": " + actr.GetMediumReport() + "\r\n";
		}

		return s;
	}

	std::string CommandProcessor::unitInfo(std::string& name)
	{
		std::string s;

		if(DedcomEngine::Instance()._party.HasActor(name)) { // player party, drones give full info
			s += DedcomEngine::Instance()._party.GetActor(name).GetReport();
		} else if(DedcomEngine::Instance()._enc_eng.GetParty().HasActor(name)) { // foe party, partial info
			s += DedcomEngine::Instance()._party.GetActor(name).GetMediumReport();
		} else {
			s = "Unit " + name + " not found.";
		}

		return s;
	}

	std::string CommandProcessor::protocolInfo(std::string& actor_name, std::string& action_name)
	{
		if(!DedcomEngine::Instance()._party.HasActor(actor_name)) return "Drone not found.";

		Actor& actr = DedcomEngine::Instance()._party.GetActor(actor_name);

		if(!actr.HasAction(action_name)) return actor_name + " did not recognise protocol " + action_name;

		return actr.GetAction(action_name).GetDescriptionAndRequirements();
	}

	// ================= PROCESS ===================

	Command CommandProcessor::Process(Command c)
	{
		Command response;

		switch(c.GetType()) {
			case Command::Type::Text :
				{
					std::cout << c.GetContextBuffer() << std::endl;
				}
				break;

			case Command::Type::Status :
				{
					//response.SetType(Command::Type::Text);
					//CSText cst;
					//cst.FillContextFromString(response, "Server is running, dedcom is online");
					if(DedcomEngine::Instance()._enc_eng.HasEncounter()) {
						c.SetType(Command::Type::EncounterStatus);
						response = Process(c);
					} else {
						c.SetType(Command::Type::ExplorationStatus);
						response = Process(c);
					}
				}
				break;

			case Command::Type::ServerStatus :
				{
					response.SetType(Command::Type::Text);
					CSText cst;
					cst.FillContextFromString(response, "Server is running.");
					// TODO add session count
				}
				break;
			case Command::Type::EventInfo :
				{
					response.SetType(Command::Type::Text);
					CSText cst;
					cst.FillContextFromString(response, CommandProcessor::eventInfoTxt());
				}
				break;
			case Command::Type::EventResolve :
				{
					CSUInt csui;
					csui.Read(c);
					int option_index = (int)csui.GetNumber() - 1;

					ExplorationOption& curr_option = 
						DedcomEngine::Instance().GetExplorationEngine().GetCurrentOption();

					response.SetType(Command::Type::Text);
					CSText cst;

					if(!curr_option.pchoice) {
						cst.FillContextFromString(response, "No Event to resolve.");
					} else if(!curr_option.pchoice.HasOption(option_index)) {
						cst.FillContextFromString(response, "Event has no such option.");
					} else {
						curr_option.pchoice.Resolve(option_index);
						curr_option.pchoice.Clear();
						cst.FillContextFromString(response, "Event has been resolved.");
					}
				}
				break;
			case Command::Type::ExplorationStatus :
				{
					response.SetType(Command::Type::Text);
					CSText cst;

					cst.FillContextFromString(response, CommandProcessor::explorationStatusTxt());
				}
				break;
			case Command::Type::ExploreOption :
				{
					CSUInt csui;
					csui.Read(c);
					int option_index = (int)csui.GetNumber() - 1; 

					response.SetType(Command::Type::Text);
					CSText cst;

					ExplorationEngine& expl_eng = DedcomEngine::Instance().GetExplorationEngine();
					ExplorationOption& curr_option = expl_eng.GetCurrentOption();

					if(expl_eng.HasOption(option_index)) {

						if(curr_option.HasEncounter()) {
							cst.FillContextFromString(response, "Can not leave the encounter.");
						} else if(curr_option.pchoice) {
							cst.FillContextFromString(response, "An event must be resolved.");
						} else {
							DedcomEngine::Instance().GetExplorationEngine().ExploreOption(option_index);
							cst.FillContextFromString(response, CommandProcessor::explorationStatusTxt());
						}

					} else {
						cst.FillContextFromString(response, "No option with such index available.");
					}

				}
				break;
			case Command::Type::EncounterStatus :
				{
					response.SetType(Command::Type::Text);
					CSText cst;

					if(DedcomEngine::Instance()._enc_eng.HasEncounter()) {
						cst.FillContextFromString(response, CommandProcessor::encounterStatusTxt());
					} else {
						cst.FillContextFromString(response, "No encounter active.");
					}
				}
				break;
			case Command::Type::PartyStatus :
				{
					response.SetType(Command::Type::Text);
					CSText cst;

					cst.FillContextFromString(response, CommandProcessor::partyInfo());
				}
				break;
			case Command::Type::UnitInfo :
				{
					response.SetType(Command::Type::Text);
					CSText cst;

					CSText cstCommand;
					cstCommand.Read(c);

					std::string unit_name = cstCommand.GetTxt();
					cst.FillContextFromString(response, CommandProcessor::unitInfo(unit_name));
				}
				break;
			case Command::Type::ProtocolInfo :
				{
					response.SetType(Command::Type::Text);
					CSText cst;

					CSText cstCommand;
					cstCommand.Read(c);

					std::string line = cstCommand.GetTxt();
					std::string drone_name;
					std::string action_name;
					size_t delimpos = line.find(' ');

					if(delimpos == std::string::npos) {
						cst.FillContextFromString(response, "Command requires drone identification and action identification separated by a space.");
					} else {
						drone_name = line.substr(0, delimpos);
						action_name = line.substr(delimpos+1);

						if(!DedcomEngine::Instance()._party.HasActor(drone_name)) {
							cst.FillContextFromString(response, "Drone with matching designation not found");
						} else if(!DedcomEngine::Instance()._party.GetActor(drone_name).HasAction(action_name)) {
							cst.FillContextFromString(response, drone_name + " has no access to protocol " + action_name);
						} else {
							cst.FillContextFromString(response, CommandProcessor::protocolInfo(drone_name, action_name));
						}
					}
				}
				break;
			case Command::Type::AlterProtocol :
				{
					response.SetType(Command::Type::Text);
					CSText cst;

					CSText cstCommand;
					cstCommand.Read(c);

					std::string line = cstCommand.GetTxt();
					std::string drone_name;
					std::string action_name;
					size_t delimpos = line.find(' ');

					if(delimpos == std::string::npos) {
						cst.FillContextFromString(response, "Command requires drone identification and action identification separated by a space.");
					} else {
						drone_name = line.substr(0, delimpos);
						action_name = line.substr(delimpos+1);

						if(!DedcomEngine::Instance()._party.HasActor(drone_name)) {
							cst.FillContextFromString(response, "Drone with matching designation not found");
						} else if(!DedcomEngine::Instance()._party.GetActor(drone_name).CanAct(action_name)) {
							cst.FillContextFromString(response, drone_name + " is unable to perform " + action_name);
						} else {
							DedcomEngine::Instance()._enc_eng.QueuePlayerAction(drone_name, action_name);

							c.SetType(Command::Type::EncounterStatus);
							response = Process(c);
						}
					}
				}
				break;
			case Command::Type::CommitProtocol :
				{
					response.SetType(Command::Type::Text);
					CSText cst;

					if(!DedcomEngine::Instance()._enc_eng.HasEncounter()) {
						cst.FillContextFromString(response, "Not in an encounter, command ignored.");
					} else {
						DedcomEngine::Instance()._enc_eng.ResolveTurn();

						if(!DedcomEngine::Instance()._enc_eng.HasEncounter()) {
							cst.FillContextFromString(response, "Encounter is resolved.");
						} else {
							c.SetType(Command::Type::EncounterStatus);
							response = Process(c);
						}
					}
				}
				break;
			default:
				throw std::runtime_error("Unrecognised command type in CommandProcessor::Process.");
				break;
		}

		return response;
	}

}} // hapnmc::dedcom