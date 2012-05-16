////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <boost/asio.hpp>
#include <cstdlib>
#include "ftk/Console.h"

#include "json/json.h"

//#define DEBUG_SOCKET

#define MAXBUF 10000000

#define DEBUG_CONSOLE
#define DEBUG_SOCKET

namespace ftk{

using boost::asio::ip::tcp;

void FtkConsole::start(){

	#ifdef DEBUG_CONSOLE
		std::cerr << "Inside Routine" << std::endl;
	#endif

	std::string myInput(MAXBUF, 'b');

	try{

		//routine
		while(true)
		{

			std::cout << "> " ;
			std::getline ( std::cin, myInput);

			if( myInput.compare("exit")!= 0 && myInput.compare("quit") != 0){


				boost::asio::write( m_MySocket, boost::asio::buffer( myInput ) );

				/*

				#ifdef DEBUG_SOCKET
					std::cout << "Thread: i've sent Input line "<< myInput << " of size "<< myInput.size() <<" now waiting for the response: " << std::endl;
				#endif

				// waiting for messages

				boost::system::error_code myError;

				std::string readed_string;

				int readed = 0;

				do
				{
					// resetting the counter
					readed = 0;

					try
					{

						boost::array<char, MAXBUF> myBuf = {{' '}};

						readed = m_MySocket.read_some( boost::asio::buffer(myBuf), myError );

						if ( readed > 0 )
							readed_string.append( std::string(myBuf.data()) );

						#ifdef DEBUG_SOCKET
							std::cout << "Console: I have received a partial response, now is: " << readed_string << std::endl;
						#endif

					}
					catch (boost::system::system_error)
					{
						std::cerr << "Console: Error on reading socket "<< readed  << " bytes" << std::endl;
					}

					#ifdef DEBUG_SOCKET
						std::cerr << "Console: Received "<< readed << " bytes" << std::endl;
					#endif

					if (myError == boost::asio::error::eof)
					{
						std::cerr << "ERROR: found eof on socket, connection may have been closed " << std::endl;
						break; // Connection closed cleanly by peer.
					}
					else if (myError)
					{
						std::cerr << "- FtkConsole : found error reading socket " << std::endl;
						break;
					}

				} while ( readed == 16384 );  // 16384 is the number of characters of a full buffer

				std::stringstream myStream;
				myStream << readed_string ;

				Json::Value root;   // will contain the root value after parsing.
				Json::Reader reader;

				bool parsingSuccessful = reader.parse( myStream , root, false );
				if ( !parsingSuccessful )
				{
					// report to the user the failure and their locations in the document.
					std::cout  << "Failed to parse configuration" << std::endl << reader.getFormatedErrorMessages() << std::endl;
					std::cout << " String returned: " << std::endl << myStream.str();
					//return ;
				}

				// Printing the result
				std::cout << root;
				*/
			}
			else
			{
				std::cout << "\n---------- ftkconsole terminating ------------ \n\n";
				break;  // from the for(;;) loop
			}
		}

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
  
}// routine

}// namespace

