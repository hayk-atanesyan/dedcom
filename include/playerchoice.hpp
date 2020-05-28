#ifndef DEDCOM_PLAYERCHOICE_HPP
#define DEDCOM_PLAYERCHOICE_HPP

#include <vector>
#include <string>
#include <functional>
#include <random>

namespace hapnmc { namespace dedcom {

	// ====== choice option effect functions =========

	unsigned int GetScaledChoiceAmount(unsigned int amnt);

	void PCE_AddArmorToParty(unsigned int amnt);

	void PCE_AddPowerToPartyAndDamage(unsigned int amnt);

	/**
	* Stores and activates choices with associated outcomes for exploration cells
	*
	*/
	class ChoiceOption
	{
	public:
	//private:
		

		std::string _txt; // description of this option

		std::function<void()> _effect; // this will be called upon choosing this option

		ChoiceOption(std::string&& txt, std::function<void()> eff)
		: _txt(txt), _effect(eff)
		{

		}
		ChoiceOption(std::string&& txt)
		: _txt(txt)
		{

		}
	//public:
		const std::string& GetTxt() { return _txt; }
		void Resolve() { if(_effect) _effect(); }
	};


	/**
	* Stores and activates choice options with associated outcomes for exploration cells
	*
	*/
	class Playerchoice
	{

	private:
		std::string _flavour_txt; // displayed above available options

		std::vector<ChoiceOption> _options;


	public:
		Playerchoice(){}

		Playerchoice(std::string&& txt) : _flavour_txt(txt)
		{
			
		}

		void AddOption(ChoiceOption& co) { _options.push_back(co); }
		void AddOption(ChoiceOption&& co) { _options.push_back(co); }

		operator bool() const {
			return !_flavour_txt.empty() && !_options.empty();
		} 

		const std::string& GetTxt() { return _flavour_txt; }
		const std::vector<ChoiceOption>& GetOptions() { return _options; }

		void Resolve(int index) { _options.at(index).Resolve(); }
		bool HasOption(int index) { return index >= 0 && index < (int)_options.size(); }

		void Clear() {
			_flavour_txt = "";
			_options.clear();
		}
	};


	/**
	* Generates choices, not much else
	*
	*/
	class PlayerchoiceFactory
	{
	private:
		std::random_device _rd;
		std::mt19937 _gen;

		/** list of all possible choices with variable impact (scales with depth) */
		std::vector<Playerchoice> _prefab_choices;

		void init();
	public:
		
		PlayerchoiceFactory()
		: _gen(_rd())
		{
			init();
		}

		/** Generates a choice scaling the effect based on the current exploration option's depth */
		Playerchoice Generate();
	};

}} // hapnmc::dedcom

#endif // DEDCOM_PLAYERCHOICE_HPP