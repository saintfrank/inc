////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

//#include <ftk/Configuration.h>
#include <ftk/RemoteConnector.h>

#include <list>
#include <boost/thread.hpp>
#include <boost/ref.hpp>

#define DEBUG

using namespace ftk;
using boost::asio::ip::tcp;

int main(int argc, char **argv)
{
  try
  { 

	if (argc != 3)
	{
	   std::cerr << "Usage: ftk_monitor_server <cratename> <port_number>\n";
	        return 1;
	}

	// Retrieving the crate name
	std::string crateName = argv[1];

	// Retrieving the port number
	int port = -1;
	std::stringstream ss;
	ss << argv[2];
	ss >> port;

	std::cout << std::endl <<"---------   Starting Crate " << crateName << " port " << port << "   -----------------------------" << std::endl;
    std::cout << std::endl <<"---------   Initializer: Configuration   --------------------------------" << std::endl;

    /** *****************************************
     *
     *  Reading the monitors needed from the configuration file
     *
     *****************************************
	
	ftk::FtkConfiguration * myConfiguration = ftk::FtkConfiguration::getInstance();
	if ( ! myConfiguration -> start( crateName, true ) )
	{
		std::cerr << "\n\n\n###### Failed to initialize partition ###### \n\n\n" << std::endl;
		return false ;
	}

	std::cout << "The root is :\n" << myConfiguration->getAllJson() << std::endl;

	FtkTimer * myTimer = myConfiguration->getTimer() ;

	if(myTimer == 0)
	{
		std::cerr << "\n\n\n Timer pointer is null \n\n\n" << std::endl;
		return false ;
	}

	boost::thread timerThread( boost::ref(*myTimer) );
*/
	RemoteConnector server(port);

	boost::thread runningRemoteConnector(boost::ref(server));

	std::cout << "---------   Initializer: Stoppo i Monitors   ----------------------------" << std::endl;
/*
	if ( myConfiguration->stopMonitor("StatusMonitor1") )
		std::cout << "---------   Initializer: Stoppo IS OK   -------------------" << std::endl;
	else
		std::cout << "---------   Initializer: Stoppo IS FALSE   -------------------" << std::endl;

	timerThread.join();*/
	runningRemoteConnector.join();
    
  }
  catch ( std::exception& e)
  {
    std::cerr << "- initializer Error:" << e.what() << std::endl;
  } 
  
  return 0;
}
