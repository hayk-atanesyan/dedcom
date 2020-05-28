
#include <action_fixtures.hpp>
#include <actor.hpp>
#include <party.hpp>
#include <dedcom_engine.hpp>

namespace hapnmc { namespace dedcom {

	bool ActionShoot::CanDo(Actor& a)
	{
		return a.GetAmmo() >= _ammo_cost;
	}

	void ActionShoot::Do(Actor& a, double effect_multiplier)
	{
		a.DrainAmmo(_ammo_cost);
		a.DrainPower(_power_cost);

		Party& opponent_party = a.GetOpponentParty();

		if(_area) {
			opponent_party.TakeDamage(_dmg * effect_multiplier);
		} else {
			opponent_party.GetRandomTarget().TakeDamage(_dmg * effect_multiplier);
		}
	}

	std::string ActionShoot::GetDescription()
	{
		std::string descr(_description);

		auto ind = descr.find("[d]", 0);
		if (ind != std::string::npos) {
			descr.replace(ind, 3, std::to_string(_dmg));
		}

		return descr;
	}

	std::string ActionShoot::GetRequirementsTxt()
	{
		std::string reqs = "";

		if(_ammo_cost > 0) reqs += std::to_string(_ammo_cost) + " Ammo ";
		if(_power_cost > 0) reqs += std::to_string(_power_cost) + " Pwr ";

		if(_ammo_cost <= 0 && _power_cost <= 0) reqs += "Free";

		return reqs;
	}

	void ActionShoot::Upgrade()
	{
		_dmg += GetUpgradeAmount();
	}

	int ActionShoot::GetProjectedImpact() const
	{
		return (_area ? _dmg * 3 : _dmg);
	}

	// =================== REPO ===========================

	void ActionRepo::Init()
	{
		// ==== Basic Hits ===========

		AddAction(
			std::unique_ptr<Action>(new ActionShoot("Assault", "Attacks dealing [d] damage",
				5))
		);


		// ==== Basic Shots ==========

		AddAction(
			std::unique_ptr<Action>(new ActionShoot("Slugshot", "Launches a heavy metal slug dealing [d] damage",
				10, 1))
		);

		AddAction(
			std::unique_ptr<Action>(new ActionShoot("Flamer", "Sets all foes ablaze dealing [d] damage to each",
				5, 2, 0, true))
		);

		AddAction(
			std::unique_ptr<Action>(new ActionShoot("Plasmashot", "Superheats a projectile dealing [d] damage",
				20, 1, 1))
		);

		AddAction(
			std::unique_ptr<Action>(new ActionShoot("Electrocute", "Uses reserve power to electrocute for [d] dmg",
				10, 0, 1))
		);

		AddAction(
			std::unique_ptr<Action>(new ActionShoot("Discharge", "Uses reserve power to electrocute all foes for [d] dmg",
				10, 0, 4))
		);
	}

}} // hapnmc::dedcom








