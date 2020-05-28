#ifndef DEDCOM_EXPLORATIONOPTIONS_HPP
#define DEDCOM_EXPLORATIONOPTIONS_HPP

#include <list>
#include <vector>
#include <string>
#include <functional>
#include <playerchoice.hpp>
#include <unordered_map>
#include <random>

namespace hapnmc { namespace dedcom {

	// ---------------- exploration option events list -------------------------

	/** Takes the provided amount for the event effect and scales it using current area's eventEffectMultiplier */
	unsigned int GetModifiedEffectAmount(unsigned int amnt);

	void EOE_DamageParty(unsigned int amnt);
	void EOE_DrainPowerFromParty(unsigned int amnt);

	/**
	 * Stores basic info about one of possible choices for further exploration.
	 *
	 */
	class ExplorationOption
	{
	public:
		std::string name;

		uint threatLevel{0}; // used for generating encounters

		Playerchoice pchoice; // after clearing encounter (if any) this is presented to the user

		float eventEffectMultiplier{1.0f}; // used by some events for variety

		// these are triggered in encounter and can affect all actors
		std::function<void()> OnEnter;

		std::function<void()> OnNewTurnEvent;
		std::function<void()> OnTurnEndEvent;

		std::function<void()> OnClear; // trigger after entering area and after any encounters are finalized


		/** Whether the encounter engine should generate an encounter here, should be based on encounter point costs */
		bool HasEncounter() { return threatLevel > 0; } // TODO

		ExplorationOption& operator=(const ExplorationOption& opt)
		{
			name = opt.name;
			threatLevel = opt.threatLevel;

			pchoice.Clear();
			pchoice = opt.pchoice;

			eventEffectMultiplier = opt.eventEffectMultiplier;

			OnEnter = opt.OnEnter;
			OnNewTurnEvent = opt.OnNewTurnEvent;
			OnTurnEndEvent = opt.OnTurnEndEvent;
			OnClear = opt.OnClear;

			return *this;
		}

	};

	/**
	* Responsible for generating options, stores a predefined list of possibilities.
	*
	*/
	class ExplorationOptionFactory
	{
	private:
 
		// rands generic
		std::random_device _rd;
		std::mt19937 _gen;
    	std::uniform_int_distribution<> _dis_int;
    	std::uniform_real_distribution<double> _dis_double;

    	// letters
    	const int _letters_len = 26;
    	char _letters[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    	std::uniform_int_distribution<> _dis_letters;

    	char randLetter();
    	std::string randName();

    	// lists of allowed variable functions for exploration option events
    	const unsigned int _per_option_multiplier = 3; // used to increase the intensity of an event if it triggers once per option instead of every turn
    	std::vector<std::function<void(unsigned int)> > _per_turn_events;
    	std::vector<std::function<void(unsigned int)> > _per_option_events;
    	double _per_turn_chance_threshold = 0.75f;
    	double _per_option_chance_threshold = 0.6f;

    	// playerchoice
    	PlayerchoiceFactory pcf;
    	double _playerchoice_chance_threshold = 0.8f;

		void init();

	public:
		ExplorationOptionFactory()
		: 	_gen(_rd()),
			_dis_int(0, 99), 
			_dis_double(0.0, std::nextafter(1.0, std::numeric_limits<double>::max())),
			_dis_letters(0, _letters_len-1)
		{
			init();
		}

		/** Generates an option scaling some values based on specified depth, has a large degree of randomness */
		ExplorationOption Generate(int depth);
	};

}} // hapnmc::dedcom

#endif // DEDCOM_EXPLORATIONOPTIONS_HPP