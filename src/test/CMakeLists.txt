# TODO: use ADD_TEST instead

set(SOURCES_LIST
  test_Layer.cpp
  test_Assembly.cpp
  runTests.cpp
)

add_executable( runTests ${SOURCES_LIST} )
target_link_libraries( runTests
  MultiTrace
  ${Boost_LIBRARIES}
)

#ADD_TEST(AssemblyTest ${CMAKE_CURRENT_BINARY_DIR}/SimpleTest Hello )