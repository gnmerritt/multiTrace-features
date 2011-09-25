# TODO: use ADD_TEST instead

set(SOURCES_LIST
  test_Layer.cpp
  test_Assembly.cpp
  runTests.cpp
)

#add_executable( runTests ${SOURCES_LIST} )
#target_link_libraries( runTests
#  MultiTrace
#  ${Boost_LIBRARIES}
#)

ENABLE_TESTING()

add_executable(AssemblyTest test_Assembly.cpp)
target_link_libraries( AssemblyTest
  MultiTrace
  ${Boost_LIBRARIES}
)
add_test(AssemblyTest ${CMAKE_CURRENT_BINARY_DIR}/AssemblyTest Hello )
