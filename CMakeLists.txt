project( multiTrace )
cmake_minimum_required( VERSION 2.6.0 )

set(CMAKE_CXX_FLAGS "-g -Wall")
ENABLE_TESTING()

find_package(Boost)
find_package(PythonLibs)

link_directories (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${Boost_INCLUDE_DIRS}
)

add_subdirectory(src)
