//
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#define MAXBUF 1024

using boost::asio::ip::tcp;

class client {
public:
	client(boost::asio::io_service& io_service, const std::string& server, const std::string& path) :
			resolver_(io_service), socket_(io_service) {


		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		std::ostream request_stream(&request_);
		request_stream << "CIAO \n";

		// Start an asynchronous resolve to translate the server and service names
		// into a list of endpoints.
		tcp::resolver::query query(server, "http");
		resolver_.async_resolve(
				query,
				boost::bind(&client::handle_resolve, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::iterator));
	}

private:


	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator) {

		if (!err) {
			// Attempt a connection to each endpoint in the list until we
			// successfully establish a connection.
			boost::asio::async_connect(
					socket_,
					endpoint_iterator,
					boost::bind(&client::handle_connect, this,
							boost::asio::placeholders::error));
		} else {
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	void handle_connect(const boost::system::error_code& err) {
		if (!err) {
			// The connection was successful. Send the request.
			boost::asio::async_write(
					socket_,
					request_,
					boost::bind(&client::handle_read_content, this,
							boost::asio::placeholders::error));

			std::cout << "fatta la seconda write !!";

		} else {
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	void handle_read_content(const boost::system::error_code& err) {
		if (!err) {
			// Write all of the data that has been read so far.
			std::string myInput(MAXBUF, 'b');

			//routine
			while (true) {

				int readed;
				std::string readed_string;

				try {
					boost::array<char, MAXBUF> myBuf = { { ' ' } };

					readed = socket_.read_some(boost::asio::buffer(myBuf));

					if (readed > 0)
						readed_string.append(std::string(myBuf.data()));

					std::cout
							<< "Console: I have received a partial response, now is: "
							<< readed_string << std::endl;

				} catch (boost::system::system_error) {
					std::cerr << "Console: Error on reading socket " << readed
							<< " bytes" << std::endl;
				}

				std::cout << "> ";
				std::getline(std::cin, myInput);

				if (myInput.compare("exit") != 0
						&& myInput.compare("quit") != 0) {

					boost::asio::write(socket_, boost::asio::buffer(myInput));

				} else {
					std::cout
							<< "\n---------- ftkconsole terminating ------------ \n\n";
					break; // from the for(;;) loop
				}
			}

		} else if (err != boost::asio::error::eof) {
			std::cout << "Error: " << err << "\n";
		}
	}

	tcp::resolver resolver_;
	tcp::socket socket_;
	boost::asio::streambuf request_;
	boost::asio::streambuf response_;
};

int main(int argc, char* argv[]) {

	try {

		if (argc != 3) {
			std::cout << "Usage: async_client <server> <path>\n";
			std::cout << "Example:\n";
			std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
			return 1;
		}

		boost::asio::io_service io_service;
		client c(io_service, argv[1], argv[2]);
		io_service.run();
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << "\n";
	}

	return 0;
}
