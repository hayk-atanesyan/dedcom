#ifndef DEDCOM_DEDCOMENGINE_HPP
#define DEDCOM_DEDCOMENGINE_HPP

#include <exploration_engine.hpp>
#include <encounter_engine.hpp>
#include <party.hpp>
#include <action_fixtures.hpp>
#include <random>

namespace hapnmc { namespace dedcom {


	class Rng
	{
	private:
		std::random_device _rd;
		std::mt19937 _gen;
		std::uniform_real_distribution<double> _dis_double;
	public:
		Rng() : _gen(_rd()), _dis_double(0.0, 1.0)
		{

		}

		double GetRndDbl() { return _dis_double(_gen); }
	};
	/** 
	 *	Responsible for game mechanics, entry point for specific modules.
	 *  
	 */
	class DedcomEngine
	{
	public:

        ActionRepo _action_repo;
		Party _party;
		ExplorationEngine _expl_eng;
		EncounterEngine _enc_eng;

		//State _state;
		Rng rng;
		

		/*enum State: uint32_t {
			Startup, 
			Exploration,
			Encounter,

			Finished
		};*/

		static DedcomEngine& Instance()
		{
			static DedcomEngine instance;
			return instance;
		}
        DedcomEngine(DedcomEngine const&) = delete;
        void operator=(DedcomEngine const&) = delete;

        //State GetState() { return _state; }

        //std::string GetStateName();

	//private: // no time


		

		Party& GetParty() { return _party; }

		void initDefaultParty();

		ExplorationEngine& GetExplorationEngine() { return _expl_eng; }
		EncounterEngine& GetEncounterEngine() { return _enc_eng; }

		void Init();
	private:
		

		DedcomEngine() //: _state(State::Startup)
		{
			
		}

		

		

	};

}} // hapnmc::dedcom

#endif // DEDCOM_DEDCOMENGINE_HPP