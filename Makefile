CC = clang++
CFLAGS = -Wall -g -std=c++11
TARGET = clique
SRCS = main.cpp sequential.cpp test.cpp openMP.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)