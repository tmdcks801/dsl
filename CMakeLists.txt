cmake_minimum_required(VERSION 3.14)

if(NOT MSCV)
	set(CMAKE_CXX_COMPILER "clang++")
endif()

project(rvc-controller CXX)

option(USE_MULL "Enable Mull mutation testing" OFF)
if(USE_MULL)
    set(MULL_PLUGIN "/usr/lib/mull-ir-frontend-21") 
    set(MULL_FLAGS "-fpass-plugin=${MULL_PLUGIN}" "-fembed-bitcode=all" "-grecord-command-line" "-fno-inline")
endif()

# 테스트 활성화
enable_testing()

# C++ 20 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(/utf-8)
endif()

option(USE_COVERAGE "Enable code coverage" OFF)

if(USE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# gTest & gMock 가져오기
include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

# 정적 코드 분석 설정 (Cpp check)
find_program(CPPCHECK_EXE NAMES "cppcheck")
if(CPPCHECK_EXE)
	message(STATUS "C++ check found: ${CPPCHECK_EXE}")

	set(CMAKE_CXX_CPPCHECK
		 ${CPPCHECK_EXE}
		 "--enable=all"		# 모든 체크 활성화 (style, performance 등)
		 "--inconclusive"	# 확실하지 않지만 버그 가능성이 있는 항목도 표시
		 "--force"			# 분석 중단 없이 강제 진행
		 "--inline-suppr" 	# 코드 내 주석으로 특정 경고 무시 가능
		 "--suppress=missingIncludeSystem" # 시스템 헤더 누락 경고 무시
		 "--quiet"			# 너무 많은 출력 방지
	)
else()
	message(WARNING "C++ check not found. Please install cppcheck for static code analysis.")
endif()

# 정적 코드 분석 설정 (Clang-Tidy)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
	message(STATUS "Clang-Tidy found: ${CLANG_TIDY_EXE}")
	set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} "-checks=-*,readability-*,bugprone-*")
else()
	message(WARNING "Clang-Tidy not found. Please install clang-tidy for static code analysis.")
endif()

# 하위 디렉터리 추가
add_subdirectory(src)
add_subdirectory(tests)
