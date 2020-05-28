#ifndef DEDCOM_CLIENT_HPP
#define DEDCOM_CLIENT_HPP

#include <boost/asio.hpp>
#include <command.hpp>

using boost::asio::ip::tcp;

namespace hapnmc { namespace dedcom {

	/** 
	 *	Responsible for managing connection to server.
	 *  
	 */
	class Client
	{
	private:
		boost::asio::io_service& _io_service;
		tcp::socket _socket;
		Command _last_command;
		Command _last_response;
		char _hdr_buff[Command::HDR_LEN];

		void connect(tcp::resolver::iterator endpoint_iterator);

		void processResponse();

	public:
		Client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
		: _io_service(io_service), _socket(io_service)
		{
			connect(endpoint_iterator);
		}

		void Send(Command& c);
		void ReadResponse();
	};

}} // hapnmc::dedcom

#endif // DEDCOM_CLIENT_HPP