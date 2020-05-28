

#include <party.hpp>
#include <stdexcept>
#include <unordered_map>

namespace hapnmc { namespace dedcom {

	void Party::AddArmor(unsigned int amnt)
	{
		for(Actor& a : _actors)
		{
			a.AddArmor(amnt);
		}
	}

	void Party::TakeDamage(unsigned int amnt)
	{
		for(Actor& a : _actors)
		{
			a.TakeDamage(amnt);
		}
	}

	void Party::DrainPower(unsigned int amnt)
	{
		for(Actor& a : _actors)
		{
			a.DrainPower(amnt);
		}
	}
	void Party::AddPower(unsigned int amnt)
	{
		for(Actor& a : _actors)
		{
			a.AddPower(amnt);
		}
	}

	Actor& Party::GetRandomActor()
	{
		std::uniform_int_distribution<int> distr(0, _actors.size() - 1);

		auto it = _actors.begin();

		std::advance(it, distr(_gen));

		return *it;
	}
	
	Actor& Party::GetHighestThreatActor()
	{
		Actor* hta = &(_actors.front());
		for(Actor& a : _actors) {
			if(a.GetThreatLvl() > hta->GetThreatLvl()) {
				hta = &a;
			}
		}

		return *hta;
	}

	Actor& Party::GetRandomTarget()
	{
		std::uniform_real_distribution<double> dis_double(0.0, 1.0);

		if(dis_double(_gen) < 0.75) {
			return GetHighestThreatActor();
		} else {
			return GetRandomActor(); // NB still can return highest threat actor but that's ok, the 75 is just the guarantee range
		}
	}

	bool Party::HasActor(const std::string& name)
	{
		for(const Actor& actr : _actors) {
			if(actr.GetName() == name) return true;
		}

		return false;
	}

	Actor& Party::GetActor(const std::string& name)
	{
		for(Actor& actr : _actors) {
			if(actr.GetName() == name) return actr;
		}

		throw std::invalid_argument("No actor with name " + name + " found in party.");
	}

	void Party::ClearTheDead()
	{
		_actors.remove_if([](const Actor& actr){
			return actr.IsDead();
		});
	}

	std::string Party::HeadcountTxt() const
	{
		std::unordered_map<std::string, int> _headcount;

		for(const Actor& actr : _actors) {
			if(_headcount.count(actr.GetName()) < 1) {
				_headcount[actr.GetName()] = 1;
			} else {
				_headcount[actr.GetName()] = _headcount[actr.GetName()] + 1;
			}
		}

		std::string s;

		for(auto pair : _headcount) {
			s += pair.first + ": " + std::to_string(pair.second) + "; ";
		}

		return s;
	}

}} // hapnmc::dedcom