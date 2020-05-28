#ifndef DEDCOM_ACTOR_FIXTURES_HPP
#define DEDCOM_ACTOR_FIXTURES_HPP

#include <actor.hpp>
#include <list>
#include <random>

namespace hapnmc { namespace dedcom {

	/** Holds a number of premade foes to throw at the player bc we're hardcoding everything here yeeeboi */
	class ActorRepoFoes
	{
	private:
		std::random_device _rd;
		std::mt19937 _gen;

		std::vector<Actor> _actors;

		

		void randomize(Actor& actor);
	public:

		ActorRepoFoes() : _gen(_rd())
		{
			//init();
		}

		void Init();

		void AddActor(Actor a);

		/** returns true if an actor template of threat lvl or lower exists */
		bool HasActor(int max_threat_lvl);
		/** randomizes some values and returns a new actor based on the template, the actor is guaranteed to be of provided threat lvl or lower */
		Actor GetRandomActorCopy(int max_threat_lvl);

	};

}} // hapnmc::dedcom

#endif // DEDCOM_ACTOR_FIXTURES_HPP