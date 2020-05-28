#ifndef DEDCOM_EXPLORATIONENGINE_HPP
#define DEDCOM_EXPLORATIONENGINE_HPP

#include <string>
#include <vector>
#include <exploration_options.hpp>

namespace hapnmc { namespace dedcom {

	/** 
	 *	Responsible for handling generation of
	 *  events and encounters in a run.
	 */
	class ExplorationEngine
	{
	private:

		std::vector<ExplorationOption> _options;
		ExplorationOption _current_option;

		int _depth{0}; // affects new option generation, incremented when exploring an option

		ExplorationOptionFactory _options_fctry;

		void init();

		void generateNewOptions();

		void exploreOption(ExplorationOption& eo);

		void processCurrentOption();

	public:

		ExplorationEngine()
		{
			init();
		}

		ExplorationOption& GetCurrentOption() { return _current_option; };
		const std::vector<ExplorationOption>& GetOptions() const { return _options; }

		/** Sets the new option as current and processes it, also generates new options */
		bool HasOption(int index) const { return index >= 0 && index < (int)_options.size(); }
		void ExploreOption(int index);
		void ExploreOption(const std::string& name);

		int GetDepth() const { return _depth; }


	};

}} // hapnmc::dedcom

#endif // DEDCOM_EXPLORATIONENGINE_HPP