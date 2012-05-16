#ifndef FTK_CONSOLE_H
#define FTK_CONSOLE_H

#include <boost/asio.hpp>

#include "ftk/core.h"

#include <string>

using boost::asio::ip::tcp;

namespace ftk{

class FtkConsole{
	
	public:

	FtkConsole(boost::asio::io_service& ioParam, boost::asio::ip::tcp::socket& sockParam)
	: m_IoService(ioParam), m_MySocket(sockParam){ }

	void start();

	~FtkConsole(){}
	
	private:
	
	boost::asio::io_service &m_IoService;
    boost::asio::ip::tcp::socket &m_MySocket;
	
};
}
#endif
