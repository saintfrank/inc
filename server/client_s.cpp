#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <json/json.h>

//#define DEBUG_SOCKET

#define MAXBUF 4096

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {

		// Checking parameters
		if (argc != 3) {
			std::cout << "Usage: sync_client <server> <port>\n";
			std::cout << "Example:\n";
			std::cout << "  sync_client www.boost.org /LICENSE_1_0.txt\n";
			return 1;
		}

		// Asio Objects
		boost::asio::io_service io_service;
		tcp::resolver	resolver(io_service);
		tcp::socket		socket  (io_service);


		////////////////////////////////////////////
		///////         Connecting to the server
		try{


			tcp::resolver::query query(argv[1], argv[2]);
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			// Try each endpoint until we successfully establish a connection.
			boost::asio::connect(socket, endpoint_iterator);
			std::cout   <<  "---------------------------------------------------------"<< std::endl
						<< "+ Connected to host " << argv[1] << ", port " << argv[2] << std::endl
						<<  "---------------------------------------------------------"<< std::endl;

		}
		catch(...)
		{
			std::cout   <<  "----------------------------------------------------------------------------" << std::endl
						<< "- Impossible to connect to host. Please verify hostname and port provided. "   << std::endl
						<<  "----------------------------------------------------------------------------" << std::endl;
			return 1;
		}
		///////			End of server connection
		////////////////////////////////////////////


		std::string myInput(MAXBUF, 'b');
		size_t readed = 0;

		// The loop routine
		while (true)
		{


			////////////////////       Reading line and sending the command through the socket
			//
			std::cout << "> " ;
			std::getline ( std::cin, myInput);

			// If exit words are found, then returning
			if( myInput.compare("exit") == 0 || myInput.compare("quit") == 0)
			{
				std::cout << "\n---------- ftkconsole terminating ------------ \n\n";
				return 0;  // from the for(;;) loop
			}

			boost::system::error_code myWriteError;

			boost::asio::write( socket, boost::asio::buffer( myInput ), myWriteError );

			#ifdef DEBUG_SOCKET
				std::cout << "Thread: i've sent Input line:\n"<< myInput << "\nof size: "<< myInput.size() << "\n\n" << std::endl;
			#endif

			if (myWriteError)
			{
				std::cerr << "\n---------- error : found error writing socket ------------ \n\n";
				return 0;
			}
			//
			//////////////////// 	End of Line Read and write




			////////////////////       Reading Response from socket
			//
			std::string readed_string;

			boost::system::error_code myReadError;

			do{ // Reading loop. Performed until the socket will be entirely red.

				// resetting the counter
				readed = 0;

				boost::array<char, MAXBUF> myBuf = {{' '}};

				readed = socket.read_some(boost::asio::buffer(myBuf, MAXBUF));

				if ( readed > 0 )
					readed_string.append( std::string(myBuf.data()) );

				#ifdef DEBUG_SOCKET
					std::cerr << "Received "<< readed << " bytes, still" << socket.available() << " to go" << "\n" << std::endl;
				#endif

			} while ( socket.available() > 0 );  // If there are other information to read we iterate
			//
			//////////////////// 	End of read from socket




			////////////////////    Composing the Json responde from the received data.
			//
			std::stringstream myStream (readed_string);

			Json::Value root;   // will contain the root value after parsing.
			Json::Reader reader;

			bool parsingSuccessful = reader.parse( myStream , root, false );
			if ( !parsingSuccessful )
			{
				// report to the user the failure and their locations in the document.
				std::cout  << "Failed to parse configuration" << std::endl << reader.getFormatedErrorMessages() << std::endl;
				std::cout << " String returned: \n" << readed_string;
				return 1 ;
			}

			// Printing the result
			std::cout << root;
			//
			/////////////////////    End of composing Json

		} // End of Routine while loop

		return 0;
}
