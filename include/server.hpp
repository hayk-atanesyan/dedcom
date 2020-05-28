#ifndef DEDCOM_SERVER_HPP
#define DEDCOM_SERVER_HPP

#include <iostream>
#include <session.hpp>

#include <boost/asio.hpp>

#include <list>
#include <memory>
#include <dedcom_engine.hpp>

using boost::asio::ip::tcp;

namespace hapnmc { namespace dedcom {

	/** 
	 *	Responsible for managing connections from clients.
	 *  
	 */
	class Server
	{
	private:
		tcp::acceptor _acceptor;
  		tcp::socket _socket;

  		std::list< std::shared_ptr<Session> > _sessions;

  		/** Wait for a connection then pass it to the Session class for processing. */
  		void acceptConnection()
  		{
		    _acceptor.async_accept(_socket,
		        [this](boost::system::error_code ec)
		        {
					if (!ec)
					{
						std::cout << "Client connected, creating a session..." << std::endl;

						std::shared_ptr<Session> s = std::make_shared<Session>(std::move(_socket));

						//Session s(std::move(_socket)); // TODO maybe keep track / make shared?
						_sessions.push_back(s);

						/*if(_sessions.size() == 1) {
							std::cout << "Re-checking dedcom engine initialization." << std::endl;
							DedcomEngine::Instance();
						}*/

					} else {
						std::cout << "Server::acceptConnection async_accept encountered error: "
							<< ec.message() << std::endl;
					}

					acceptConnection();
		        });
  		}

	public:
		Server(boost::asio::io_service& io_service, const tcp::endpoint& endpoint)
		: _acceptor(io_service, endpoint), _socket(io_service)
		{
			std::cout << "Re-checking dedcom engine initialization." << std::endl;
			DedcomEngine::Instance().Init();
			std::cout << "Server up, waiting for connections..." << std::endl;
			acceptConnection();
		}

		int GetSessionCount() { return _sessions.size(); }

	};

}} // hapnmc::dedcom

#endif // DEDCOM_SERVER_HPP