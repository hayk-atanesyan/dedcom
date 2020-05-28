#ifndef DEDCOM_ENCOUNTERENGINE_HPP
#define DEDCOM_ENCOUNTERENGINE_HPP

#include <arena.hpp>
#include <party.hpp>
#include <actor_fixtures.hpp>
#include <list>
#include <unordered_map>
#include <string>

namespace hapnmc { namespace dedcom {

	/** 
	 *	Responsible for controlling the encounter flow
	 *  including sides taking turns.
	 */
	class EncounterEngine
	{
	private:
		Party _foe_party; /** any foes are put here for the duration of encounter */
		const int _max_num_foes = 20;

		ActorRepoFoes _repo_foes;

		std::list<Actor*> getAllActorsBySpeed();

		std::unordered_map<std::string, std::string> _queued_player_actions;

		double getEffectMultiplierForFoes();
	public:
		void Init() { _repo_foes.Init(); }

		Party& GetParty() { return _foe_party; }

		/** Generates a new foe party and environment */
		void GenerateEncounter(uint threatLvl);

		/** Clears foe party and environment */
		void ClearEncounter();

		void ResolveTurn();

		bool HasEncounter();

		void QueuePlayerAction(const std::string& drone_name, const std::string& action_name);

		bool HasQueuedAction(const std::string& drone_name);
		std::string GetQueuedAction(const std::string& drone_name);
	};

}} // hapnmc::dedcom

#endif // DEDCOM_ENCOUNTERENGINE_HPP