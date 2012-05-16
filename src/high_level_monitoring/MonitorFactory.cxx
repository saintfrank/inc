////////////////////////////////
//
//	Francesco Cervigni 2010/2011
//
//
////////////////////////////////

#include <ftk/MonitorFactory.h>

#define DEBUG_MONITOR_FACTORY

using namespace boost;

namespace ftk{

std::vector<FtkMonitor*> FtkMonitorFactory::createMonitors( std::vector<OksMonitorRepresentation> monitorRepresentations ){


    std::vector<FtkMonitor*> createdMonitors;
    
    for ( unsigned int i=0; i < monitorRepresentations.size() ; i++ )
	{

    	//Getting the type name
		std::string monitorConcreteClass = monitorRepresentations.at(i).m_monitor_type;


		std::cerr << "Comparing ." << monitorConcreteClass << ". and .StatusMonitor." << std::endl;
	
		if ( monitorConcreteClass.compare("StatusMonitor") == 0)
		{
			// In this case the monitor is a status monitor

			#ifdef DEBUG_FACTORY
			std::cerr << "+ FtkMonitorFactory : Calling Status Monitor constructor" << monitorConcreteClass << std::endl;
			#endif
			FtkStatusMonitor* newMonitor= new FtkStatusMonitor( monitorRepresentations.at(i) );
			createdMonitors.push_back(newMonitor);
		}
		else	// if is not the standard status monitor
		{
			FtkConfiguration * myConfig = FtkConfiguration::getInstance();

			// In this case the monitor has to be created by the plugin
			FtkCardRepresentation & thisCard = myConfig->getCardFromSlot( monitorRepresentations.at(i).m_slot );

			if(thisCard.m_name == "")
			{
				throw daq::ftk::CardAccessException( ERS_HERE, " In MonitorFactory ");
			}
			else
			{
				iFtkPlugin * pointer = thisCard.m_plugin.get();

				if(!pointer)
				{	// if pointer given by the plugin is null
					throw daq::ftk::PluginNotFoundException( ERS_HERE, " In MonitorFactory ");
				}

				#ifdef DEBUG_MONITOR_FACTORY
					std::cout << "MonitorFactory : Load Plugin : pointer to plugin form plugin is: " << pointer << std::endl;
				#endif

				FtkMonitor * newMon = thisCard.m_plugin->getMonitor( monitorRepresentations.at(i) ) ;

				if ( newMon == NULL)
				{
					throw daq::ftk::PluginNotFoundException( ERS_HERE, " In MonitorFactory ");
				}

				createdMonitors.push_back(newMon);

			}

			std::cerr << "Done" << std::endl;

		}

	}
	 
    return createdMonitors; 
	
}

} // namespace ftk
