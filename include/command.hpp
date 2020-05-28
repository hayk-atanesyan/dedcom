#ifndef DEDCOM_COMMAND_HPP
#define DEDCOM_COMMAND_HPP

#include <cinttypes>
#include <boost/asio.hpp>

namespace hapnmc { namespace dedcom {

	/** 
	 * Stores information about a single command including type and context.
	 */
	class Command
	{
	public:

		static const uint32_t MAX_CTX_LEN = 1024;
		static const uint32_t HDR_LEN = 8;

		static uint32_t AvailableStorage() { return MAX_CTX_LEN - HDR_LEN; }

		enum Type: uint32_t {
			Status, // reports actual state and overall info
			ServerStatus,
			ExplorationStatus,
			EncounterStatus,
			EncounterTurnReport,

			ExploreOption, // explore a specific exploration option

			EventInfo, // info on playerchoice
			EventResolve, // one of the choices are selected and the effect applied

			PartyStatus,
			UnitInfo, // used to get info on a drone or on one of the targets
			ProtocolInfo, // get info on action from one of the drones

			AlterProtocol, // used to change drone behavior
			CommitProtocol, // used to end turn allowing engine to resolve an necounter turn

			Text, // command type used simply to send text

			UintReport, // versatile command used for simple reports like yes/no/amount/index

			NC // null command basically
		};

		Command():
		_type(Type::NC),
		_context_length(0)
		{}
		Command(Command::Type t):
		_type(t),
		_context_length(0)
		{}

		void SetType(Command::Type t) { _type = t; }
		void SetContextLength(uint32_t len) { _context_length = len; }
		Command::Type GetType() { return _type; }
		uint32_t GetContextLength() { return _context_length; }

		void ReadHeader(char* hdr);
		void WriteHeader(char* hdr);

		char* GetContextBuffer() { return _context; }
		const char* GetContextBufferConst() const { return _context; }

	private:

		Command::Type _type;
		uint32_t _context_length;
		char _context[MAX_CTX_LEN];
	};

}} // hapnmc::dedcom

#endif // DEDCOM_COMMAND_HPP