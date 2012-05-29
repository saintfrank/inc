### Author : Francesco Cervigni
### Date : May 2012

# Compiling the server
CCPP=clang++
CPP_FLAGS= -I/Users/showandtell/boost_1_49_0 -I../includes -DJSON_IS_AMALGAMATION -g
CPP_LINK_OPT=   -lboost_system -lboost_thread -lboost_date_time 

# relative output for the Scala code
SCALA_BUILD_PATH ?= build/

programs = ftk_server ftk_client


all: $(programs)



ftk_server: jsoncpp.o mime_types.cpp server.cpp connection.cpp   request_parser.cpp  main.cpp  reply.cpp  request_handler.cpp
	$(CCPP) $(CPP_FLAGS) $(CPP_LINK_OPT) mime_types.cpp server.cpp connection.cpp   request_parser.cpp  main.cpp  reply.cpp  request_handler.cpp jsoncpp.o  -o server

ftk_client: jsoncpp.cpp client_a.cpp
	$(CCPP) $(CPP_FLAGS) $(CPP_LINK_OPT)  jsoncpp.cpp client_a.cpp -o client

jsoncpp.o: jsoncpp.cpp
	$(CCPP) $(CPP_FLAGS) -c jsoncpp.cpp


clean:
	rm -rf *.o $(programs)
