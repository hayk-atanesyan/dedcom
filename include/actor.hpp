#ifndef DEDCOM_ACTOR_HPP
#define DEDCOM_ACTOR_HPP

#include <base_entity.hpp>
#include <action.hpp>
#include <string>
#include <list>
#include <memory>


namespace hapnmc { namespace dedcom {

	class Party;

	/** 
	 *	Class for representing drones in encounters.
	 *  NB: do not add maximum capacities as it saves time and design does not suffer terribly
	 */
	class Actor : public BaseEntity
	{
	private:


		double _hi_action_selection_threshold = 0.8f;

		std::string _name;
		std::string _description;

		int _base_integrity{1}; // diverging too far from this is costly
		int _integrity{1}; // structural integrity of actor

		int _armor{0}; // does not affect integrity but it subtracted before it

		int _ammunition{0}; // some actions deplete this

		bool _shield_is_on{true}; // whether shield is used to mitigate or is bypassed 
		int _shield_amnt{0}; // max amount of mitigation per event
		int _shield_drain{0}; // power drain per mitigation event

		int _power{0}; // power reserve, used for especially power hungry operations
		int _power_gen{0}; // power generated per turn

		int _base_speed{10}; // used in recalculations when changing armour value and such
		int _speed{10}; // determines if the actor can act before others, higher is better, zero means can't move

		void recalculateSpeed(); // recalculate speed based on armor and other factors

		int _threat_lvl{0}; // this number indicates how likely the actor is to be targeted compared to the rest

		std::list<std::unique_ptr<Action>> _actions; // list of action instances, they can store actor-specific data
		std::list<std::string> _blocked_actions; // these actions are currently unavailable, list is managed by actions mostly

		std::list<std::string> _tags; // used for special actions/interactions which check for specific tags

		Party* _party;

		void onIntegrityLoss();

		void doShieldDrain() { DrainPower(_shield_drain); }

		void die();

		void init();
	public:
		Actor(std::string name, std::string description, 
			int integr, int armor, int ammo, int sh_amnt, int sh_drain, 
			int pwr, int pwr_gen, int speed, int thrt) :
		 _name(name), _description(description), _base_integrity(integr), _integrity(integr), _armor(armor), _ammunition(ammo),
		_shield_amnt(sh_amnt), _shield_drain(sh_drain), _power(pwr),
		_power_gen(pwr_gen), _base_speed(speed), _speed(speed), _threat_lvl(thrt)
		{
			init();
		}

		Actor(const Actor& a);

		void ReadStats(const Actor& a);
		void ReadActions(const Actor& a);

		/** determines whether actor can act at all, dead actors get cleaned up at turn end */
		bool IsDead() const { return _integrity < 1; }

		void SetParty(Party& p) { _party = &p; }
		void SetParty(Party* p) { _party = p; }

		Party& GetParty() { return *_party; }
		Party& GetOpponentParty();
		bool IsInPlayerParty();

		/** Performs an available action by name, action should exist in an internal list */
		void Act(std::string action_name, double effect_multiplier = 1.0f);
		bool CanAct(std::string action_name);

		/** not entirely randomly, has some basic behavior where highest impact actions are prioritized*/
		void ActRandomly(double effect_multiplier = 1.0f); 

		/** If shield works, might still be false if turned on bc ran out of power or anything else */
		bool IsShieldUp();

		void DrainPower(unsigned int amnt);

		int GetAmmo() const { return _ammunition; }
		void SetAmmo(int ammo) { _ammunition = ammo; }
		void DrainAmmo(unsigned int amnt) { _ammunition -= amnt; }

		int GetSpeed() const { return _speed; }
		int GetBaseSpeed() { return _base_speed; }
		void SetBaseSpeed(int base_speed) { 
			_base_speed = base_speed; 
			recalculateSpeed();
		}

		const std::string& GetName() const { return _name; }


		/** Takes into account any mitigating factors including shields and armor, subtracts used resources  */
		void TakeDamage(unsigned int amnt);
		

		// ------- accessors ---------
		int GetIntegrity() const { return _integrity; }
		int GetBaseIntegrity() const { return _base_integrity; }
		void ResetIntegrity(int new_integrity) { /** resets base and current integrity to value */
			_base_integrity = _integrity = new_integrity; 
			recalculateSpeed();
		} 

		int GetArmor() const { return _armor; }
		void SetArmor(int armor) { 
			_armor = armor; 
			recalculateSpeed(); 
		}
		void AddArmor(unsigned int amnt);

		int GetShieldAmount() const { return _shield_amnt; }
		void SetShieldAmount(int shield_amnt) { _shield_amnt = shield_amnt; }

		int GetPower() const { return _power; }
		void SetPower(int power) { _power = power; }
		void AddPower(unsigned int amnt);

		int GetThreatLvl() const { return _threat_lvl; }

		void AddAction(std::string&& action_name); /** gets the action by name from the repository */
		bool HasAction(const std::string& action_name);
		Action& GetAction(const std::string& action_name);
		const std::list<std::unique_ptr<Action>>& GetActionsList() { return _actions; }

		void AddTag(std::string&& tag_name);
		bool HasTag(const std::string& tag_name);

		std::string GetShortReport() const;

		std::string GetMediumReport() const;

		std::string GetReport() const;

		// ------- events -----
		void OnNewTurn();

		void OnBeforeAction();
		void OnAfterAction();

		void OnTurnEnd();

	};

}} // hapnmc::dedcom

#endif // DEDCOM_ACTOR_HPP