/** @author Francesco Cervigni
 *
 */

#include <boost/asio.hpp>
#include "ftk/Console.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace ftk;

int main(int argc, char **argv){

try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: ftk_console <host> <port>\n";
      return 1;
    }

    std::cout << std::endl << "--------------- Connecting to the Host --------------- "<< std::endl;

    boost::asio::io_service myIoService;
    boost::asio::ip::tcp::socket mySocket(myIoService);
    
    tcp::resolver resolver(myIoService);
    tcp::resolver::query query(argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query); 
    tcp::resolver::iterator end;
    

    if(iterator != end)
	{
		// per ora prendo la prima degi host risolti, a logica dovrà essere solo uno.
        boost::asio::ip::tcp::endpoint endpoint = *iterator;
    	std::cout << "+ The resolved resolved is: "<< endpoint << std::endl;
		mySocket.connect(endpoint);
	}
	else
	{
	    std::cout << "- Host not found. " << std::endl;
	}

    //  ricevi porta
  
    boost::array<char, 10> mybuf= {{' '}};
		    
    boost::system::error_code error;
		
    size_t len = mySocket.read_some(boost::asio::buffer(mybuf), error);

    if (error == boost::asio::error::eof)
    {
		std::cerr << " found eof on socket " << std::endl;
		// Connection closed cleanly by peer.
    }
    else if (error)
    {
       	std::cerr << "- Console_init : found error reading socket " << std::endl;
       	throw boost::system::system_error(error); // Some other error.
    }
            
    if( len == 0 )
		std::cout << "- ftkConsole: ERROR: Thread: Received empty port number: " << std::endl;     
    else
		std::cout << "+ The FTK RemoteConnector has retruned the port: " << mybuf.data() << std::endl;     

	
    mySocket.close();

    // porta ricevuta, connessione al Thread Personale

    tcp::resolver myResolver(myIoService);
    tcp::resolver::query myQuery(argv[1], mybuf.data()); // todo cambiare costruttore
    tcp::resolver::iterator myIterator = resolver.resolve(myQuery); 
    
    if(myIterator != end)
	{
		// per ora prendo la prima dei host risolti, a logica dovrà essere solo uno.
        boost::asio::ip::tcp::endpoint endpoint = *myIterator;
    	mySocket.connect(endpoint);
		std::cout << "+ Connected to assigned host service: " << endpoint << std::endl
        <<  "------------------------------------------------------"<< std::endl;
	}
	else
	{
	    std::cout << "- Assigned Host not found. " << std::endl;
	}

	#ifdef DEBUG_CONSOLE
    std::cerr << "Starting The console" << std::endl ;
	#endif

	// Starting the console routine
    FtkConsole myCons(myIoService, mySocket);
    myCons.start();

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl ;
  }


return 0;
}
