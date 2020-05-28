#ifndef DEDCOM_ACTION_FIXTURES_HPP
#define DEDCOM_ACTION_FIXTURES_HPP

#include <action.hpp>
#include <unordered_map>
#include <memory>

namespace hapnmc { namespace dedcom {

	/**
	 * Just stores action templates
	 */
	class ActionRepo
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<Action>> _actions;

		
	public:
		ActionRepo()
		{
			//init();
		}

		void Init();

		void AddAction(std::unique_ptr<Action>&& a)
		{
			//std::unique_ptr<Action> actn(a);
			_actions.emplace( 
				std::make_pair(
					std::string(a->GetName()), std::move(a)
				) // pair
			);
		}

		std::unique_ptr<Action> GetActionCopy(std::string name)
		{
			return _actions.at(name).get()->clone();
		}

		const std::unique_ptr<Action>& GetAction(std::string name)
		{
			return _actions.at(name);
		}

	};

	/**
	 * A generic direct damage action with modifiable damage and ammo consumption.
	 * If the area flag is set then the action affects ALL foes.
	 */
	class ActionShoot : public Action
	{
	protected:
		int _dmg;
		int _ammo_cost;
		int _power_cost;
		bool _area;

	public:
		ActionShoot(std::string name, std::string description, int dmg, int ammo_cost = 0, int power_cost = 0, bool area = false)
		: Action(name, description), _dmg(dmg), _ammo_cost(ammo_cost), _power_cost(power_cost), _area(area)
		{
			// setting upgrade amount for damage value to half of base amount
			_upgrade_amnt = _dmg / 2;
			if(_upgrade_amnt < 1) _upgrade_amnt = 1;
		}

		virtual void Do(Actor& a, double effect_multiplier = 1.0f) override;
		virtual bool CanDo(Actor& a) override;

		/** Gets base dmg from the repo and adds a fixed amount, diminishing returns are intentional */
		virtual void Upgrade() override;

		virtual std::string GetDescription() override;
		virtual std::string GetRequirementsTxt() override;

		virtual int GetProjectedImpact() const override;

		virtual std::unique_ptr<Action> clone() const override { 
			return std::unique_ptr<Action>( new ActionShoot( *this ) ); 
		}
	};

}} // hapnmc::dedcom

#endif // DEDCOM_ACTION_FIXTURES_HPP