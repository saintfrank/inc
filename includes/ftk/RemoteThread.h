#ifndef FTK_REMOTE_THREAD_H
#define FTK_REMOTE_THREAD_H

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include "ftk/core.h"

using boost::asio::ip::tcp;

namespace ftk{

class RemoteThread{
	
	public:

	RemoteThread(boost::shared_ptr<tcp::acceptor> paramAcceptor, boost::shared_ptr<boost::asio::io_service> paramIoServ):m_OwnAcceptor(paramAcceptor), m_OwnIoService(paramIoServ){ }

	~RemoteThread(){}

	void operator()() { run(); }
	
	void run();

	private:
	boost::shared_ptr<tcp::acceptor> m_OwnAcceptor;
	boost::shared_ptr<boost::asio::io_service> m_OwnIoService;
	
};

}

#endif
