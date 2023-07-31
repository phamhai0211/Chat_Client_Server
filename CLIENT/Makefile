CXX = g++
CXXFLAGS = -Wall -Wextra -fpermissive -std=c++14 `wx-config --cppflags --libs`
LIBS    = -lboost_system -lboost_thread -pthread `wx-config --cppflags --libs`
# Target executable name
TARGET = ChatClient

# Source files
SRCS = ChatClient.cpp Main.cpp 

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = ChatClient.hpp Protocol.hpp

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
