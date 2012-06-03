
# include <sstream>
#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "request_handler.hpp"


namespace http {
namespace server3 {

connection::connection( boost::asio::io_service& io_service ) : strand_(io_service), socket_(io_service){ }

boost::asio::ip::tcp::socket& connection::socket() { return socket_; }

void connection::start() {


	std::cout << "Reading ... \n";

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

			std::cout << "Received " <<  buffer_.data() << " !\n";

			std::stringstream prova;

			prova << buffer_.data() << buffer_.data();

			std::cout << "Sending " <<  prova.str() << " !\n";


			boost::asio::async_write(
										socket_,
										boost::asio::buffer( prova.str() ),
										strand_.wrap(
														boost::bind(
																	&connection::handle_write,
																	shared_from_this(),
																	boost::asio::placeholders::error
																	)
													)
									);


	}

}

void connection::handle_write(const boost::system::error_code& e) {

	if (e)
		std::cerr << "ERRORE !!!!!!" ;
	else
	{


		std::cout << "Reading, buffer... :" << buffer_.data()  << ":";


		boost::array<char, 8192> newArray  = {{'\0'}};

		buffer_ = newArray;

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
}


void connection::close(const boost::system::error_code& e) {
	if (!e) {
		// Initiate graceful connection closure.
		boost::system::error_code ignored_ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
				ignored_ec);
	}

}

} // namespace server3
} // namespace http
