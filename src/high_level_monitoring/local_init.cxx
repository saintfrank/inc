////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <list>
#include <ftk/RemoteConnector.h>
#include <boost/thread.hpp>

using namespace std;
using boost::asio::ip::tcp;
using namespace ftk;

int main()
{
  try
  { 
    // starting the Remote Connector
    RemoteConnector server;

    cout << "----- Initializer: RemoteConnector Thread created --------" << endl;  

    boost::thread runningRemoteConnector(server);

      
    //join 
    runningRemoteConnector.join();
    
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
  }

  return 0;
}
