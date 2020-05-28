#include <boost/asio.hpp>

#include <server.hpp>
#include <iostream>
//#include <command_factory.hpp>

using boost::asio::ip::tcp;
using namespace hapnmc::dedcom;

int main(int argc, char* argv[])
{
	//CommandFactory::Init();

	boost::asio::io_service io_service;

	int port = 51515;
	if(argc > 1) port = std::atoi(argv[1]);
	
	tcp::endpoint endp(tcp::v4(), port);

	Server serv(io_service, endp);

	io_service.run();

	return 0;
}