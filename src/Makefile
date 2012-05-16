# Compiling the server
CCPP=g++
CPP_FLAGS= -I/Users/showandtell/boost_1_49_0 -I../includes -DJSON_IS_AMALGAMATION -g
CPP_LINK_OPT= -L/Users/showandtell/boost_1_49_0/lib -lboost_system -lboost_thread -lboost_date_time

# relative output for the Scala code
SCALA_BUILD_PATH ?= build/

programs = ftk_server ftk_console


all: $(programs)

ftk_server: RemoteConnector.o RemoteThread.o jsoncpp.o initializer.o	
	$(CCPP) $(CPP_LINK_OPT) RemoteConnector.o RemoteThread.o jsoncpp.o initializer.o -o ftk_server

ftk_console: Console.o Console_init.o jsoncpp.o
	$(CCPP) $(CPP_LINK_OPT) Console.o Console_init.o jsoncpp.o -o ftk_console

RemoteConnector.o: high_level_monitoring/RemoteConnector.cxx
	$(CCPP) $(CPP_FLAGS) -c high_level_monitoring/RemoteConnector.cxx 

initializer.o: high_level_monitoring/initializer.cxx
	$(CCPP) $(CPP_FLAGS) -c high_level_monitoring/initializer.cxx

RemoteThread.o: high_level_monitoring/RemoteThread.cxx
	$(CCPP) $(CPP_FLAGS) -c high_level_monitoring/RemoteThread.cxx 

Console.o: high_level_monitoring/Console.cxx 
	$(CCPP) $(CPP_FLAGS) -c high_level_monitoring/Console.cxx 

Console_init.o: high_level_monitoring/Console_init.cxx
	$(CCPP) $(CPP_FLAGS) -c high_level_monitoring/Console_init.cxx

jsoncpp.o: common/jsoncpp.cpp
	$(CCPP) $(CPP_FLAGS) -c common/jsoncpp.cpp


clean:
	rm -rf *.o $(programs)
