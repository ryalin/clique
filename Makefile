CXX = clang++
CPPFLAGS = -Wall -g -std=c++11 -fopenmp
LDFLAGS = -pthread
TARGET = clique
SRCS = main.cpp sequential/sequential.cpp test.cpp util.cpp parallel/parallel.cpp
OBJS = $(SRCS:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
