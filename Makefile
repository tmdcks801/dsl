CXX = g++
CXXFLAGS = -Wall -pthread
LIBS = -lgtest -lgtest_main

TARGET = run_tests
SRCS = test_dsl.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)