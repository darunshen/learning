cmake_minimum_required(VERSION 3.0.0)
project(LittleRain VERSION 0.1.0)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
include_directories("./")
add_subdirectory(src)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
option(UNIT_TEST_SWITCH "unit test" ON)
if (${UNIT_TEST_SWITCH} MATCHES ON)
	ADD_SUBDIRECTORY(unit_test)
endif()
#above is for unit test