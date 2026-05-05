 # 테스트용 include
include_directories(${PROJECT_SOURCE_DIR}/tests)
include_directories(${PROJECT_SOURCE_DIR}/include)

# 테스트 실행 파일 생성
add_executable(run_tests
	sensor/sensor_interface.cpp

	sensor/dust_sensor_controller.cpp
	sensor/obstacle_sensor_controller.cpp

	actuator/actuator_interface.cpp

	actuator/cleaner_controller.cpp
	actuator/drive_controller.cpp
)

if(MSVC)
    target_compile_options(run_tests PRIVATE /W4) 
else()
    target_compile_options(run_tests PRIVATE -Wall -Wextra)
endif()


# 라이브러리 연결 (로직 + gTest)
target_link_libraries(run_tests
	rvc_core 
	GTest::gtest_main 
	GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(run_tests)
