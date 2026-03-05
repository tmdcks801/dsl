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
	@echo "1. [환경 테스트] GCC 버전 확인"
	$(CXX) --version
	@echo ""
	@echo "2. [실행 테스트] 프로그램 출력 확인"
	./$(TARGET)