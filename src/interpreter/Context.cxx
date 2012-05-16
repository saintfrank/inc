////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <ftk/Context.h>

namespace ftk{

Context::Context(){}	
	
/**
* Adds the variable to symbol table
*
* @return 1 if the operation succeded. 0 if an element with the same name was already present
*/
bool Context::addVariable(Var::Ptr & newVar){
	
	if(newVar)	
	{
		std::cout << "+ Context : adding the variable of pointer " << newVar << std::endl;		
	
		std::pair < std::map < std::string, Var::Ptr >::iterator , bool > ret;
			
		ret = symbolsTable.insert ( std::pair <std::string, Var::Ptr > ( newVar->getName(), newVar) );
	
		return ret.second;	
	}
	else
	{
		std::cout << "+ ERROR : Context : Requested to add NULL Pointer to variable" << newVar << std::endl;		
		return false;	
	}

}

/**
* Returns the variable
*
* @return The variable associate with the name 'name'. NULL if not present
*/
Var::Ptr Context::getVariable(std::string & name){
	
	  std::map< std::string,Var::Ptr >::iterator foundVar;
	
      foundVar = symbolsTable.find(name);
      
      if (foundVar == (symbolsTable.end()) ) 
	  {
		    Var::Ptr nullPtr;
			return nullPtr;
			std::cout << "+ Context : variablie non trovata " << std::endl;  
	  }
	  else
	  {
		  	std::cout << "+ Context : found variable " << name << " : " << foundVar->second << std::endl;  
			return (foundVar->second);
	  }
}


void Context::getVariablesList(Json::Value & root){
	
	Json::Value variables;
	
	std::map< std::string, Var::Ptr >::const_iterator it;
	
				
	for ( it = symbolsTable.begin() ; it != symbolsTable.end(); it++ )
		{
			std::cout << "Iterating on the Variables " << std::endl;
			std::string name ( it->first );
			Var::Ptr var( it->second );
			
			if(var)
			{
				std::stringstream value;
				value << var->toString() << " ( " << var->getType() << " ) ";
				variables[name] = value.str();								
			}
			else
			{
				variables[name] = "Value Unreachable";				
			}
		}	
		
	root["Variables : "] = variables;
		
}

} // end namepsace
