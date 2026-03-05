CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17
TARGET = my_app.exe
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

run: all
	./$(TARGET)

test: all
	@echo "버전 확인"
	$(CXX) --version
	@echo "\n\n"
	@echo "프로그램 main 확인\n"
	./$(TARGET)