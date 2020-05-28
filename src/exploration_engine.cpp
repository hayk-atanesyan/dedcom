#include <exploration_engine.hpp>
#include <exception>
#include <dedcom_engine.hpp>

namespace hapnmc { namespace dedcom {

	void ExplorationEngine::init()
	{
		_current_option.name = "Entrance";

		generateNewOptions();
	}

	void ExplorationEngine::generateNewOptions()
	{
		_options.clear();

		for(int i=0; i<3; ++i) {
			_options.push_back(_options_fctry.Generate(_depth));
		}
	}

	void ExplorationEngine::processCurrentOption()
	{
		if(_current_option.OnEnter) _current_option.OnEnter();

		if(_current_option.HasEncounter()) {
			DedcomEngine::Instance()._enc_eng.GenerateEncounter(_current_option.threatLevel);
		} else {
			if(_current_option.OnClear) _current_option.OnClear();
		}

		/*if(_current_option.pchoice) { // already displayed in exploration info
			// TODO show has option
		}*/

	}

	void ExplorationEngine::exploreOption(ExplorationOption& eo)
	{
		_current_option.pchoice.Clear();
		_current_option = eo;
		_depth++;
		generateNewOptions();

		processCurrentOption();
	}

	void ExplorationEngine::ExploreOption(int index)
	{
		exploreOption(_options[index]);
	}

	void ExplorationEngine::ExploreOption(const std::string& name)
	{
		int index = -1;

		for(int i=0; i<(int)_options.size(); ++i) {
			if(_options[i].name == name) {
				index = i;
				break;
			}
		}

		if(index == -1) throw std::invalid_argument("ExplorationEngine::ExploreOption couldn't find option with the specified name."); 

		ExploreOption(index);
	}

}} // hapnmc::dedcom