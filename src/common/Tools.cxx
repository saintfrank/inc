#include "ftk/Tools.h"

namespace ftk{

bool tools::getFtkPath( std::string & target ){

	char * pPath = 0;
	pPath = getenv("FTK_PATH");

	if( pPath == 0 )
		return false;

	target = pPath;
	return true;

} // getFtkPath

} // namespace ftk
