#include <encounter_engine.hpp>
#include <dedcom_engine.hpp>
#include <iostream>

namespace hapnmc { namespace dedcom {

	void EncounterEngine::ClearEncounter()
	{
		_foe_party.Clear();
		DedcomEngine::Instance()._expl_eng.GetCurrentOption().threatLevel = 0;

		if(DedcomEngine::Instance()._expl_eng.GetCurrentOption().OnClear) {
			DedcomEngine::Instance()._expl_eng.GetCurrentOption().OnClear();
		}
	}

	void EncounterEngine::GenerateEncounter(uint threatLvl)
	{	

		//std::cout << "Attempting to generate encounter with threat lvl " << threatLvl << std::endl;

		_foe_party.Clear();

		int threatLvlRemaining = threatLvl;
		int count = 0;
		while(threatLvlRemaining > 0 && count < _max_num_foes)
		{
			count++;

			if(_repo_foes.HasActor(threatLvlRemaining)) {
				Actor actor = _repo_foes.GetRandomActorCopy(threatLvlRemaining);
				threatLvlRemaining -= actor.GetThreatLvl();
				_foe_party.AddActor(actor);
			} else {
				break;
			}
		}

		if(_foe_party.GetActorsList().empty()) ClearEncounter();
	}

	std::list<Actor*> EncounterEngine::getAllActorsBySpeed()
	{
		std::list<Actor*> actors_by_speed;
		for(Actor& a : DedcomEngine::Instance()._party.GetActorsList()) {
			actors_by_speed.push_back(&a);
		}
		for(Actor& a : _foe_party.GetActorsList()) {
			actors_by_speed.push_back(&a);
		}
		actors_by_speed.sort([](const Actor* a, const Actor* b){
			if(a->GetSpeed() >= b->GetSpeed()) return true;
			else return false;
		});

		return actors_by_speed;
	}

	void EncounterEngine::ResolveTurn()
	{
		if(DedcomEngine::Instance()._expl_eng.GetCurrentOption().OnNewTurnEvent) {
			DedcomEngine::Instance()._expl_eng.GetCurrentOption().OnNewTurnEvent();
		}

		// TODO actor on turn starts here

		// act
		for(Actor* actr : getAllActorsBySpeed()) {
			if(actr->IsDead()) continue;

			if(actr->IsInPlayerParty()) { // player
				if(!HasQueuedAction(actr->GetName())) {
					actr->ActRandomly(); // player abstained, drone decides for itself
				} else {
					actr->Act(_queued_player_actions.at(actr->GetName())); // player queued action
				}
			} else { // foe
				actr->ActRandomly(getEffectMultiplierForFoes());
			}
		}

		_foe_party.ClearTheDead();
		DedcomEngine::Instance()._party.ClearTheDead();

		// TODO actor on turn ends here

		_queued_player_actions.clear();

		if(DedcomEngine::Instance()._expl_eng.GetCurrentOption().OnTurnEndEvent) {
			DedcomEngine::Instance()._expl_eng.GetCurrentOption().OnTurnEndEvent();
		}

		if(_foe_party.GetActorsList().empty()) ClearEncounter(); //
	}

	bool EncounterEngine::HasEncounter()
	{
		return DedcomEngine::Instance()._expl_eng.GetCurrentOption().HasEncounter();
	}

	void EncounterEngine::QueuePlayerAction(const std::string& drone_name, const std::string& action_name)
	{
		_queued_player_actions[drone_name] = action_name;
	}

	bool EncounterEngine::HasQueuedAction(const std::string& drone_name)
	{
		return _queued_player_actions.count(drone_name) > 0;
	}

	std::string EncounterEngine::GetQueuedAction(const std::string& drone_name)
	{
		if(!HasQueuedAction(drone_name)) return "";

		return _queued_player_actions[drone_name];
	}

	double EncounterEngine::getEffectMultiplierForFoes()
	{
		return 1.0 + (double)( DedcomEngine::Instance()._expl_eng.GetDepth() ) / 40.0;
	}

}} // hapnmc::dedcom