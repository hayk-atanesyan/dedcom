#ifndef DEDCOM_COMMANDSTRUCTURES_HPP
#define DEDCOM_COMMANDSTRUCTURES_HPP

#include <command.hpp>
#include <string>
#include <cinttypes>


namespace hapnmc { namespace dedcom {

	/**
	* Stores data for a specific command type, base class.
	*/
	class CommandStructure
	{
	public:
		/** read data from a params text likely provided by user and store it */
		virtual void Read(const std::string& params) = 0;
		/** read data from a command and store it */
		virtual void Read(const Command& c) = 0;

		/** write stored data into command context */
		virtual void FillContext(Command& c) = 0;

		void FillContextFromString(Command& c, const std::string& params) 
		{
			Read(params);
			FillContext(c);
		}
	};

	/** no params empty context */
	class CSNoparam : public CommandStructure
	{
	public:
		void Read(const std::string& params) override {}
		void Read(const Command& c) override {}
		void FillContext(Command& c) override {}
	};

	/** context only stores text */
	class CSText : public CommandStructure
	{
	private:
		std::string _txt;
	public:
		void Read(const std::string& params) override { _txt = params; }
		void Read(const Command& c) override { _txt = c.GetContextBufferConst(); }
		void FillContext(Command& c) override;

		std::string GetTxt() const { return _txt; }
	};

	/** context only stores a single standartised unsigned integer */
	class CSUInt : public CommandStructure
	{
	private:
		uint32_t _number;
	public:
		void Read(const std::string& params) override;
		void Read(const Command& c) override;
		void FillContext(Command& c) override;

		uint32_t GetNumber() const { return _number; }
	};

}} // hapnmc::dedcom


#endif // DEDCOM_COMMANDSTRUCTURES_HPP