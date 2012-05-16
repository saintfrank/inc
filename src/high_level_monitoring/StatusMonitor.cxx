/**
 *  @author Francesco Cervigni
 *  @date 2010/2011
 *
 */
#include <iostream>
#include <ftk/StatusMonitor.h>
#include <ftk/Configuration.h>

using namespace std;
using namespace ftk;

FtkStatusMonitor::FtkStatusMonitor(OksMonitorRepresentation monitorRepresentation ){

	#ifdef DEBUG_MONITORS	
	cout <<  "+ Beginning Status Monitor constructor" << endl;
   	#endif
	
	// initializing, just once, at the first instantiation, the tdaq static connections for the superclass FtkMonitor, that all the sublasses' objects will use
	if(s_initialized==false)
		{
		cout << "+ FtkStatusMonitor : Initializing the static monitors' TDAQ references"<<endl;

		FtkConfiguration * myConfig = FtkConfiguration::getInstance();

		s_partition = myConfig->getPartition();
		s_isDictionary = myConfig->getIsDictionary();
		s_rawProvider = myConfig->getOhProvider();
		s_isServerName= myConfig->getIsServerName();
		s_initialized=true;
		}
	

	m_interval = monitorRepresentation.m_interval ;
	m_card_to_monitor = monitorRepresentation.m_slot ;
	m_name = monitorRepresentation.m_name ;
	m_active = false;

	#ifdef DEBUG_MONITORS
	cout 	<< "+ FtkStatusMonitor : Arguments are " << m_interval <<" , " << m_card_to_monitor
			<< " , " << m_name << endl;
	#endif

    /** Defining the HIstogram to use*/
	m_Histogram = new TH1F("newHistogram","newRegHistogram",10,-50.5,50.5);


}

/**
 * The main purpose of the monitor. Performing the command and publish the information gathered.
 * In this case the result of the Read Register and published in IS and OH.
 */
bool FtkStatusMonitor::publish(){


	try
	{

	   #ifdef DEBUG_SHOW_MONITORS
	   std::cout <<  "+ I'm Publishing in the Is server: " <<  s_isServerName << std::endl ;
	   #endif

	   FtkConfiguration * myConfig = FtkConfiguration::getInstance();

	   //
	   int staticAddress = -1;
	   if( myConfig->getCardStaticAddress(m_card_to_monitor, staticAddress ) )
	   {
		   std::cout << "- Error : FtkStatusMonitor - Reading at a card with static address base 0x" << std::hex << staticAddress << std::endl ;
	   }

	   Json::Value regs;
	   if ( ! myConfig->getLocalRegisters(m_card_to_monitor, regs ) )
	   {
		   std::cout << "- Error : FtkStatusMonitor - Card Type AmBoard not found in configuration."<< std::endl ;
	   }

	   std::cout << "Registers: " << regs << std::endl;

	   //IFtkApi *myApi= FtkApi::getInstance();

	   Json::Value registers = regs["Registers"];

	   /** @todo RIMETTERE LA FUNZIONE SENZA FTKAPI Reading the status */
	   //if ( myApi->readStatus(m_card_to_monitor,registers, staticAddress) == false )
	   if(false)
	   {
		   /** If error while reading the status*/
		   std::cout << "+ FtkStatusMonitor : Error reading status on Slot: " << m_card_to_monitor << std::endl ;
		   ERS_INFO( "+ FtkStatusMonitor : Error reading status on Slot: " << m_card_to_monitor ) ;
		   return false;
	   }


	   for ( unsigned int i=0; i< registers.size(); i++ )
	   {
	   		std::string name = registers[i].get("name", "_").asString();
	   		int addr = registers[i].get("address", "_").asInt();
	   		const std::string value_attribute = registers[i].get("value", "_").asString();

	   		//ISInfoLong value(value_attribute);
	   		int val;
	   		std::stringstream ss;
	   		ss << std::hex << value_attribute ;
	   		ss >> val;

	   		ISInfoString value( value_attribute );

	   		std::cout << "Status Monitor: Publishing " << name << " : " << value_attribute << std::endl;

	   		/** @todo Dovrei anche fare l'update dei valori in FtkConfig */

	   		// Updating Value
	   		if ( ! myConfig->setCardRegisterValue(m_card_to_monitor, name, val) )
	   		{
	   			std::cerr << "\n\n ERROR: Error updating register " << name << " in card at slot " << m_card_to_monitor << std::endl;
	   		}


			//publishing on IS

	   		std::stringstream newStream;
			newStream << s_isServerName << "."<< "AmBoardSlot" << m_card_to_monitor << "." << name ;
			std::string newName = newStream.str();

			/*
			#ifdef DEBUG_SHOW_MONITORS
				std::cout << "+ FtkStatusMonitor : Slot: " << m_card_to_monitor << " publishing " << name << " at addr. " << addr << " with value " << value_attribute << std::endl ;
				ERS_INFO( "+ FtkStatusMonitor : Slot: " << m_card_to_monitor << " publishing " << name << " at addr. " << addr << " with  value " << value_attribute ) ;
			#endif
			*/

			// Publish information in the MyServer IS serverF
			ISInfoDictionary(s_partition).checkin( newName , value );

	   } // for


//	   std::vector<int> edroInfo;
//	   Json::Value info;
//
//	   /** Reading the edro regarding variables <nevt, npatt,ntimer>*/
//	   bool result = myApi->edro_inter(m_card_to_monitor,edroInfo, info);
//	   if (  result == false || edroInfo.size() != 3 )
//	   {
//	      /** If error while reading the status*/
//	      std::cout << "+ FtkStatusMonitor : Error reading status on Slot: " << m_card_to_monitor << std::endl ;
//	      ERS_INFO( "+ FtkStatusMonitor : Error reading status on Slot: " << m_card_to_monitor ) ;
//	      return false;
//	   }

	   /** @todo Dovrei anche fare l'update dei valori in FtkConfig */

	   //publishing on IS

	   /**
	   std::stringstream nevt;
	   nevt << s_isServerName << ".AmBoardSlot" << m_card_to_monitor << "_Nevt";
	   std::stringstream npatt;
	   npatt << s_isServerName << ".AmBoardSlot" << m_card_to_monitor << "_Npatt";
	   std::stringstream ntimer;
	   ntimer << s_isServerName << ".AmBoardSlot" << m_card_to_monitor << "_Ntimer";

	   ISInfoDictionary(s_partition).checkin( nevt.str() , ISInfoInt( edroInfo.at(0) ) );
	   ISInfoDictionary(s_partition).checkin( npatt.str() , ISInfoInt( edroInfo.at(1) ) );
	   ISInfoDictionary(s_partition).checkin( ntimer.str() , ISInfoInt( edroInfo.at(2) ) );

	   float stat = ( (float) edroInfo.at(1) )/edroInfo.at(2) ;

	   ERS_INFO( "+ FtkStatusMonitor : Stat: " << stat ) ;

	   */
	   //m_Histogram->Fill(stat);
	   //s_rawProvider -> publish( *m_Histogram, "Nevt on Ntimer", true );


	}
	catch (daq::ftk::VmeError ex)
	{
		ERS_INFO( "+ FtkStatusMonitor : ERROR: Found Vme Error reading register" ) ;
	}
	catch (daq::is::Exception ex)
	{
		ers::error( ex);
	}
	catch(daq::ipc::NotInitialized & e)
	{
		std::cout << "FTK - IPC ERROR. IPC not initialized ";
  	}

	return true;
}
