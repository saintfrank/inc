#ifndef FTK_TIMER_H
#define FTK_TIMER_H

#include <iostream>
#include <list>
#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

#include <ftk/Monitor.h>
#include <ftk/core.h>

namespace ftk{

class FtkTimer{

	public:

	FtkTimer(boost::asio::io_service&, std::list<FtkMonitor*>);

	~FtkTimer(){ 
	/*
	std::map 	< boost::posix_time::time_duration , 
				std::pair < 
					boost::asio::deadline_timer*, 
					std::list < FtkMonitor* > * 
					> *
				>::iterator iter;   
	for( iter = timersMap.begin(); iter != timersMap.end(); iter++ ) 
		{
		//TODO : debuggare il distruttore	
		delete iter->second->first; 	// deleting the timer
		std::list<FtkMonitor*>* monitorsList= iter->second->second; 	//
		
		// deleting all the monitors from the list
		std::list<FtkMonitor*>::iterator monitorIterator;
		for(monitorIterator=monitorsList->begin(); monitorIterator != monitorsList->end(); ++monitorIterator) 
			delete *monitorIterator ;	// deleting every monitor
		
		delete iter->second->second; 	// deleting the monitor pointers list
		delete iter->second;			// deleting the associated pair
		
		}
	*/		
	}	
	
	void operator()()	{ run(); }

	void addMonitor		( FtkMonitor & newMonitor );	
	void addMonitor		( std::list< FtkMonitor * >	);	

	
	bool stopMonitor	( std::string monitorName ); 	
	bool startMonitor	( std::string monitorName );
	
	
	void run();

	bool getMonitors	( Json::Value & root );

	private:

	std::map < boost::posix_time::time_duration , 
		   std::pair < 
			      boost::asio::deadline_timer*, 
			      std::list < FtkMonitor* > * 
			     > *
		 > timersMap;
	
	boost::asio::io_service& myIo;
	
	void publish	(const boost::system::error_code&, 
					std::pair< boost::asio::deadline_timer*, std::list<FtkMonitor*>* >,
					boost::posix_time::time_duration);
	
	// utility
	FtkMonitor* getMonitor	(std::string monitorName);

		
};

}
#endif
