CXX = clang++
CPPFLAGS = -Wall -g -std=c++11 -fopenmp
LDFLAGS = -pthread
TARGET = clique
SRCS = main.cpp \
       ../sequential/sequential.cpp \
       ../sequential/sequentialRecursive.cpp \
       ../test.cpp \
       ../util.cpp \
       omp.cpp
OBJS = $(SRCS:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)