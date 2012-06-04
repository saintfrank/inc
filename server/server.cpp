#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

#define DEBUG_SOCKET

namespace http {
namespace server3 {

server::server(const std::string& address, const std::string& port, std::size_t thread_pool_size) :
		thread_pool_size_(thread_pool_size), signals_(io_service_), acceptor_(
				io_service_), new_connection_(){
	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
	signals_.add(SIGINT);
	signals_.add(SIGTERM);

	#if defined(SIGQUIT)
		signals_.add(SIGQUIT);
	#endif // defined(SIGQUIT)

	signals_.async_wait(boost::bind(&server::handle_stop, this));

	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	start_accept();
}

void server::run() {
	// Create a pool of threads to run all of the io_services.
	std::vector < boost::shared_ptr<boost::thread> > threads;

	for (std::size_t i = 0; i < thread_pool_size_; ++i) {

		boost::shared_ptr < boost::thread > thread(
													new boost::thread(
																		boost::bind(
																				&boost::asio::io_service::run,
																				&io_service_)
																	 )
												  );

		threads.push_back(thread);
	}

	// Wait for all threads in the pool to exit.
	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();
}

void server::start_accept() {
    
	new_connection_.reset(new connection(io_service_));

	#ifdef DEBUG_SOCKET
		std::cout << "\n---- Waiting for connections ----" << std::endl;
	#endif

	acceptor_.async_accept(
							new_connection_->socket(),
							boost::bind(
										&server::handle_accept,
										this,
										boost::asio::placeholders::error
										)
							);

}

void server::handle_accept(const boost::system::error_code& e) {

	if (!e)
	{
		new_connection_->start();
	}

	start_accept();
}

void server::handle_stop() {

	io_service_.stop();

}

} // namespace server3
} // namespace http





int main(int argc, char* argv[]) {
	try {
		// Check command line arguments.
		if (argc != 4) {
			std::cerr
					<< "Usage: http_server <address> <port> <threads>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    receiver 0.0.0.0 80 1 \n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    receiver 0::0 80 1 \n";
			return 1;
		}

		// Initialise the server.
		std::size_t num_threads = boost::lexical_cast < std::size_t > (argv[3]);
		http::server3::server s(argv[1], argv[2], num_threads);

		// Run the server until stopped.
		s.run();
	} catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}

