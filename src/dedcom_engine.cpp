#include <dedcom_engine.hpp>
#include <exception>
#include <iostream>

namespace hapnmc { namespace dedcom {

	void DedcomEngine::initDefaultParty()
	{
		Actor a_shockwave("shockwave", "Carries a large powerbank and a reliable generator which allows it to utilize a shield projector",
			10, 0, 0, 5, 2,
			6, 2, 10, 20);
		a_shockwave.AddAction("Electrocute");
		a_shockwave.AddAction("Discharge");
		a_shockwave.AddTag("mech");
		a_shockwave.AddTag("elctr");
		_party.AddActor(a_shockwave);

		Actor a_boar("boar", "A heavy, armored drone capable of engaging most targets",
			30, 20, 20, 0, 0,
			1, 0, 8, 30);
		a_boar.AddAction("Slugshot");
		a_boar.AddAction("Assault");
		a_boar.AddTag("mech");
		_party.AddActor(a_boar);

		Actor a_vector("vector", "Has a powerful main gun and advanced targeting equipment, but no armor or shields",
			20, 0, 10, 0, 0,
			1, 1, 10, 20);
		a_vector.AddAction("Slugshot");
		a_vector.AddAction("Plasmashot");
		a_vector.AddTag("mech");
		_party.AddActor(a_vector);
	}

	void DedcomEngine::Init()
	{
		std::cout << "Initiating action repo." << std::endl;
		_action_repo.Init();

		std::cout << "Initiating encounter engine and foe actor repo." << std::endl;
		_enc_eng.Init();

		std::cout << "Initiating default party." << std::endl;
		initDefaultParty();
	}

	/*std::string DedcomEngine::GetStateName()
	{
		switch(_state)
		{
			case State::Startup:
				return "Startup";
				break;
			case State::Exploration:
				return "Exploration";
				break;
			case State::Encounter:
				return "Encounter";
				break;
			case State::Finished:
				return "Finished";
				break;
			default:
				throw std::runtime_error("Unrecognised state in DedcomEngine::GetStateName().");
				break;
		}
	}*/

}} // hapnmc::dedcom