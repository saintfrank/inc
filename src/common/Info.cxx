////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <ftk/Info.h>
#include <ftk/core.h>

namespace ftk {
	
void getMemberValue(Json::Value & json, std::string desiredMember, Json::Value & result)
{
		
		if (json.isArray())
		{
			for(int i = 0; i < json.size(); i++)
			{
				getMemberValue( json[i], desiredMember, result );	
			}
		}		
		else 
		{
			 if( json.isMember(desiredMember) )
				{
					result[desiredMember] = json.get(desiredMember, "no Value");
					
					#ifdef DEBUG_JSON
					std::cout << "trovato "  << desiredMember << " : " << std::endl <<  result[desiredMember] << std::endl;					
					#endif
				}
		}
	
}

bool addMemberValue( std::string memberName, Json::Value value, Json::Value & destinationJson )
{
		
		#ifdef DEBUG_JSON
		std::cout << "+ addMemberValue for " << value << " on " << destinationJson << std::endl;					
		#endif	
			
		if( destinationJson.empty() )		
		{
			Json::Value val;
			val[memberName] = value;
									
			destinationJson.append(val); // TODO qui mettere SET
			return true;
		}
				
		if ( destinationJson.isArray() )
		{
			#ifdef DEBUG_JSON
			std::cout << "Iterating for "  << memberName << " array " << destinationJson << std::endl;					
			#endif
			
			// if list the JsonCpp Function Get() would file and assert
			for(int i = 0; i < destinationJson.size(); i++)
			{
				addMemberValue( memberName, value, destinationJson[i] )	;
			}
		}		
		else if (destinationJson.isObject())
		{
			// destinationJson is not and array. Is an Object // TODO controllare se è solo un valore
			#ifdef DEBUG_JSON
			std::cout << "+ Info.cxx : Adding to Object  " <<  destinationJson << " the member "
			          << memberName << " with value " <<  value << std::endl;					
			#endif
			
			if( destinationJson.isMember(memberName) )
			{
					
				// apppending
				if( destinationJson[memberName].isArray() )
				{
					#ifdef DEBUG_JSON
					std::cerr << "### Json Error : adding "<< value<<" on array" << std::endl;
					#endif					
					
					Json::Value & myDest = destinationJson[memberName] ;
					unsigned int i = 0;
					if( myDest.get(i, "nothing").type() != value.type() )
					{
						#ifdef DEBUG_JSON
						std::cerr << "### Json Error : Member " << value << " has already different types of values: "<< myDest.get(i, "nothing") << std::endl;
						#endif
						return false;
					}
						
					myDest[memberName].append(value);
					#ifdef DEBUG_JSON
					std::cerr << "### Json Error : added "<< value<<" on array" << std::endl;
					#endif					
					return true;
				}
				else
				{ 	
					#ifdef DEBUG_JSON
					std::cerr << "### Substituting the old value with object "<< value<<" on object" << std::endl;
					#endif
					// create the new array 
					Json::Value newValue;
					newValue.append( destinationJson[memberName] );
					newValue.append( value );
					// removing the old member
					destinationJson.removeMember(memberName);
					// adding the new array member 
					destinationJson[memberName] = newValue ;
					#ifdef DEBUG_JSON
					std::cerr << "### Json Error : added "<< value<<" on object" << std::endl;
					#endif					
				}
			}
			else
			{
				#ifdef DEBUG_JSON
				std::cout << "+ Info.cxx : adding new member "<< std::endl;					
				#endif
				destinationJson[memberName] = value;
			}
			
			return true;
		}
		else 
		{ 
			// if is just a value
			
			#ifdef DEBUG_JSON
			std::cout << "### Json Error : Called add Member Value to a value "<< std::endl;					
			#endif
			return false;			
		}
	
}	

bool addAllMembers( Json::Value & sourceJson, Json::Value & destinationJson)
{
		
	if( sourceJson.isArray() )
	{
		
		for( unsigned int i = 0; i < sourceJson.size() ; i++ )
			if( ! addAllMembers( sourceJson[i], destinationJson ) )
				{
					std::cerr << "### Json Error : impossible to add member of list " << i << std::endl;
					return false;
				}
		
		return true;	
		
	}		
	
	// if is an object
	Json::Value::Members members;
	members = sourceJson.getMemberNames();
		
	for( unsigned int i = 0; i < members.size() ; i++ )
		if( ! addMemberValue( members.at(i), sourceJson[members.at(i)], destinationJson ) )
			{
				std::cerr << "### Json Error : impossible to add member " << members.at(i) << std::endl;
				//return false;
			}
				
	return true;
		
}	

void InfoContainer::addInfo(Info *newInfo){
	
	infoVector.push_back(newInfo);
	
	}
	
void InfoContainer::clear(){
	
	#ifdef DEBUG_JSON
	std::cout << "+ Info : Beginning clearing container" << std::endl ;
	#endif
			
	for ( unsigned int i=0 ; i < infoVector.size(); i++)
		{
		infoVector[i]->clear();
		delete infoVector[i];
		}
	
	infoVector.clear();	
		
	#ifdef DEBUG_JSON
	std::cout << "+ Info : End clearing container" << std::endl ;
	#endif
			
	}	

Json::Value InfoContainer::getJson( ){
	
	Json::Value infoList;
	
	for ( unsigned int i=0 ; i < infoVector.size(); i++)
		{
		infoVector[i]->getJson(infoList);
		}
	
	infoList;
			
	return infoList;	
}

void Info::addInfo(Info *newInfo){
	infoVector.push_back(newInfo);
	}

void Info::clear(){
	
	#ifdef DEBUG_JSON
	std::cout << "+ Info : Beginning clearing Info" << std::endl ;
	#endif	
	
	for ( unsigned int i=0 ; i < infoVector.size(); i++)
	{
		infoVector[i]->clear();
		delete infoVector[i];
	}
			
	infoVector.clear();		
		
	#ifdef DEBUG_JSON
	std::cout << "+ Info : End clearing info " << std::endl ;
	#endif	
		
}

void Info::getJson(Json::Value & root){
	
	Json::Value newInfo;
	
	#ifdef DEBUG_JSON
	std::cout << "+ Info : Begin " << std::endl ;	
	#endif
	
	for ( unsigned int i=0 ; i < infoVector.size(); i++)
		{
		infoVector[i]->getJson(newInfo);
		#ifdef DEBUG_JSON
		std::cout << "+ Info [ " << i << "]" << std::endl ;	
		#endif
		}
		
	#ifdef DEBUG_JSON
	std::cout << "+ Info : Finito " << std::endl ;	
	#endif
		
	root.append(newInfo);
			
}


CommandInfo::CommandInfo(std::string stringParam)
				:commandString(stringParam) { }

void CommandInfo::getJson(Json::Value & root){
	
	#ifdef DEBUG_JSON
	std::cout << "+ Called Command.getJson()" << std::endl ;	
	#endif
		
	Json::Value commandValue;
	commandValue["command"] = commandString;
	addAllMembers( commandValue , root );
		
}
void CommandInfo::clear(){
		#ifdef DEBUG_JSON
		std::cout << "+ Info : Clearing Command" << std::endl ;
		#endif
}

ResultInfo::ResultInfo(std::string stringParam):resultString(stringParam) ,resultInt(-1){ }

ResultInfo::ResultInfo(int intParam):resultInt(intParam){ }

void ResultInfo::getJson(Json::Value& root){
			
	Json::Value resultValue;
			
	if(resultInt==-1)
		resultValue["result"] = resultString ;
	else 
		resultValue["result"] = resultInt ;
		
	addAllMembers( resultValue , root );
		
}

void ResultInfo::clear(){

		#ifdef DEBUG_JSON
		std::cout << "+ Info : Clearing Result" << std::endl ;
		#endif
}
		
ErrorInfo::ErrorInfo(std::string stringParam):errorString(stringParam){ }

void ErrorInfo::getJson(Json::Value & root){
	
	Json::Value errorValue;
	errorValue["error"] = errorString;
	addAllMembers( errorValue , root );
					
}

void ErrorInfo::clear(){
	#ifdef DEBUG_JSON
	std::cout << "+ Info : Clearing Error" << std::endl ;
	#endif
}
		
InterpretationInfo::InterpretationInfo(std::string stringParam):interpretationString(stringParam){ }

InterpretationInfo::InterpretationInfo(Json::Value jsonParam):jsonVal(jsonParam){ }


void InterpretationInfo::getJson(Json::Value& root){
	
	Json::Value newVal;
	
	if(interpretationString.size()==0)
		newVal["info"] = jsonVal ;
	else
		newVal["info"] = interpretationString ;		
	
	addAllMembers( newVal , root);	
		
}

void InterpretationInfo::clear(){
		#ifdef DEBUG_JSON
		std::cout << "+ Info : Clearing Interpretation" << std::endl ;
		#endif
}	
		
CleanInfo::CleanInfo(Json::Value jsonParam):jsonVal(jsonParam){ }

void CleanInfo::getJson(Json::Value& root){
	
	#ifdef DEBUG_JSON
	std::cerr << "+ CleanInfo : jsonVal: "<< jsonVal <<" root is " << root << std::endl ;
	#endif
	
	addAllMembers( jsonVal, root );
	
	#ifdef DEBUG_JSON
	std::cerr << "\n\n+ CleanInfo : After CleanInfo Root is "<< root << "\n\n" << std::endl ;
	#endif
	
}

void CleanInfo::clear(){
	#ifdef DEBUG_JSON
	std::cout << "+ Info : Clearing Clean" << std::endl ;
	#endif
}	


} // namespace ftk
