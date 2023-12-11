CXX = mpic++
MPICXX = mpic++
CPPFLAGS = -Wall -g -std=c++11 -fopenmp
LDFLAGS = -pthread
OMP_NUM_THREADS = 8

TARGET = clique
SRCS = main.cpp \
       ../sequential/sequentialRecursive.cpp \
       ../test.cpp \
       ../util.cpp
OBJS = $(SRCS:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(MPICXX) $(CPPFLAGS) $(LDFLAGS) -fopenmp -o $@ $^

%.o: %.cpp
	$(MPICXX) $(CPPFLAGS) -fopenmp -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	mpirun -np 8 ./$(TARGET)