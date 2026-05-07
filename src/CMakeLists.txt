# 인터페이스 경로 포함
include_directories(${PROJECT_SOURCE_DIR}/include)

# 여기에 다른 구현체 .cpp 파일 추가 예정
add_library(rvc_core
  hardware_interface.cpp
  sensor/sensor_interface.cpp
  sensor/sensor_controller.cpp

  actuator/actuator_interface.cpp
  actuator/cleaner_controller.cpp
  actuator/drive_controller.cpp

  core/rvc_system.cpp

  state/rvc_state.cpp
  state/avoidance_state.cpp
  state/backward_state.cpp
  state/error_state.cpp
  state/forward_state.cpp
)

if(USE_MULL)
    target_compile_options(rvc_core PRIVATE ${MULL_FLAGS})
    target_link_options(rvc_core PRIVATE "-fembed-bitcode=all")
endif()

# 실제 프로그램 실행 파일
add_executable(rvc_app main.cpp)
target_link_libraries(rvc_app rvc_core)
