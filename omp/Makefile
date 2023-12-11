CXX = mpic++
MPICXX = mpic++
CPPFLAGS = -Wall -g -std=c++11 -fopenmp
LDFLAGS = -pthread

TARGET = clique
SRCS = main.cpp \
       ../sequential/sequentialRecursive.cpp \
       ../test.cpp \
       ../util.cpp \
       omp.cpp
OBJS = $(SRCS:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(MPICXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(MPICXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)