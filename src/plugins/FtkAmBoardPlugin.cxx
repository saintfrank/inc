/**
 *  @author Francesco Cervigni
 */

#include <iostream>

#include "ftk/IFtkPlugin.h"
#include "ftk/BoardFunctionsFactory.h"
#include "ftk/Configuration.h

namespace ftk {

class myPlugin: public iFtkPlugin {

	public:

		virtual bool run (const std::string & funcName, std::vector<std::string> parameterStrings, Var::Ptr ret , FtkCardRepresentation & cardRep,  Json::Value & root )
		{


			std::vector<Var::Ptr> parameters;

			std::cout << "Running " << funcName  << std::endl;

			// Retrieving the function
			std::string temp(funcName);
			Function * myFun = functionsSet.getFunction(temp);

			if( myFun == NULL )
			{
				// If the function was not found
				std::cout << "+ FtkInterpreter : Found NULL function "<< std::endl;
				root["info"].append("Function not found" ) ;
				return false;
			}
			else
			{
				std::cout << "+ FtkInterpreter : Found a function named " << myFun->getName() << std::endl;
			}

			for(unsigned i=0; i < parameterStrings.size(); i++)
			{
				std::string name ( parameterStrings[i] );


				std::vector<Var::Ptr> funcParameters = myFun -> getParameters();
				std::cout << "+ FtkInterpreter : " << name << ", creating an instance of "<< funcParameters[i]->getName() << " : "<< funcParameters[i]->getType() << std::endl;
				Var::Ptr originalVar( funcParameters[i] );
				Var::Ptr newVar = originalVar->createCopyWithValue(parameterStrings[i]) ;
				if(newVar)
					parameters.push_back(newVar);
				else
				{
					// If the paramenter string cannot be converted into the desired type
					std::stringstream message ;
					message << "The " << i+1 << "¡ parameter is not of the expected type: " << funcParameters[i]->getName() << " : " << funcParameters[i]->getType()  ;
					root["info"].append( message.str() ) ;
					return false;
				}

			}

			// Qui' si tratta della variabile di ritorno

			std::cout << "+ FtkInterpreter : return variable " << " existed ? " << std::endl ;

			// creating a new dummy variable
			std::string s("Dummy");
			ret = myFun->createNewReturnVariable(s);
			std::cout << "+ FtkInterpreter : Created a new empty variable for the return of the function: " << myFun->getName() << std::endl;



			// Launching the function
			if ( myFun->run(ret, parameters, root) )
				root["Returned"]="Success";
			else
				root["Returned"]="Error";

			return true;

		}

		virtual Function * getFunction( const std::string & funcName )
		{


			std::cerr<< "Someone is looking for " << funcName << std::endl;


			std::string s = funcName;


			return 0; //functionsSet.getFunction(s);

		}


		virtual bool getFunctionsDescription	(Json::Value & root){

			std::cout << "\n\n\nPlugin.Called getFunctions.\n\n\n"  << std::endl;

			return true;

		}

	private:

		BoardFunctionsFactory functionsSet ;

};

extern "C" iFtkPlugin * create()			{ return new myPlugin;  }

extern "C" bool destroy( iFtkPlugin * p )	{ delete p; return true; }

}
