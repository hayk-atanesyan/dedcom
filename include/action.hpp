#ifndef DEDCOM_ACTION_HPP
#define DEDCOM_ACTION_HPP

#include <string>
#include <memory>

namespace hapnmc { namespace dedcom {

	class Actor;
	class Party;

	/** 
	 *	Represents a single action for a given actor, like movement to another cell.
	 *  Actions are instanced per actor and can store actor specific data. 
	 */
	class Action
	{
	protected:
		std::string _name;
		std::string _description;

		int _upgrade_amnt;

		/** can be used as a flat amount or magnitude by specific action classes */
		int GetUpgradeAmount() { return _upgrade_amnt; }

	public:
		Action(std::string name, std::string description) 
		: _name(name), _description(description), _upgrade_amnt(0)
		{}

		//void SetOwner(Actor& a) { _owner = &a; }

		/** effect_multiplier can be utilized in certain actions to increase the effect, especially in lower depths */
		virtual void Do(Actor& a, double effect_multiplier = 1.0f) {};
		virtual bool CanDo(Actor& a) { return true; }

		const std::string& GetName() const { return _name; }
		virtual std::string GetRequirementsTxt() { return ""; }

		/** can be used to increase the effect */
		virtual void Upgrade() {};

		virtual std::string GetDescription() { return _description; }
		std::string GetDescriptionAndRequirements() { return GetDescription() + " | " + GetRequirementsTxt(); }

		/** used to determine which actions are more impactful, those will be prioritized by the computer */
		virtual int GetProjectedImpact() const { return 0; };

		virtual std::unique_ptr<Action> clone() const { 
			return std::unique_ptr<Action>( new Action( *this ) ); 
		}
	};

}} // hapnmc::dedcom

#endif // DEDCOM_ACTION_HPP