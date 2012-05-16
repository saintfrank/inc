////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include "ftk/Timer.h"

using namespace std;
using namespace ftk;

FtkTimer::FtkTimer(boost::asio::io_service& ioParam, std::list<FtkMonitor*> monitorsList):myIo(ioParam){ 
	addMonitor(monitorsList);
	}

void FtkTimer::run(){	
	myIo.run();
}

void FtkTimer::addMonitor(FtkMonitor & newMonitor){ // in realtà è un puntatore a FtkMonitor 

	//estrai il tempo dal monitor
	boost::posix_time::time_duration monit_time = newMonitor.get_time();
	
	std::map<boost::posix_time::time_duration, 
		 std::pair	<
					boost::asio::deadline_timer*,
					std::list<FtkMonitor*>* 
					>*
		>::iterator corrispondentPair;

	corrispondentPair = timersMap.find(monit_time);
   
	if (corrispondentPair != (timersMap.end()) ) 
		{ //if there is already a timer at that time
		corrispondentPair->second->second->push_back(&newMonitor);
		#ifdef DEBUG_TIMER		
		cout << "+ FtkTimer : At the time .. already another timer was present, adding the monitor to the lost " << endl ;	  
		#endif
		}
    	else
        	{
		#ifdef DEBUG_TIMER		
		std::cout << "+ FtkTimer : The time duration is not in my_map, creating the timer ..." ;
		#endif
				
		//if is a new timing
		boost::asio::deadline_timer *newTimer=new boost::asio::deadline_timer(myIo, monit_time);

		#ifdef DEBUG_TIMER		
		std::cout << "Done." << std::endl << "+ FtkTimer : Creating the new list ...";
		#endif
				
		std::list<FtkMonitor*>* newList= new std::list<FtkMonitor*>();
							
		newList->push_back(&newMonitor);
		
		#ifdef DEBUG_TIMER		
		std::cout << "Done." << std::endl << "+ FtkTimer : Creating pair ...";
		#endif
		
		std::pair<
			boost::asio::deadline_timer*,
   	 		std::list<FtkMonitor*>* 
			> 
			*newPair= new std::pair <
					boost::asio::deadline_timer*,
			   		std::list<FtkMonitor*>* 
					>(newTimer,newList);
		
		#ifdef DEBUG_TIMER		
		std::cout << "Done." << std::endl << "+ FtkTimer : Adding it to the map ...";
		#endif
		
		timersMap.insert(std::pair	< 
						boost::posix_time::time_duration, 
					   	std::pair < boost::asio::deadline_timer*, 
							   std::list<FtkMonitor*>* >* 
					   	> (monit_time,newPair));

		#ifdef DEBUG_TIMER		
		std::cout << "Done." << std::endl << "+ FtkTimer : Calling the timer ...";
		#endif
		
		newTimer->async_wait( boost::bind(	&FtkTimer::publish, 
							this, 
							boost::asio::placeholders::error, 
							*newPair,
							monit_time));
		
		#ifdef DEBUG_TIMER		
		std::cout << "Done." << std::endl;
		#endif
		
		std::cout << "+ FtkTimer : Monitor correctly added." << std::endl;
		
			
		}

}	

void FtkTimer::addMonitor(std::list<FtkMonitor*> monitorsList){ 
	list<FtkMonitor*>::iterator it;

  	#ifdef DEBUG_TIMER		
	std::cout << "+FtkTimer : addMonitor called with a monitors list " << std::endl;
	#endif
	for ( it=monitorsList.begin() ; it != monitorsList.end(); it++ )
    		{
				#ifdef DEBUG_TIMER		
				std::cout << "+FtkTimer : Adding monitor " << std::endl;
				#endif			
				addMonitor(*(*it));
		    
		    }

}

void FtkTimer::publish	(const boost::system::error_code& e, 
						std::pair< boost::asio::deadline_timer*, std::list<FtkMonitor*>* > pairParam,
						boost::posix_time::time_duration interval){

	// estraggo dal paio sia il timer che la stringa	
	list<FtkMonitor*>::iterator it;
	
	std::list<FtkMonitor*> *monitorsList=pairParam.second;
		
	boost::asio::deadline_timer* timer=pairParam.first;

	for(it=monitorsList->begin(); it != monitorsList->end(); ++it) 
		{
		
		if( (*it)->isActive() )		
			(*it)->publish();
		
		#ifdef DEBUG_SHOW_TIMER
		cout << "+ FtkTimer : Timer " << boost::posix_time::to_simple_string ( timer->expires_from_now() ) << " milliseconds: called a publish" << endl;
		#endif
		}
			
	timer->expires_at( timer->expires_at() + interval );
	timer->async_wait( boost::bind( &FtkTimer::publish, this, e , pairParam, interval ) );
}


bool FtkTimer::stopMonitor(std::string monitorName)
{
	FtkMonitor* myMonitor = getMonitor( monitorName );
	
	if( myMonitor == NULL )
		return false;

	myMonitor->setActive(false);	
	return true;
	
} 	

bool FtkTimer::startMonitor(std::string monitorName)
{
	FtkMonitor* myMonitor = getMonitor( monitorName );
	
	if( myMonitor == NULL )
		return false;

	myMonitor->setActive(true);	
	return true;
	
} 

FtkMonitor* FtkTimer::getMonitor(std::string monitorName)
{
	
	std::map< 	boost::posix_time::time_duration, 
				std::pair<boost::asio::deadline_timer*,std::list < FtkMonitor* > * > *
			>::iterator it;
			
	for(it = timersMap.begin(); it != timersMap.end(); it++) 
    {
	
		std::pair<boost::asio::deadline_timer*,std::list < FtkMonitor* > * > * myPairPtr = it->second;
		
		if( myPairPtr == NULL )
		{	// lancia eccezione TIMER ERROR ____ TODO 
		
		}	
		else
		{
			std::list < FtkMonitor* > *	myMonitListPtr = myPairPtr->second;
		
			if( myMonitListPtr == NULL )
			{	// lancia eccezione TIMER 
			}	
			else
			{
				
				std::list < FtkMonitor* > myMonitList = *myMonitListPtr; // just to don't mess with pointers.
				list<FtkMonitor*>::iterator it;
								
				for( it = myMonitList.begin() ; it != myMonitList.end() ; ++it ) 
				{
					#ifdef DEBUG_TIMER		
					std::cout << "+FtkTimer : Checking for closing " << (*it)->getName() << std::endl;
					#endif
					
					if( ( (*it)->getName() ).compare(monitorName) == 0 ) // success
						return (*it);
				}
	
			}
			
		}
		
		
	}
	
	return NULL;
		 
} 

bool FtkTimer::getMonitors( Json::Value & root ){

	std::map< 	boost::posix_time::time_duration,
				std::pair<boost::asio::deadline_timer*,std::list < FtkMonitor* > * > *
			>::iterator it;

	#ifdef DEBUG_TIMER
	std::cout << "+FtkTimer : Beginning getMonitors " << std::endl;
	#endif

	for(it = timersMap.begin(); it != timersMap.end(); it++)
    {

		std::pair<boost::asio::deadline_timer*,std::list < FtkMonitor* > * > * myPairPtr = it->second;

		if( myPairPtr == NULL )
		{	// lancia eccezione TIMER ERROR ____ TODO
			return false;
		}
		else
		{
			std::list < FtkMonitor* > *	myMonitListPtr = myPairPtr->second;

			if( myMonitListPtr == NULL )
			{	// lancia eccezione TIMER
				return false;
			}
			else
			{

				std::list < FtkMonitor* > myMonitList = *myMonitListPtr; // just to don't mess with pointers.
				list<FtkMonitor*>::iterator it;

				for( it = myMonitList.begin() ; it != myMonitList.end() ; ++it )
				{
					#ifdef DEBUG_TIMER
					std::cout << "+FtkTimer : Inserting " << (*it)->getName() << std::endl;
					#endif


					/** Creating the new monitor JSON representation  */
					std::string monName ( (*it)->getName() );

					Json::Value newMonitor;

					if ( (*it)->isActive() )
						newMonitor[monName]="Running";
					else
						newMonitor[monName]="Stopped";

					root.append(newMonitor);
				}

			}

		}


	}

	return true;

}
