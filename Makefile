CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: server client

server: src/server.cpp
	$(CXX) $(CXXFLAGS) -o build/server src/server.cpp

client: src/client.cpp
	$(CXX) $(CXXFLAGS) -o build/client src/client.cpp

clean:
	rm -f build/server build/client