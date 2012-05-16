/**
 * @author Francesco Cervigni
 * @date 2010/2011
 *
 */

#include <ftk/Configuration.h>

using namespace ftk;
using namespace boost;

namespace ftk{


FtkConfiguration *FtkConfiguration::pInstance = 0;

FtkConfiguration *FtkConfiguration::getInstance(){

	if (pInstance==0)
	{
		pInstance = new FtkConfiguration;
	}

	return pInstance;
}

FtkConfiguration::FtkConfiguration():m_emptyConfig("null"){}


bool FtkConfiguration::start( std::string crateName, bool activateServices, std::string manualConfigFile ){

	m_crateName = crateName ;  /**< @todo TOGLIERE e prendere dalla linea di comando O fare un matching con l'hostname */

	std::cerr << "FtkConfiguration : Creating FTK Configuration" << std::endl ;

	// Creating the standard functions
	createStandardFunctions () ;

	// load the configuration databases names

	if( manualConfigFile != "")
	{
		// Setting the member data m_tdaqDbDataFile
		m_tdaqDbDataFile = manualConfigFile;

		std::cout << "FtkConfiguration : Using manual configuration file = " << manualConfigFile << std::endl ;

		m_oksConfigHandler.setFileName( manualConfigFile ) ;
	}
	else if ( ! getTdaqDbData() ) // in this case it is automathically loaded from the TDAQ_DB_DATA environment variable
	{
		throw daq::ftk::BadConfigFileException( ERS_HERE, "Error retrieving the Configuration Data.") ;
	}

	// load the configuration databases names
	if ( ! getTdaqPartition() )
	{
		throw daq::ftk::BadConfigFileException( ERS_HERE, "Error retrieving the Partition informations from the Configuration Data.") ;
	}

	// qui metto la lettura dei dati dal crate
	if ( ! getCrateAttributes() )
	{
		throw daq::ftk::BadConfigFileException( ERS_HERE, "Crate") ;
	}

	// qui metto la lettura dei dati dal crate
	if ( ! getCrateCards() )
	{
		throw daq::ftk::BadConfigFileException( ERS_HERE, "Crate Cards informations") ;
	}

	if ( ! getCrateCardsAttributes() )
	{
		throw daq::ftk::BadConfigFileException( ERS_HERE, "Crate Cards Attributes") ;
	}

	if ( ! getMonitors() )
	{
		throw daq::ftk::BadConfigFileException( ERS_HERE, "Monitors") ;
	}

	// Initializing Services

	if ( activateServices )
	{
		std::cerr << "+ FtkConfiguration : Now Initializing services: \n";

		if ( ! initializeServices() )
		{
			std::cerr << "+ FtkConfiguration : Initializing services returned false \n"  ;
			return false;
		}
	}
	else
	{
		std::cerr << "+ FtkConfiguration : NOT Initializing services: \n";
	}

	return true;

}

bool FtkConfiguration::getTdaqDbData(){

	char * pPath = 0;
	pPath = getenv("TDAQ_DB_DATA");

	if( pPath == 0 )
	{
		ERS_INFO("\n\nERROR - No TDAQ_DB_DATA Variable found. Impossible to retrieve the configuration database file.\nPlease source rc_start.sh \n\n");
		return false;
	}

	std::string ftkDbPath(pPath);

	// Setting the member data m_tdaqDbDataFile
	m_tdaqDbDataFile = ftkDbPath;

	std::cout << "FtkConfiguration : TDAQ_DB_DATA = " << m_tdaqDbDataFile << std::endl ;

	m_oksConfigHandler.setFileName( m_tdaqDbDataFile ) ;

	return true;

}

bool FtkConfiguration::getTdaqPartition(){

	char * pPath = 0;
	pPath = getenv("TDAQ_PARTITION");

	if( pPath == 0 )
	{
		ERS_INFO("\n\nERROR - No TDAQ_PARTITION Variable found. Impossible to retrieve the configuration database file.\nPlease source rc_start.sh \n\n");
		return false;
	}

	std::string ftkPartition(pPath);

	// Setting the member data m_partitionName
	m_partitionName = ftkPartition;

	std::cout << "FtkConfiguration : TDAQ_PARTITION = " << m_partitionName << std::endl;

	return true;

}

bool FtkConfiguration::getCrateAttributes(){

	// Checking the existence of the OKS Crate Definition in TDAQ_DB_DATA

	if ( ! m_oksConfigHandler.findObject("FtkCrate", m_crateName) )
	{
		// if problems encountered
		std::cerr << "\nERROR: No object FtkCrate found in the $TDAQ_DB_DATA \n";
		return false;
	}

	std::cerr << "FtkConfiguration : Crate " << m_crateName << " found in the Configuration file " << std::endl ;

	// Loading main Crate Attributes


	if ( ! m_oksConfigHandler.loadAttribute( "FtkCrate" , m_crateName , "IS_Server", m_isServerName ) )
	{
		// if problems encountered
		std::cerr << "\n\n\n ERROR: FtkConfiguration : No attribute IS_Server found in the configuration file  \n\n\n";
		return false;
	}

	m_ftkData.push_back( ConfigData( "IsServer" , m_isServerName ) );

	std::cerr << "FtkConfiguration : IsServer "<< m_isServerName << " found. " << std::endl ;

	ConfigData myCrateNameData;

	if ( ! m_oksConfigHandler.loadAttribute( "FtkCrate" , m_crateName , "OH_Server", m_ohServerName ) )
	{
		// if problems encountered
		std::cerr << "\n\n\n ERROR: FtkConfiguration : No attribute IS_Server found in the configuration file  \n\n\n";
		return false;
	}

	m_ftkData.push_back( ConfigData( "OhServer", m_ohServerName ) );

	std::cerr << "FtkConfiguration : OhServer " << m_ohServerName << " found. " << std::endl ;

	return true;

}

bool FtkConfiguration::getCrateCards(){

    /** @todo ricavare da linea di comando */
	std::vector< std::pair< std::string, std::string > > cards;

	if ( ! m_oksConfigHandler.getRelationshipObjectNames ("FtkCrate", m_crateName, "Cards" , cards ) )
	{
		// if not possible to
		throw daq::ftk::PluginNotFoundException( ERS_HERE, " FtkCard ");
	}

	std::cerr << "FtkConfiguration : Retrieved from Superclass FtkCard :  " << cards.size() << " Cards " << std::endl ;

	for( unsigned int i = 0 ; i < cards.size() ; i++ )
	{

		std::string cardName  = cards.at(i).first;
		std::string cardClass = cards.at(i).second;

		std::cout << "FtkConfiguration : Read card " << cardName << " of class " << cardClass << std::endl;

		std::vector<FtkRegisterRepresentation> myRegisters;

		FtkCardRepresentation myCard( cardClass.substr( 7, cardClass.size() ), cardName );

		// Retrieving the card

		std::string tempAdd;

		if ( ! m_oksConfigHandler.loadAttribute( cardClass , cardName , "VmeAddress", tempAdd ) )
		{
			std::stringstream ssss;	ssss << "VmeAddress of" << cardName ;
			throw daq::ftk::BadConfigFileException( ERS_HERE, ssss.str() );
		}

		std::stringstream rr;
		rr << tempAdd;
		rr >> myCard.m_staticAddress;

		if( myCard.m_staticAddress != 0 && (tempAdd != "") )
			myCard.m_isStaticAddress = true;

		rr >> myCard.m_staticAddress;

		std::cerr << "FtkConfiguration : VmeAddress : " << tempAdd << std::endl;

		// Retrieving the card

		std::string tempSlot;

		if ( ! m_oksConfigHandler.loadAttribute( cardClass , cardName , "Slot", tempSlot ) )
		{
			std::stringstream ssss;	ssss << " the slot of card " << cardName ;
			throw daq::ftk::BadConfigFileException( ERS_HERE, ssss.str() );
		}

		std::stringstream ss;
		ss << tempSlot;
		ss >> myCard.m_slot;

		// Loading the registers
		std::vector< std::pair< std::string, std::string > > registers;

		/** @todo Qui' aggiungere l'ereditarietà e leggere gli FtkCard, poi a seconda de*/

		m_oksConfigHandler.getRelationshipObjectNames ( cardClass, cardName, "Registers" , registers ) ;

		std::cerr << "FtkConfiguration : Retrieved "<< registers.size() << " registers from Card "<< cardName << std::endl ;

		for( unsigned int j = 0 ; j < registers.size() ; j++ )
		{

			std::string registerName = registers.at(j).first;
			std::string registerClass = registers.at(j).second;

			FtkRegisterRepresentation newRegister;

			newRegister.m_name = registerName ;

			// Retrieving the Attribute

			std::string tempAddress;

			if ( ! m_oksConfigHandler.loadAttribute( registerClass , registerName , "address", tempAddress ) )
			{
				std::stringstream ssss;	ssss << " the name of register " << registerName << " of Card " << cardName  ;
				throw daq::ftk::BadConfigFileException( ERS_HERE, ssss.str() );
			}

			std::stringstream tt; tt << tempAddress; tt >> newRegister.m_address;

			std::cerr << "FtkConfiguration : Address "<< newRegister.m_address << std::endl ;

			std::string tempValue;

			if ( ! m_oksConfigHandler.loadAttribute( registerClass , registerName , "value", tempValue ) )
			{
				std::stringstream ssss;	ssss << " the value of register " << registerName << " of Card " << cardName  ;
				throw daq::ftk::BadConfigFileException( ERS_HERE, ssss.str() );
			}

			std::stringstream yy; yy << tempValue; yy >> newRegister.m_value;

			std::cerr << "FtkConfiguration : Value "<< newRegister.m_value << std::endl ;

			/** @todo  aggiungere il value */

			myCard.addRegister(newRegister.m_name, newRegister);
		}


		// Loading the attributes
		std::vector< std::pair< std::string, std::string > > attributes;

		/** @todo Qui' aggiungere l'ereditarietà e leggere gli FtkCard, poi a seconda de*/

		m_oksConfigHandler.getRelationshipObjectNames ( cardClass, cardName, "Attributes" , attributes ) ;

		std::cerr << "FtkConfiguration : Retrieved "<< attributes.size() << " attributes from card "<< cardName << std::endl ;

		for( unsigned int j = 0 ; j < attributes.size() ; j++ )
		{

			std::string attributeName = attributes.at(j).first;
			std::string attributeClass = attributes.at(j).second;

			FtkAttributeRepresentation newAttribute;

			newAttribute.m_name = attributeName ;

			// Retrieving the Attribute

			std::string tempValue;

			if ( ! m_oksConfigHandler.loadAttribute( attributeClass , attributeName , "value", tempValue ) )
			{
				std::stringstream ssss;	ssss << " the value of attribute "<< attributeName << " of Card " << cardName ;
				throw daq::ftk::BadConfigFileException( ERS_HERE, ssss.str() );
			}

			std::stringstream yy; yy << tempValue; yy >> newAttribute.m_value;

			std::cerr << "FtkConfiguration : Value "<< newAttribute.m_value << std::endl ;

			/** @todo  aggiungere il value */

			myCard.addAttribute(newAttribute.m_name, newAttribute);
		}


		if ( ! loadPlugin( myCard ) ) /* @todo mettere le altre schede */
		{
			throw daq::ftk::PluginNotFoundException( ERS_HERE, "Plugin for AmBoard. Is the plugin correctly installed") ;
		}
		else
		{
			std::cout << "FtkConfiguration : Plugin loaded." << std::endl;
		}

		// At this point the slot was assigned to the object myCard
		addCard( myCard.m_slot, myCard );

		Json::Value root_2;
		root_2 = myCard.m_plugin->getFunctionsDescription(root_2);

		std::cout << "FtkConfiguration, constructor : Load Plugin : Just taken the json description from plugin" << myCard.m_plugin.get() <<  ": " << root_2 << "___" << std::endl;

		std::cout << "FtkConfiguration : The pointer AFTER DONE that I added is : " << myCard.m_plugin.get()  << std::endl;


		/** @todo Togliere queste verbosissime print e stampare solo quello finale  */

		Json::Value myStruct;
		myCard.getAllJson(myStruct);
		std::cout << "FtkConfiguration : The Struct is :\n" << myStruct << std::endl;

		Json::Value myRegs;
		getLocalRegisters(15, myRegs);
		std::cout << "FtkConfiguration : The Registers are :\n" << myRegs << std::endl;

		Json::Value myAtts;
		getLocalRegisters(15, myAtts);
		std::cout << "FtkConfiguration : The Attributes are :\n" << myAtts << std::endl;

		Json::Value myTot;
		getCardsJson(myTot);
		std::cout << "FtkConfiguration : The Cards are :\n" << myTot << std::endl;

		std::cerr << "FtkConfiguration : Added Card: " << cardName << std::endl ;

	}



	return true;

}

bool FtkConfiguration::getCrateCardsAttributes(){

    /** @todo ricavare da linea di comando */
	if ( ! m_oksConfigHandler.findObject("FtkCrate", m_crateName) )
	{
		std::stringstream ssss;	ssss << " Crate "<< m_crateName  ;
		throw daq::ftk::BadConfigFileException( ERS_HERE, ssss.str() );
	}

	return true;

}


bool FtkConfiguration::getMonitors(){

	// Retrieving the OksMonitor representation
	if ( ! getMonitorRepresentation() )
	{
		return false;
	}

	// Creating the actual Monitors
	FtkMonitorFactory myFact;
	std::vector<FtkMonitor*> createdMonitors = myFact.createMonitors( m_monitors );

	/** starting the Timer
	 *  @todo vedere perchè quì ho usato i monitors */

	std::list<FtkMonitor*> myMonitorList;

    for ( unsigned int i=0 ; i< createdMonitors.size() ; i++ )
	{
         myMonitorList.push_back(createdMonitors[i]);
    }

    // Creating the timer
	m_timer = new FtkTimer( m_ioService, myMonitorList );

	return true ;

}

bool FtkConfiguration::getMonitorRepresentation(){

	std::vector< std::pair< std::string, std::string > > monitorNames;

	if ( ! m_oksConfigHandler.getRelationshipObjectNames ("FtkCrate", m_crateName, "Monitors" , monitorNames ) )
	{
		// if not possible to
		std::cerr << "\n\n\n ERROR: Environment Variable FTK_PATH not defined. Please source the FTK setup. \n\n\n";
		return false;
	}

	std::cerr << "FtkConfiguration : Retrieved "<< monitorNames.size() << " monitors \n";

	for( unsigned int i = 0 ; i < monitorNames.size() ; i++ )
	{
		std::string monitorName  = monitorNames.at(i).first;
		std::string monitorClass = monitorNames.at(i).second;

		OksMonitorRepresentation newMonitor;

		newMonitor.m_name = monitorName ;

		if ( ! m_oksConfigHandler.loadAttribute( monitorClass , newMonitor.m_name , "type", newMonitor.m_monitor_type ) )
		{
			std::cerr << "\n Impossible to load the name of monitor "<< newMonitor.m_name  << " of Crate " <<  m_crateName << "\n";
		}

		std::string tempString; /** Using temporary string */

		if ( ! m_oksConfigHandler.loadAttribute( monitorClass , monitorName , "interval", tempString  ) )
		{
			std::cerr << "\n Impossible to load the type of monitor "<< newMonitor.m_name  << " of Crate " <<  m_crateName << "\n";
		}

		/** Getting the integer value */
		std::stringstream tt;
		tt << tempString;
		tt >> newMonitor.m_interval;

		/** Resetting the temp string */
		tempString = "";

		if ( ! m_oksConfigHandler.loadAttribute( monitorClass , monitorName , "slot", tempString ) )
		{
			std::cerr << "\n Impossible to load the slot of monitor "<< newMonitor.m_name  << " of Crate " <<  m_crateName << "\n";
		}

		/** Getting the integer value */
		std::stringstream ss;
		ss << tempString;
		ss >> newMonitor.m_slot;

		m_monitors.push_back(newMonitor);

	}

	std::cerr << "FtkConfiguration : Monitors Loaded" << std::endl ;

	return true;

}

bool FtkConfiguration::initializeServices(){


    std::cerr << "+ FtkConfiguration : TDAQ Initializing services with partition name: " << m_partitionName << " and is server name "<< m_isServerName << " .... " ;

    int x=0;
    char dummy_s[3]={'d','u','m'};
    char *dummy=dummy_s;
    IPCCore::init(x,&dummy);

    IPCPartition * partition_ptr = new IPCPartition( m_partitionName.c_str() );

    try
    {

		m_isDictionary = new ISInfoDictionary	( *partition_ptr );
        m_ohProvider   = new OHRootProvider	( *partition_ptr , m_ohServerName.c_str(), "FtkHistograms" );

    }
    catch( daq::is::Exception & ex )
    {
		std::cout 	<< "\n\n FTK EXCEPTION : Failed the creation of a IS information Object on \n partition: "<< m_partitionName <<" server : "
					<< m_isServerName << "\n\n";
		throw daq::ftk::PartitionProblem(ERS_HERE, "IS Server" );
    }
	catch( daq::oh::Exception & ex )
	{
		std::cout 	<< "\n\n FTK EXCEPTION : Failed the creation of a OH provider Object on \n partition: "<< m_partitionName <<" server : "
					<< m_ohServerName.c_str() << "\n\n";
		throw daq::ftk::PartitionProblem(ERS_HERE, "OH Server" );
    }
	catch( daq::ipc::AlreadyInitialized )
	{
		std::cout 	<< "\n\n IPC already initialized at partition: " << m_partitionName << "\n\n";
		return false;
	}
	catch( daq::ipc::CannotInitialize )
	{
		std::cout 	<< "\n\n IPC Cannot initialized partition: "<< m_partitionName << "\n\n";
		return false ;
	}

    std::cout << "FtkConfiguration : Success. " << std::endl ;

    return true;

}

FtkCardRepresentation & FtkConfiguration::getCardFromSlot( int slot ){

	std::map<int, FtkCardRepresentation>::iterator it;

	// returns "if the card is present"
    it = m_cardsData.find(slot);

	if ( it == m_cardsData.end() )
		return m_nullCard;
	else
		return it->second;

}

bool FtkConfiguration::getPluginFromSlot( int slot, iFtkPlugin::Ptr & destination ){

	return true ; /*toglieri*/

}

bool FtkCardRepresentation::getAttributeValue	( const std::string & attributeName, std::string & destination ){

	// Retrieving the Attribute
	std::map<std::string, FtkAttributeRepresentation >::iterator it;

	it = m_attributes.find(attributeName);

	if (it == m_attributes.end() )
	{
		return false;				// If attribute not found
	}
	else
	{
		destination = (it->second).m_value;
		return true;
	}

}


bool FtkCardRepresentation::getRegisterValue	( const std::string & registerName, int & destination ){

	// Retrieving the Register
	std::map<std::string, FtkRegisterRepresentation >::iterator it;

	it = m_registers.find(registerName);

	if (it == m_registers.end() )
	{
		return false;				// If register not found
	}
	else
	{
		destination = (it->second).m_value;
		return true;
	}

}

bool FtkCardRepresentation::setAttributeValue	( const std::string & attributeName, std::string newValue ){

	// Retrieving the Attribute
	std::map<std::string, FtkAttributeRepresentation >::iterator it;

	it = m_attributes.find(attributeName);

	if (it == m_attributes.end() )
	{
		return false;				// If attribute not found
	}
	else
	{
		(it->second).m_value = newValue;
		return true;
	}

}


bool FtkCardRepresentation::setRegisterValue	( const std::string & registerName, int newValue ){

	// Retrieving the Register
	std::map<std::string, FtkRegisterRepresentation >::iterator it;

	it = m_registers.find(registerName);

	if (it == m_registers.end() )
	{
		return false;				// If register not found
	}
	else
	{
		(it->second).m_value = newValue;
		return true;
	}

}


void FtkAttributeRepresentation::getJson( Json::Value & root, bool brief){

	if( brief )
	{
		Json::Value newVal;
		newVal[m_name] = m_value;
		root["Attributes"].append(newVal);

	}
	else
	{
		Json::Value newVal;
		newVal["name"] = m_name;
		newVal["value"] = m_value;
		root["Attributes"].append(newVal);
	}

}


void FtkRegisterRepresentation::getJson( Json::Value & root, bool brief ){

	if(brief)
	{
		Json::Value newVal;

		std::stringstream valAndAddr;

		std::stringstream ss;
		ss << "0x" <<std::hex << m_value;

		newVal[m_name] = ss.str();

		root["Registers"].append(newVal);

	}
	else
	{
		Json::Value newVal;
		newVal["name"] = m_name;
		newVal["address"] = m_address;
		newVal["staticAddress"] = m_isStaticAddress;



		newVal["value"] = m_value;

		root["Registers"].append(newVal);
	}


}

bool FtkCardRepresentation::addRegister(const std::string & regName, FtkRegisterRepresentation reg)
{

	// preparing the return value for the opration
	std::pair<std::map<std::string, FtkRegisterRepresentation>::iterator,bool> ret;

	// inserting
	ret = m_registers.insert( std::pair<std::string, FtkRegisterRepresentation>( regName , reg ) );

	// checking for duplicates.
	return ret.second ;

}

bool FtkCardRepresentation::addAttribute(const std::string & attName, FtkAttributeRepresentation att)
{

	// preparing the return value for the opration
	std::pair<std::map<std::string, FtkAttributeRepresentation>::iterator,bool> ret;

	// inserting
	ret = m_attributes.insert( std::pair<std::string, FtkAttributeRepresentation>( attName , att ) );

	// checking for duplicates.
	return ret.second ;

}



void FtkCardRepresentation::getRegistersRepresentation ( Json::Value & root, bool brief  ){

	// iterating the
	std::map<std::string, FtkRegisterRepresentation >::iterator it;

	for ( it = m_registers.begin() ; it != m_registers.end() ; it++ )
	{
		(it->second).getJson(root, brief);
	}

}

void FtkCardRepresentation::getAttributesRepresentation ( Json::Value & root, bool brief ){

	// iterating the
	std::map<std::string, FtkAttributeRepresentation >::iterator it;

	for ( it = m_attributes.begin() ; it != m_attributes.end() ; it++ )
	{
		(it->second).getJson(root, brief);
	}

}


void FtkCardRepresentation::getAllJson ( Json::Value & root, bool brief ){

	if ( brief )
	{
		Json::Value status;

		getRegistersRepresentation( status , brief);
		getAttributesRepresentation( status , brief );

		Json::Value newVal;
		newVal[m_name] = status;

		root["Cards"].append(newVal);


	}
	else
	{
		Json::Value newVal;

		newVal["name"] = m_name;
		newVal["type"] = m_type;
		newVal["plugin"] = m_plugin_name;
		newVal["slot"] = m_slot;

		if( m_isStaticAddress )
			newVal["staticAddress"] = m_staticAddress;

		getRegistersRepresentation( newVal, brief );
		getAttributesRepresentation( newVal, brief );

		root["Cards"].append(newVal);

	}


}


bool FtkConfiguration::getCardAttributeValue	( int slot, const std::string & attributeName, std::string & destination ){

	FtkCardRepresentation & myCard = getCardFromSlot( slot );

	if(myCard.m_name == "")
	{
		// if the card was not found
		return false;
	}
	else
	{
		return myCard.getAttributeValue( attributeName, destination );
	}
}


bool FtkConfiguration::getCardRegisterValue	( int slot, const std::string & registerName, int & destination ){

	FtkCardRepresentation & myCard = getCardFromSlot( slot );

	if(myCard.m_name == "")
	{
		// if the card was not found
		return false;
	}
	else
	{
		return myCard.getRegisterValue( registerName, destination );
	}
}

bool FtkConfiguration::setCardAttributeValue	( int slot, const std::string & attributeName, std::string newVal ){

	FtkCardRepresentation & myCard = getCardFromSlot( slot );

	if(myCard.m_name == "")
	{
		// if the card was not found
		return false;
	}
	else
	{
		return myCard.setAttributeValue( attributeName, newVal );
	}
}


bool FtkConfiguration::setCardRegisterValue	( int slot, const std::string & registerName, int newVal){

	FtkCardRepresentation & myCard = getCardFromSlot( slot );

	if(myCard.m_name == "")
	{
		// if the card was not found
		return false;
	}
	else
	{
		return myCard.setRegisterValue( registerName, newVal );
	}
}

bool FtkConfiguration::loadPlugin( FtkCardRepresentation & cardParam) {

	// extracting the name of the library from the card type
	std::stringstream ss;	ss << "lib" << cardParam.m_type << "Plugin.so";

	///////////////////
	////////  Loading the plugin

	void* plug = dlopen( ss.str().c_str() , RTLD_LAZY);

	if (!plug)
	{
			std::cerr << "Cannot load plugin for card " << cardParam.m_type << "\n" << dlerror() << std::endl;
			return false;
	}

	create_ftk_plugin* create_plug = (create_ftk_plugin*) dlsym(plug, "create");

	destroy_ftk_plugin* destroy_plug = (destroy_ftk_plugin*) dlsym(plug, "destroy");

	if (!create_plug || !destroy_plug)
	{
		std::cerr << "Cannot load function symbols: " << dlerror() << std::endl;
		return false;
	}

	iFtkPlugin * myPlug = create_plug();

	if (! myPlug )
	{
		std::cerr << "Cannot load symbols: " << dlerror() << std::endl;
		return false;
	}

	///////////////////////
	///////////////  finished loading the plugin
	//////////////////

	std::cerr << "The plugin pointer is " << myPlug << std::endl;

	cardParam.m_plugin.reset(myPlug);

	Json::Value root;
	root = cardParam.m_plugin->getFunctionsDescription(root);

	std::cout << "Load Plugin : Just taken the json description from plugin" << cardParam.m_plugin.get() <<  ": " << root << "___" << std::endl;

	// checking for duplicates.
	return true;


}

bool FtkConfiguration::addCard( int slot, FtkCardRepresentation newCard ){

	// preparing the return value for the operation
	std::pair<std::map<int, FtkCardRepresentation>::iterator,bool> ret;

	// inserting
	ret = m_cardsData.insert( std::pair<int, FtkCardRepresentation>( slot , newCard ) );

	// checking for duplicates.
	return ret.second ;

}



/*   JSON Representation Getters	*/

Json::Value FtkConfiguration::getAllJson(bool brief){
	
	Json::Value completeJson;
	Json::Value tempList;
		
	getCardsJson	(tempList, brief);
	
	completeJson[m_crateName] = tempList;
		
	return completeJson; 
}

bool FtkConfiguration::getCardsJson	(Json::Value & root, bool brief){

	std::map<int, FtkCardRepresentation>::iterator it;

	if( brief )
	{
		// Adding CardNames and positions
		for( it = m_cardsData.begin() ; it != m_cardsData.end() ; it++)
		{
			Json::Value myCard;
			std::string slot_string("slot ");

			std::string slot_number;
			std::stringstream ss; ss << (it->second).m_slot ; ss >> slot_number ;

			slot_string.append( slot_number );
			myCard[slot_string] = (it->second).m_name;
			root["Cards"].append( myCard );
		}
	}
	else
	{
		for( it = m_cardsData.begin() ; it != m_cardsData.end() ; it++)
				(it->second).getAllJson(root);

	}

	return true;

}

bool FtkConfiguration::getCardJson	(int slot, Json::Value & root, bool brief){
	
	FtkCardRepresentation & myCard = getCardFromSlot( slot );

	if ( myCard.m_name.size() == 0)
	{
		std::cout << "FtkConfiguration: ERROR : Card at slot " << slot << " not found" << std::endl;
		return false;
	}
	
	Json::Value temp;

	myCard.getAllJson(temp, brief);

	/** @todo migliorare */
	Json::Value myJson = temp.get("Cards", "no Cards");
	Json::Value myJson2 = myJson[0u];
	root = myJson2;

	return true;

}

bool FtkConfiguration::getCardStaticAddress( int slot, int & staticAddress ){

	FtkCardRepresentation & myCard = getCardFromSlot( slot );

	if ( myCard.m_isStaticAddress )
	{
		staticAddress = myCard.m_staticAddress ;
		return true ;
	}
	else
		return false ;

}

Json::Value FtkConfiguration::getGlobalFtkJson(){
	
	Json::Value x;
	return x;

}
		
Json::Value FtkConfiguration::getGlobalCardsJson(){
	
	Json::Value x;
	return x;

}

Json::Value FtkConfiguration::getLocalCardJson(){

	Json::Value x;
	return x;


}

bool FtkConfiguration::getLocalRegisters(int slot, Json::Value & root){

	FtkCardRepresentation & myCard = getCardFromSlot( slot );

	myCard.getRegistersRepresentation ( root );

	/** @todo quì va aggiunta la parte dell'indirizzo base */

	return true;
}

/////////////////////////////////////////////////////////////


void FtkConfiguration::createStandardFunctions () {

	std::vector<Var::Ptr> parameters;

	{
		Json::Value help  ;
		help["Description"] = "Displays the available commands if used without , if used with a parameter displays instructions and examples about that command" ;
		help["Example"].append("help ;") ;
		help["Example"].append("help am_init ;") ;
		parameters.clear();
		Function helpFun( "help", Var::Ptr( new Variable<int>("ret", "-1" ) ), new helpSemantic(), parameters, help);
		m_standardFunctions.addFunction(helpFun);
	}

	{
		Json::Value help  ;
		help["Description"] = "Displays the monitors list " ;
		help["Example"] 	= "monitors ;" ;
		parameters.clear();
		Function monitorsFun( "monitors", Var::Ptr( new Variable<int>("ret", "-1" ) ), new monitorsSemantic(), parameters, help);
		m_standardFunctions.addFunction(monitorsFun);
	}

	{
		Json::Value help  ;
		help["Description"] = "Starts the monitor given as parameter " ;
		help["Example"] 	= "startmonitor Monitor_123456_lnpisa ;" ;
		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<std::string>("ret", "-1" ) ));
		Function startmonitorFun( "startmonitor", Var::Ptr( new Variable<int>("ret", "-1" ) ), new startmonitorSemantic(), parameters, help);
		m_standardFunctions.addFunction(startmonitorFun);
	}

	{
		Json::Value help  ;
		help["Description"] = "Stops the monitor given as parameter " ;
		help["Example"] 	= "stopmonitor Monitor_123456_lnpisa ;" ;
		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<std::string>("ret", "-1" ) ));
		Function stopmonitorFun( "stopmonitor", Var::Ptr( new Variable<int>("ret", "-1" ) ), new stopmonitorSemantic(), parameters, help);
		m_standardFunctions.addFunction(stopmonitorFun);
	}

}


}// namespace ftk
