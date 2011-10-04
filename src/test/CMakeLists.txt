
unset(EXECUTABLE_OUTPUT_PATH)

# Single Assembly only
add_executable(AssemblyTest test_Assembly.cpp)
target_link_libraries( AssemblyTest
  MultiTrace
  ${Boost_LIBRARIES}
)
add_test(AssemblyTest AssemblyTest)

# Layer-level tests (the majority)
add_executable(LayerTest test_Layer.cpp)
target_link_libraries( LayerTest
  MultiTrace
  ${Boost_LIBRARIES}
)
add_test(LayerTest LayerTest )
