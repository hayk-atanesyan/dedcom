#include <exploration_options.hpp>
#include <dedcom_engine.hpp>
#include <iostream>

namespace hapnmc { namespace dedcom {

	// ---------------- exploration option events list -------------------------

	unsigned int GetModifiedEffectAmount(unsigned int amnt)
	{
		double modified_amnt = (double)amnt * 
			DedcomEngine::Instance()._expl_eng.GetCurrentOption().eventEffectMultiplier;

		return (unsigned int)std::round(modified_amnt);
	}

	// ------------------------------------------------

	void EOE_DamageParty(unsigned int amnt)
	{
		auto modified_amnt = GetModifiedEffectAmount(amnt);
		if(modified_amnt == 0) return; // prevents excessive processing and, more importantly, logging

		DedcomEngine::Instance().GetParty().TakeDamage(modified_amnt);
	}

	void EOE_DrainPowerFromParty(unsigned int amnt)
	{
		auto modified_amnt = GetModifiedEffectAmount(amnt);
		if(modified_amnt == 0) return;

		DedcomEngine::Instance().GetParty().DrainPower(modified_amnt);
	}



	// ------------- factory methods --------------------

	char ExplorationOptionFactory::randLetter() { return _letters[_dis_letters(_gen)]; }

	std::string ExplorationOptionFactory::randName()
	{
		std::string name;
		name.push_back(randLetter());
		name.push_back(randLetter());

		return name + std::to_string(_dis_int(_gen));
	}

	ExplorationOption ExplorationOptionFactory::Generate(int depth)
	{
		ExplorationOption eo;

		eo.name = randName();

		if(_dis_double(_gen) > 0.4f) {
			eo.threatLevel = _dis_double(_gen) * (depth * (2.0f + _dis_double(_gen)));
			//std::cout << "Generated option with threatLevel " << eo.threatLevel << std::endl;
		} else {
			eo.threatLevel = 0;
		}

		eo.eventEffectMultiplier = _dis_double(_gen) + _dis_double(_gen) * (double)depth/100.0;

		// events NB not sure if lists are going to be dynamic just keep this
		std::uniform_int_distribution<> per_turn_dist(0, _per_turn_events.size() - 1);
		std::uniform_int_distribution<> per_option_dist(0, _per_option_events.size() - 1);


		//per turn event generation, either on turn start or on end, never both
		if(_dis_double(_gen) > _per_turn_chance_threshold) {

			unsigned int param = 1 + depth/10;
			auto f = std::bind(_per_turn_events[per_turn_dist(_gen)], param);

			if(_dis_double(_gen) > 0.5f) {
				eo.OnNewTurnEvent = f;
			} else {
				eo.OnTurnEndEvent = f;
			}
		}

		// per option event generation, either on area enter or clear, never both
		if(_dis_double(_gen) > _per_option_chance_threshold) {

			unsigned int param = 1 + depth/10;
			auto f = std::bind(_per_option_events[per_option_dist(_gen)], param);

			if(_dis_double(_gen) > 0.5f) {
				eo.OnEnter = f;
			} else {
				eo.OnClear = f;
			}
		}

		// playerchoice generation
		if(_dis_double(_gen) > _playerchoice_chance_threshold) {
			eo.pchoice = pcf.Generate();
		} else {
			eo.pchoice.Clear();
		}

		return eo;
	}



	// ---------------- INIT ----------------------

	void ExplorationOptionFactory::init()
	{
		//_gen.seed(_rd());

		// ========= Populating Lists ====================

		// EOE_DamageParty ---	
		_per_turn_events.push_back([this](unsigned int amnt){
			EOE_DamageParty(amnt);
		});
		_per_option_events.push_back([this](unsigned int amnt){
			EOE_DamageParty(_per_option_multiplier * amnt);
		});

		// EOE_DrainPowerFromParty ---	
		_per_turn_events.push_back([this](unsigned int amnt){
			EOE_DrainPowerFromParty(amnt);
		});
		_per_option_events.push_back([this](unsigned int amnt){
			EOE_DrainPowerFromParty(_per_option_multiplier * amnt);
		});

	}

}} // hapnmc::dedcom
