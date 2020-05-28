#include <command.hpp>
#include <cstring>


namespace hapnmc { namespace dedcom {

	void Command::WriteHeader(char* hdr)
	{
		uint32_t type_int = _type;
		uint32_t type_int_n = htonl(type_int);
		uint32_t clen_n = htonl(_context_length);

		std::memcpy(hdr, &type_int_n, sizeof(uint32_t));
		std::memcpy(hdr + sizeof(uint32_t), &clen_n, sizeof(uint32_t));
	}
	void Command::ReadHeader(char* hdr)
	{
		uint32_t type_int_n;
		uint32_t clen_n;

		std::memcpy(&type_int_n, hdr, sizeof(uint32_t));
		std::memcpy(&clen_n, hdr + sizeof(uint32_t), sizeof(uint32_t));

		_type = static_cast<Command::Type>(ntohl(type_int_n));
		_context_length = ntohl(clen_n);
	}


}} // hapnmc::dedcom