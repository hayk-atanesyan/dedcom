#include <actor.hpp>
#include <dedcom_engine.hpp>
#include <stdexcept>

namespace hapnmc { namespace dedcom {

	Actor::Actor(const Actor& a)
	{
		ReadStats(a);
		ReadActions(a);

		init();
	}

	void Actor::ReadStats(const Actor& a)
	{
		_hi_action_selection_threshold = a._hi_action_selection_threshold;
		_name = a._name;
		_description = a._description;

		_base_integrity = a._base_integrity;
		_integrity = a._integrity;

		_armor = a._armor;
		_ammunition = a._ammunition;

		_shield_is_on = a._shield_is_on;
		_shield_amnt = a._shield_amnt;
		_shield_drain = a._shield_drain;

		_power = a._power;
		_power_gen = a._power_gen;

		_threat_lvl = a._threat_lvl;

		_tags = a._tags;

		_party = a._party;
	}

	void Actor::ReadActions(const Actor& a)
	{
		_blocked_actions = a._blocked_actions;

		_actions.clear();
		for(const std::unique_ptr<Action>& actn : a._actions)
		{
			_actions.emplace_back(actn.get()->clone());
		}
	}

	void Actor::init()
	{

	}

	bool Actor::CanAct(std::string action_name)
	{
		if(!HasAction(action_name)) return false;

		Action* actn_ptr;
		for(const auto& actn : _actions) {
			if(actn.get()->GetName() == action_name) {
				actn_ptr = actn.get();
				break;
			}
		}

		return actn_ptr->CanDo(*this);
	}

	void Actor::Act(std::string action_name, double effect_multiplier)
	{
		if(!CanAct(action_name)) return;

		Action* actn_ptr;
		for(const auto& actn : _actions) {
			if(actn.get()->GetName() == action_name) {
				actn_ptr = actn.get();
				break;
			}
		}

		actn_ptr->Do(*this, effect_multiplier);
	}

	void Actor::recalculateSpeed()
	{
		int spd_reduction_from_armour = ((double)_armor / (double)_base_integrity) * (_base_speed/2 - 1);
		int spd_reduction_from_integrity = ((double)_integrity / (double)_base_integrity) * (_base_speed/2 - 1);

		_speed = _base_speed - spd_reduction_from_armour - spd_reduction_from_integrity;
	}

	bool Actor::IsShieldUp()
	{
		return _shield_is_on && _shield_drain <= _power;
	}

	void Actor::DrainPower(unsigned int amnt)
	{
		_power -= amnt;
		if(_power < 0) _power = 0;
	}

	void Actor::die()
	{
		// ...
	}

	// TODO add calls to threshold events or a generic onDamageTaken
	void Actor::TakeDamage(unsigned int amnt)
	{
		// shield
		if(IsShieldUp()) {
			doShieldDrain();

			if((int)amnt <= _shield_amnt) {
				return;
			} else {
				amnt -= _shield_amnt;
			}
		}

		// armor
		if((int)amnt <= _armor) {
			_armor -= amnt;
			recalculateSpeed();
			return;
		} else {
			amnt -= _armor;
			_armor = 0;
			recalculateSpeed();
		}

		// integrity
		_integrity -= amnt;

		if(_integrity < 1) die();
	}

	void Actor::AddArmor(unsigned int amnt)
	{
		_armor += amnt;

		recalculateSpeed();
	}

	void Actor::AddPower(unsigned int amnt)
	{
		_power += amnt;
	}

	Party& Actor::GetOpponentParty()
	{
		Party& p = GetParty();
		Party& player_party = DedcomEngine::Instance().GetParty();

		if(&p == &player_party) return player_party;

		return DedcomEngine::Instance()._enc_eng.GetParty();
	}

	bool Actor::IsInPlayerParty()
	{
		Party& p = GetParty();
		Party& player_party = DedcomEngine::Instance().GetParty();

		if(&p == &player_party) return true;
		else return false;
	}

	void Actor::AddAction(std::string&& action_name)
	{
		//_actions.push_back(DedcomEngine::Instance()._action_repo.GetActionCopy(action_name));

		_actions.emplace_back(
			DedcomEngine::Instance()._action_repo.GetActionCopy(action_name)
			);
	}

	bool Actor::HasAction(const std::string& action_name)
	{
		for(const std::unique_ptr<Action>& actn : _actions) {
			if(actn.get()->GetName() == action_name) return true;
		}

		return false;
	}

	void Actor::AddTag(std::string&& tag_name)
	{
		_tags.push_back(tag_name);
	}

	bool Actor::HasTag(const std::string& tag_name)
	{
		return std::find(_tags.begin(), _tags.end(), tag_name) != _tags.end();
	}

	void Actor::ActRandomly(double effect_multiplier)
	{
		// sort in a way that the most impactful actions are checked first
		_actions.sort([](const std::unique_ptr<Action>& a, const std::unique_ptr<Action>& b){
			if(a.get()->GetProjectedImpact() >= b.get()->GetProjectedImpact()) return true;
			else return false;
		});

		// try to find an action based on thereshold NB this loop can fail, that is intentional
		for(const std::unique_ptr<Action>& actn : _actions) {
			if(CanAct(actn.get()->GetName()) && 
				DedcomEngine::Instance().rng.GetRndDbl() < _hi_action_selection_threshold) {
				Act(actn.get()->GetName());
				return;
			}
		}

		// try to find any applicable action with highest impact if the rng didn't work out
		for(const std::unique_ptr<Action>& actn : _actions) {
			if(CanAct(actn.get()->GetName())) {
				Act(actn.get()->GetName());
				return;
			}
		}
	}

	std::string Actor::GetShortReport() const
	{
		std::string report;

		float ingtr_perc = (float)_integrity / (float)_base_integrity;
		if(ingtr_perc > 0.9) report += "undamaged";
		else if(ingtr_perc > 0.5) report += "damaged";
		else if(ingtr_perc > 0.001) report += "critical";
		else report += "destroyed";

		report += "|";

		if(DedcomEngine::Instance().GetEncounterEngine().HasQueuedAction(_name))
		{
			report += DedcomEngine::Instance().GetEncounterEngine().GetQueuedAction(_name);
		} else {
			report += "autonom";
		}

		return report;
	}

	std::string Actor::GetMediumReport() const
	{
		std::string report;

		report += "Integr " + std ::to_string(_integrity) + "/" + std::to_string(_base_integrity) + "; ";
		report += "Ammo " + std::to_string(_ammunition) + "; ";
		report += "Pwr " + std::to_string(_power) + "(" + std::to_string(_power_gen) + "); ";

		return report;
	}


	std::string Actor::GetReport() const
	{
		std::string report;

		//report += "Intgr | Armr | Ammo | Shld | Pwr | Spd | Tags | Actions\r\n"; 

		report += "Intgr: " + std::to_string(_integrity) + "/" + std::to_string(_base_integrity) + " | ";
		report += "Armr: " + std::to_string(_armor) + " | ";
		report += "Ammo: " + std::to_string(_ammunition) + "\r\n";
		report += "Shld: " + std::to_string(_shield_amnt) + "(" + std::to_string(_shield_drain) + ") | ";
		report += "Pwr: " + std::to_string(_power) + "(" + std::to_string(_power_gen) + ") | ";
		report += "Spd: " + std::to_string(_speed) + "/" + std::to_string(_base_speed) + "\r\n";

		report += "Tags: ";
		for(const std::string& t : _tags) {
			report += t + ", ";
		}
		report += "\r\n";


		report += "Actions: ";
		for(const std::unique_ptr<Action>& actn : _actions) {
			report += actn.get()->GetName() + ", ";
		}
		report += "\r\n";

		return report;
	}

	Action& Actor::GetAction(const std::string& action_name)
	{
		for(const std::unique_ptr<Action>& actn : _actions) {
			if(actn.get()->GetName() == action_name) return *actn;
		}

		throw std::invalid_argument("Protocol " + action_name + " not found.");
	}

}} // hapnmc::dedcom