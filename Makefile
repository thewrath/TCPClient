CFLAGS = -g -std=c++17 -fpermissive -pthread -lprotobuf -Wall -Werror -ljsoncpp
LIB_DIR = lib/
LIBS= 
SRC_DIR = src/
BUILD_DIR = bin/
FIND_RECUR = $(shell find src -type f -name "*.cpp")

objects:  
	g++ -c $(SRC_DIR)*.cpp $(FIND_RECUR) $(CFLAGS)

client: objects
	@echo "** Building the client"
	g++ -o $(BUILD_DIR)client.out *.o $(CFLAGS) $(LIBS)

run_client:
	@echo "Start client"
	./bin/client.out
	
clean:
	@echo "** Removing object files ..."
	rm -f *.o