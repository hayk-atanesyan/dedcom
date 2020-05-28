#include <command_structures.hpp>

namespace hapnmc { namespace dedcom {

	void CSText::FillContext(Command& c)
	{
		std::size_t len = _txt.copy(c.GetContextBuffer(), _txt.length());
		c.GetContextBuffer()[len] = '\0';
		c.SetContextLength(len+1);
	}

	// ---------------------------------------------

	void CSUInt::Read(const std::string& params)
	{
		_number = std::stoul(params);
	}
	void CSUInt::Read(const Command& c)
	{
		uint32_t num;
		std::memcpy(&num, c.GetContextBufferConst(), sizeof(uint32_t));
		_number = ntohl(num);
	}
	void CSUInt::FillContext(Command& c)
	{
		uint32_t num = htonl(_number);
		std::memcpy(c.GetContextBuffer(), &num, sizeof(uint32_t));
		c.SetContextLength(sizeof(uint32_t));
	}

	// ---------------------------------------------

	

}} // hapnmc::dedcom