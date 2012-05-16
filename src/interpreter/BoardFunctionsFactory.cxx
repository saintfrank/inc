////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <ftk/BoardFunctionsFactory.h>

//#define DEBUG_FUNCTIONS

namespace ftk{

BoardFunctionsFactory::BoardFunctionsFactory () {
		
	createFunctions();
		
}

void BoardFunctionsFactory::createFunctions () {
		
	std::vector<Var::Ptr> parameters;
	
	{
		// creating read
		Json::Value help  ;
		help["Description"] = "Reading operation on vme register" ;
		help["Syntax"]  = "read slot address_offset static_base_vme_address ;" ;
		help["Example"] = "read 15 0x3f -1 ;" ;

		parameters.push_back( Var::Ptr( new Variable<int>("card", "-1" ) ) );
		parameters.push_back( Var::Ptr( new Variable<int>("offset", "-1" ) ) );
		parameters.push_back( Var::Ptr( new Variable<int>("base_vme_addr", "-1" ) ) );
		Function readFun( "read", Var::Ptr(new Variable<int>("ret", "-1" ) ), new ReadSemantic(), parameters, help );
		addFunction(readFun);
	}

	// creating write
	{
		Json::Value help  ;
		help["Description"] = "Write operation on vme register" ;
		help["Syntax"]  = "write slot address_offset static_base_vme_address value ;" ;
		help["Example"] = "write 15 0x3f -1 0x11;" ;

		parameters.push_back( Var::Ptr( new Variable<int>("card", "-1" ) ) );
		parameters.push_back( Var::Ptr( new Variable<int>("register", "-1" ) ) );
		parameters.push_back( Var::Ptr( new Variable<int>("base_vme_addr", "-1" ) ) );
		parameters.push_back( Var::Ptr( new Variable<int>("value", "-1" ) ));
		Function writeFun( "write", Var::Ptr( new Variable<int>("ret", "-1" ) ) , new WriteSemantic(), parameters, help );
		addFunction(writeFun);
	}

	// creating writepatterns
	{
		Json::Value help  ;
		help["Description"] = "Loads Pattern bank into AmBoard" ;
		help["Syntax"]  = "writepatterns filePath slot useLaymap thr ;" ;
		help["Example"] = "writepatterns patt_file_lungo.patt 15 0 6 ;" ;
	
		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<std::string>("filePath", "" ) )); // string
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("useLaymap", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("thr", "-1" ) ));
		Function writepatternsFun( "writepatterns", Var::Ptr( new Variable<int>("ret", "-1" )) , new WritepatternsSemantic(), parameters, help );
		addFunction(writepatternsFun);
	}

	// creating writepatterns
	{
		Json::Value help  ;
		help["Description"] = "Simulates hits input from edro on AmBoard card " ; /**@todo mettere una bella descrizione*/
		help["Syntax"]  = "amfeed_edro slot patternFilePath sort thr slot disbitm slow step road_limit mask ;" ;
		help["Example"] = "amfeed_edro patt_file_lungo.patt 0 6 15 0 0 0 0 0  ;"   ;
	
		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<std::string>("patternFilePath", "-1" ) )); // string
		parameters.push_back( Var::Ptr( new Variable<int>("sort", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("thr", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("disbitm", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("slow", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("step", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("road_limit", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("mask", "-1") ));
		Function amfeed_edroFun( "amfeed_edro", Var::Ptr( new Variable<int>("ret", "-1" ) ), new Amfeed_edroSemantic(), parameters, help);
		addFunction(amfeed_edroFun);
	}
	
	// creating ambslim_vme
	{
		Json::Value help  ;
		help["Description"] = "todo" ; /**@todo mettere una bella descrizione*/
		help["Syntax"]  = "ambslim_vme slot ;" ;
		help["Example"] = "ambslim_vme 15 ;  " ;
	
		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		//parameters.push_back( Var::Ptr( new Variable<int>("loop", "-1" ) ));
		Function ambslim_vmeFun( "ambslim_vme", Var::Ptr( new Variable<int>("ret", "-1" )) , new Ambslim_vmeSemantic(), parameters, help );
		addFunction(ambslim_vmeFun);
	}
	
	// creating am_init

	{
		Json::Value help  ;
		help["Description"] = "Initializes an AmBoard card" ;
		help["Syntax"]  = "am_init slot ;" ;
		help["Example"] = "am_init 15 ;"   ;

		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));

		Function am_initFun( "am_init", Var::Ptr( new Variable<int>("ret", "Success" ) ), new Am_initSemantic(), parameters, help );

		addFunction(am_initFun);

	}

	
	// creating leggififo
	{
		Json::Value help  ;
		help["Description"] = "Reads AmBaord FIFO" ;
		help["Syntax"]  = "leggififo slot verbose ;" ;
		help["Example"] = "leggififo 15 1 ;"   ;
	
		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("verbose", "-1" ) ));
		Function leggififoFun( "leggififo", Var::Ptr( new Variable<int>("ret", "-1" )) , new LeggififoSemantic(), parameters, help );
		addFunction(leggififoFun);
	}
	
	// creating leggireg0
	{
		Json::Value help  ;
		help["Description"] = "Reads AmBaord's first register and other interesting values" ;
		help["Syntax"]  = "leggireg0 15 ;" ;
		help["Example"] = "leggireg0 slot ;"   ;

		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("cont", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("verbose", "-1" ) ));
		Function leggireg0Fun( "leggireg0", Var::Ptr( new Variable<int>("ret", "-1" ) ), new Leggireg0Semantic(), parameters, help );
		addFunction(leggireg0Fun);
	}
		// creating settamask
	{
		Json::Value help  ;
		help["Description"] = "Sets AmBoard mask register" ;
		help["Syntax"]  = "settamask slot mask ;" ;
		help["Example"] = "settamask 15 0x3f00 ;"   ;

		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("mask", "-1" ) ));
		//parameters.push_back( Var::Ptr( new Variable<int>("statusm", "-1" ) ));
		Function settamaskFun( "settamask", Var::Ptr( new Variable<int>("ret", "-1" ) ), new SettamaskSemantic(), parameters, help);
		addFunction(settamaskFun);
	}
	
	// creating settamask
	
	{
		Json::Value help  ;
		help["Description"] = "Verifies pattern recognition by looping on introducing single patterns as hits and verifying matches. " ;
		help["Syntax"]  = "check_patterns slot 0 useLaymap verbose ;" ;
		help["Example"] = "check_patterns 15 0 6 0 ;"   ;

		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<std::string>("filePath", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("useLaymap", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("thr", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("verbose", "-1" ) ));
		Function checkpatternsFun( "check_patterns", Var::Ptr( new Variable<int>("ret", "-1" ) ), new checkpatternsSemantic(), parameters, help );
		addFunction(checkpatternsFun);
	}

	{
		Json::Value help  ;
		help["Description"] = "Prints out I/O spybuffer content" ;
		help["Syntax"]  = "leggiIOspy slot ;" ;
		help["Example"] = "leggiIOspy 15 ;"   ;

		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("input", "-1" ) ));
		parameters.push_back( Var::Ptr( new Variable<int>("statusm", "-1" ) ));
		Function leggiIOspyFun( "leggiIOspy", Var::Ptr( new Variable<int>("ret", "-1" ) ), new leggiIOspySemantic(), parameters, help);
		addFunction(leggiIOspyFun);
	}

	{
		Json::Value help  ;
		help["Description"] = "Performs a short test on AmBoard" ;
		help["Syntax"]  = "short_test slot ;" ;
		help["Example"] = "short_test 15 ;"   ;

		parameters.clear();
		parameters.push_back( Var::Ptr( new Variable<int>("slot", "-1" ) ));
		Function short_testFun( "short_test", Var::Ptr( new Variable<int>("ret", "-1" ) ), new short_testSemantic(), parameters, help);
		addFunction(short_testFun);
	}
		
}

/**
* Adds the function to function table
*
* @return 1 if the operation succeded. 0 if an element with the same name was already present
*/
bool BoardFunctionsFactory::addFunction(Function & newFun){
			
	std::pair < std::map <std::string,Function>::iterator , bool > ret;
		
	ret = functionsTable.insert ( std::pair<std::string, Function> ( newFun.getName(), newFun) );
	
	return ret.second;	

}

/**
* Returns the variable
*
* @return A pointer to the funcion to the function associated with the name 'name'. NULL if not present
*/
Function * BoardFunctionsFactory::getFunction(std::string & name){
	
	  std::map<std::string,Function>::iterator foundFun;
	
      foundFun = functionsTable.find(name);
      
      if ( foundFun == ( functionsTable.end() ) ) 
				return NULL;
	  else
				return &(foundFun->second);
	
}

void BoardFunctionsFactory::getFunctionsList(Json::Value & root){
	
	Json::Value fun;
	
	std::map<std::string,Function>::const_iterator it;
	
	unsigned int i=0;			
	for ( it = functionsTable.begin() ; it != functionsTable.end(); it++ )
		{
			#ifdef DEBUG_FUNCTIONS
			std::cout << "Iterating on the Functions " << std::endl;
			#endif
			std::string name ( it->first );
			fun["Functions available"][i] = name ;
			i++;								
		}	
		
	root = fun;
}

} // namespace
