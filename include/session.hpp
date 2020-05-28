#ifndef DEDCOM_SESSION_HPP
#define DEDCOM_SESSION_HPP

#include <utility>
#include <command.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <exception>

using boost::asio::ip::tcp;

namespace hapnmc { namespace dedcom {

	/** 
	 *	Responsible for managing communications with a single client.
	 *  
	 */
	class Session
	{
	private:
		tcp::socket _socket;
		Command _last_command;
		Command _last_response;
		char _hdr_buff[Command::HDR_LEN];

		/** read a single command and pass it to be processed */
		void readCommand();

		/** send the response to last command back to the client in command form */
		void writeResponse();

		void processLastCommand();
	public:
		Session(tcp::socket socket)
		: _socket(std::move(socket))
		{
			std::cout << "Session created reading commands..." << std::endl;
			
			try {
				readCommand();
			} catch (std::exception& exc) {
				std::cout << exc.what() << std::endl;
			}
		}

	};

}} // hapnmc::dedcom

#endif // DEDCOM_SESSION_HPP