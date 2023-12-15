CXX = mpic++
MPICXX = mpic++
CPPFLAGS = -Wall -g -std=c++11 -fopenmp
LDFLAGS = -pthread
OMP_NUM_THREADS = 8

TARGET = clique
SRCS = parallel-recursive.cpp recursive.cpp ../test.cpp ../util.cpp
OBJS = $(SRCS:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(MPICXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(MPICXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	mpirun -np 8 ./$(TARGET) $(FLAGS)