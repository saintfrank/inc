////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
/////////////////////////////////
/**This monitor is
 *
 * @author Francesco Cervigni
 *
 *
 *
 */
#include <iostream>
#include <ftk/IsMonitor.h>

using namespace std;
using namespace ftk;

FtkIsMonitor::FtkIsMonitor(std::vector<std::string> paramVectorPassed, FtkConfig* confPassed){
	
	config=confPassed;
	
	#ifdef DEBUG_MONITORS	
	cout <<  "+ Beginning Is Monitor constructor" << endl;
   	#endif
	
	// initializing, just once, at the first instantiation, the tdaq static connections for the superclass FtkMonitor, that all the sublasses' objects will use
	if(s_initialized==false)
		{
		cout << "+ FtkIsMonitor : Initializing the static monitors' TDAQ references"<<endl;
		s_partition = config->getPartition();
		s_isDictionary = config->getIsDictionary();
		s_rawProvider = config->getOhProvider();
		s_isServerName= config->getIsServerName();		
		s_initialized=true;
		}
	
	int paramSize=paramVectorPassed.size();
	if(paramSize !=5)
	{
	// wrong parameters, abort the constructor
	std::cerr << "\n\n\n Impossible to create IS Monitor. Wrong parameters number. Please check Configuration File. \n\n\n" << std::endl;
	}
	else
	{
		m_interval =strtol(paramVectorPassed[1].c_str(), NULL, 10);		
		m_card_to_watch=strtol(paramVectorPassed[2].c_str(), NULL, 10);		
		m_register_to_watch=strtol(paramVectorPassed[3].c_str(), NULL, 10);
		m_name = paramVectorPassed[4];
		m_active = true;
				
		#ifdef DEBUG_MONITORS		
		cout 	<< "+ FtkIsMonitor : Arguments are " << m_interval <<" , " << m_card_to_watch 
				<< " , " << m_register_to_watch << " , " << m_name << endl; 
		#endif		
	}	

}
/**
 * The main purpose of the monitor. Performing the command and publish the information gathered.
 * In this case the result of the Read Register and published in IS and OH.
 */
bool FtkIsMonitor::publish(){
 
   #ifdef DEBUG_SHOW_MONITORS
   std::cout <<  "+ I'm Publishing in the IS server: " <<  s_isServerName << std::endl ;
   #endif    
   
   Json::Value temp;
   
   /**@todo mettere la funzione qu“*/
   //IFtkApi *myApi= FtkApi::getInstance();
   //
   long newValue = 0 ;
		   //myApi->readRegister(m_card_to_watch, m_register_to_watch, temp);


   #ifdef DEBUG_SHOW_MONITORS
   cout << "the value "<< newValue << endl;
   #endif    
 
   // Create the instances of the information
   // that will be published, and initialise them
   ISInfoLong value(newValue);
      
   std::stringstream newStream;
   newStream << s_isServerName << ".newFtkInfo" ;
   std::string newName = newStream.str();


   // Publish information in the MyServer IS server
   try	
   {
		s_isDictionary->insert( newName , value );
   }
   catch(daq::is::InfoAlreadyExist & e)
   {
		try
		{
			s_isDictionary->update(newName,value);
		} 
		catch (daq::is::Exception ex) {
			ers::error( ex);
      	}
   }
   catch(daq::ipc::NotInitialized & e)
   {
		std::cout << "FTK - IPC ERROR. IPC not initialized ";
   }

return true;
}
