#ifndef FTK_REMOTE_CONNECTOR_H
#define FTK_REMOTE_CONNECTOR_H

#include <iostream>
#include <string>
#include <boost/date_time.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <ftk/core.h>


using boost::asio::ip::tcp;

namespace ftk{

class RemoteConnector{
	public:	
	RemoteConnector( int portParameter = 9999 ):m_portNumber(portParameter){};
	//this overload makes the object callable
	void operator()(){ routine(); }	
	
	int routine();
	
	private:
	
	int creaThread();

	int m_portNumber;

};

}

#endif
