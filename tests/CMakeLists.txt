# 테스트용 include
include_directories(${PROJECT_SOURCE_DIR}/tests)
include_directories(${PROJECT_SOURCE_DIR}/include)

# 테스트 활성화
enable_testing()

# 테스트 실행 파일 생성
add_executable(run_tests
	test_main.cpp 
	# 여기에 다른 테스트 파일 추가 예정
)

# 라이브러리 연결 (로직 + gTest)
target_link_libraries(run_tests
	rvc_core 
	GTest::gtest_main 
	GTest::gmock
)

# CTest에 등록
add_test(NAME AllTests COMMAND run_tests)