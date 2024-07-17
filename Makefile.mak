CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

SRC_RECEIVER = src/main_receiver.cpp src/message_module.cpp
SRC_SENDER = src/main_sender.cpp src/message_module.cpp

OBJ_RECEIVER = $(SRC_RECEIVER:.cpp=.o)
OBJ_SENDER = $(SRC_SENDER:.cpp=.o)

all: receiver sender

receiver: $(OBJ_RECEIVER)
	$(CXX) $(CXXFLAGS) -o receiver $(OBJ_RECEIVER) -lrt

sender: $(OBJ_SENDER)
	$(CXX) $(CXXFLAGS) -o sender $(OBJ_SENDER) -lrt

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o receiver sender

.PHONY: all clean
