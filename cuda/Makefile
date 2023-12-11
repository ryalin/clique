CXX = clang++
NVCC = nvcc
CPPFLAGS = -g -std=c++11
TARGET = clique
SRCS_CPP = main.cpp \
           ../sequential/sequential.cpp \
           ../sequential/sequentialRecursive.cpp \
           ../test.cpp \
           ../util.cpp

SRCS_CUDA = cuda.cu

OBJS = $(SRCS_CPP:%.cpp=%.o) $(SRCS_CUDA:%.cu=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(NVCC) $(CPPFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

%.o: %.cu
	$(NVCC) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)