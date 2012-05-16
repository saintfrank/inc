////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <iostream>
#include <ftk/Monitor.h>

using namespace std;
using namespace ftk;

// Static data member initialization
std::string FtkMonitor::s_partition= "";
ISInfoDictionary *FtkMonitor::s_isDictionary= NULL;
OHRootProvider *FtkMonitor::s_rawProvider=NULL;
std::string FtkMonitor::s_isServerName;
bool FtkMonitor::s_initialized=false;


// Static Getters 
OHRootProvider * FtkMonitor::s_getRawProvider(){
return FtkMonitor::s_rawProvider;
}

ISInfoDictionary* FtkMonitor::s_getIsDictionary(){
return FtkMonitor::s_isDictionary;
}

std::string FtkMonitor::s_getPartition(){
return FtkMonitor::s_partition;
}

boost::posix_time::time_duration FtkMonitor::get_time(){
return boost::posix_time::millisec(m_interval);
}




