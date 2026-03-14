CXX = g++
CXXFLAGS = -Wall -pthread
LIBS = -lgtest -lgtest_main

TARGET = run_tests
SRCS = test_dsl.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS)

run: all
	./$(TARGET)

# 정적 분석
cppcheck:
	cppcheck --enable=all --language=c++ --std=c++17 --suppress=missingIncludeSystem .
tidy:
	clang-tidy $(SRCS) -checks=-*,boost-*,bugprone-*,performance-*,readability-*,modernize-*,cppcoreguidelines-*,-modernize-use-trailing-return-type,-cppcoreguidelines-avoid-magic-numbers,-llvmlibc-* -- -std=c++17
#-------여기까지

test: all
	@echo "버전 확인"
	$(CXX) --version
	@echo "\n\n"
	@echo "프로그램 main 확인\n"
	./$(TARGET)
