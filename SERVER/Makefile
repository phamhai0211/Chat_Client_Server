CXX = g++
CXXFLAGS = -Wall -Wextra -fpermissive -std=c++14
LIBS    = -lboost_system -lboost_thread -pthread
# Target executable name
TARGET = ChatServer

# Source files
SRCS = ChatParticipant.cpp ChatRoom.cpp ChatServer.cpp ChatSession.cpp Main.cpp Timestamp.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = ChatParticipant.hpp ChatRoom.hpp ChatServer.hpp ChatSession.hpp Timestamp.hpp Protocol.hpp

# Default rule: build the executable
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(BOOST_LIBS) -o $(TARGET) $(OBJS)

# Rule to build object files from source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
