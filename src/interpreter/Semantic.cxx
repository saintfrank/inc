////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

/**
 * @section sem Sematinc
 * The Semantic:
 *
 * Here are executed the functions in FtkApi.
 *
 * It is possible to add informations to the Json::Value
 *
 * It is also possible to interact to the FtkConfiguration to update
 * configuration data as consequence of some function execution and success as follews:
 * @code
 *
 *  FtkConfiguration * myConfig = FtkConfiguration::getInstance();
 *  myConfig->updateAttributeValue("AmBoard", "default", "mask", destination) ;
 *
 * @endcode
 *
 * Command execution / effects (that will be written as "effects")
 *
 *   - settamask --> to update mask on card configuration
 *   - writepatterns --> to update pattern bank name on card configuration
 *
 * @todo Settare che gli update e il nome delle schede corrispondono allo slot !!!!
 *
 */

#include <sstream>
#include <boost/cast.hpp>

#include <ftk/Configuration.h>
#include <ftk/Semantic.h>


namespace ftk{


bool monitorsSemantic::run( Var::Ptr /*ret*/, std::vector<Var::Ptr> /*parameters*/, Json::Value & root){

	FtkConfiguration * myconfig = FtkConfiguration::getInstance();

	if ( ! myconfig->getMonitors( root ) )
		root["error"]= "No monitor found";

	return true;
}

bool startmonitorSemantic::run( Var::Ptr /*ret*/, std::vector<Var::Ptr> parameters, Json::Value & root){

	// Parameters castings. The types have already been checked
	Variable<std::string> * myVar0 = boost::polymorphic_downcast<Variable<std::string> *>(parameters[0].get() );
	std::string mon 	= myVar0->getValue<std::string>();

	FtkConfiguration * myconfig = FtkConfiguration::getInstance();

	if ( ! myconfig->startMonitor( mon ) )
		root["result"]= "success";
	else
		root["result"]= "No monitor found";

	return true;
}

bool stopmonitorSemantic::run( Var::Ptr /*ret*/, std::vector<Var::Ptr> parameters, Json::Value & root){


	// Parameters castings. The types have already been checked
	Variable<std::string> * myVar0 = boost::polymorphic_downcast<Variable<std::string> *>(parameters[0].get() );
	std::string mon 	= myVar0->getValue<std::string>();

	FtkConfiguration * myconfig = FtkConfiguration::getInstance();

	if ( ! myconfig->stopMonitor( mon ) )
		root["result"]= "success";
	else
		root["result"]= "No monitor found";

	return true;
}

bool helpSemantic::run( Var::Ptr /*ret*/, std::vector<Var::Ptr> /*parameters*/, Json::Value & /*root*/){


	return true;
}


} // end namepsace
