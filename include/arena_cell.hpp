#ifndef DEDCOM_ARENACELL_HPP
#define DEDCOM_ARENACELL_HPP

#include <actor.hpp>
#include <neutral_object.hpp>

#include <memory>


namespace hapnmc { namespace dedcom {

	/** 
	 *	Class for representing a single cell/tile in encounter arena.
	 *  
	 */
	class ArenaCell
	{
	private:
		std::unique_ptr<NeutralObject> _uptr_no;
		std::weak_ptr<Actor> _wptr_act;
	public:
		bool IsTraversable() const;
		

	};

}} // hapnmc::dedcom

#endif // DEDCOM_ARENACELL_HPP