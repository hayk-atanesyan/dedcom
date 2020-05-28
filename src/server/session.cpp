#include <session.hpp>

#include <iostream>
#include <command_processor.hpp>
#include <string>

namespace hapnmc { namespace dedcom {

	void Session::readCommand()
	{
		//std::cout << "Session ready to read" << std::endl;

	    boost::asio::async_read(_socket, boost::asio::buffer(_hdr_buff, Command::HDR_LEN),
	        [this](boost::system::error_code ec, std::size_t length)
	        {
	        	//std::cout << "Read (header?) len: " + std::to_string(length) << std::endl;
				if (!ec) {
					_last_command.ReadHeader(_hdr_buff);

					//std::cout << "Received header for command " << _last_command.GetType() << std::endl;
					// read context
				    boost::asio::async_read(_socket, boost::asio::buffer(_last_command.GetContextBuffer(), _last_command.GetContextLength()),
				        [this](boost::system::error_code ec, std::size_t /*length*/)
				        {
							if (!ec) {
								//std::cout << "Received body for command " << _last_command.GetType() << std::endl;
								processLastCommand();
								readCommand();
							} else {
								std::cout << "Session::readCommand context read encountered error: "
										<< ec.message() << std::endl;
							}
				        });
				    // end context

				} else {
					std::cout << "Session::readCommand header read encountered error: "
							<< ec.message() << std::endl;
				}
	        });
	}

	void Session::processLastCommand()
	{

		//std::cout << "Received command " << _last_command.GetType() << "|"
		//	<< _last_command.GetContextLength() << std::endl;

		_last_response = CommandProcessor::Process(_last_command);
		//std::cout << "Constructed response: " << _last_response.GetContextBuffer() << std::endl;

		writeResponse();
	}

	void Session::writeResponse()
	{
		_last_response.WriteHeader(_hdr_buff);

		//std::cout << "Sending header buffer: " << _hdr_buff << std::endl;
		boost::asio::async_write(_socket, boost::asio::buffer(_hdr_buff, Command::HDR_LEN),
		[this](boost::system::error_code ec, std::size_t /*length*/){
			if(!ec) {

				//std::cout << "Command header sent." << std::endl;

				boost::asio::async_write(_socket, boost::asio::buffer(_last_response.GetContextBuffer(), _last_response.GetContextLength()),
					[this](boost::system::error_code ec, std::size_t /*length*/){
						if(!ec) {

							//std::cout << "Command context sent: " << _last_response.GetContextBuffer() << std::endl;

						} else {
							std::cout << "Failed to send command context: " << ec.message() << std::endl;
						}
				});

			} else {
				std::cout << "Failed to send command header: " << ec.message() << std::endl;
			}
		});
	}

}} // hapnmc::dedcom