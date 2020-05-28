#ifndef DEDCOM_PARTY_HPP
#define DEDCOM_PARTY_HPP

#include <actor.hpp>
#include <list>
#include <random>
#include <string>

namespace hapnmc { namespace dedcom {

	/** 
	 *	Class for keeping track of one side's actors.
	 *  
	 */
	class Party
	{
	private:
		std::random_device _rd;
		std::mt19937 _gen;

		std::list<Actor> _actors;
	public:
		Party()
		:_gen(_rd())
		{}
		// TODO these are wrapped bc certain actors should be prioritized over others for specific resource,
		// e.g. one gets most of the armour, other takes all of the power
		// TODO use actor tags to determine which
		void TakeDamage(unsigned int amnt);
		void DrainPower(unsigned int amnt);

		void AddArmor(unsigned int amnt);
		void AddPower(unsigned int amnt);

		Actor& GetRandomActor();

		void AddActor(Actor& a)
		{
			a.SetParty(this);
			_actors.push_back(a);
		}

		void Clear()
		{
			_actors.clear();
		}

		std::list<Actor>& GetActorsList() { return _actors; }

		Actor& GetHighestThreatActor();

		/** random actor heavily weighted towards the highest threat one (~75%?) */
		Actor& GetRandomTarget();

		bool HasActor(const std::string& name);
		Actor& GetActor(const std::string& name);

		void ClearTheDead();

		std::string HeadcountTxt() const;

	};

}} // hapnmc::dedcom

#endif // DEDCOM_PARTY_HPP