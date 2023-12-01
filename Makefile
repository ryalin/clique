CXX = clang++
CPPFLAGS = -Wall -g -std=c++11
LDFLAGS = -pthread
TARGET = clique
SRCS = main.cpp sequential.cpp test.cpp openMP.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)