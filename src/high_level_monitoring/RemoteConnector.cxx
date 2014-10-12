////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <ftk/RemoteConnector.h>
#include <ftk/RemoteThread.h>

using namespace std;
using namespace ftk;
using boost::asio::ip::tcp;

/** Crea un nuovo socket e un nuovo thread e ritorna la porta del socket creato
*
*/
int RemoteConnector::creaThread(){

    // a new socket newAcceptor is created, with port 0, that means that the operating system will assign an available port
    boost::shared_ptr<boost::asio::io_service> newIoService ( new boost::asio::io_service() );

    boost::shared_ptr<tcp::acceptor> newAcceptor ( new tcp::acceptor(*newIoService, tcp::endpoint(tcp::v4(), 0)) );

    // retrieving the new port number to send back to the apache server
    int port = newAcceptor->local_endpoint().port();
    #ifdef DEBUG_CONNECTIONS
    	cout << "+ RemoteConnector : new socket has port: " << port << endl;
    #endif
    RemoteThread myth(newAcceptor, newIoService);

	  //it actually makes a copy
    boost::thread runningThread(myth);

    return port;

}

int RemoteConnector::routine()
{
  try
  {
    // a listening socket is opened at port 9999
    boost::asio::io_service listeningIoService;
    tcp::acceptor listeningAcceptor(listeningIoService, tcp::endpoint(tcp::v4(), m_portNumber));

    cout << "---------   RemoteConnector: waiting for connections at port " << m_portNumber << "   -----\n\n" ;

    while(true)
    {
      // accept the requestform the Apache server
      tcp::socket socket(listeningIoService);
      listeningAcceptor.accept(socket);

      #ifdef DEBUG_CONNECTIONS
      	  cout << "+ RemoteConnector : connection received, creating new socket " << endl;
      #endif

      // Ovviamente dovrˆ tracciare a breve questi thread creati
      int newSocketPort = creaThread();

      #ifdef DEBUG_CONNECTIONS
      	  std::cout << "+ RemoteConnector : created socket at port : " << newSocketPort << std::endl;
      #endif

      // conversione di un intero in una stringa
      std::stringstream tempStream;
      tempStream << newSocketPort;
      std::string returnMessage = tempStream.str();

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(returnMessage),boost::asio::transfer_all(), ignored_error);

      socket.close();

      #ifdef DEBUG_CONNECTIONS
      	  std::cout << "RemoteConnector:  port sent to pspserver" << std::endl;
      #endif

    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

