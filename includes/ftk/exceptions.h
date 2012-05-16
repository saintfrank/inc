/**
 *  @author Francesco Cervigni
 *  @date Dec 2010
 *
 *  @file Declares FTK exceptions
 *
 */


#ifndef FTK_EXCEPTIONS_H
#define FTK_EXCEPTIONS_H

/////////////////////
//	
//	In order to catch one of these errors:
//
//	try
//	{
//	...yourcode...
//	}
//	catch ( daq::ftk::PartitionProblem& e)
//  {
//    std::cerr << "- Error during your code :" << e.what() << std::endl;
//  }
//
//////////////////////

#include <ers/ers.h>
#include <ipc/exceptions.h>
#include <is/exceptions.h>

namespace daq{

       			
    ERS_DECLARE_ISSUE(	ftk, 
    					PartitionProblem,
                        "The parameter '" << parameter << "' was not found in the configuration file",
                        ((std::string)parameter )
					 )
	
	ERS_DECLARE_ISSUE(	ftk, 
    					VmeError,
    					"Found error when accessing the vme library: " << parameter ,
    					((std::string)parameter )
					 )
	
	ERS_DECLARE_ISSUE(	ftk, 
    					BadConfigFileException,
                        "'" << parameter << "' was not found in the configuration file",
                        ((std::string)parameter )
    				 )
    			
    ERS_DECLARE_ISSUE(	ftk, 
    					TypeException,
    					"Error: Type exception" << parameter ,
    					((std::string)parameter )
    			     )
    			
    ERS_DECLARE_ISSUE(	ftk, 
    					WrongParameters,
    					"Error: Wrong Parameters"  << parameter ,
    					((std::string)parameter )
    				 )
    			
    ERS_DECLARE_ISSUE(	ftk, 
    				 	CoralException,
                        "Error: Interacting with coral"  << parameter ,
                        ((std::string)parameter )
    			     )
    
    ERS_DECLARE_ISSUE(	ftk, 
    					CoolException,
                        "Error: Interacting with cool"  << parameter ,
                        ((std::string)parameter )
    				 )
	ERS_DECLARE_ISSUE(	ftk,
						LibraryNotFoundException,
						"Error: Library not found "  << parameter ,
						((std::string)parameter )
					 )

	ERS_DECLARE_ISSUE(	ftk,
						PluginNotFoundException,
						"Error: Plugin not found in the context. "  << parameter ,
						((std::string)parameter )
					 )

	ERS_DECLARE_ISSUE(	ftk,
						CardAccessException,
						"Error: accessing card representation data in the context. "  << parameter ,
						((std::string)parameter )
					 )

	ERS_DECLARE_ISSUE(	ftk,
						OksAccessException,
						"Error: accessing OKS. "  << parameter ,
						((std::string)parameter )
					 )

    ERS_DECLARE_ISSUE(	ftk,
						ISException,
						"Error: Publishing on IS "  << parameter ,
						((std::string)parameter )
					 )

	ERS_DECLARE_ISSUE(	ftk,
						OHException,
						"Error: Publishing the histogram "  << parameter ,
						((std::string)parameter )
					 )

	ERS_DECLARE_ISSUE(	ftk,
						EnvironmentValueException,
						"Error: "  << parameter ,
						((std::string)parameter )
					 )

}

#endif
