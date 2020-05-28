#include <playerchoice.hpp>
#include <dedcom_engine.hpp>

namespace hapnmc { namespace dedcom {

	// =============== effects =================

	unsigned int GetScaledChoiceAmount(unsigned int amnt)
	{
		unsigned int scaled_amnt = amnt * (1 + DedcomEngine::Instance()._expl_eng.GetDepth() / 10);

		return scaled_amnt;
	}

	void PCE_AddArmorToParty(unsigned int amnt)
	{
		auto scaled_amnt = GetScaledChoiceAmount(amnt);
		if(scaled_amnt == 0) return;

		DedcomEngine::Instance().GetParty().AddArmor(scaled_amnt);
	}

	void PCE_AddPowerToPartyAndDamage(unsigned int amnt)
	{
		auto scaled_amnt = GetScaledChoiceAmount(amnt);
		if(scaled_amnt == 0) return;

		DedcomEngine::Instance().GetParty().AddPower(scaled_amnt);

		// take dmg
		auto scaled_dmg_amnt = scaled_amnt / 3;
		if(scaled_dmg_amnt == 0) scaled_dmg_amnt = 1;

		DedcomEngine::Instance().GetParty().TakeDamage(scaled_dmg_amnt);
	}

	// ================================

	Playerchoice PlayerchoiceFactory::Generate()
	{
		std::uniform_int_distribution<> prefab_choices_dist(0, _prefab_choices.size() - 1);

		return _prefab_choices[prefab_choices_dist(_gen)];
	}






	// ======== factory =======

	void PlayerchoiceFactory::init()
	{
		//_gen.seed(_rd());

		// Add Armour
		Playerchoice pc_armor("Compatible materials discovered: armor plates");
		pc_armor.AddOption(ChoiceOption("Avoid"));
		pc_armor.AddOption(ChoiceOption("Acquire", std::bind(PCE_AddArmorToParty, 1)));
		_prefab_choices.push_back(pc_armor);

		// Add Power
		Playerchoice pc_power("Modifiable power outlet discovered");
		pc_power.AddOption(ChoiceOption("Avoid"));
		pc_power.AddOption(ChoiceOption("Hijack and recharge power reserves", std::bind(PCE_AddPowerToPartyAndDamage, 1)));
		_prefab_choices.push_back(pc_power);
	}

}} // hapnmc::dedcom