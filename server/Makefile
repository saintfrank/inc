### Author : Francesco Cervigni
### Date : May 2012

# Compiling the server
CCPP=clang++
CPP_FLAGS= -I/Users/showandtell/boost_1_49_0 -I../includes -DJSON_IS_AMALGAMATION -g
CPP_LINK_OPT=   -lboost_system -lboost_thread -lboost_date_time 


programs = server client


all: $(programs)

server: jsoncpp.o server.cpp connection.cpp main.cpp  
	@printf "Compiling Server..."
	@$(CCPP) $(CPP_FLAGS) $(CPP_LINK_OPT) server.cpp connection.cpp  main.cpp jsoncpp.o  -o server
	@echo "OK."

client: jsoncpp.cpp client_s.cpp
	@printf "Compiling Client..."
	@$(CCPP) $(CPP_FLAGS) $(CPP_LINK_OPT)  jsoncpp.cpp client_s.cpp -o client
	@echo "OK."

jsoncpp.o: jsoncpp.cpp
	@printf "Compiling Json..."
	@$(CCPP) $(CPP_FLAGS) -c jsoncpp.cpp
	@echo "OK"


clean:
	rm -rf *.o $(programs)
