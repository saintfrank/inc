////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <iostream>
#include <ftk/OhMonitor.h>

using namespace std;
using namespace ftk;


FtkOhMonitor::FtkOhMonitor(vector<string>paramVectorPassed, FtkConfig* confPassed){
	
	config=confPassed;
	
	#ifdef DEBUG_MONITORS	
	cout <<  "+ Beginning Oh Monitor constructor" << endl;
   	#endif

	// initializing, just once, at the first instantiation, the tdaq static connections for the superclass FtkMonitor, that all the sublasses' objects will use
	if( s_initialized == false )
	{
		cout << "+ FtkOhMonitor : Initializing the static monitors' TDAQ references"<<endl;
		s_partition = config->getPartition();
		s_isDictionary = config->getIsDictionary();
		s_rawProvider = config->getOhProvider();
		s_isServerName= config->getIsServerName();		
		s_initialized = true;
	}

	int paramSize=paramVectorPassed.size();
	
	if( paramSize != 5 )
	{
		// wrong parameters, abort the constructor
		std::cerr << "\n\n\n Impossible to create OH monitor. Wrong parameters number. Please check Configuration File. \n\n\n" << std::endl;
	}
	else
	{
		m_interval = strtol(paramVectorPassed[1].c_str(), NULL, 10);		
		m_card_to_watch = strtol(paramVectorPassed[2].c_str(), NULL, 10);		
		m_register_to_watch = strtol(paramVectorPassed[3].c_str(), NULL, 10);
		m_name = paramVectorPassed[4];
		m_active = true;
		
		#ifdef DEBUG_MONITORS
		cout 	<< "+ FtkOhMonitor : Arguments are " << m_interval <<" , " << m_card_to_watch << " , " 
				<< m_register_to_watch << " , " << m_name << endl; 
		#endif		
	}

	m_Histo = new TH1F("newHistogram","newRegHistogram",10,-10,50);

}

bool FtkOhMonitor::publish(){

   Json::Value temp;

   //IFtkApi *myApi= FtkApi::getInstance();
   long newValue = 0;
		   //myApi->readRegister(m_card_to_watch, m_register_to_watch, temp);
	
   #ifdef DEBUG_SHOW_MONITORS
   std::cout <<  "+ I'm Publishing on OH the value : " << newValue << std::endl;
   #endif

   std::ostringstream histoNameStream;
   histoNameStream << " Register n. " << m_register_to_watch << " of card n. " << m_card_to_watch << " values monitoring.";
	
   m_Histo->Fill(newValue);
   s_rawProvider -> publish( *m_Histo, histoNameStream.str(), true );

   
return true;
}
