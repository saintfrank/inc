//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
# include <sstream>


#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "request_handler.hpp"



namespace http {
namespace server3 {

connection::connection(boost::asio::io_service& io_service) :
		strand_(io_service), socket_(io_service){
}

boost::asio::ip::tcp::socket& connection::socket() {
	return socket_;
}

void connection::start() {


	std::cout << "Reading ... !\n";

	socket_.async_read_some(

			boost::asio::buffer(buffer_),
			strand_.wrap(
							boost::bind (
											&connection::handle_read, shared_from_this(),
											boost::asio::placeholders::error,
											boost::asio::placeholders::bytes_transferred
										)
						)
			);
}

void connection::handle_read( const boost::system::error_code& e, std::size_t bytes_transferred ) {

	if (!e) {

		//boost::tribool result;

		//boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
		//		request_, buffer_.data(), buffer_.data() + bytes_transferred);



			std::stringstream prova;

			std::cout << "Now received " <<  buffer_.data() << " !\n";

			prova << buffer_.data() << buffer_.data();



			boost::asio::async_write(
										socket_,
										boost::asio::buffer(prova.str()),
										strand_.wrap(
														boost::bind(
																	&connection::handle_write,
																	shared_from_this(),
																	boost::asio::placeholders::error
																	)
													)
									);


			handle_read( e, bytes_transferred );

		//}else
		//	std::cerr << "ERRORE !!!" ;

	}

	// If an error occurs then no new asynchronous operations are started. This
	// means that all shared_ptr references to the connection object will
	// disappear and the object will be destroyed automatically after this
	// handler returns. The connection class's destructor closes the socket.
}

void connection::handle_write(const boost::system::error_code& e) {
	if (!e) {
		// Initiate graceful connection closure.
		boost::system::error_code ignored_ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
				ignored_ec);
	}

	// No new asynchronous operations are started. This means that all shared_ptr
	// references to the connection object will disappear and the object will be
	// destroyed automatically after this handler returns. The connection class's
	// destructor closes the socket.
}

} // namespace server3
} // namespace http
