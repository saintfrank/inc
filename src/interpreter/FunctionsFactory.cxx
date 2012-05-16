////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <ftk/FunctionsFactory.h>

//#define DEBUG_FUNCTIONS

namespace ftk{


/**
* Adds the function to function table
*
* @return 1 if the operation succeded. 0 if an element with the same name was already present
*/
bool FunctionsFactory::addFunction(Function & newFun){
			
	std::pair < std::map <std::string,Function>::iterator , bool > ret;
		
	ret = functionsTable.insert ( std::pair<std::string, Function> ( newFun.getName(), newFun) );
	
	return ret.second;	

}

/**
* Adds the function to function table
*
* @return 1 if the operation succeded. 0 if an element with the same name was already present
*/
bool FunctionsFactory::addFunctions(std::vector<Function> & newFunVec){

	for ( unsigned int i = 0 ; i < newFunVec.size() ; i++ )
	{
		if( ! addFunction( newFunVec.at(i) ) )
			return false;
	}

	return true;

}

/**
* Returns the variable
*
* @return A pointer to the funcion to the function associated with the name 'name'. NULL if not present
*/
Function * FunctionsFactory::getFunction(std::string & name){
	
	  std::map<std::string,Function>::iterator foundFun;
	
      foundFun = functionsTable.find(name);
      
      if ( foundFun == ( functionsTable.end() ) ) 
				return NULL;
	  else
				return &(foundFun->second);
	
}

void FunctionsFactory::getFunctionsList(Json::Value & root){
	
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
