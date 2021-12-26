cmake_minimum_required(VERSION 3.1)
project(fibonacci_sum)

set(FIRST_VERSION_MAJOR 1)
set(FIRST_VERSION "${FIRST_VERSION_MAJOR}")

set(SOURCES main.cpp)
set(HEADERS my_allocator.h my_stack.h)

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/googletest"
	"googletest"
	)

add_executable(stack ${SOURCES} ${HEADRS})
add_executable(test_stack test/test.cpp)

target_include_directories(
	test_stack
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

set_target_properties(
	stack PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	#COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -lpthread"
	)

target_compile_options(
	stack 
	PRIVATE
	-Wall -Wextra -pedantic -Werror
	)

target_link_libraries(
	test_stack
	PRIVATE
	gtest gtest_main)
