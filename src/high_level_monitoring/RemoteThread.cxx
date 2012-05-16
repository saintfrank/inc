////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <ftk/RemoteThread.h>
//#include <ftk/Interpreter.h>
#include "json/json.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

#define LOGGING_SOCKET

#define DEBUG_CONSOLE
#define DEBUG_SOCKET

using namespace std;
using namespace ftk;
using boost::asio::ip::tcp;

//#define DEBUG_SOCKET

#define MAXBUF 1000000

void RemoteThread::run()  
{  
    
    //FtkInterpreter inter;
    
        	          	    
	while(true){
		
		#ifdef DEBUG_SOCKET	
		std::cout << "+  Remote Thread: waiting for connections: " << std::endl;    
		#endif

		tcp::socket newSocket(*m_OwnIoService);
	    m_OwnAcceptor->accept(newSocket);
				
		for(;;)
		{		    		
			// waiting for messages


			#ifdef LOGGING_SOCKET
			std::cout << "+  Remote Thread: Waiting for messages " << std::endl;
			#endif

			boost::array<char, MAXBUF> myBuf = {{' '}};
		    
      		boost::system::error_code error;

			try
			{
				
				#ifdef LOGGING_SOCKET
					std::cerr << "+ RemoteThread : receiving :" <<  std::endl;
				#endif

				size_t len = newSocket.read_some(boost::asio::buffer(myBuf), error);

				#ifdef LOGGING_SOCKET
					std::cerr << "+ RemoteThread : received :" << myBuf.data() <<  std::endl;
				#endif


				if (error == boost::asio::error::eof)
				{
					cerr << "- Remote Thread : terminating. connection closed by the client" << endl;
					break; // Connection closed cleanly by peer.
				}
				else if (error)
				{
					cerr << "- Remote Thread : terminating. found error reading socket " << endl;
				}

				if( len == 0 )
				{
					#ifdef DEBUG_SOCKET
						std::cout << "+ Remote Thread: ERROR: Thread: Received empty string from ftkserver: " << std::endl;
					#endif
				}

			}
			catch (...)
			{
				#ifdef DEBUG_SOCKET
					std::cout << "+ Remote Thread: ERROR: Found exception on socket: " << std::endl;
					break;
				#endif

			}
			
			// interpreting the message
			#ifdef LOGGING_SOCKET
				std::cerr << "+ RemoteThread : received :" << myBuf.data() <<  std::endl;
			#endif

			#ifdef DEBUG_SOCKET
				std::cerr << "+ RemoteThread: Sending the response: " << std::endl;
			#endif

			std::string str(myBuf.data());

			Json::StyledWriter writer;
			//TODO rimettere :    std::string message = writer.write( inter.interpret(str) );
			std::string message = "risposta";

			#ifdef LOGGING_SOCKET
    		std::cerr << " Sending " << (message.size() * sizeof(char) ) << " bytes"  << std::endl;
			#endif

    		// sending the answer
    		try
    		{
    		    boost::asio::write(newSocket, boost::asio::buffer(message));
    		}
    		catch ( std::exception e )
    		{
    			std::cerr << " Remote Thread: found Error on socket write:" <<  e.what() ;
    		}


	    }
	}        	  
} 
