#include "prova.h"

Json::Value nullJsonValue;
Json::Value prova("sandy");

void add( Json::Value & root, std::string stre ){

   root["steps"].append(stre);

}

void ciao ( Json::Value & root ){

  add( root , "gagagaga");

  std::cout << root;

}

bool check ( Json::Value & root  )
{

  if ( root.isObject() )
  	if ( root.isMember("DefaultNullObject") )
		  return true;
  

   return false; 

}


int main (){

 ciao(Json::Value());
 ciao();
 ciao();
 ciao();


 std::cout << "Ildefault : \n" << prova << "\n" << check(prova) << std::endl ;

 Json::Value aa;
 aa["lalala"] = 3.2;
 aa["altro"] = "adag";
 aa["prova"].append(23);

 std::cout << "Compose : \n " << aa << "\n" << check(aa) << std::endl ;


 Json::Value bb;
 bb.append(12);
 bb.append(32);

 std::cout << "Lista : \n " << bb << "\n" << check(bb) << std::endl ;
}
