CXX := g++
CXXFLAGS := -std=c++17 -Wall -pedantic

TARGET := sniff

SRCS := main.cpp setup.cpp
OBJS := $(SRCS:.cpp=.o)
HEADERS := setup.h raw_socket.h

.PHONY: all clean

all: $(TARGET)

# --- Rule to Link the Executable ---
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@


# --- Rule to Compile Source Files to Object Files ---
# This rule applies to ALL .cpp files and uses the header dependencies
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)