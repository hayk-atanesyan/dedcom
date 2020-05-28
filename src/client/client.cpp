#include <client.hpp>
#include <iostream>
#include <client_command_processor.hpp>
#include <string>

namespace hapnmc { namespace dedcom {

	void Client::connect(tcp::resolver::iterator endpoint_iterator)
	{
	    boost::asio::async_connect(_socket, endpoint_iterator,
	        [this](boost::system::error_code ec, tcp::resolver::iterator)
	        {
				if (!ec) {
					std::cout << "Connected to server." << std::endl;
					std::cout << "=== DEDCOM - Deep Exploration Drone Command ===" << std::endl
						<< "Damage report: primary connection failed, sensors offline." << std::endl
						<< "Fallback interface activated - type help for the list of commands. Awaiting operator input..." << std::endl;
				} else {
					std::cout << "Client::connect async_connect encountered error: "
										<< ec.message() << std::endl;
				}
	        });
	}

	void Client::Send(Command& c)
	{
		_last_command = c;
		_last_command.WriteHeader(_hdr_buff);

		_io_service.post([this](){
			//std::cout << "Sending header buffer: " << _hdr_buff << std::endl;
			boost::asio::async_write(_socket, boost::asio::buffer(_hdr_buff, Command::HDR_LEN),
			[this](boost::system::error_code ec, std::size_t length){
				if(!ec) {

					//std::cout << "Command header sent, len/ctx len: " + std::to_string(length) + "/" + std::to_string(_last_command.GetContextLength()) << std::endl;

					boost::asio::async_write(_socket, boost::asio::buffer(_last_command.GetContextBuffer(), _last_command.GetContextLength()),
						[this](boost::system::error_code ec, std::size_t /*length*/){
							if(!ec) {

								//std::cout << "Command context sent." << std::endl;
								ReadResponse();

							} else {
								std::cout << "Failed to send command context: " << ec.message() << std::endl;
							}
					});

				} else {
					std::cout << "Failed to send command header: " << ec.message() << std::endl;
				}
			});
		});
	}

	void Client::ReadResponse()
	{
	    boost::asio::async_read(_socket, boost::asio::buffer(_hdr_buff, Command::HDR_LEN),
	        [this](boost::system::error_code ec, std::size_t /*length*/)
	        {
				if (!ec) {
					_last_response.ReadHeader(_hdr_buff);

					//std::cout << "Received header for command " << _last_response.GetType() << std::endl;
					// read context
				    boost::asio::async_read(_socket, boost::asio::buffer(_last_response.GetContextBuffer(), _last_response.GetContextLength()),
				        [this](boost::system::error_code ec, std::size_t /*length*/)
				        {
							if (!ec) {
								//std::cout << "Received body for command " << _last_response.GetType() << std::endl;
								processResponse();
							} else {
								std::cout << "Client::ReadResponse context read encountered error: "
										<< ec.message() << std::endl;
								std::cout << "  ------------  " << std::endl << std::endl;
							}
				        });
				    // end context

				} else {
					std::cout << "Client::ReadResponse header read encountered error: "
							<< ec.message() << std::endl;
					std::cout << "  ------------  " << std::endl << std::endl;
				}
	        });
	}

	void Client::processResponse()
	{
		ClientCommandProcessor::Process(_last_response);

		std::cout << std::endl << "  ------------  " << std::endl << std::endl;
	}

}} // hapnmc::dedcom