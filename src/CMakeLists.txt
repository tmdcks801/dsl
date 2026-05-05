# 인터페이스 경로 포함
include_directories(${PROJECT_SOURCE_DIR}/include)

# 여기에 다른 구현체 .cpp 파일 추가 예정
add_library(rvc_core
  "hardware_interface.cpp"
  "sensor/sensor_interface.cpp"
  "sensor/sensor_controller.cpp"
)

# 실제 프로그램 실행 파일
add_executable(rvc_app main.cpp)
target_link_libraries(rvc_app rvc_core)
