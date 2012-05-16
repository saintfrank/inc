/**
 * @author Francesco Cervigni
 * @date 2010/2011
 *
 */

#include <iostream>
#include <sstream>
#include <typeinfo>

#include "ftk/Configuration.h"
#include "ftk/Info.h"
#include "ftk/Interpreter.h"

using boost::algorithm::trim_copy ;
using boost::algorithm::trim ;

#define DEBUG_INTERPRETER

namespace ftk{

static InfoContainer answersRoot;

static Context sessionContext;
	
static FunctionsFactory * functions;

FtkInterpreter::FtkInterpreter(){}


Var::Ptr FtkInterpreter::interpretSingleExpressionStrings ( std::string & name,  std::string /*nameRetVariable*/, std::vector<std::string> parameterStrings ){

	std::cout << "+ FtkInterpreter : received  _" << name << "_, size dell'array  "<< parameterStrings.size() << std::endl;

	// The returning message
	Json::Value message;
	std::vector<Var::Ptr> parameters;

	// The return variable
	Var::Ptr ret;

	// Creating the answer
	Info * newInfo = new Info();

	try
	{

		FtkConfiguration * myConfig = FtkConfiguration::getInstance();

		// See if the function is a general function
		Function * generalFunc;
		generalFunc = myConfig->getFunctions().getFunction(name);

		if ( generalFunc != NULL )
		{

			std::cout << "+ FtkInterpreter : it is general, checking parameters  " << std::endl;

			// if it is a general function

			//////////////////////////// creating the variables

			for(unsigned i=0; i < parameterStrings.size(); i++)
			{
				std::string name ( parameterStrings.at(i) );


				std::vector<Var::Ptr> funcParameters = generalFunc -> getParameters();
				std::cout << "+ FtkInterpreter : " << name << ", creating an instance of "<< funcParameters.at(i)->getName() << " : "<< funcParameters.at(i)->getType() << std::endl;
				Var::Ptr originalVar( funcParameters.at(i) );
				Var::Ptr newVar = originalVar->createCopyWithValue( parameterStrings.at(i) ) ;
				if(newVar)
					parameters.push_back(newVar);
				else
				{
					// If the paramenter string cannot be converted into the desired type
					std::stringstream sss ;
					sss << "The " << i+1 << "¡ parameter is not of the expected type: " << funcParameters.at(i)->getName() << " : " << funcParameters.at(i)->getType()  ;
					message["error"].append( sss.str() ) ;
					throw daq::ftk::WrongParameters(ERS_HERE, sss.str() ) ;
				}

			}

			///////////////////////////////
			ret = generalFunc->createNewReturnVariable("Dummy");

			//launching the function

			if ( ! generalFunc->run( ret, parameters, message ) )
			{
				message["Returned"] = "Failure";
			}
			else
			{
				message["Returned"] = "Success";
			}
		}
		else
		{

			if ( parameterStrings.size() == 0 )
			{
				message["Returned"] = "Failure";
				message["Error"] 	= "Missing slot paramenter for non general function. Please define slot";
			}
			else
			{

				// If it is a system function [help, startmonitor, stopmonitor]
				int slotNum = 0 ;

				std::stringstream ss;
				ss << parameterStrings.at(0);
				ss >> slotNum;

				FtkCardRepresentation & thisCard = myConfig->getCardFromSlot(slotNum);

				if(thisCard.m_name == "")
				{
					#ifdef DEBUG_PLUGIN
						std::cerr << "+ FtkInterpreter : Plugin Found for card " << name << "Now executing the function" << std::endl;
					#endif
				}
				else
				{
					// If it is a specific card
					if ( ! thisCard.m_plugin->run( name, parameterStrings, ret, &thisCard, message ) )
					{
						#ifdef DEBUG_INTERPRETER
							std::cout << "+ FtkInterpreter : Function run end. " << std::endl;
						#endif

						message["Returned"] = "Failure";
					}
					else
					{
						#ifdef DEBUG_INTERPRETER
							std::cout << "+ FtkInterpreter : Function run end. " << std::endl;
						#endif

						message["Returned"] = "Success";
					}

				}
			}

		}


	}
	catch (daq::ftk::LibraryNotFoundException & e)
	{
		// In case Library was not found
		message["Result"] = "Failure" ;
		newInfo->addInfo ( new ErrorInfo( e.what() ) );
	}
	catch (daq::ftk::VmeError & e)
	{
		message["Result"] = "Failure" ;
		newInfo->addInfo ( new ErrorInfo( e.what() ) );
	}
	catch (daq::ftk::WrongParameters & e)
	{
		message["Result"] = "Failure" ;
		newInfo->addInfo ( new ErrorInfo( e.what() ) );
	}
	catch (daq::ftk::TypeException & e)
	{
		message["Result"] = "Failure" ;
		newInfo->addInfo ( new ErrorInfo( e.what() ) );
	}

	newInfo->addInfo ( new CleanInfo( message ) );

	answersRoot.addInfo(newInfo);

	return ret;
}

void printVariables(){

	Info *newInfo = new Info();
	
	Json::Value variables;
	
	sessionContext.getVariablesList(variables);
		
	newInfo->addInfo( new InterpretationInfo(variables ) );
	
	answersRoot.addInfo( newInfo );

}

void printMonitors(){

	FtkConfiguration * myconfig = FtkConfiguration::getInstance();
	Json::Value root;

	if ( myconfig->getMonitors( root ) )
	{
		answersRoot.addInfo( new CleanInfo( root ) );
	}
	else
	{
		answersRoot.addInfo( new ResultInfo( "Error" ) );
	}


}

void startMonitor(std::string name){

	FtkConfiguration * myconfig = FtkConfiguration::getInstance();

	if( myconfig->startMonitor( name ) )
		answersRoot.addInfo( new ResultInfo( "Success" )  );
	else
		answersRoot.addInfo( new ResultInfo( "Error" ) );

}

void stopMonitor (std::string name){

	FtkConfiguration * myconfig = FtkConfiguration::getInstance();

	if( myconfig->stopMonitor( name ) )
		answersRoot.addInfo( new ResultInfo( "Success" )  );
	else
		answersRoot.addInfo( new ResultInfo( "Error" ) );

}

void getstatus(std::string cardType, std::string cardName){
	
	FtkConfiguration * myconfig = FtkConfiguration::getInstance();
	
	try{
	
		// Ftk general information
		
		if( (cardType.compare("*") == 0) && (cardName.compare("*") == 0) )
		{
			Json::Value cards;

			myconfig->getCardsJson(cards, true);

			answersRoot.addInfo( new CleanInfo( cards ) );
			return;
		}
		
		// to take the card Type information
		
		if( cardType.compare("*") && (cardName.compare("*") == 0) )
		{

			Json::Value cardTypeInfo ;

			int slot;
			std::stringstream ss;
			ss << cardType;
			ss >> slot;

			myconfig->getCardJson(slot, cardTypeInfo, true );


			// success

			answersRoot.addInfo( new CleanInfo( cardTypeInfo ) );
			return;

		}
		
		// to take the card Name information
		
		if( cardType.compare("*") && cardName.compare("*") )
		{
	
			return ;

		}

	
	}
	catch(...)
	{
		std::string err("found exception while parsing the Configuration Json. Check Configuration retrieving.");
		answersRoot.addInfo( new ErrorInfo( err ) );
	}
		
	answersRoot.addInfo( new ErrorInfo("Bad use of getstatus function. Examples : ...") );
	
}
	
void help(){
	
	Json::Value functs;
	
	functions->getFunctionsList(functs);



	CleanInfo *newInfo = new CleanInfo( functs ) ;
		
	answersRoot.addInfo( newInfo );
}

void help(std::string functionName)
{	
	Json::Value descJson;

	Function * myFun=functions->getFunction(functionName);
	
	if( myFun )
	{
		myFun->getHelp(descJson);
	}
	else
		descJson["error"] = "No Function found";

	answersRoot.addInfo( new CleanInfo( descJson ) );

}

struct BothAre
{
    char c;
    BothAre(char r) : c(r) {}
    bool operator()(char l, char r) const
    {
            return r == c && l == c;
    }
};

std::string FtkInterpreter::myTrim(std::string & str)
{
    std::stringstream ss;
    std::string::iterator i = unique(str.begin(), str.end(), BothAre(' '));
    std::copy(str.begin(), i, std::ostream_iterator<char>(ss, ""));

    return ss.str();
}


Json::Value FtkInterpreter::interpret(char *)
{
	std::cout << "Old interpreter called"  << std::endl; /**@todo togliere*/

}

Json::Value FtkInterpreter::interpret(std::string & input_string)
{

	answersRoot.clear();

    std::cout << "Interpreting " << input_string << std::endl;


	std::vector<std::vector<std::string> > commands;

	std::string token;

	std::istringstream iss(input_string);

	while ( getline(iss, token, ';') )
	{
		token = trim_copy( myTrim(token) );

		if(! token.empty() )
		{
			std::vector<std::string> elements;
			std::istringstream iss_2(token);

			while ( getline(iss_2, token, ' ') )
			{
				elements.push_back(token);
				std::cout << token << std::endl;
			}

			commands.push_back(elements);
		}
	}

	// debugging
	for (unsigned int i = 0; i < commands.size(); i++)
	{
		#ifdef DEBUG_INTERPRETER
	    	    std::cout << "Command " << i << " :" << std::endl;
		#endif

	    // Extracting the first element as function name
	    std::string funcName = commands.at(i).at(0);
	    commands.at(i).erase(commands.at(i).begin());

		#ifdef DEBUG_INTERPRETER
	    	for (unsigned int j = 0; j < commands.at(i).size(); j++)
	    		std::cout << commands.at(i).at(j) << " ";
	    	std::cout << std::endl;
	    #endif

	    //try
	    //{
	    	interpretSingleExpressionStrings ( funcName,  "null", commands.at(i) ) ;
	    //}
	    //catch( ... )
	    //{
	    //	std::cerr << "Huge Error interpreting" << std::endl;
	    //}

	}
	///////////////////

	return answersRoot.getJson();

}



int assignment(std::string varName, Var::Ptr varParam){
	
	Info *newInfo = new Info();	
	std::stringstream myStream;
	
	if(varParam)
	{
		std::cout << "++ Assigning the variable: " << varName << " the value " << varParam->toString() << std::endl;	
		
		Var::Ptr ret;
		ret = sessionContext.getVariable(varName);
	
		if(ret) 
		{
			ret->copyValueFrom(varParam); //TODO controllare la eccezione di tipo
	
			myStream << varName << " <- " << varParam->toString() ;
			newInfo->addInfo ( new ResultInfo ( myStream.str() ) );
				
		}
		else
		{
			std::cout << "++ Accedo al nome"<< std::endl;	
			varParam->setName(varName);
			
			std::cout << "++ Aggiungo la variabile"<< std::endl;	
			std::cout << varParam->getName() << std::endl;	
		
			sessionContext.addVariable(varParam);	
			
			myStream << "Created new variable: " << varName  <<" <- " << varParam->toString() ;
			newInfo->addInfo ( new ResultInfo ( myStream.str() ) );
		}
	}
	else
	{
		myStream << "Assign variable to" << varName << " null value" ;
		newInfo->addInfo ( new ResultInfo ( myStream.str() ) );
	}	
		
	answersRoot.addInfo(newInfo);
	
return 0;
}


}

