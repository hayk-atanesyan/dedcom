#include <boost/asio.hpp>

#include <client.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <command_factory.hpp>
#include <exception>

using boost::asio::ip::tcp;
using namespace hapnmc::dedcom;



int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;

	std::string host{"localhost"};
	std::string port{"51515"};
	if(argc > 2) {
		host = argv[1];
		port = argv[2];
	}
	
	std::cout << "Resolving endpoint..." << std::endl;
	tcp::resolver resolver(io_service);
	auto endpoint_iterator = resolver.resolve({ host.c_str(), port.c_str() });

	std::cout << "Initializing connection client..." << std::endl;
	Client client(io_service, endpoint_iterator);

	boost::asio::io_service::work work_lock(io_service); // keeps io_service up and running waiting for more work
	std::thread t([&io_service](){ io_service.run(); });

	// get ready to process commands
	std::cout << "Initializing command interface..." << std::endl;
	CommandFactory::Init();

	// read next command and pass it to client class which will send it to server
	std::cout << "Input module ready." << std::endl;
	std::string line;
	while(std::getline(std::cin, line)) {
		try {
			if(line.empty()) continue;
			if(line.compare("exit")==0 || line.compare("quit")==0) break;

			// TODO client
			std::string alias;
			std::string params;
			size_t delimpos = line.find(' ');

			if(delimpos == std::string::npos) {
				alias = line;
			} else {
				alias = line.substr(0, delimpos);
				params = line.substr(delimpos + 1);
			}

			if(alias == "help") {

				std::cout << " ----------- " << std::endl;
				CommandFactory::Help(params);
				std::cout << " ----------- " << std::endl << std::endl;

			} else {
				Command c = CommandFactory::Construct(alias, params);

				client.Send(c);
			}

		} catch (std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
			std::cout << " ----------- " << std::endl << std::endl;
		} catch (...) {
			std::cout << "Unknown exception has occured." << std::endl;
			std::cout << " ----------- " << std::endl << std::endl;
		}
	}

	t.join();
	//io_service.run();

	return 0;
}