/////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
/////////////////////////////////

#include <ftk/Function.h>

namespace ftk{


std::vector<Var::Ptr> Function::getParameters()		{ return parameters; }


// TODO : dovrei approfondire il problemao, o attraverso variabile di ritorno, o attraverso eccezione o attraverso json error 	
bool Function::checkParameters ( std::vector<Var::Ptr> givenParameters ){
	
	unsigned int paramSize = parameters.size();
	
	std::cout << "Expected : " << givenParameters.size() << ". Passed : " << paramSize << std::endl;
		
	if ( givenParameters.size() != paramSize )
		return false;
	
	unsigned int i = 0;
	
	for ( i=0 ; i < paramSize; i++)
		{
			std::cout << "confronto passata : " << givenParameters[i]->getType() << "e originale" << parameters[i]->getType() << std::endl;
			
			if( ! ( givenParameters[i] -> isSameType(parameters[i]) ) )
				return false;
		}	
	return true;
		
	}

bool Function::checkReturnVariable (Var::Ptr myVar){
	
	if( myVar != 0 )
	{	
		if( myVar -> isSameType(returnedVariable))
				return true;	
	}
		
	return false;
	
	}


Var::Ptr Function::getRetunVariable() {  return returnedVariable; }

bool Function::run(Var::Ptr ret, std::vector<Var::Ptr> params, Json::Value & root ){
	
	std::cout << "+ Function: checking the type of the return variable ... ";	
	
	if ( ! checkReturnVariable(ret) )
		throw daq::ftk::TypeException( ERS_HERE , "wrong return parameter type ")	;

	std::cout << "OK. " << std::endl << "+ Function: Checking the types of the parameters ...";

	if ( ! checkParameters(params) )
		throw daq::ftk::WrongParameters( ERS_HERE , "wrong parameters")	;
				
	std::cout << "OK. " << std::endl << "Calling the semantic " << std::endl;
	
	return functionSemantic->run( ret, params, root);
	
}

Var::Ptr Function::createNewReturnVariable(std::string newName){ return returnedVariable->createCopy(newName); }
	

} // end namespace
